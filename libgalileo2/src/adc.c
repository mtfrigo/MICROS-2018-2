#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <galileo2io.h>
#include <time.h>
#include <stdio.h>
#include <byteswap.h>
#include <stdlib.h>
#include <adc.h>
#include <math.h>


double adc0_get_avg(struct sensors data[], int length)
{
	double avg = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	avg += bswap_16(data[i].adc0_raw);
  	}

  	return avg/length;
}

double adc1_get_avg(struct sensors data[], int length)
{
	double avg = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	avg += bswap_16(data[i].adc1_raw);
  	}

  	return avg/length;
}

double adc0_get_std_deviation(struct sensors data[], int length, double avg, float scale)
{
	double std_deviation = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	std_deviation += (bswap_16(data[i].adc0_raw) * scale - avg * scale) * (bswap_16(data[i].adc0_raw) * scale - avg * scale) ;
  	}

	std_deviation = sqrt(std_deviation/length);

  	return std_deviation;
}

double adc1_get_std_deviation(struct sensors data[], int length, double avg, float scale)
{
	double std_deviation = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	std_deviation += (bswap_16(data[i].adc1_raw) * scale - avg * scale) * (bswap_16(data[i].adc1_raw) * scale - avg * scale) ;
  	}

	std_deviation = sqrt(std_deviation/length);

  	return std_deviation;
}

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