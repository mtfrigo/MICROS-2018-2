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

#define DATA_POINTS 10
#define SAMPLING_PERIOD 1e-2


static volatile int run=1;

#define MAX_RAW 4096


void quit(int signal)
{
    run=0;
}

void setup_servo(char pwmId)
{
  enablePWM(pwmId);
  setPeriod("20000000");
  
  char str[100];
  
  snprintf(str,sizeof str,"%d\n", 1500000);
  setDutycycle(str, pwmId);
}

int main(int argc, char *argv[])
{
  float avg, avg_raw, std;
  
  int servo_duty_cicle;
  struct adc_sensors data[DATA_POINTS];
  int samples;
  
  //Liga servo (pwm1)
  setup_servo('1');

  //Liga LCD
  int lcd_fd = lcd_init();
  lcd_backlight_init(lcd_fd);
  lcd_backlight_set(lcd_fd, 255, 255, 255);

  char lcd_2[16] = "";
  char lcd_1[16] = "";
  
  int fd_adc = adc_cont_setup(DATA_POINTS, SAMPLING_PERIOD);

  adc0_cont_enable();

  double scale;
  scale = adc0_cont_get_scale();

  setDutycycle("0", '1');

  int i = 0;
  float maxLight = 0;
  int maxLightRaw = 0;
  for(i = 0; i < MAX_RAW/50; i++)
  {
    adc_cont_sample(DATA_POINTS, SAMPLING_PERIOD);
    
    //Lê as amostras do sensores
    samples = read(fd_adc, data, sizeof data) / sizeof(struct adc_sensors);

    lseek(fd_adc, 0, SEEK_SET);
    
    avg_raw = adc0_cont_get_avg(data, samples);
    avg = avg_raw * scale;

    float angle;
    angle = util_map(i*50, 0, MAX_RAW, -90, 90);

    float rad_angle ;
    rad_angle = (angle * M_PI)/180;
    
    servo_duty_cicle = (rad_angle/M_PI_2)*825000+1500000;

    if(maxLight < avg) {
      maxLight = avg;
      maxLightRaw = i*50;
      lcd_backlight_set(lcd_fd, 255, 0, 0);
    }
    else {
      lcd_backlight_set(lcd_fd, 0, 255, 0);
    }
    char dc_str[100];
    snprintf(dc_str,sizeof dc_str,"%d\n", servo_duty_cicle);
    setDutycycle(dc_str, '1');

    snprintf(lcd_1, sizeof lcd_1, "Max value = %.3f", maxLight);
    snprintf(lcd_2, sizeof lcd_2, "Light = %.3f", avg);

    lcd_write_words(lcd_fd, lcd_1, lcd_2);

    printf("Values 1: %f\t%d\t%f\n", angle, servo_duty_cicle, avg);

    usleep(5000);
  }


  float angle;
  angle = util_map(maxLightRaw, 0, MAX_RAW, -90, 90);

  float rad_angle ;
  rad_angle = (angle * M_PI)/180;
  servo_duty_cicle = (rad_angle/M_PI_2)*825000+1500000;
  char dc_str[100];
  snprintf(dc_str,sizeof dc_str,"%d\n", servo_duty_cicle);
  setDutycycle(dc_str, '1');

  lcd_backlight_set(lcd_fd, 0, 0, 255);

  adc0_cont_disable();
  // adc1_cont_disable();
  adc_cont_disable();

  close(fd_adc);

  
  return 0;
}

