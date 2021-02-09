#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#define PAT "/etc/"

int main(void)
{
    DIR *dir;
    struct dirent *data;

    dir = opendir(PAT);
    if (dir == NULL) {
        perror("opendir()");
        exit(1);
    }

    while ((data = readdir(dir)) != NULL) {
        puts(data->d_name);
    }

    closedir(dir);
    exit(0);
}

