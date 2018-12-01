/*
  pgets.c: Gets a string from a file, given its path.
  
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
#include <unistd.h>
#include <errno.h>

#include <i2cutil.h>
#include <jhdlcd.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <galileo2io.h>

void i2c_error(const char *msg)
{
    perror(msg);
    exit(-errno);
}

int lcd_init()
{
        int fd_lcd;

        if((fd_lcd=open("/dev/i2c-0",O_WRONLY)) < 0) {
                i2c_error("Opening /dev/i2c-0");
                return -1;
        }

        usleep(30000);	/* Wait for 30 ms after power on */

        /* LCD initialization */
        if(ioctl(fd_lcd,I2C_SLAVE,LCD_ADDR) < 0) {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd_lcd,LCD_C0,LCD_FUNCTIONSET | LCD_2LINE);
        usleep(40);  /* Wait for more than 39 us */

        i2c_write_reg(fd_lcd,LCD_C0,LCD_DISPLAYSWITCH | LCD_DISPLAYON |
                LCD_CURSOROFF | LCD_BLINKOFF);
        usleep(40);	/* Wait for more then 39 us */

        i2c_write_reg(fd_lcd,LCD_C0,LCD_SCREENCLEAR);
        usleep(1600);	/* Wait for more then 1.53 ms */

        i2c_write_reg(fd_lcd,LCD_C0,LCD_INPUTSET | LCD_ENTRYLEFT | LCD_DECREMENT);

        return fd_lcd;
}

int backlight_init(int fd_lcd)
{
        /* Backlight initialization */
        if(ioctl(fd_lcd,I2C_SLAVE,BL_ADDR) < 0) {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd_lcd,BL_MODE1,0);
        i2c_write_reg(fd_lcd,BL_LEDOUT,BL_RED_GRPPWM | BL_GREEN_GRPPWM |
                BL_BLUE_GRPPWM);
        i2c_write_reg(fd_lcd,BL_MODE2,BL_DMBLNK);

        i2c_write_reg(fd_lcd,BL_RED,255);	//Escala RGB
        i2c_write_reg(fd_lcd,BL_GREEN,0);
        i2c_write_reg(fd_lcd,BL_BLUE,0);

        return 0;
}

int write_words_lcd(int fd_lcd, char* palavra1, char* palavra2)
{
        int i, palavra1len, palavra2len;

        palavra1len = strlen(palavra1);
        palavra2len = strlen(palavra2);

        if(ioctl(fd_lcd,I2C_SLAVE,LCD_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");


        i2c_write_reg(fd_lcd,LCD_C0,LCD_DDRAMADDRSET | 0);
        if(ioctl(fd_lcd,I2C_SLAVE,LCD_ADDR) < 0) {
                i2c_error("ioctl on /dev/i2c-0");    
                return -1; 
        }
        for(i=0;i < palavra1len;i++) i2c_write_reg(fd_lcd,LCD_RS,palavra1[i]);

        i2c_write_reg(fd_lcd,LCD_C0,LCD_DDRAMADDRSET | 0x40);
        if(ioctl(fd_lcd,I2C_SLAVE,LCD_ADDR) < 0) {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }
        for(i=0;i < palavra2len;i++) i2c_write_reg(fd_lcd,LCD_RS,palavra2[i]);

        return 0;
}