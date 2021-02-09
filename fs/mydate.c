/*
 *  -y: year 2020或者20
 *  -m: month
 *  -d: day
 *  -H: hour 12小时制或24小时制
 *  -M: minute
 *  -S: second
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1024

int main(int argc, char **argv)
{   
    time_t stamp;
    struct tm *tm;
    char buf[BUFFSIZE];
    int c;
    char fmtstr[BUFFSIZE];
    FILE *fp = stdout;

    fmtstr[0] = '\0';

    stamp = time(NULL);
    tm = localtime(&stamp);

    while(1) {

        c = getopt(argc, argv, "-H:MSy:md");
        /* If  all  command-line  options  have  been  parsed,  then
         * getopt() returns -1
         */
        if (c < 0)
            break;
        switch (c) {
            /* If the first character of optstring is '-', then each
             * nonoption argv-element is handled as if it were the
             * argument of an option with character code 1.
             */
            case 1:
                if (fp == stdout) {
                    /* The variable optind is the index of the next
                     * element to  be  processed in  argv.
                     */
                    fp = fopen(argv[optind-1], "w");
                    if (fp == NULL) {
                        perror("fopen()");
                        fp = stdout;
                    }
                }
                break;
            case 'H':
                // optarg 接收H后一个参数
                if (strcmp(optarg, "12") == 0)
                    strncat(fmtstr, "%I(%P) ", BUFFSIZE - 1);
                else if (strcmp(optarg, "24") == 0)
                    strncat(fmtstr, "%H(%P) ", BUFFSIZE - 1);
                else
                    fprintf(stderr, "Invalid argumnt of H");
                break;
            case 'M':
                strncat(fmtstr, "%M ", BUFFSIZE - 1);
                break;
            case 'S':
                strncat(fmtstr, "%S ", BUFFSIZE - 1);
                break;
            case 'y':
                if (strcmp(optarg, "2") == 0)
                    strncat(fmtstr, "%y ", BUFFSIZE - 1);
                else if (strcmp(optarg, "4") == 0)
                    strncat(fmtstr, "%Y ", BUFFSIZE - 1);
                else
                    fprintf(stderr, "Invalid argumnt of y");
                break;
            case 'm':
                strncat(fmtstr, "%m ", BUFFSIZE - 1);
                break;
            case 'd':
                strncat(fmtstr, "%d ", BUFFSIZE - 1);
                break;
            default:
                break;
        }
    }
    strncat(fmtstr, "\n", BUFFSIZE - 1);
    strftime(buf, BUFFSIZE - 1, fmtstr, tm);
    fputs(buf, fp);
    
    if (fp != stdout)
        fclose(fp);

    exit(0);
}
