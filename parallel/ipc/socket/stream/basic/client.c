#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "proto.h"

int main(int argc, char *argv[])
{
    int sd;
    struct sockaddr_in raddr;
    long long stamp;
    FILE *fp;
    if (argc < 2) {
        fprintf(stderr, "Usage ./client [ip] [port]\n");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    //bind();

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT/*argv[2]*/));
    inet_pton(AF_INET, argv[1], &raddr.sin_addr);
    if (connect(sd, (void *)&raddr, sizeof(raddr)) < 0) {
        perror("connect()");
        exit(1);
    }

    fp = fdopen(sd, "r+");
    if (fp == NULL) {
        perror("fdopen()");
        exit(1);
    }

    if (fscanf(fp, FMT_STAMP, &stamp) < 1) {
        fprintf(stderr, "Bad format!\n");
    } else {
        fprintf(stdout, "stamp = %lld\n", stamp);
    }
    fclose(fp);

    //recv();
    //close();

    exit(0);
}