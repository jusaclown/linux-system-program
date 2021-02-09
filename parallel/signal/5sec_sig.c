#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// volatile 去到真正存放这个值的地址取值，不要轻信内存空间的值
static volatile int loop = 1;

static void alrm_handler(int s)
{
    loop = 0;
}

int main()
{
    int64_t count = 0;

    //signal 在alarm之前使用
    signal(SIGALRM, alrm_handler);
    alarm(5);

    while (loop)
        ++count;

    printf("%lld\n", count);

    exit(0);
}
