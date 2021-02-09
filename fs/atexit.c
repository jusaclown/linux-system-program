/*
 *  钩子函数
 *
 */

#include <stdio.h>
#include <stdlib.h>

static void f1(void)
{
    puts("f1() is working!");
}


static void f2(void)
{
    puts("f2() is working!");
}

static void f3(void)
{
    puts("f3() is working!");
}


int main()
{


    puts("Begin!");

    atexit(f1);
    atexit(f2);
    atexit(f3);

    puts("End");


    exit(0);
}
