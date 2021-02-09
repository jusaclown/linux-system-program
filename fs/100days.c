/*
 * 计算100天以后是哪一天
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFSIZE 1024

int main()
{   
    time_t stamp;
    struct tm *tm;
    char buf[BUFFSIZE];


    stamp = time(NULL);
    tm = localtime(&stamp);
    strftime(buf, BUFFSIZE - 1, "Now: %F", tm);
    puts(buf);

    tm->tm_mday += 100;
    (void)mktime(tm);
    strftime(buf, BUFFSIZE - 1, "100 days later: %F", tm);
    puts(buf);

    exit(0);
}
