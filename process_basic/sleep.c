#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main()
{
    pid_t pid;

    puts("Begin!");

    fflush(NULL);

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(0);
    } else if (pid == 0) {
        execl("/bin/sleep", "sleep", "10", NULL);
        perror("execl()");
        exit(1);
    }   

    wait(NULL);

    puts("End!");

    exit(0);
}
