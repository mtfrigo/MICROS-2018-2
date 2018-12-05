#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <eeprom.h>
#include <string.h>
#include <i2c.h>
#include <lcd.h>
#include <time.h>
#include <poll.h>
#include <math.h>
#include <util.h>
#include <pwm.h>
#include <adc.h>
#include <galileo2io.h>
#include <byteswap.h>
#include <fcntl.h>
#include <stdint.h>

#define DATA_POINTS 100
#define SAMPLING_PERIOD 1e-3


static volatile int run=1;


void quit(int signal)
{
    run=0;
}

void setup_servo(char pwmId)
{
  enablePWM(pwmId);
  setPeriod("1000000");
  
  char str[100];
  
  snprintf(str,sizeof str,"%d\n", 1500000);
  setDutycycle(str, pwmId);
}

int main(int argc, char *argv[])
{
  float avg, avg_raw, std;
  
  int buzzer_dc;
  struct adc_sensors data[DATA_POINTS];
  int samples;

  char pwmId = '1';
  
  //Liga servo (pwm1)
  setup_servo(pwmId);

  //Liga LCD
  int lcd_fd = lcd_init();
  lcd_backlight_init(lcd_fd);
  lcd_backlight_set(lcd_fd, 255, 255, 255);

  char lcd_1[16] = "";
  char lcd_2[16] = "";
  
  int fd_adc = adc_cont_setup(DATA_POINTS, SAMPLING_PERIOD);

  adc0_cont_enable();

  double scale;
  scale = adc0_cont_get_scale();

  while(run)
  {
    adc_cont_sample(DATA_POINTS, SAMPLING_PERIOD);
    
    //Lê as amostras do sensores
    samples = read(fd_adc, data, sizeof data) / sizeof(struct adc_sensors);

    lseek(fd_adc, 0, SEEK_SET);
    
    avg_raw = adc0_cont_get_avg(data, samples);
    avg = avg_raw * scale;

    buzzer_dc = util_map(avg_raw, 0, 4096, 500000, 1000000);

    char str[100];
    snprintf(str,sizeof str,"%d\n", buzzer_dc);
    setDutycycle(str, pwmId);

    snprintf(lcd_1, sizeof lcd_1, "Volts %.4f", avg);
    snprintf(lcd_2, sizeof lcd_2, "Hertz: %d", buzzer_dc);

    lcd_write_words(lcd_fd, lcd_1, lcd_2);

  }

  adc0_cont_disable();
  adc_cont_disable();

  close(fd_adc);

  
  return 0;
}

