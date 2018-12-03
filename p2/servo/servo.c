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

static volatile int run = 1;

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

int main(int argc,char *argv[])
{
        struct sigaction act;
        int p_fd;
        char p_raw_data[80];
        int p_raw;
        int servo_duty_cicle;
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

        int lcd_fd = lcd_init();
        lcd_backlight_init(lcd_fd);
        lcd_backlight_set(lcd_fd, 255, 255, 255);

        p_fd = openADC('0');
        setup_servo('1');

        pputs("/sys/class/gpio/gpio1/edge","falling");

        char lcd_angle[16] = "";
        char lcd_saved_angle[16] = "";
        float saved_angle = 0;
        float rad_saved_angle = 0;

        while(run)
        {
                lseek(p_fd,0,SEEK_SET);
                read(p_fd, p_raw_data, sizeof p_raw_data);
                p_raw = atoi(p_raw_data);
                
                float angle = util_map(p_raw, 0, 4096, -90, 90);
                float rad_angle = (angle * M_PI)/180;

                int b_poll_result = poll(&b_pfd, 1, 1);

                // Se o botao foi clicado
                if(b_poll_result != -1 && b_poll_result != 0)
                {
                        lseek(b_pfd.fd,0,SEEK_SET);
                        read(b_pfd.fd,&b_value,1);

                        saved_angle = angle;
                        rad_saved_angle = rad_angle;
                }

                servo_duty_cicle = (rad_angle/M_PI_2)*900000+1500000;

                if(angle > 0)
                {
                        if(angle > 1.1*saved_angle)
                        {
                                lcd_backlight_set(lcd_fd, 255, 0, 0);
                        }
                        else if(angle < 0.9*saved_angle)
                        {
                                lcd_backlight_set(lcd_fd, 255, 255, 0);
                        }
                        else
                        {
                                lcd_backlight_set(lcd_fd, 0, 255, 0);
                        }
                }
                else
                {
                        if(angle > 0.9*saved_angle)
                        {
                                lcd_backlight_set(lcd_fd, 255, 0, 0);
                        }
                        else if(angle < 1.1*saved_angle)
                        {
                                lcd_backlight_set(lcd_fd, 255, 255, 0);
                        }
                        else
                        {
                                lcd_backlight_set(lcd_fd, 0, 255, 0);
                        }
                }

                char str[100];
                snprintf(str,sizeof str,"%d\n", servo_duty_cicle);
                setDutycycle(str, '1');

                snprintf(lcd_angle, sizeof lcd_angle, "%.2f;%.2f", angle, rad_angle);
                snprintf(lcd_saved_angle, sizeof lcd_angle, "%.2f;%.2f", saved_angle, rad_saved_angle);

                lcd_write_words(lcd_fd, lcd_angle, lcd_saved_angle);

                usleep(50000);
        }

        close(p_fd);
        pputs("/sys/class/gpio/gpio1/edge","none");

        close(b_pfd.fd);

	return 0;
}