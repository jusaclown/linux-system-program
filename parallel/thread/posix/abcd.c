/*
 *  4个线程 分别打印a b c d
 *  要求 输出结果为 abcdabcdabcd...
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM      4

static pthread_mutex_t mut[THRNUM];

static int next (int n)
{
    if (n + 1 == THRNUM)
        return 0;
    return n+1;
}

static void *thr_func(void *p)
{
    int n = (int)p;
    int c = 'a' + n;

    while(1) {
        pthread_mutex_lock(mut+n);
        write(1, &c, 1);
        pthread_mutex_unlock(mut+next(n));
    }

    pthread_exit(NULL);
}

int main()
{
    int i, err;
    pthread_t tid[THRNUM];

    for (i = 0; i < THRNUM; ++i) {
        pthread_mutex_init(mut+i, NULL);
        pthread_mutex_lock(mut+i);

        err = pthread_create(tid+i, NULL, thr_func, (void *)i);
        if (err) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }
    pthread_mutex_unlock(mut);

    alarm(5);
    
    for (i = 0; i < THRNUM; ++i) {
        pthread_join(tid[i], NULL);
    }

    exit(0);
}
