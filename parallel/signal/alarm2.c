/*
 *  最后一个alarm生效
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{

    alarm(10);
    alarm(1);
    alarm(5);

    alarm(15);
    while(1)
        pause();


    exit(0);
}
