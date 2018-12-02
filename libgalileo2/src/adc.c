#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <galileo2io.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

double getScale(char *data)
{
	return atof(data)/1000.0;
}

int getRawScale(char *data, char n)
{
	char buffer[80];
	char path[50];

	if(n == '0')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
	}
	else if(n == '1')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage1_scale");
	}
	else if(n == '2')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage2_scale");
	}
	else if(n == '3')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage3_scale");
	}
	else if(n == '4')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage4_scale");
	}

	pgets(buffer,sizeof buffer, path);

    strncpy(data, buffer, sizeof buffer);

    return 0;
}

int openADC(char n)
{

	// Expected path for ADC0 = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
	// Expected path for ADC1 = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
	// Expected path for ADC2 = "/sys/bus/iio/devices/iio:device0/in_voltage2_raw";
	// Expected path for ADC3 = "/sys/bus/iio/devices/iio:device0/in_voltage3_raw";
	// Expected path for ADC4 = "/sys/bus/iio/devices/iio:device0/in_voltage4_raw";
	
	int fd;

	char path[48];

	if(n == '0')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage0_raw");
	}
	else if(n == '1')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage1_raw");
	}
	else if(n == '2')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage2_raw");
	}
	else if(n == '3')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage3_raw");
	}
	else if(n == '4')
	{
		strcpy(path, "/sys/bus/iio/devices/iio:device0/in_voltage4_raw");
	}

	if((fd=open(path, O_RDONLY)) < 0)
	{
			perror("Opening in_voltage0raw:");
			return -1;
	}

	return fd;
}