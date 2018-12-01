#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <i2c.h>

int i2c_write_reg(int fd,unsigned char reg,unsigned char data)
{
    unsigned char buf[]={reg,data};

    return write(fd,buf,sizeof buf);
}

void i2c_error(const char *msg)
{
    perror(msg);
    exit(-errno);
}