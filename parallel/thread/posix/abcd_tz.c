/*
 *  4个线程 分别打印a b c d
 *  要求 输出结果为 abcdabcdabcd...
 *  通知法实现
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM      4

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

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
        pthread_mutex_lock(&mut);
        while (num != n)
            pthread_cond_wait(&cond, &mut);
        write(1, &c, 1);
        num = next(num);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

int main()
{
    int i, err;
    pthread_t tid[THRNUM];

    for (i = 0; i < THRNUM; ++i) {
        err = pthread_create(tid+i, NULL, thr_func, (void *)i);
        if (err) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }

    alarm(5);
    
    for (i = 0; i < THRNUM; ++i) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
    exit(0);
}
