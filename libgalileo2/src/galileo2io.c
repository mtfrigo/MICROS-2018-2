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

int pputs(const char path[],const char s[])
{
	int fd;
	int n;
	
	if((fd=open(path,O_WRONLY)) == -1) return -1;
	n=write(fd,s,strlen(s));
	close(fd);

	return n;
}

int *getDate(char str[16])
{
	char buffer[16];
	time_t t;
	time_t t0=0;
	struct tm lt;
	
	while((t=time(NULL)) == t0);
	t0=t;
	localtime_r(&t,&lt);

	strftime(buffer,sizeof buffer,"%a %d %b %Y",&lt);

	strncpy(str, buffer, 16);
	
	return 0;
}

int *getTime(char str[16])
{

	char buffer[16];
	time_t t;
	time_t t0=0;
	struct tm lt;

	while((t=time(NULL)) == t0);
	t0=t;
	localtime_r(&t,&lt);

	strftime(buffer,sizeof buffer,"%T %Z", &lt);

	strncpy(str, buffer, 16);
	
	return 0;
}