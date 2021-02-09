/*
 *  用3个子进程来计算201个数是不是素数
 *  交叉分配
 *  用wait收尸
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT    30000000
#define RIGHT   30000200
#define N       3


int main()
{
    int i, j, n, mark;
    pid_t pid;

    for (n = 0; n < N; n++) {
        pid = fork();
        if (pid < 0) {
            perror("fork()");
            exit(0);
        } else if (pid == 0) {
            for (i = LEFT + n; i <= RIGHT; i+=N) {
                mark = 1;
                for (j = 2; j < i/2; j++) {
                    if (i % j == 0) {
                        mark = 0;
                        break;
                    }
                } 
                if (mark)
                    printf("[%d]%d is a primer\n", n, i);
            }
            exit(0);
        }
    }

    for (i = 0; i < N; i++) {
        wait(NULL);
    }

    exit(0);
}
