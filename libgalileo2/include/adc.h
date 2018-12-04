#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// One-shot
int getRawScale(char *data, char n);
double getScale(char *data);
int openADC(char id);

// Continuous

// Assumes the data format for Galileo Gen2
struct adc_sensors
{
    uint16_t adc0_raw; // be:u12/16>>0
    uint16_t adc1_raw; // be:u12/16>>0
    uint16_t adc2_raw; // be:u12/16>>0
    uint16_t adc3_raw; // be:u12/16>>0
    int64_t timestamp; // le:s64/64>>0
};

int adc_cont_setup(int data_points, double sampling_period);

void adc_cont_sample(int data_points, double sampling_period);

void adc0_cont_enable();
void adc1_cont_enable();

double adc0_cont_get_scale();
double adc1_cont_get_scale();

double adc0_cont_get_avg(struct adc_sensors data[], int length);
double adc1_cont_get_avg(struct adc_sensors data[], int length);

double adc0_cont_get_std_deviation(struct adc_sensors data[], int length, double avg, float scale);
double adc1_cont_get_std_deviation(struct adc_sensors data[], int length, double avg, float scale);

void adc0_cont_disable();
void adc1_cont_disable();

void adc_cont_disable();

#endif