#include <stdio.h>
#include <stdlib.h>

int main()
{
    
    printf("\033[2J\033[1;1H\033[31;4mhello\n\033[0m");     // \033VT控制码的标识
    
    return 0;
}
