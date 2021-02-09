#include <stdio.h>
#include <stdlib.h>

int main()
{
    char buf[1024];
    int year = 2014, month = 5, day = 13;
    
    sprintf(buf, "%d-%d-%d", year, month, day);
    puts(buf);

    //char str[] = "123a456";
    //printf("%d\n", atoi(str));

    exit(0);
}
