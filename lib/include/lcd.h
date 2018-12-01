#ifndef LCD_H
#define LCD_H

int lcd_init();
int lcd_backlight_init(int fd);
int lcd_write_words(int fd_lcd, char* word1, char* word2);
int lcd_backlight_set(int fd, int red, int blue, int green);

#endif