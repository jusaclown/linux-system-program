
#<center>linux C 系统开发学习笔记</center>

##I/O: input & output，是一切实现的基础
###1. stdio（标准I/O）: FILE类型贯穿始终  

* errno:  /usr/include/asm-generic/errno-base.h
          /usr/include/asm-generic/errno.h
          $ errno -l
* fopen();  
* fclose();  
* 可用ulimit -a查看最大打开文件个数  
* 文件创建权限计算公式： 0666 & ～umask  

+ fgetc();  
+ fputc();  
+ fgets();  
```c
#define SIZE 5
char buf[SIZE];
fgets(buf, SIZE, stream);
/*
两种情况会结束读取
1. 读取了 size-1 个字符
2. 读到了 '\n'
若读取 abcd 则会读取两次
1-> a b c d '\0'
2-> '\n' '\0' ...
*/
```
+ fputs();  

+ fread();  
```c
fread(buf, size, nmemb, fp);
//1. 数据量足够
//2. 只有5个字节
fread(buf, 1, 10, fp);
//1-> 返回值为10 ->10字节
//2-> 返回值为5 ->5字节
fread(buf, 10, 1, fp);
//1-> 返回值为1 ->10字节
//2-> 返回值为0 ->无法知道字节
```
+ fwrite();  

+ printf();  
+ scanf();  

+ fseek();  
+ ftell();  
+ rewind();   
+ fseeko();  
+ ftello();   

+ fflush();  
+ 在vim中，把光标放在某个函数上，然后进入命令模式，按shitf+k可直接进入man手册  
  
+ getline();  

+ 临时文件：1.如何不冲突  2.及时销毁  
    + tmpnam();  
    + tmpfile();  
    
###2.sysio（系统调用I/O或文件I/O）
fd是在文件IO章中贯穿始终的类型  


文件描述符的概念: 整型数，数组下表，文件描述符优先使用当前可用范围内最小的  
| stream | stdin | stdout | stderr |  
|---|---|---|--- |  
| fd | 0 | 1 | 2 |  

文件IO操作： 
+ open();  
fopen|open  
---|---  
r  | O_RDONLY  
r+ | O_RDWR  
w  | O_WRONLY|O_CREAT|O_TRUNC  
w+ | O_RDWR|O_TRUNC|O_CREAT  
a  | O_WRONLY|O_CREAT|O_APPEND  
a+ | O_RDWR|O_CREAT|O_APPEND  
> + 如果flag中有O_CREAT，则需要使用三参的实现模式，否则用两参  
> + open用变参实现  
> + 给open传多个参数，编译若报的是语法错误，则说明是定参，重载实现    
> + 编译没有报错误，变参实现  

+ close
+ read
+ write
+ lseek  

+ 文件IO与标准IO的区别  
区别：响应速度&吞吐量  
面试：如何使一个程序变快？   
从吞吐量和响应速度两个方面来回答  
提醒：标准IO和系统IO不可混用  
转换：fileno() 将标准IO转换为系统IO  
      fdopen() 将系统IO转换未标准IO  

+ IO的效率问题  
习题：将mycp.c程序更改，将BUFFSIZE的值放大，观察程序执行时间，注意性能最佳
拐点出现时的BUFFSIZE值，以及合适程序会出问题

+ time 用来测试后面语句的执行时间
> time ./mycp /etc/services /tmp/out  
> real    0m0.001s  # 总时间 user + sys + 其他  
> user    0m0.001s  # 在用户态执行的时间  
> sys     0m0.000s  # 在系统态执行的时间  

+ 文件共享:  
面试：写程序，删除一个文件的第十行  
补充函数：truncate/ftruncate  

+ 原子操作:不可分割的操作   
原子：不可分割的最小单位  
原子操作的作用：解决竞争的冲突  


+ 程序中的重定向：  
  dup();  
  dup2();    

+ 同步: 
  sync(); 
  fsync();  
  fdatasync();    

+ fcntl();  文件描述符相关的魔术都来自于这个函数  
+ ioctl();  设备相关的内容

+ /dev/fd/目录：虚目录，显示的是当前进程的文件描述符信息
  
##文件系统

类ls的实现，如myls
###1.文件和目录
####1）获取文件属性
    stat() ：通过文件路径获取属性，面对符号链接文件时获取的时所指向的目标文件的属性
    fstat()：通过文件描述符获取属性
    lstat()：面对符号链接时获取的时符号链接文件的属性

