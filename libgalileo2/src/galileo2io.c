#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <galileo2io.h>

char *pgets(char *s,int size,const char path[])
{
	int fd;
	
	if((fd=open(path,O_RDONLY)) ==-1) return NULL;
	read(fd,s,size);
	close(fd);
    
	return s;
}

int pputs(const char path[],const char s[])
{
	int fd;
	int n;
	
	if((fd=open(path,O_WRONLY)) == -1) return -1;
	n=write(fd,s,strlen(s));
	close(fd);

	return n;
}