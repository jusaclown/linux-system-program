/*
 *  实现ls的部分功能
 *  分析：
 *      1. 需要解析命令行参数getopt_long() 参数/目录
 *      2. 对于目录 需要获取目录下每个文件的名字glob()
 *         对于文件 直接输出名字
 *      3. ls -l 获取文件属性
 *         文件类型/权限 硬链接数 用户名 组名 大小 最后修改时间 文件名
 *         使用lstat();
 *         类型/权限：lstat提供的是数字，需要转换为drwx格式
 *         用户名/组名：lstat提供的是ID，需要转换未用户名
 *                      getpwnam()/getgrnam()
 *         时间：lstat提供的时间戳需转换为本地时间 localtime();
 *
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char []);
void do_stat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int argc, char **argv)
{
    if (argc == 1)
        do_ls(".");
    else
        do_ls(argv[1]);


    exit(EXIT_SUCCESS);
}

void do_ls(char dirname[])
/*
 *  利用glob解析目录，暂且只能解析目录 且格式为/etc
 */
{
    
    int err, i;
    glob_t filenames;
    
    strcat(dirname, "/*");
    err = glob(dirname, 0, NULL, &filenames);
    if (err) {
        perror("glob");
    }
    
    for (i = 0; i < filenames.gl_pathc; i++) {
//        puts(filenames.gl_pathv[i]);
        do_stat(filenames.gl_pathv[i]);
    }

    globfree(&filenames);
}

void do_stat(char *filename)
{
    struct stat info;
    if (lstat(filename, &info) < 0) {
        perror("lstat()");
    } else {
        show_file_info(filename, &info);
    }
}

void show_file_info(char * filename, struct stat * info_p)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];

    mode_to_letters(info_p->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4+ctime(&info_p->st_mtim)); //表示显示一个长度不大于12的字符串
    printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");
    // 判断文件类型
    if (S_ISREG(mode))      str[0] = '-';
    if (S_ISDIR(mode))      str[0] = 'd';
    if (S_ISCHR(mode))      str[0] = 'c';
    if (S_ISBLK(mode))      str[0] = 'b';
    if (S_ISFIFO(mode))     str[0] = 'p';
    if (S_ISLNK(mode))      str[0] = 'l';
    if (S_ISSOCK(mode))     str[0] = 's';

    //确定文件权限
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

/* uid 转换为用户名 */
char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}

/* gid 转换为组名 */
char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
}







