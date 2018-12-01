#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <galileo2io.h>
#include <eeprom.h>

int eeprom_write(char *data)
{
    if(pputs("/sys/bus/i2c/devices/0-0054/eeprom", data) < 0)
	{
		perror("EEPROM write error");

		return errno;
	}

    return 0;
}

int eeprom_read(char data[EEPROM_LENGTH], int length)
{
    char buffer[EEPROM_LENGTH + 1];

    if(pgets(buffer, sizeof buffer, "/sys/bus/i2c/devices/0-0054/eeprom") < 0)
    {
        perror("EEPROM read error");

        return errno;
	}

    strncpy(data, buffer, length);

    data[length] = '\0';

    return 0;
}