####2）文件访问权限
    mode_t：16位的位图，用于表示文件类型，文件访问权限，特殊权限位，可用
            man inode查看
    S_IFMT     0170000  0开始意味8进制数
    文件类型：dcb-lsp目录/字符设备/块设备/常规/符号链接/socket/管道文件

####3）umask
    作用：防止产生权限过松的文件
    计算：0666 & umask   （0666 & 0022 = 0644; rw-r--r--）

####4）文件权限的更改/管理
    chmod() 
    fchmod()

####5）粘住位
    t位：给某一个可执行二进制文件保留使用痕迹，使下次装载比较快

####6）文件系统：FAT，UFS
    文件系统：文件或数据的存储和管理
    FAT16/32：静态存储的单链表

####7）硬链接，符号链接
    硬链接是目录项的同义词，指向inode,并且不能跨分区建立，不能给目录建立
    符号链接：可跨分区，可以给目录建立
    link()；
    unlink()；
    remove()；  -> rm
    rename()；  -> mv

####8）utime
    可以更改文件的最后读的时间和最后修改的时间

####9）目录的创建和销毁
    mkdir();
    rmdir();

####10）更改当前工作路径
    chdir();
    fchdir();
    getcwd(); ->  pwd

####11）分析目录/读取目录内容
    glob(): 解析模式/通配符 
    globfree()

    opendir()  
    closedir()  
    readdir(3)  
    rewinddir()  
    seekdir()  
    telldir()  
    
    block/2就相当于以K为单位  
    如果一个变量在递归之前或之后，可以把它放到静态区  

###2.系统数据文件和信息
####1) /etc/passwd
    getpwuid()
    getpwnam()

####2) /etc/group
    getgrgid()
    getgrnam()

####3) /etc/shadow
    getspnam()
    crypt()
    getpass()

####4) 时间戳:time_t  char *   struct tm
    time()
    gmtime()
    localtime()
    mktime() 
    strftime()
```c
time_t stamp;
struct tm *tm;
time(&stamp);
stamp = time(NULL);
tm = localtime(&stamp);

strftime(buf, BUFFSIZE, "%Y-%m-%d", tm);
puts(buf);
```
###3.进程环境
####1) main函数
```c
    int main(int argc, char *argv[])
```  

####2）进程的终止（**一字不差背下来**）   
    正常终止：  
        从main函数返回(进程的返回值是给父进程看的)  
        调用exit
        调用_exit或_Exit
        最后一个线程从其启动例程返回
        最后一个线程调用了pthread_exit  
     
    异常终止：  
        调用abort  
        接到一个信号并终止  
        最后一个线程对其取消请求作出响应  

    atexit():钩子函数

####3）命令行参数的分析
    getopt()
    getopt_long()

####4）环境变量
    KET = VALUE
    envriron 存放环境变量的字符数组  
    getenv()
    setenv()
    putenv()

####5）C程序的存储空间布局
    pmap(1)

####6）库
    动态库
    静态库
    手工装载库
    dlopen()
    dlclose()
    dlerror()
    dlsym()

####7）函数跳转
    goto()：不太行
    setjmp():设置跳转点
    longjmp()：跳转

####8）资源的获取及控制  
    getrlimit()  
    setrlimit()

## 进程
####1. 进程标识符pid
    类型pid_t:传统而言是有符号16位整数
    命令：ps  
    进程号是顺次向下使用
    getpid();
    getppid();

####2. 进程的产生
    fork();  
    注意理解关键字：duplicating，意味着拷贝，克隆，一模一样等等  
    fork后父子进程的区别：fork的返回值不同，pid不同，ppid也不同，
    未决信号和文件锁不继承，资源利用量清0  
    init进程：1号进程，是所有进程的祖先进程  
    调度器的调度策略来决定父子进程谁先运行  
    fflush的重要性  
    父进程和子进程对文件（？）操作谁改谁拷贝

    vfork(); 快被废弃了

####3. 进程的消亡及释放资源
    wait();
    waitpid();
    waitid();
    wait3();
    wait4();

####4. exec函数组
    定参实现
    execl();
    execlp();
    execle();
    变参实现
    execv();
    execvp();  
    **注意fflush的使用**

####5. 用户权限及组权限
    u+s: 用户使用这个文件的时候，身份会切换为该文件属于者的身份
    g+s：同上，组
    getuid();
    geteuid();
    getgid();
    getegid();
    setuid();
    setgid();
    setreuid();
    setregid();
    seteuid();
    setegid();


####6. 观摩: 解释器函数
    #!/bin/bash: 装载一个二进制文件用来解释该文件
    比如这句话意思就是用/bin/bash来解释这个文件
    如果是#!/bin/cat就是用cat来解释这个文件

