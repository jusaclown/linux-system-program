#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *fp;
    int count = 0;
    while(1)
    {
        fp = fopen("tmp", "r");
        if(fp == NULL)
        {
            perror("fopen()");  //打印错误信息
            break;
        }
        count++;
    }

    printf("count = %d\n", count);
    exit(0);
}
