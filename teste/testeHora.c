#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <eeprom.h>
#include <i2c.h>
#include <lcd.h>
#include <time.h>
#include <galileo2io.h>

static volatile int run = 1;

void quit(int signal)
{
        run=0;
}

int main(int argc,char *argv[])
{
        struct sigaction act;
        
        char palavra1[16], palavra2[16];

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        int fd = lcd_init();
        lcd_backlight_init(fd);

        while(run){

                getDate(palavra1);
                getTime(palavra2);      

                lcd_write_words(fd, argv[1], palavra2);

                lcd_backlight_set(fd, rand(), rand(), rand());

                sleep(1);
        }

        

	return 0;
}


