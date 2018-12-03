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
#include <pwm.h>
#include <adc.h>
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

        char pwmId = '1';
        char adcId = '1';

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        int fd_lcd = lcd_init();
        lcd_backlight_init(fd_lcd);

        fd = openADC(adcId);

        getRawScale(data_str, adcId);
        scale = getScale(data_str);

        // Period for all pwm
        setPeriod("20000000");

        enablePWM(pwmId);

        while(run)
        {
                lseek(fd,0,SEEK_SET);
                read(fd,data_str,sizeof data_str);

                raw=atoi(data_str);

                sprintf(voltageStr, "Voltage = %f V", raw*scale);

                printf("Raw=%d\tScale=%f\tVoltage=%fV\n",raw,scale,raw*scale);

                lcd_backlight_set(fd_lcd, rand(), rand(), rand());

                angle = util_map(raw, 0, 4096, -90, 90)  ;

                if(angle > 90) angle = 90 *  M_PI/180.0;
                else angle = angle *  M_PI/180.0;

                duty_cycle = angle/M_PI_2*925000+1500000;
                snprintf(str,sizeof str,"%d\n",duty_cycle);

                sprintf(rawStr, "Dc = %d; Max = 4096", duty_cycle);
                snprintf(angleStr,sizeof angleStr,"%f; %f", util_map(raw, 0, 4096, -90, 90)  , util_map(4096, 0, 4096, -90, 90) );

                setDutycycle(str, pwmId);

                lcd_write_words(fd_lcd, rawStr, angleStr);

                sleep(1);
        }

        close(fd);

	return 0;
}


