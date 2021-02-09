/*
 * 计算201个数是不是素数
 * 多线程实现
 * 池类算法
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>


#define LEFT    30000000
#define RIGHT   30000200
#define THRNUM  4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

struct thr_arg_st {
    int n;
};

static void *thr_prime(void *p);

int main()
{
    int i, err; 
    pthread_t tid[THRNUM];
    struct thr_arg_st *p;
    void *ptr;

    for (i = 0; i < THRNUM; ++i) {
        p = malloc(sizeof(*p));
        if (p == NULL) {
            perror("malloc()");
            exit(1);
        }
        p->n = i;

        err = pthread_create(tid+i, NULL, thr_prime, p);
        if (err) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }
    
    for (i = LEFT; i <= RIGHT; ++i) {
        pthread_mutex_lock(&mut_num);
        while (num != 0) {
            pthread_mutex_unlock(&mut_num);
            sched_yield(); //出让调度器给别的线程
            pthread_mutex_lock(&mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
    }

    pthread_mutex_lock(&mut_num);
    while (num != 0) {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);

    for (i = 0; i < THRNUM; ++i) {
        pthread_join(tid[i], &ptr);
        free(ptr);
    }

    pthread_mutex_destroy(&mut_num);
    exit(0);
}

static void *thr_prime(void *p)
{
    int i, j, mark, thread_num;
    thread_num = ((struct thr_arg_st *)p)->n;

    while (1) {
        pthread_mutex_lock(&mut_num);
        while(num == 0) {
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        if (num == -1) {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        i = num;
        num = 0;
        pthread_mutex_unlock(&mut_num);

        mark = 1;
        for (j = 2; j < i/2; j++) {
            if (i % j == 0) {
                mark = 0;
                break;
            }
        }
        if (mark)
            printf("[%d]%d is a primer\n", thread_num, i);
    }
    
    pthread_exit(p);
}
