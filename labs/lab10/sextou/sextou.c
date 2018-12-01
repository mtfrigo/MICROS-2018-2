/*
  clocklcd.c: Clock in a I2C LCD with RGB backlight

  Copyright (c) 2016 Walter Fetter Lages <w.fetter@ieee.org>
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    You can also obtain a copy of the GNU General Public License
    at <http://www.gnu.org/licenses>.
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include <i2c.h>
#include <jhdlcd.h>
#include <string.h>

#include <lcd.h>

static volatile int run=1;


void quit(int signal)
{
        run=0;
}

int main(int argc,char *argv[])
{
        int fd_lcd;
        struct sigaction act;
        char palavra1[16];
        char palavra2[16];
        //Acesso fácil á interface I2C


        if(argc != 3)
    	{
    		printf("writelcd\n");
    		printf("\tUsage:\t%s <string>\t%s <string>\n",argv[0], argv[1]);
    		return -1;
    	}


        

        fd_lcd = lcd_init();

        lcd_backlight_init(fd_lcd);
        

        act.sa_handler=quit;
        sigaction(SIGINT,&act,NULL);
        sigaction(SIGTERM,&act,NULL);

        strcpy(palavra1, argv[1]);
        strcpy(palavra2, argv[2]);

        lcd_write_words(fd_lcd, palavra1, palavra2);

        while(run)
        {
                lcd_backlight_set(fd_lcd, rand(), rand(), rand());
                usleep(990000);
        }

        close(fd_lcd);

	return 0;
}


