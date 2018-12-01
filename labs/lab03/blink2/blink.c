#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    char state=’0’;
    int fd;

    fd=open("/sys/class/gpio/gpio62/value",O_WRONLY);

    for(;;)
    {
        lseek(fd,0,SEEK_SET);
        write(fd,&state,sizeof state);
        sleep(1);
        state^=’0’^’1’;
    }

    close(fd);

    return 0;
}