####7. system();
    相当于fork(), execl(), wait()的封装  可以类比few.c

####8. 进程会计
    acct(); 了解一哈，纯方言    

####9. 进程时间
    times();

####10. 守护进程
    会话 session 标识sid
    终端
    setsid();
    getpgrp();
    getpgid();
    setpdig();
    进程组是一组相关进程的集合，会话是一组相关进程组的集合。 
    shell中协同工作的进程属于同一个进程组
    特点：ppid为1，pid、pgid、sid相同，tty为？
    单实例守护进程：用锁文件/var/run/name.pid来控制单实例
    启动脚本文件：/etc/rc....

####11. 系统日志
    syslogd服务
    openlog();
    syslog();
    closelog();
    archlinux: 查看日志 journalctl

## 并发
异步事件的处理：  
通知法：发生频率较低  
查询法：发生频率较高  

```c
typedef:
typedef int ARR[6]; 
ARR a; ---> int a[6];

struct node_st
{
    int i;
    float f;
}
typedef struct node_st NODE;
NODE a; ---> struct node_st a;

typedef struct node_st *NODEP;
NODEP p; ---> struct node_st *p;

typedef struct
{
    int i;
    float f;
}NODE, *NODEP;

typedef int FUNC(int); -> int(int) FUNC;
FUNC f; ---> int f(int);

typedef int *FUNCP(int);
FUNCP p; ---> int *p(int);

typedef int *(*FUNCP)(int);
FUNCP p; ---> int *(*p)(int);  
//定义了一个指向指针函数的指针变量，函数返回值为int *, 参数为int

```

###一、信号
####1. 信号的概念
    信号是软件中断。
    信号的响应依赖于中断。
    查看信号kill -l 
    1-31 标准信号
    34-64 实时信号

####2. signal()
    void (*signal(int sig, void (*func)(int)))(int);
    信号会打断阻塞的系统调用。

####3. 信号的不可靠
    信号的行为不可靠

####4. 可重入函数
    第一次调用还没有结束就发生了第二次调用，而且不会出错的函数
    所有的系统调用都是可重入的，一部分库函数也是可重入的，
    如：memcpy(); localtime_r();
    有"_r"版本的函数，在信号这一块都需要用"_r"版本

####5. 信号的响应过程
    信号从收到到响应有一个不可避免的延迟
    思考：如何忽略掉一个信号的？ 
          标准信号为什么要丢失？ 由于位图
    标准信号的响应没有严格的顺序。
    不能从信号处理函数中随意的往外跳 
    （sigsetjmp, siglongjmp, setjmp, longjmp   ）

####6. 常用函数
    kill();
    raise();

    alarm();
    setitimer();
    例：使用单一计时器，构造一组函数，实现任意数量的计时器

    puase();
    abort();
    system();

    sleep();
    nanosleep();
    usleep();
    select(-1, NULL, NULL, NULL, time);

####7. 信号集
    信号集类型：sigset_t
    sigemptyset();
    sigfillset();
    sigaddset();
    sigdelset();
    sigismember();

####8. 信号屏蔽字和pending集的处理
    sigprocmask(); 控制mask
    sigprnding();

####9. 扩展
    sigsuspend();
    sigaction(); 用来替代signal();

####10. 实时信号    
    /usr/include/bits/signum.h
    先响应标准信号 后响应实时信号



###二、线程
####1. 线程的概念
    一个正在运行的函数
    posix 线程是一套标准，而不是实现
    openmp线程
    线程标识：pthread_t 
    pthread_equal();
    pthread_self();
    最好不要和信号大规模混用

####2. 线程的创建
    pthread_create();
    **线程的调度取决于调度器策略**
   
    线程的终止
        3种方式：
            + 线程从启动例程返回，返回值就是线程的推出码
            + 线程可以被同一进程中的其他线程取消
            + 线程叫用pthread_exit()函数
        pthread_join(); --> 相当于wait();收尸函数

    栈的清理
        pthread_cleanup_push();
        pthread_cleanup_pop();
        这两句话必须一对出现

    线程的取消选项
        线程取消：pthread_cancel();
        取消有两种状态：允许和不允许
        允许取消又分为：异步cancel，推迟cancel（默认）->推迟至cancel再响应
        cancel点：POSIX定义测cancel点，都是可能引发阻塞的系统调用
        pthread_setcancelstate():设置是否允许取消
        pthread_setcanceltype():设置取消方式
        pthread_testcancel():本函数什么都不做，就是一个取消点

    线程分离
        pthread_detach(); 已经分离的不能再用pthread_join()收尸，各安天命


