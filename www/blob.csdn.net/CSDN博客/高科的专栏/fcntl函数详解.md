
# fcntl函数详解 - 高科的专栏 - CSDN博客

2012年09月13日 14:06:02[高科](https://me.csdn.net/pbymw8iwm)阅读数：25212


功能描述：根据文件描述词来操作文件的特性。
\#include <unistd.h>
\#include <fcntl.h>
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);
[描述]
fcntl()针对(文件)描述符提供控制。参数fd是被参数cmd操作(如下面的描述)的描述符。针对cmd的值，fcntl能够接受第三个参数int arg。
[返回值]
fcntl()的返回值与命令有关。如果出错，所有命令都返回－1，如果成功则返回某个其他值。下列三个命令有特定返回值：F_DUPFD , F_GETFD , F_GETFL以及F_GETOWN。
F_DUPFD   返回新的文件描述符
F_GETFD   返回相应标志
F_GETFL , F_GETOWN   返回一个正的进程ID或负的进程组ID
fcntl函数有5种功能：
1. 复制一个现有的描述符(cmd=F_DUPFD).
2. 获得／设置文件描述符标记(cmd=F_GETFD或F_SETFD).
3. 获得／设置文件状态标记(cmd=F_GETFL或F_SETFL).
4. 获得／设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN).
5. 获得／设置记录锁(cmd=F_GETLK , F_SETLK或F_SETLKW).
**1. cmd值的F_DUPFD ：**
F_DUPFD返回一个如下描述的(文件)描述符：
·最小的大于或等于arg的一个可用的描述符
·与原始操作符一样的某对象的引用
·如果对象是文件(file)的话，则返回一个新的描述符，这个描述符与arg共享相同的偏移量(offset)
·相同的访问模式(读，写或读/写)
·相同的文件状态标志(如：两个文件描述符共享相同的状态标志)
·与新的文件描述符结合在一起的close-on-exec标志被设置成交叉式访问execve(2)的系统调用
实际上调用dup(oldfd)；
等效于
fcntl(oldfd, F_DUPFD, 0);
而调用dup2(oldfd, newfd)；
等效于
close(oldfd)；
fcntl(oldfd, F_DUPFD, newfd)；
**2. cmd值的F_GETFD和F_SETFD：**
F_GETFD取得与文件描述符fd联合的close-on-exec标志，类似FD_CLOEXEC。如果返回值和FD_CLOEXEC进行与运算结果是0的话，文件保持交叉式访问exec()，否则如果通过exec运行的话，文件将被关闭(arg 被忽略)
F_SETFD设置close-on-exec标志，该标志以参数arg的FD_CLOEXEC位决定，应当了解很多现存的涉及文件描述符标志的程序并不使用常数 FD_CLOEXEC，而是将此标志设置为0(系统默认，在exec时不关闭)或1(在exec时关闭)
在修改文件描述符标志或文件状态标志时必须谨慎，先要取得现在的标志值，然后按照希望修改它，最后设置新标志值。不能只是执行F_SETFD或F_SETFL命令，这样会关闭以前设置的标志位。
**3. cmd值的F_GETFL和F_SETFL：**
F_GETFL取得fd的文件状态标志，如同下面的描述一样(arg被忽略)，在说明open函数时，已说明
了文件状态标志。不幸的是，三个存取方式标志 (O_RDONLY , O_WRONLY , 以及O_RDWR)并不各占1位。(这三种标志的值各是0 , 1和2，由于历史原因，这三种值互斥 — 一个文件只能有这三种值之一。) 因此首先必须用屏蔽字O_ACCMODE相与取得存取方式位，然后将结果与这三种值相比较。
F_SETFL设置给arg描述符状态标志，可以更改的几个标志是：O_APPEND，O_NONBLOCK，O_SYNC 和 O_ASYNC。而fcntl的文件状态标志总共有7个：O_RDONLY , O_WRONLY , O_RDWR , O_APPEND , O_NONBLOCK , O_SYNC和O_ASYNC
可更改的几个标志如下面的描述：
O_NONBLOCK非阻塞I/O，如果read(2)调用没有可读取的数据，或者如果write(2)操作将阻塞，则read或write调用将返回-1和EAGAIN错误
O_APPEND强制每次写(write)操作都添加在文件大的末尾，相当于open(2)的O_APPEND标志
O_DIRECT最小化或去掉reading和writing的缓存影响。系统将企图避免缓存你的读或写的数据。如果不能够避免缓存，那么它将最小化已经被缓存了的数据造成的影响。如果这个标志用的不够好，将大大的降低性能
O_ASYNC当I/O可用的时候，允许SIGIO信号发送到进程组，例如：当有数据可以读的时候
**4. cmd值的F_GETOWN和F_SETOWN：**
F_GETOWN取得当前正在接收SIGIO或者SIGURG信号的进程id或进程组id，进程组id返回的是负值(arg被忽略)
F_SETOWN设置将接收SIGIO和SIGURG信号的进程id或进程组id，进程组id通过提供负值的arg来说明(arg绝对值的一个进程组ID)，否则arg将被认为是进程id
**5.****cmd值的****F_GETLK, F_SETLK或F_SETLKW****：**获得／设置记录锁的功能，成功则返回0，若有错误则返回-1，错误原因存于errno。
F_GETLK通过第三个参数arg(一个指向flock的结构体)取得第一个阻塞lock description指向的锁。取得的信息将覆盖传到fcntl()的flock结构的信息。如果没有发现能够阻止本次锁(flock)生成的锁，这个结构将不被改变，除非锁的类型被设置成F_UNLCK
F_SETLK按照指向结构体flock的指针的第三个参数arg所描述的锁的信息设置或者清除一个文件的segment锁。F_SETLK被用来实现共享(或读)锁(F_RDLCK)或独占(写)锁(F_WRLCK)，同样可以去掉这两种锁(F_UNLCK)。如果共享锁或独占锁不能被设置，fcntl()将立即返回EAGAIN
F_SETLKW除了共享锁或独占锁被其他的锁阻塞这种情况外，这个命令和F_SETLK是一样的。如果共享锁或独占锁被其他的锁阻塞，进程将等待直到这个请求能够完成。当fcntl()正在等待文件的某个区域的时候捕捉到一个信号，如果这个信号没有被指定SA_RESTART, fcntl将被中断
当一个共享锁被set到一个文件的某段的时候，其他的进程可以set共享锁到这个段或这个段的一部分。共享锁阻止任何其他进程set独占锁到这段保护区域的任何部分。如果文件描述符没有以读的访问方式打开的话，共享锁的设置请求会失败。
独占锁阻止任何其他的进程在这段保护区域任何位置设置共享锁或独占锁。如果文件描述符不是以写的访问方式打开的话，独占锁的请求会失败。
结构体flock的指针：
struct flcok
{
short int l_type; /* 锁定的状态*/
//以下的三个参数用于分段对文件加锁，若对整个文件加锁，则：l_whence=SEEK_SET, l_start=0, l_len=0
short int l_whence; /*决定l_start位置*/
off_t l_start; /*锁定区域的开头位置*/
off_t l_len; /*锁定区域的大小*/
pid_t l_pid; /*锁定动作的进程*/
};
l_type 有三种状态：
F_RDLCK   建立一个供读取用的锁定
F_WRLCK   建立一个供写入用的锁定
F_UNLCK   删除之前建立的锁定
l_whence 也有三种方式：
SEEK_SET   以文件开头为锁定的起始位置
SEEK_CUR   以目前文件读写位置为锁定的起始位置
SEEK_END   以文件结尾为锁定的起始位置
fcntl文件锁有两种类型：建议性锁和强制性锁
建议性锁是这样规定的：每个使用上锁文件的进程都要检查是否有锁存在，当然还得尊重已有的锁。内核和系统总体上都坚持不使用建议性锁，它们依靠程序员遵守这个规定。
强制性锁是由内核执行的：当文件被上锁来进行写入操作时，在锁定该文件的进程释放该锁之前，内核会阻止任何对该文件的读或写访问，每次读或写访问都得检查锁是否存在。
系统默认fcntl都是建议性锁，强制性锁是非POSIX标准的。如果要使用强制性锁，要使整个系统可以使用强制性锁，那么得需要重新挂载文件系统，mount使用参数 -0 mand 打开强制性锁，或者关闭已加锁文件的组执行权限并且打开该文件的set-GID权限位。
建议性锁只在cooperating processes之间才有用。对cooperating process的理解是最重要的，它指的是会影响其它进程的进程或被别的进程所影响的进程，举两个例子：
(1) 我们可以同时在两个窗口中运行同一个命令，对同一个文件进行操作，那么这两个进程就是cooperating  processes
(2) cat file | sort，那么cat和sort产生的进程就是使用了pipe的cooperating processes
使用fcntl文件锁进行I/O操作必须小心：进程在开始任何I/O操作前如何去处理锁，在对文件解锁前如何完成所有的操作，是必须考虑的。如果在设置锁之前打开文件，或者读取该锁之后关闭文件，另一个进程就可能在上锁/解锁操作和打开/关闭操作之间的几分之一秒内访问该文件。当一个进程对文件加锁后，无论它是否释放所加的锁，只要文件关闭，内核都会自动释放加在文件上的建议性锁(这也是建议性锁和强制性锁的最大区别)，所以不要想设置建议性锁来达到永久不让别的进程访问文件的目的(强制性锁才可以)；强制性锁则对所有进程起作用。
fcntl使用三个参数 F_SETLK/F_SETLKW， F_UNLCK和F_GETLK 来分别要求、释放、测试record locks。record locks是对文件一部分而不是整个文件的锁，这种细致的控制使得进程更好地协作以共享文件资源。fcntl能够用于读取锁和写入锁，read
 lock也叫shared lock(共享锁)， 因为多个cooperating process能够在文件的同一部分建立读取锁；write lock被称为exclusive lock(排斥锁)，因为任何时刻只能有一个cooperating process在文件的某部分上建立写入锁。如果cooperating processes对文件进行操作，那么它们可以同时对文件加read lock，在一个cooperating process加write lock之前，必须释放别的cooperating process加在该文件的read
 lock和wrtie lock，也就是说，对于文件只能有一个write lock存在，read lock和wrtie lock不能共存。
**下面的例子使用F_GETFL获取****fd的文件****状态标志****。**
\#include<fcntl.h>
\#include<unistd.h>
\#include<iostream>
\#include<errno.h>
using namespace std;
int main(int argc,char* argv[])
{
int fd, var;
//  fd=open("new",O_RDWR);
if (argc!=2)
{
perror("--");
cout<<"请输入参数，即文件名！"<<endl;
}
if((var=fcntl(atoi(argv[1]), F_GETFL, 0))<0)
{
strerror(errno);
cout<<"fcntl file error."<<endl;
}
switch(var & O_ACCMODE)
{
case O_RDONLY : cout<<"Read only.."<<endl;
break;
case O_WRONLY : cout<<"Write only.."<<endl;
break;
case O_RDWR   : cout<<"Read wirte.."<<endl;
break;
default  : break;
}
if (val & O_APPEND)
cout<<",append"<<endl;
if (val & O_NONBLOCK)
cout<<",noblocking"<<endl;
cout<<"exit 0"<<endl;
exit(0);
}


