#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <i2c.h>
#include <lcd.h>
#include <jhdlcd.h>
#include <galileo2io.h>

int lcd_init()
{
        int fd;

        if((fd=open("/dev/i2c-0", O_WRONLY)) < 0)
        {
                i2c_error("Opening /dev/i2c-0");
                return -1;
        }

        usleep(30000);	/* Wait for 30 ms after power on */

        if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0)
        {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd, LCD_C0, LCD_FUNCTIONSET | LCD_2LINE);
        usleep(40); // Wait for more than 39 us

        i2c_write_reg(fd,LCD_C0,LCD_DISPLAYSWITCH | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
        usleep(40); // Wait for more then 39 us

        i2c_write_reg(fd,LCD_C0,LCD_SCREENCLEAR);
        usleep(1600); // Wait for more then 1.53 ms

        i2c_write_reg(fd, LCD_C0, LCD_INPUTSET | LCD_ENTRYLEFT | LCD_DECREMENT);

        return fd;
}

int lcd_backlight_init(int fd)
{
        if(ioctl(fd, I2C_SLAVE, BL_ADDR) < 0)
        {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd, BL_MODE1, 0);
        i2c_write_reg(fd, BL_LEDOUT, BL_RED_GRPPWM | BL_GREEN_GRPPWM | BL_BLUE_GRPPWM);
        i2c_write_reg(fd, BL_MODE2, BL_DMBLNK);

        i2c_write_reg(fd, BL_RED, 255);
        i2c_write_reg(fd, BL_GREEN, 0);
        i2c_write_reg(fd, BL_BLUE, 0);

        return 0;
}

int lcd_write_words(int fd, char *word1, char *word2)
{
        int i, word1_len, word2_len;

        word1_len = strlen(word1);
        word2_len = strlen(word2);

        if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0)
        {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd, LCD_C0, LCD_DDRAMADDRSET | 0);

        if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0) {
                i2c_error("ioctl on /dev/i2c-0");    
                return -1; 
        }

        for(i=0;i < word1_len;i++) i2c_write_reg(fd, LCD_RS, word1[i]);

        i2c_write_reg(fd, LCD_C0,LCD_DDRAMADDRSET | 0x40);
        
        if(ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0)
        {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }
        
        for(i=0;i < word2_len;i++) i2c_write_reg(fd, LCD_RS, word2[i]);

        return 0;
}

int lcd_backlight_set(int fd, int red, int blue, int green)
{
        if(ioctl(fd, I2C_SLAVE, BL_ADDR) < 0)
        {
                i2c_error("ioctl on /dev/i2c-0");
                return -1;
        }

        i2c_write_reg(fd, BL_RED, red);
        i2c_write_reg(fd, BL_GREEN, blue);
        i2c_write_reg(fd, BL_BLUE, green);

        return 0;
}