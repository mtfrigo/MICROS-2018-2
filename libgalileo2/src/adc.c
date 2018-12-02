#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <galileo2io.h>
#include <time.h>
#include <stdio.h>

char *pgets(char *s,int size,const char path[])
{
	int fd;
	
	if((fd=open(path,O_RDONLY)) ==-1) return NULL;
	read(fd,s,size);
	close(fd);
    
	return s;
}