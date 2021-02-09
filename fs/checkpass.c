/*
 *  密码校验程序
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char **argv)
{
    char *crypted_pass;
    char *input_pass;
    struct spwd *shadow_line;

    if (argc != 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    input_pass = getpass("Password:");
    
    shadow_line = getspnam(argv[1]);
    puts(shadow_line->sp_pwdp);
    crypted_pass = crypt(input_pass, shadow_line->sp_pwdp);

    if(strcmp(shadow_line->sp_pwdp, crypted_pass) == 0)
        puts("OK!");
    else
        puts("Fail");
    exit(0);
}
