#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


static void cleanup_func(void *p)
{
    puts(p);
}

static void *func(void *p)
{
    puts("Thread is working!");

    pthread_cleanup_push(cleanup_func, "cleanup:1");
    pthread_cleanup_push(cleanup_func, "cleanup:2");
    pthread_cleanup_push(cleanup_func, "cleanup:3");

    puts("push over!");

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int err;

    puts("Begin!\n");

    err = pthread_create(&tid, NULL, func, NULL);
    if (err) {
        fprintf(stderr, "pthread_create(): %s\n", strerror(err));
    }


    pthread_join(tid, NULL);
    puts("End!\n");

    exit(0);
}
