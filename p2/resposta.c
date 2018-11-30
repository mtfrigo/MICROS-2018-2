#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include <string.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include <galileo2io.h>
#include <i2cutil.h>
#include <jhdlcd.h>

static volatile int run = 1;

void quit(int signal){
    run = 0;
}

static void i2c_error(const char * msg){
    perror(msg);
    exit(-errno);
}

int main(int argc, char * argv[])
{
    int duty_cycle;
    double angle;
    double angleref = 0;
    char str[100], str2[100];
    int fd1;
    char raw_str[80];

    int raw;
    int fd;
    int i;
    int button_fd;
    char s;

    int n;

    struct sigaction act;

    /* Pull on IO18 */
    pputs("/sys/class/gpio/export", "57");
    pputs("/sys/class/gpio/gpio57/direction", "out");
    pputs("/sys/class/gpio/gpio57/value", "1");

    /* Select I2C on IO18 and IO19 */
    pputs("/sys/class/gpio/export", "60");
    pputs("/sys/class/gpio/gpio60/direction", "out");
    pputs("/sys/class/gpio/gpio60/value", "0");

    /* Pull up IO19 */
    pputs("/sys/class/gpio/export", "59");
    pputs("/sys/class/gpio/gpio59/direction", "out");
    pputs("/sys/class/gpio/gpio59/value", "1");

    //
    if((fd = open("/dev/i2c-0", O_WRONLY)))
}