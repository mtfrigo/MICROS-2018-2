#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#include <galileo2io.h>

int main(int argc,char * argv[])
{
    unsigned char c;
    struct pollfd pfd;

    if((pfd.fd=open("/sys/class/gpio/gpio6/value",O_RDONLY)) < 0)
    {
        perror("Opening gpio6:");
        return -1;
    }

    /* Clear old values */
    read(pfd.fd,&c,1);

    pfd.events = POLLPRI;

    puts("Waiting for interrupt...");

    pputs("/sys/class/gpio/gpio6/edge","both");

    poll(&pfd,1,-1);

    lseek(pfd.fd,0,SEEK_SET);
    read(pfd.fd,&c,1);

    pputs("/sys/class/gpio/gpio6/edge","none");

    close(pfd.fd);
    return 0;
}
