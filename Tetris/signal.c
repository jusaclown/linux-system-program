#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void int_handler(int s)
{
    printf("Get SIGINT\n");
}


int main()
{

    signal(SIGINT, int_handler);
    while (1) {
        printf("while(1)");
        sleep(1);
    }

    exit(1);


}
