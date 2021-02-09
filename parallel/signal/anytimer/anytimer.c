#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#include "anytimer.h"

static struct at_jobs *jobs[JOB_MAX];
static struct sigaction alrm_sa_save;
static int inited = 0;

struct at_jobs {
    int sec;
    at_jonfunc_t *f;
    char *arg;
    int flag;
};

static void alrm_hander(int s, siginfo_t *infop, void *unused)
{
    int i;

    if(infop->si_code != SI_KERNEL)
        return;

    for (i = 0; i < JOB_MAX; ++i) {
        if (jobs[i] != NULL) {
            if ((--jobs[i]->sec) <= 0) {
                jobs[i]->f(jobs[i]->arg);
                jobs[i]->flag = 0;
            }
        }
    }
}

static void module_unload(void)
{
    int i;
    struct itimerval itv;

    sigaction(SIGALRM, &alrm_sa_save, NULL);

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);

    for (i = 0; i < JOB_MAX; ++i)
        free(jobs[i]);
}

static void module_load(void)
{
    struct sigaction sa;
    struct itimerval itv;

    sa.sa_sigaction = alrm_hander;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &sa, &alrm_sa_save);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);

    atexit(module_unload);
}

static int get_free_pos()
{
    int i;

    for (i = 0; i < JOB_MAX; i++) {
        if (jobs[i] == NULL)
            return i;
    }
    return -1;
}

int at_addjob(int sec, at_jonfunc_t *jobp, void *arg)
{
    struct at_jobs *me;
    int pos;

    if (!inited) {
        module_load();
        inited = 1;
    }

    if (sec < 0)
        return -EINVAL;

    if ((pos = get_free_pos()) < 0)
        return -ENOSPC;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return -ENOMEM;

    me->sec = sec;
    me->f = jobp;
    me->arg = arg;
    me->flag = 1;

    jobs[pos] = me;
    printf("job[%d] is added!\n", pos);

    return pos;
}


int at_canceljob(int id)
{
    if (id < 0 || id >= JOB_MAX || jobs[id] == NULL)
        return -EINVAL;
    jobs[id] = NULL;

    return 0;
}


int at_waitjob(int id)
{
    if (jobs[id] != NULL && jobs[id]->flag == 0) {
        jobs[id] = NULL;
    }
    return 0;
}