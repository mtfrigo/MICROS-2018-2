#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <i2c.h>
#include <lcd.h>

static volatile int run = 1;


void quit(int signal)
{
        run=0;
}

int main(int argc,char *argv[])
{
        int fd_lcd;
        struct sigaction act;

        act.sa_handler=quit;
        sigaction(SIGINT,&act,NULL);
        sigaction(SIGTERM,&act,NULL);

        while(run)
        {
        }

	return 0;
}


