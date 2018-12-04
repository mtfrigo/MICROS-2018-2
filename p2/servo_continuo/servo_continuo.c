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
  float avg[2], avg_raw[2];
  float std[2];

  int servo_duty_cicle;



  
  char data_str[80];
  struct sensors data[DATA_POINTS];
  float scale[2];
  int samples;
  int fd;

  //Liga servo (pwm1)
  setup_servo('1');

  //Liga LCD
  int lcd_fd = lcd_init();
  lcd_backlight_init(lcd_fd);
  lcd_backlight_set(lcd_fd, 255, 255, 255);


  //Desliga o buffer para fazer a configuração
  pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","0");

  //Pega o scale do adc 0
  pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
  scale[0] = atof(data_str)/1000.0;

  pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/iio:device0/in_voltage1_scale");
  scale[1] = atof(data_str)/1000.0;

  printf("scale 0: %f; scale 1: %f; ", scale[0], scale[1]);
  
  //Dá o enable no canal 0
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage0_en", "1");
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage1_en", "1");
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage2_en", "0");
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage3_en", "0");
  
  //Dá enable pro modo contínuo
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en", "1");

  //Indica que vai coletar 100 amostras por buffer (DATA_POINTS = 100)
  snprintf(data_str,sizeof data_str,"%d", DATA_POINTS);
  pputs("/sys/bus/iio/devices/iio:device0/buffer/length", data_str);
  
  //indica o trigger atual
  pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/trigger0/name");
  pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger",data_str);
  
  //Frequência de trigger é o inverso do 1ms programado
  snprintf(data_str,sizeof data_str,"%d", (int)round(1.0/SAMPLING_PERIOD));
  pputs("/sys/bus/iio/devices/trigger0/frequency", data_str);
  
  //Abre o arquivo que contém as amostras
  if((fd = open("/dev/iio:device0",O_RDONLY)) < 0)
  {
    perror("Opening /dev/iio:device0:");
    return -1;
  }

  char lcd_values[16] = "";
  char lcd_position[16] = "";

  while(run)
  {
    //Dá enable no buffer
    pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","1");
    
    //Fica off por 1 segundo
    usleep(DATA_POINTS * SAMPLING_PERIOD * 1000000);
    
    //Desliga o buffer
    pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","0");
    
    //Lê as amostras do sensores
    samples = read(fd,data, sizeof data) /sizeof(struct sensors);

    lseek(fd, 0, SEEK_SET);
    
    avg_raw[0] = adc0_get_avg(data, samples);
    avg[0] = avg_raw[0] * scale[0];
    std[0] = adc0_get_std_deviation(data, samples, avg_raw[0], scale[0]);

    avg_raw[1] = adc1_get_avg(data, samples);
    avg[1] = avg_raw[1] * scale[1];
    std[1] = adc1_get_std_deviation(data, samples, avg_raw[1], scale[1]);

    float angle[2];
    angle[0] = util_map(avg_raw[0], 0, 4096, -90, 90);
    angle[1] = util_map(avg_raw[1], 0, 4096, -90, 90);

    float rad_angle[2] ;
    rad_angle[0] = (angle[0] * M_PI)/180;
    rad_angle[1] = (angle[1] * M_PI)/180;
    
    servo_duty_cicle = (rad_angle[0]/M_PI_2)*900000+1500000;

    char str[100];
    snprintf(str,sizeof str,"%d\n", servo_duty_cicle);
    setDutycycle(str, '1');

    //snprintf(lcd_values, sizeof lcd_values, "%.4f;%.4f", avg, std);
    snprintf(lcd_values, sizeof lcd_values, "%.4f;%.4f", avg[0], avg[1]);
    snprintf(lcd_position, sizeof lcd_position, "%.2f;%.2f", angle[0], rad_angle[0]);

    lcd_write_words(lcd_fd, lcd_values, lcd_position);

    printf("Values 1: %lf\t%lf\t%lf\n", avg_raw[0], avg[0], std[0]);
    printf("Values 2: %lf\t%lf\t%lf\n", avg_raw[1], avg[1], std[1]);



  }

  //Desliga o trigger
  pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger","\n");
  //Reseta o buffer
  pputs("/sys/bus/iio/devices/iio:device0/buffer/length","2");
  //Desabilita canal 0
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage0_en","0");
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage1_en","0");
  //Reseta a timestamp
  pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en","0");

  close(fd);

  
  return 0;
}

