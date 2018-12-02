#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <galileo2io.h>
#include <time.h>
#include <stdio.h>


int setPeriod(char *period)
{
	pputs("/sys/class/pwm/pwmchip0/device/pwm_period", period);

	return 0;
}

int setDutycycle(char *dc, char n)
{

	if(n == '1')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", dc);
	}
	else if(n == '3')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm3/duty_cycle", dc);
	}
	else if(n == '5')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm5/duty_cycle", dc);
	}
	else if(n == '7')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm7/duty_cycle", dc);
	}
	else if(n == '9')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm9/duty_cycle", dc);
	}
	

	return 0;

}

int enablePWM(char n)
{
	if(n == '1')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm1/enable","1");
	}
	else if(n == '3')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm3/enable","1");
	}
	else if(n == '5')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm5/enable","1");
	}
	else if(n == '7')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm7/enable","1");
	}
	else if(n == '9')
	{
        pputs("/sys/class/pwm/pwmchip0/pwm9/enable","1");
	}

	return 0;
}