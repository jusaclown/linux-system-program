#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main()
{
    pid_t pid;

    
    printf("[%d]Begin!\n", getpid());
    
    /* 
     * 这句话非常重要，在fork之前使用fflush刷新缓冲区！！！ 
     * 如果不刷新，则子进程会把缓冲区也复制一份
     * 那么，在这个程序中Begin会打印两次，切均为父进程的pid
     */
    fflush(NULL);

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    } else if (pid == 0) {  //child
        printf("[%d]:Child is working!\n", getpid());
    } else {                //parent
        printf("[%d]:Parent is working!\n", getpid());
    }

    
    printf("[%d]End!\n", getpid());
    exit(0);
}
