#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "anytimer.h"

static void f2(void *p)
{
    printf("f2(): %s\n", (char *)p);
}

static void f1(void *p)
{
    printf("f1(): %s\n", (char *)p);
}

int main()
{
    int job1, job2, job3;
    puts("Begin!");

    job1 = at_addjob(5, f1, "aaa");
    if (job1 < 0) {
        fprintf(stderr, "at_addjob(1): %s\n", strerror(-job1));
    }

    job2 = at_addjob(2, f2, "bbb");
    if (job2 < 0) {
        fprintf(stderr, "at_addjob(2): %s\n", strerror(-job1));
    }

    job3 = at_addjob(7, f1, "ccc");
    if (job3 < 0) {
        fprintf(stderr, "at_addjob(3): %s\n", strerror(-job1));
    }

    puts("End!");

    while(1) {
        write(1, ".", 1);
        pause();
        at_waitjob(job1);
        at_waitjob(job2);
        at_waitjob(job3);
//        sleep(1);
    }
        

    exit(0);
}