####3. 线程同步
    互斥量：类型：pthread_mutex_t
            pthread_mutex_init();
            pthread_mutex_destroy();
            pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

            pthread_mutex_lock();
            pthread_mutex_trylock();
            pthread_mutex_unlock();
            
            pthread_once(); 用来实现某个模块的单次初始化
    条件变量：
            pthread_cond_t
            pthread_cond_init();
            pthread_cond_destroy();

            pthread_cond_broadcast();
            pthread_cond_signal();
           
            pthread_cond_wait();
            pthread_cond_timewait();
    信号量：
    读写锁：读锁 -> 共享锁
            写锁 -> 互斥锁

####4. 线程属性
    pthread_attr_init();
    pthread_attr_destroy();
    pthread_attr_setstacksize();
    见man pthread_attr_init的see also

    线程同步的属性
        互斥量属性：
            pthread_mutexattr_init();
            pthread_mutexattr_destroy();
            pthread_mutexattr_getpshared(); getp这个p指的是是否跨进程使用
            phtread_mutexattr_setpshared();
            clone();
            pthread_mutexattr_gettype();
            pthread_mutexattr_settype();
        条件变量属性：
            pthread_condattr_init();
            pthread_condattr_destroy();
        读写锁属性：
        ..............

####5. 重入
    多线程中的IO
    线程和信号的关系
        pthread_pending();
        sigwait();
        pthread_kill();

    线程与fork
####6. openmp(www.OpenMP.org)


###高级IO
    非阻塞IO 
    补充：有限状态机编程

####1. 非阻塞IO
    简单流程：如果一个程序的自然流程是结构化的
    复杂流程：如果一个程序的自然流程不是结构化的

####2. IO多路转接
    select(); 以事件为单位组织文件描述符
    poll(); 以文件描述符来组织事件
    epoll(); linux方言
        epoll_create();
        epoll_ctl();
        epoll_wait();

####3. 其他读写函数
    readv();
    writev();

####4. 存储映射IO
    mmap();
    munmap();
####5. 文件锁
    fcntl();
    lockf();
    flock();

##进程间通信
###管道
内核提供，单工，自同步机制
####匿名管道
    pipe();

####命名管道
    mkfifo();

###XSI -> SysV
    IPC -> Inter-Process Communication
    
    主动端：先发包的一方
    被动端：先收包的一方（先运行）


    key:ftok(); 
    xxxget  xxxop   xxxctl
    xxx-> msg sem shm

    Message Queues
    msgget();
    msgop();
    msgctl();

    Semaphore Arrays
    semget();
    semop(); 
    semctl();

    Shared Memory 
    shmget();
    shmop();
    shmctl();
    
###网络套接字socket
    讨论：跨主机的传输要注意的问题
    1. 字节序问题：
        大端：低地址处放高字节
        小端：低地址出放低字节
        主机字节序：host
        网络字节序：network
        解决：_ to _ _: htons, htonl, ntohs, ntohl (s 两字节， l四字节)

    2. 对齐
        struct {
            int i;
            float f;
            char ch;
        };
        解决：不对齐

    3. 类型长度问题：
        int
        char
        解决： int32_t, unit32_t, int64_t, int8_t, uint8_t
    
    SOCKET是什么
    

    **被动端（先运行）**
    1. 取得socket
    2. 给socket取得地址
    3. 收/发消息
    4. 关闭socket
    **主动端**
    1. 取得socket
    2. 给socket取得地址（可省略）
    3. 收/发消息
    4. 关闭socket

    报式套接字：
        socket();
        bind(); 
        sockaddr -> man 7 ip -> 找Address format
        sendto();
        rcvfrom();
        inet_pton(); 点分式转为二进制格式
        inet_ntop();
        setsockopt();
        getsockopt();
        多点通讯：广播（全网广播，子网广播）， 多波/组播 
        224.0.0.1 所有支持多播的节点默认都存在这个组中，并且无法离开

    流式套接字：
        **C端（主动端）**
        1. 取得socket
        2. 给socket取得地址（可省略）
        3. 发送连接
        4. 收/发消息
        5. 关闭socket
        **S端 （被动端先运行）**
        1. 取得socket
        2. 给socket取得地址
        3. 将socket设置为监听模式
        4. 接受连接
        5. 收/发消息
        6. 关闭socket

    ***Q***:
    纯文本内容就是char型数组，都是单字节
    accept()创建的新套接字和原来的不一样吗？
###俄罗斯方块
####图形

####输入设备


####并发
