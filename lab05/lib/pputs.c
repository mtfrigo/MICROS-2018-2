#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <galileo2io.h>

int pputs(const char path[],const char s[])
{
    int fd;;
    int n;

    if((fd=open(path,O_WRONLY)) == -1) 
        return -1;
        
    n=write(fd,s,strlen(s));
    close(fd);
    return n;
}
