/*
 *  打印时戳
 *  date +%s
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    puts("Begin!");

    fflush(NULL);

    execl("/bin/date", "date", "+%s", NULL);
    perror("execl()");
    exit(1);

    puts("End!");

    exit(0);
}
