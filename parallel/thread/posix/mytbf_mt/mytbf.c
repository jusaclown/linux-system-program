#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "mytbf.h"


static struct mytbf_st *job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_t tid_alrm;
static  pthread_once_t init_once = PTHREAD_ONCE_INIT;

struct mytbf_st {
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut;
};

static void *thr_alrm(void *p)
/*
 * 对各个令牌进行累加
 */
{
    int i;

    while(1) {
        pthread_mutex_lock(&mut_job);
        for (i = 0; i < MYTBF_MAX; i++) {
            if (job[i] != NULL) {
                pthread_mutex_lock(&job[i]->mut);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_mutex_unlock(&job[i]->mut);
            }
        }
        pthread_mutex_unlock(&mut_job);
        sleep(1);
    }
}

static void module_unload(void)
/*
 * 原来现场怎样 恢复成咋样
 */
{
    int i;

    pthread_cancel(tid_alrm);
    pthread_join(tid_alrm, NULL);

    for (i = 0; i < MYTBF_MAX; i++) {
        if (job[i] != NULL) {
            mytbf_destory(job[i]);
        }
    }

    pthread_mutex_destroy(&mut_job);
}

static void module_load(void)
{

    int err;

    err = pthread_create(&tid_alrm, NULL, thr_alrm, NULL);
    if (err) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(err));
    }
    atexit(module_unload);
}

static int get_free_pos_unlocked(void)
/*
 * 判断数组中是否有可用位置
 * 如果有 则返回数组下标，否则为-1
 */
{
    int i;

    for (i = 0; i < MYTBF_MAX; i++) {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

mytbf_t *mytbf_init(int cps, int burst)
/*
 * 初始化令牌桶
 */
{
    struct mytbf_st *me;
    int pos;

    pthread_once(&init_once, module_load);

    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;

    me->token = 0;
    me->cps = cps;
    me->burst = burst;
    pthread_mutex_init(&me->mut, NULL);

    pthread_mutex_lock(&mut_job);
    pos = get_free_pos_unlocked();
    if (pos < 0) {
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }
    me->pos = pos;
    job[pos] = me;
    pthread_mutex_unlock(&mut_job);

    return me;
}     

static int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
/*
 *  用户调用词函数获取令牌数量
 *  返回值为可用令牌数
 */
{
    struct mytbf_st *me = ptr;
    int n;

    if (size <= 0)
        return -EINVAL;

    pthread_mutex_lock(&me->mut);
    while (me->token <= 0) {
        pthread_mutex_unlock(&me->mut);
        sched_yield();
        pthread_mutex_lock(&me->mut);
    }

    n = min(me->token, size);
    me->token -= n;
    pthread_mutex_unlock(&me->mut);

    return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
/*
 *  用户调用此函数将多余的令牌返还
 *  返回值为可用令牌数
 */
{
    struct mytbf_st *me = ptr;

    if (size <= 0)
        return -EINVAL;

    pthread_mutex_lock(&me->mut);
    me->token += size;
    if (me->token > me->burst)
        me->token = me->burst;
    pthread_mutex_unlock(&me->mut);

    return size;
}

int mytbf_destory(mytbf_t *ptr)
/*
 * 删除令牌桶
 */
{
    struct mytbf_st *me = ptr;

    pthread_mutex_lock(&mut_job);
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);

    pthread_mutex_destroy(&me->mut);
    free(ptr);

    return 0;
}

