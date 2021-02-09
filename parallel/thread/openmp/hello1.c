#include <stdio.h>
#include <stdlib.h>


int main(void)
{
#pragma omp parallel sections
    {
        
        puts("Hello");
        puts("World");
    }

}
