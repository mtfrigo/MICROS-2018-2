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

int adc_cont_setup(int data_points, double sampling_period)
{
	int fd;
	char data_str[80];
	
	// Desliga o buffer para fazer a configuração
	pputs("/sys/bus/iio/devices/iio:device0/buffer/enable", "0");

	// Dá enable pro modo contínuo
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en", "1");

	snprintf(data_str, sizeof data_str, "%d", data_points);
	pputs("/sys/bus/iio/devices/iio:device0/buffer/length", data_str);

	// Indica o trigger
	pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/trigger0/name");
	pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger", data_str);

	// Frequência de trigger é o inverso do sampling period programado
	snprintf(data_str, sizeof data_str, "%d", (int)round(1.0/sampling_period));
	pputs("/sys/bus/iio/devices/trigger0/frequency", data_str);

	// Abre o arquivo que contém as amostras
	if((fd = open("/dev/iio:device0",O_RDONLY)) < 0)
	{
		perror("Opening /dev/iio:device0:");
		return -1;
	}

	return fd;
}

void adc_cont_sample(int data_points, double sampling_period)
{
	// Dá enable no buffer
    pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","1");
    
    // Fica off para realizar a leitura
    usleep(data_points * sampling_period * 1000000);
    
    // Desliga o buffer
    pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","0");
}

void adc0_cont_enable()
{
	// Habilita adc0
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage0_en", "1");
}

void adc1_cont_enable()
{
	// Habilita adc1
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage1_en", "1");
}

double adc0_cont_get_scale()
{
	char data_str[80];
	double scale;

	pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
	scale = atof(data_str)/1000.0;

	return scale;
}

double adc1_cont_get_scale()
{
	char data_str[80];
	double scale;

	pgets(data_str, sizeof data_str, "/sys/bus/iio/devices/iio:device0/in_voltage1_scale");
	scale = atof(data_str)/1000.0;

	return scale;
}

double adc0_cont_get_avg(struct adc_sensors data[], int length)
{
	double avg = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	avg += bswap_16(data[i].adc0_raw);
  	}

  	return avg/length;
}

double adc1_cont_get_avg(struct adc_sensors data[], int length)
{
	double avg = 0;
	int i;

	for(i=0;i < length; i++) 
	{
    	avg += bswap_16(data[i].adc1_raw);
  	}

  	return avg/length;
}

double adc0_cont_get_std_deviation(struct adc_sensors data[], int length, double avg, float scale)
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

double adc1_cont_get_std_deviation(struct adc_sensors data[], int length, double avg, float scale)
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

void adc0_cont_disable()
{
	// Desabilita canal 0
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage0_en","0");
}

void adc1_cont_disable()
{
	// Desabilita canal 1
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage1_en","0");
}

void adc_cont_disable()
{
	// Desliga o trigger
	pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger","\n");
	// Reseta o buffer
	pputs("/sys/bus/iio/devices/iio:device0/buffer/length","2");
	// Reseta a timestamp
	pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en","0");
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