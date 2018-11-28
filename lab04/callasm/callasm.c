#include <stdio.h>
#include <stdlib.h>
#include <asmops.h>
int main(int argc,char *argv[])
{
    int a;
    int b;
    int s;
    int d;
    if(argc != 3)
    {
        printf("Usage:\n\t%s <int> <int>\n",argv[0]);
        return -1;
    }
    a=atoi(argv[1]);
    b=atoi(argv[2]);
    s=sum(a,b);
    d=diff(&a,&b);

    printf("%d+%d=%d\n%d-%d=%d\n",a,b,s,a,b,d);
    
    return 0;
}