#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <eeprom.h>
#include <i2c.h>
#include <lcd.h>
#include <time.h>
#include <math.h>
#include <util.h>
#include <galileo2io.h>

static volatile int run = 1;

void quit(int signal)
{
        run=0;
}

int main(int argc,char *argv[])
{
        struct sigaction act;
        int fd;
        
        char data_str[80];
        double scale;
        int raw;
        char rawStr[16];
        char voltageStr[16];
        char angleStr[16];

        int duty_cycle;
        double angle;
        char str[100];

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        int fd_lcd = lcd_init();
        lcd_backlight_init(fd_lcd);

        if((fd=open("/sys/bus/iio/devices/iio:device0/in_voltage0_raw",O_RDONLY)) < 0)
        {
                perror("Opening in_voltage0raw:");
                return -1;
        }

        pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
        scale=atof(data_str)/1000.0;

        pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
        //scale=atof(data_str)/1000.0;

        
        pputs("/sys/class/pwm/pwmchip0/device/pwm_period","20000000");
        pputs("/sys/class/pwm/pwmchip0/pwm1/enable","1");

        while(run)
        {
                lseek(fd,0,SEEK_SET);
                read(fd,data_str,sizeof data_str);
                raw=atoi(data_str);

                sprintf(voltageStr, "Voltage = %f V", raw*scale);
                sprintf(rawStr, "Raw = %d; 4000", raw);

                printf("Raw=%d\tScale=%f\tVoltage=%fV\n",raw,scale,raw*scale);


                lcd_backlight_set(fd_lcd, rand(), rand(), rand());

                angle=util_map(raw, 0, 4096, -90, 90);
                if(angle > 90) angle = 90 *  M_PI/180.0;
                else angle = angle *  M_PI/180.0;
                duty_cycle=angle/M_PI_2*900000+1500000;
                snprintf(str,sizeof str,"%d\n",duty_cycle);

                sprintf(angleStr, "Raw = %d; Max = 4000", raw);
                snprintf(angleStr,sizeof angleStr,"%f; %f", util_map(raw, 0, 4096, -90, 90), util_map(4096, 0, 4096, -90, 90));
                pputs("/sys/class/pwm/pwmchip0/pwm1/duty_cycle",str);

                lcd_write_words(fd_lcd, rawStr, angleStr);


                sleep(1);
        }

        close(fd);

	return 0;
}


