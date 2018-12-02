#ifndef ADC_H
#define ADC_H


int getRawScale(char *data, char n);
double getScale(char *data);
int openADC(char id);

#endif