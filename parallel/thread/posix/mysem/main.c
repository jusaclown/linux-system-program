/*
 * 计算201个数是不是素数
 * 同一时刻只能产生N个线程
 * 信号量
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#include "mysem.h"

#define LEFT    30000000
#define RIGHT   30000200
#define THRNUM  (RIGHT-LEFT+1)
#define N       4

static void *thr_prime(void *p);
static mysem_t *sem;

int main()
{
    int i, err; 
    pthread_t tid[THRNUM];


    sem = mysem_init(N);
    if (sem == NULL) {
        fprintf(stderr, "init() failed!\n");
        exit(1);
    }

    for (i = LEFT; i <= RIGHT; ++i) {
        mysem_sub(sem, 1);
        err = pthread_create(tid+i-LEFT, NULL, thr_prime, (void *)i);
        if (err) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }


    for (i = LEFT; i <= RIGHT; ++i) {
        pthread_join(tid[i-LEFT], NULL);
    }

    mysem_add(sem, 1);
    mysem_destroy(sem);

    exit(0);
}

static void *thr_prime(void *p)
{
    int i, j, mark;
    i = (int)p;

    mark = 1;
    for (j = 2; j < i/2; j++) {
        if (i % j == 0) {
            mark = 0;
            break;
        }
    }

    if (mark)
        printf("%d is a primer\n", i);
//    sleep(10);
    mysem_add(sem, 1);
    pthread_exit(NULL);
}
