#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFSIZE     1024

int main(int argc, char *argv[])
{
    int sd;
    int len;
    struct sockaddr_in raddr;
    long long stamp;
    FILE *fp;
    char rbuf[BUFSIZE];

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
    raddr.sin_port = htons(80);
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

    fprintf(fp, "GET /text.jpg\r\n\r\n");
    fflush(fp);
    while (1) {
        len = fread(rbuf, 1, BUFSIZE, fp);
        while (len <= 0)
            break;
        fwrite(rbuf, 1, len, stdout);
    }

    exit(0);
}
