#include <stdio.h>
#include <stdlib.h>


int main(void)
{
#pragma omp parallel
    {
        puts("Hello");
        puts("World");
    }

}
