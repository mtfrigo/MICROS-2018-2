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

#define TONE_1 1000000
#define TONE_2 500000

static volatile int run = 1;

void quit(int signal)
{
        run=0;
}

void setup_buzzer(char pwmId)
{
        enablePWM(pwmId);
        setPeriod("1000000");

        char str[100];

        snprintf(str,sizeof str,"%d\n", 0);
        setDutycycle(str, pwmId);
}

int main(int argc,char *argv[])
{
        struct sigaction act;
        int p_fd;
        char p_raw_data[80];
        int p_raw;
        int buzzer_duty_cicle = TONE_1;
        char time[16] = "";
        char alarm[16] = "";
        int ringing = 0;
        int hour = 0, minutes = 0, seconds = 0;
        int time_selected = 0;
        unsigned char b_value;
        struct pollfd b_pfd;

        if((b_pfd.fd=open("/sys/class/gpio/gpio1/value",O_RDONLY)) < 0)
        {
                perror("Opening gpio1:");
                return -1;
        }

        read(b_pfd.fd,&b_value,1);
        b_pfd.events = POLLPRI;

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        int fd_lcd = lcd_init();
        lcd_backlight_init(fd_lcd);
        lcd_backlight_set(fd_lcd, 255, 255, 255);

        p_fd = openADC('1');
        setup_buzzer('1');

        pputs("/sys/class/gpio/gpio1/edge","falling");

        while(run)
        {
                int b_poll_result = poll(&b_pfd, 1, 1);

                // Se o botao foi clicado
                if(b_poll_result != -1 && b_poll_result != 0)
                {
                        lseek(b_pfd.fd,0,SEEK_SET);
                        read(b_pfd.fd,&b_value,1);

                        time_selected = (time_selected + 1) % 3;

                        printf("time_selected %d\n", time_selected);

                        if(ringing)
                        {
                                ringing = 0;
                        }
                }
                
                lseek(p_fd,0,SEEK_SET);
                read(p_fd, p_raw_data, sizeof p_raw_data);
                p_raw = atoi(p_raw_data);
                
                switch(time_selected)
                {
                        case 0:
                                hour = util_map(p_raw, 0, 4096, 0, 24);
                                break;
                        case 1:
                                minutes = util_map(p_raw, 0, 4096, 0, 60);
                                break;
                        case 2:
                                seconds = util_map(p_raw, 0, 4096, 0, 60);
                                break;
                }

                getTime(time, 2);
                snprintf(alarm, sizeof alarm, "%02d:%02d:%02d", hour, minutes, seconds);
                lcd_write_words(fd_lcd, time, alarm);

                if(strcmp(alarm, time) == 0)
                {
                        ringing = 1;
                }

                char aux_str[100];

                if(ringing)
                {
                        if(buzzer_duty_cicle == TONE_1)
                        {
                                buzzer_duty_cicle = TONE_2;
                                lcd_backlight_set(fd_lcd,255, 255, 0);
                        }
                        else
                        {
                                buzzer_duty_cicle = TONE_1;
                                lcd_backlight_set(fd_lcd, 255, 0, 0);
                        }

                        snprintf(aux_str, sizeof aux_str, "%d\n", buzzer_duty_cicle);
                        setDutycycle(aux_str, '1');
                }
                else
                {
                        snprintf(aux_str, sizeof aux_str, "%d\n", 0);
                        setDutycycle(aux_str, '1');
                        lcd_backlight_set(fd_lcd, 255, 255, 255);
                }

                usleep(50000);
        }

        close(p_fd);
        pputs("/sys/class/gpio/gpio1/edge","none");

        close(b_pfd.fd);

	return 0;
}