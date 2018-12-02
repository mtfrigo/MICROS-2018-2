#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <eeprom.h>
#include <i2c.h>
#include <lcd.h>
#include <time.h>
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
        int fd_led;

        char state='0';
        
        char data_str[80];
        double scale;
        int raw;
        char rawStr[16];
        char voltageStr[16];

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

        if((fd_led=open("/sys/class/gpio/gpio1/value",O_WRONLY)) < 0)
        {
                perror("Opening gpio1 value:");
                return -1;
        }

        pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
        scale=atof(data_str)/1000.0;

        while(run)
        {
                lseek(fd,0,SEEK_SET);
                read(fd,data_str,sizeof data_str);
                raw=atoi(data_str);

                lseek(fd_led,0,SEEK_SET);
		write(fd_led,&state,sizeof state);


                sprintf(voltageStr, "Voltage = %f V", raw*scale);
                sprintf(rawStr, "Raw = %d", raw);

                printf("Raw=%d\tScale=%f\tVoltage=%fV\n",raw,scale,raw*scale);
                printf("Prop=%f",util_map(raw, 0, 4096, 0, 100));

                lcd_write_words(fd_lcd, rawStr, voltageStr);

                lcd_backlight_set(fd_lcd, rand(), rand(), rand());

                sleep(1);

                if(util_map(raw, 0, 4096, 0, 100) > 50)
                        state='1';
                else 
                        state='0';


               
        }

        close(fd);
        close(fd_led);

	return 0;
}


