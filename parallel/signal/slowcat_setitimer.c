/*
 *  实现cat功能
 *  加入信号机制
 *  
 *  使用setitimer()实现
 *  settimer工作机制是，先对it_value倒计时，当it_value为零时触发信号，
 *  然后重置为it_interval，继续对it_value倒计时，一直这样循环下去
 *  
 *  假如it_value为0是不会触发信号的，所以要能触发信号，it_value得大于0；
 *  如果it_interval为零，只会延时，不会定时（也就是说只会触发一次信号)。
 *
 *  显示效果：开始时10s后打印10个字符 以后每秒打印10个字符
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>


#define CPS      10
#define BUFFSIZE CPS

static volatile int loop = 0;

static void alrm_handler(int s)
{
    //alarm(1);
    loop = 1;
}

int main(int argc, char **argv)
{
    int sfd, dfd = 1;
    char buf[BUFFSIZE];
    int len, ret, pos;
    struct itimerval itv;

    if(argc != 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    

    signal(SIGALRM, alrm_handler);
    //alarm(1);
    
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 10;
    itv.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
        perror("setitimer()");
        exit(1);
    }

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
        while(!loop)
            pause();
        loop = 0;


        while ((len = read(sfd, buf, BUFFSIZE)) < 0) {
            if (errno == EINTR)
                continue;
            perror("read()");
            //如果出错 只能跳出内部这个循环
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
