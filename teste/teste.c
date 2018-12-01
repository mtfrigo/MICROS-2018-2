#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <eeprom.h>
#include <i2c.h>
#include <lcd.h>

static volatile int run = 1;

void quit(int signal)
{
        run=0;
}

int main(int argc,char *argv[])
{
        struct sigaction act;

        act.sa_handler = quit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        int fd = lcd_init();
        lcd_backlight_init(fd);

        eeprom_write("My penis in your ass");

        char buffer[EEPROM_LENGTH];

        while(run)
        {
                eeprom_read(buffer, 21);

                lcd_write_words(fd, buffer, "oie");

                lcd_backlight_set(fd, 0, 255, 0);
        }

	return 0;
}


