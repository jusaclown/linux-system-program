/*
 * 缓冲区的作用：大多数情况下是好事，合并系统调用
 * 
 * 行缓冲：换行的时候刷新，满了的时候刷新，强制刷新（标准输出是这样的，因为
 * 是终端设备）
 * 
 * 全缓冲：满了的时候刷新，强制刷新（默认，除了终端设备）
 *
 * 无缓冲：如stderr，需要立即输出的内容
 * 
 *  setvbuf：可以用来改变缓冲模式
 */


#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;
    
    printf("Before while()");   
    fflush(stdout);
    // 注意：如果不加'\n',那么这句话不会打印出来
    //printf("Before while()\n");

    while(1);

    printf("After while()");
    fflush(NULL);
    exit(0);
}
