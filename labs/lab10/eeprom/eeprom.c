#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <galileo2io.h>
#include <eeprom.h>

int main(int argc,char *argv[])
{
	char s[EEPROM_LENGTH];
	
    eeprom_write(argv[1]);

    eeprom_read(s, strlen(argv[1]));

    printf("%s \n", s);

	return 0;
}