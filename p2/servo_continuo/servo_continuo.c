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
  setPeriod("20000000");
  
  char str[100];
  
  snprintf(str,sizeof str,"%d\n", 1500000);
  setDutycycle(str, pwmId);
}

int main(int argc, char *argv[])
{
  float avg[2], avg_raw[2], std[2];
  
  int servo_duty_cicle;
  struct adc_sensors data[DATA_POINTS];
  int samples;
  
  //Liga servo (pwm1)
  setup_servo('1');

  //Liga LCD
  int lcd_fd = lcd_init();
  lcd_backlight_init(lcd_fd);
  lcd_backlight_set(lcd_fd, 255, 255, 255);

  char lcd_values[16] = "";
  char lcd_position[16] = "";
  
  int fd_adc = adc_cont_setup(DATA_POINTS, SAMPLING_PERIOD);

  adc0_cont_enable();
  // adc1_cont_enable();

  double scale[2];
  scale[0] = adc0_cont_get_scale();
  // scale[1] = adc1_cont_get_scale();

  while(run)
  {
    adc_cont_sample(DATA_POINTS, SAMPLING_PERIOD);
    
    //Lê as amostras do sensores
    samples = read(fd_adc, data, sizeof data) / sizeof(struct adc_sensors);

    lseek(fd_adc, 0, SEEK_SET);
    
    avg_raw[0] = adc0_cont_get_avg(data, samples);
    avg[0] = avg_raw[0] * scale[0];
    std[0] = adc0_cont_get_std_deviation(data, samples, avg_raw[0], scale[0]);

    // avg_raw[1] = adc1_cont_get_avg(data, samples);
    // avg[1] = avg_raw[1] * scale[1];
    // std[1] = adc1_cont_get_std_deviation(data, samples, avg_raw[1], scale[1]);

    float angle[2];
    angle[0] = util_map(avg_raw[0], 0, 4096, -90, 90);
    // angle[1] = util_map(avg_raw[1], 0, 4096, -90, 90);

    float rad_angle[2] ;
    rad_angle[0] = (angle[0] * M_PI)/180;
    // rad_angle[1] = (angle[1] * M_PI)/180;
    
    servo_duty_cicle = (rad_angle[0]/M_PI_2)*900000+1500000;

    char str[100];
    snprintf(str,sizeof str,"%d\n", servo_duty_cicle);
    setDutycycle(str, '1');

    //snprintf(lcd_values, sizeof lcd_values, "%.4f;%.4f", avg, std);
    snprintf(lcd_values, sizeof lcd_values, "%.4f;%.4f", avg[0], std[0]);
    snprintf(lcd_position, sizeof lcd_position, "%.2f;%.2f", angle[0], rad_angle[0]);

    lcd_write_words(lcd_fd, lcd_values, lcd_position);

    printf("Values 1: %lf\t%lf\t%lf\n", avg_raw[0], avg[0], std[0]);
    // printf("Values 2: %lf\t%lf\t%lf\n", avg_raw[1], avg[1], std[1]);
  }

  adc0_cont_disable();
  // adc1_cont_disable();
  adc_cont_disable();

  close(fd_adc);

  
  return 0;
}

