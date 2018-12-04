#ifndef ADC_H
#define ADC_H


#include <stdint.h>

/* Assumes the data format for Galileo Gen2 */
struct sensors
{
    uint16_t adc0_raw;		/* be:u12/16>>0 */
    uint16_t adc1_raw;		/* be:u12/16>>0 */
    uint16_t adc2_raw;		/* be:u12/16>>0 */
    uint16_t adc3_raw;		/* be:u12/16>>0 */
    int64_t timestamp;	    /* le:s64/64>>0 */
};

int getRawScale(char *data, char n);
double getScale(char *data);
int openADC(char id);

double adc0_get_avg(struct sensors data[], int length);
double adc1_get_avg(struct sensors data[], int length);

double adc0_get_std_deviation(struct sensors data[], int length, double avg, float scale);
double adc1_get_std_deviation(struct sensors data[], int length, double avg, float scale);

#endif