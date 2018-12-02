#ifndef PWM_H
#define PWM_H


int setPeriod(char *period);
int setDutycycle(char *dc, char n);
int enablePWM(char n);

#endif