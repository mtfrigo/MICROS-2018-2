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


int main(int argc,char *argv[])
{
        struct sigaction act;
        unsigned char b_value;
        struct pollfd b_pfd;

        char state='0';
        int fd_led;

        if((b_pfd.fd=open("/sys/class/gpio/gpio1/value",O_RDONLY)) < 0)
        {
                perror("Opening gpio1:");
                return -1;
        }

        if((fd_led=open("/sys/class/gpio/gpio61/value",O_WRONLY)) < 0)
        {
                perror("Opening gpio61:");
                return -1;
        }

        read(b_pfd.fd,&b_value,1);
        b_pfd.events = POLLPRI;

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        pputs("/sys/class/gpio/gpio1/edge","falling");

        while(run)
        {
                int b_poll_result = poll(&b_pfd, 1, 1);

                // Se o botao foi clicado
                if(b_poll_result != -1 && b_poll_result != 0)
                {

                        lseek(b_pfd.fd,0,SEEK_SET);
                        read(b_pfd.fd,&b_value,1);

                        printf("botao apertado!\n");

                        state^='0'^'1';
                }

                lseek(fd_led, 0, SEEK_SET);
                write(fd_led, &state,sizeof state);
                

                usleep(50000);
        }

        close(fd_led);
        pputs("/sys/class/gpio/gpio1/edge","none");

        close(b_pfd.fd);

	return 0;
}