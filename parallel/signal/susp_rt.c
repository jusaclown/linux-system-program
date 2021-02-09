/* 
 * 信号屏蔽 
 *  每行打印5个*，然后暂停等^C
 *  如果在中间收到了^C，那么下一行5个*也会继续打印
 *  
 *  实时信号实现
 *  实时信号不丢失 :
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MYRTSIG     (SIGRTMIN+6)

static void mysig_handler(int s)
{
    write(1, "!", 1);

}

int main()
{
    int i, j;
    sigset_t set, oldset, saveset;

    signal(MYRTSIG, mysig_handler);
    sigemptyset(&set);
    sigaddset(&set, MYRTSIG);
    sigprocmask(SIG_UNBLOCK, &set, &saveset);
    sigprocmask(SIG_BLOCK, &set, &oldset);
    for (j = 0; j < 1000; j++) {
        for (i = 0; i < 5; i++) {
            write(1, "*", 1);
            sleep(1);
        }
        //sigprocmask(SIG_BLOCK, &set, NULL);

        //SIG_SETMASK 恢复到oldset
        write(1, "\n", 1);
        sigsuspend(&oldset);
    }

    sigprocmask(SIG_SETMASK, &saveset, NULL);

    exit(0);
}
