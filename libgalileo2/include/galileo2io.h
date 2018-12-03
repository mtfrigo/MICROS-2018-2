#ifndef GALILEO2IO_H
#define GALILEO2IO_H

char *pgets(char *s,int size,const char path[]);
int pputs(const char path[],const char s[]);
int *getDate(char str[]);
int *getTime(char str[], int fix);

#endif