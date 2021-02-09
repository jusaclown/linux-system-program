#include <stdio.h>
#include <stdlib.h>
#include <termios.h>


int main()
{
    int ch;
    struct termios new, old;

    tcgetattr(0, &old);
    tcgetattr(0, &new);
    new.c_lflag = new.c_lflag & ~(ICANON | ECHO);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new);

    while (1) {
        ch = getchar();
        if (ch == 'Q')
            break;
        printf("%x ", ch);
        fflush(NULL);
    }

    tcsetattr(0, TCSANOW, &old);

    return 0;
}
