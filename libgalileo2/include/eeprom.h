#ifndef EEPROM_H
#define EEPROM_H

#define EEPROM_LENGTH 1024

int eeprom_write(char *data);
int eeprom_read(char data[EEPROM_LENGTH], int length);

#endif