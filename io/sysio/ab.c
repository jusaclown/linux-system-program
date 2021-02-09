/*
 * 验证标准IO和系统IO不能一起使用
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    putchar('a');
    write(1, "b", 1);

    putchar('a');
    write(1, "b", 1);

    putchar('a');
    write(1, "b", 1);
    
    exit(0);
}
