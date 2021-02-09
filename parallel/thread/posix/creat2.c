#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

static void *func(void *p) 
{
    while (1)
        pause();
    pthread_exit(NULL);
}

int main()
{
    int i, err;
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024*1024);

    for (i = 0; ; i++) {
        err = pthread_create(&tid, &attr, func, NULL);
//        printf("%d\n", i);
        if (err) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            break;
        }
    }

    printf("End: %d!\n", i);

    pthread_attr_destroy(&attr);

    exit(0);
}
