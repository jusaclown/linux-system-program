/*
 *  实现cat功能
 *  加入信号机制
 *  每秒显示10个字符
 *  积攒权限（输出字符）
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define CPS      10
#define BUFFSIZE CPS
#define BURST    100

//sig_atomic_t 信号原子类型，这个类型的取值和赋值操作一条指令完成
static volatile sig_atomic_t token = 0;

static void alrm_handler(int s)
{
    alarm(1);
    ++token;
    if (token > BURST)
        token = BURST;
}

int main(int argc, char **argv)
{
    int sfd, dfd = 1;
    char buf[BUFFSIZE];
    int len, ret, pos;

    if(argc != 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    

    signal(SIGALRM, alrm_handler);
    alarm(1);

    do {
        sfd = open(argv[1], O_RDONLY);
        if(sfd < 0)
        {
            if (errno != EINTR) {
                perror("sfd_open()");
                exit(1);
            }
        }
    } while (sfd < 0);

    while(1)
    {
        while(token <= 0)
            pause();
        --token;

        while ((len = read(sfd, buf, BUFFSIZE)) < 0) {
            if (errno == EINTR)
                continue;
            perror("read()");
            break;
        } 
        if (len == 0) 
            break;

        pos = 0;
        while(len > 0)
        {
            ret = write(dfd, buf + pos, len);
            if(ret < 0)
            {
                if (errno == EINTR)
                    continue;
                perror("write()");
                exit(1);
            }
            pos += ret;
            len -= ret;
         }
        //sleep(1);
    }

    close(sfd);

    exit(0);
}
