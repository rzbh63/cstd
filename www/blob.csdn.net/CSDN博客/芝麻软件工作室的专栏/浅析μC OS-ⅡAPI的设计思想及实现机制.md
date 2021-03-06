
# 浅析μC-OS-ⅡAPI的设计思想及实现机制 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 09:58:47[seven-soft](https://me.csdn.net/softn)阅读数：808个人分类：[uCos-II																](https://blog.csdn.net/softn/article/category/6305029)



任何一个操作系统都会提供大量的API供程序员使用，μC/OS-Ⅱ也不例外。由于μC/OS-Ⅱ面向的是嵌入式开发，并不要求大而全，所以内核提供的 API也就大多和多任务息息相关。本文通过分析μC/OS-Ⅱ中提供的API来引出μC/OS-Ⅱ中API的设计思路和实现机制。
API全称Application Programming Interface，中文是应用程序编程接口的意思。API是操作系统提供给用户的一组函数，供用户在编写应用程序时调用，可以完成应用程序对操作系统的各种调用，包括进程调度、存储管理、图形设备接口及文件管理等。μC/OS-Ⅱ作为一个嵌入式操作系统，相对于其他操作系统，有很多自己的特色，在设计思路和实现机制上都和一般操作系统有很大的不同。
1． 简介
任何一个操作系统都会提供大量的API供程序员使用，μC/OS-Ⅱ也不例外。由于μC/OS-Ⅱ面向的是嵌入式开发，并不要求大而全，所以内核提供 的API也就大多和多任务息息相关。μC/OS-Ⅱ的API主要分以下几类：（1）任务类、（2）消息类、（3）同步类、（4）时间类、（5）临界区与事件类等。下面分别从这几类API分析各自的设计思路和实现机制。
2． 任务类API的设计思路和实现机制
μC/OS-Ⅱ可以管理多达64个任务，并从中保留了四个最高优先级和四个最低优先级的任务供自己使用，所以用户可以使用的只有56个任务。任务类API包括如何在用户的应用程序中建立任务、删除任务、改变任务的优先级、挂起和恢复任务，以及获得有关任务的信息等。
2.1 建立任务API
想让μC/OS-Ⅱ管理用户的任务，用户必须要先建立任务。用户可以通过传递任务地址和其它参数到以下两个函数之一来建立任务：OSTaskCreate() 或 OSTaskCreateExt()。
OSTaskCreate()与μC/OS是向下兼容的，OSTaskCreateExt()是OSTaskCreate()的扩展版本，提供了一些 附加的功能。用两个函数中的任何一个都可以建立任务。任务可以在多任务调度开始前建立，也可以在其它任务的执行过程中被建立。在开始多任务调度(即调用 OSStart())前，用户必须建立至少一个任务。任务不能由中断服务程序(ISR)来建立。
OSTaskCreate()的函数定义如下。从中可以知道，OSTaskCreate()需要四个参数：task是任务代码的指针，pdata是当任务开始执行时传递给任务的参数的指针，ptos是分配给任务的堆栈的栈顶指针，prio是分配给任务的优先级。
INT8U OSTaskCreate (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio)
用OSTaskCreateExt()函数来建立任务会更加灵活，但会增加一些额外的开销。
OSTaskCreateExt()需要九个参数！前四个参数(task,pdata,ptos和prio)与OSTaskCreate()的四个参 数完全相同，连先后顺序都一样。这样做的目的是为了使用户能够更容易地将用户的程序从OSTaskCreate()移植到 OSTaskCreateExt()上去。函数的定义如下：
INT8U OSTaskCreateExt (void   (*task)(void *pd),
void    *pdata,
OS_STK  *ptos,
INT8U    prio,
INT16U   id,
OS_STK  *pbos,
INT32U   stk_size,
void    *pext,
INT16U   opt)
2.2 删除任务API
有时候删除任务是很有必要的。删除任务,是说任务将返回并处于休眠状态，并不是说任务的代码被删除了，只是任务的代码不再被μC/OS-Ⅱ调用。通过 调用OSTaskDel()就可以完成删除任务的功能。OSTaskDel()一开始应确保用户所要删除的任务并非是空闲任务，因为删除空闲任务是不允许 的。不过，用户可以删除statistic任务。接着，OSTaskDel()还应确保用户不是在ISR例程中去试图删除一个任务，因为这也是不被允许 的。调用此函数的任务可以通过指定OS_PRIO_SELF参数来删除自己。接下来OSTaskDel()会保证被删除的任务是确实存在的。该函数的入口
 参数很简单，只需要知道要删除任务的优先级即可。
INT8U OSTaskDel (INT8U prio)
2.3 改变任务优先级API
在用户建立任务的时候会分配给任务一个优先级。在程序运行期间，用户可以通过调用OSTaskChangePrio()来改变任务的优先级。换句话说，就是μC/OS-Ⅱ允许用户动态的改变任务的优先级。函数定义如下：
INT8U OSTaskChangePrio (INT8U oldprio, INT8U newprio)
用户不能改变空闲任务的优先级，但用户可以改变调用本函数的任务或者其它任务的优先级。为了改变调用本函数的任务的优先级，用户可以指定该任务当前的 优先级或OS_PRIO_SELF，OSTaskChangePrio()会决定该任务的优先级。用户还必须指定任务的新(即想要的)优先级。因为 μC/OS-Ⅱ不允许多个任务具有相同的优先级，所以OSTaskChangePrio()需要检验新优先级是否是合法的（即不存在具有新优先级的任 务）。如果新优先级是合法的，μC/OS-Ⅱ通过将某些东西储存到OSTCBPrioTbl[newprio]中保留这个优先级。如此就使得
 OSTaskChangePrio()可以重新允许中断，因为此时其它任务已经不可能建立拥有该优先级的任务，也不能通过指定相同的新优先级来调用 OSTaskChangePrio()。接下来OSTaskChangePrio()可以预先计算新优先级任务的OS_TCB中的某些值。而这些值用来将 任务放入就绪表或从该表中移除。
2.4 挂起任务和恢复任务API
有时候将任务挂起是很有用的。挂起任务可通过调用OSTaskSuspend()函数来完成。被挂起的任务只能通过调用OSTaskResume() 函数来恢复。任务挂起是一个附加功能。也就是说，如果任务在被挂起的同时也在等待延时的期满，那么，挂起操作需要被取消，而任务继续等待延时期满，并转入 就绪状态。任务可以挂起自己或者其它任务。
OSTaskSuspend()函数的函数定义如下：
INT8U OSTaskSuspend (INT8U prio)
恢复任务OSTaskResume()函数定义为：
INT8U OSTaskResume (INT8U prio)
被挂起的任务只有通过调用OSTaskResume()才能恢复。因为OSTaskSuspend()不能挂起空闲任务，所以必须得确认用户的应用程 序不是在恢复空闲任务。注意，这个测试也可以确保用户不是在恢复优先级为OS_PRIO_SELF的任务(OS_PRIO_SELF被定义为0xFF，它 总是比OS_LOWEST_PRIO大)。
2.5 获得任务信息API
用户的应用程序可以通过调用OSTaskQuery()来获得自身或其它应用任务的信息。实际上，OSTaskQuery()获得的是对应任务的 OS_TCB中内容的拷贝。用户能访问的OS_TCB的数据域的多少决定于用户的应用程序的配置(参看OS_CFG.H)。由于μC/OS-Ⅱ是可裁剪 的，它只包括那些用户的应用程序所要求的属性和功能。
void MyTask (void *pdata)
函数参数为一指针变量，指向对应任务的OS_TCB结构地址。本函数是有用的调试工具。
3． 消息和同步类API的设计思路和实现机制
μC/OS-Ⅱ中有三种方法实现消息通信和同步：信号量、邮箱和消息队列。一个任务或者中断服务子程序可以通过事件控制块ECB（Event Control Blocks）来向另外的任务发信号。这里，所有的信号都被看成是事件（Event）。这也说明为什么上面把用于通讯的数据结构叫做事件控制块。一个任务 还可以等待另一个任务或中断服务子程序给它发送信号。这里要注意的是，只有任务可以等待事件发生，中断服务子程序是不能这样做的。对于处于等待状态的任 务，还可以给它指定一个最长等待时间，以此来防止因为等待的事件没有发生而无限期地等下去。多个任务可以同时等待同一个事件的发生。在这种情况下，当该事
 件发生后，所有等待该事件的任务中，优先级最高的任务得到了该事件并进入就绪状态，准备执行。上面讲到的事件，可以是信号量、邮箱或者消息队列等。当事件 控制块是一个信号量时，任务可以等待它，也可以给它发送消息。
μC/OS-II中的信号量由两部分组成：一个是信号量的计数值，它是一个16位的无符号整数（0 到65,535之间）；另一个是由等待该信号量的任务组成的等待任务表。用户要在OS_CFG.H中将OS_SEM_EN开关量常数置成1，这样μC/OS-II才能支持信号量。
邮箱是μC/OS-II中另一种通讯机制，它可以使一个任务或者中断服务子程序向另一个任务发送一个指针型的变量。该指针指向一个包含了特定“消息”的数据结构。为了在μC/OS-II中使用邮箱，必须将OS_CFG.H中的OS_MBOX_EN常数置为1。
消息队列是μC/OS-II中另一种通讯机制，它可以使一个任务或者中断服务子程序向另一个任务发送以指针方式定义的变量。因具体的应用有所不同，每个指针指向的数据结构变量也有所不同。为了使用μC/OS-II的消息队列功能，需要在OS_CFG.H 文件中，将OS_Q_EN常数设置为1，并且通过常数OS_MAX_QS来决定μC/OS-II支持的最多消息队列数。
μC/OS-Ⅱ提供一系列API函数供用户调用，实现各个任务之间的通信和同步功能。下面以信号量为例说明各个API的实现。
μC/OS-II提供了5个对信号量进行操作的函数。它们 是：OSSemCreate()，OSSemPend()，OSSemPost()，OSSemAccept()和OSSemQuery()函数。图 F6.5说明了任务、中断服务子程序和信号量之间的关系。图中用钥匙或者旗帜的符号来表示信号量：如果信号量用于对共享资源的访问，那么信号量就用钥匙符 号。符号旁边的数字N代表可用资源数。对于二值信号量，该值就是1；如果信号量用于表示某事件的发生，那么就用旗帜符号。这时的数字N代表事件已经发生的 次数。从图 F6.5中可以看出OSSemPost()函数可以由任务或者中断服务子程序调用，而OSSemPend()和OSSemQuery()函数只能有任务程
 序调用。
3.1建立一个信号量, OSSemCreate()
OS_EVENT *OSSemCreate (INT16U cnt)
函数参数传递的是要创建的信号量的初始值，在函数内部对任务控制块进行初始化。OSSemCreate()返回给调用函数一个指向任务控制块的指针。以后对信号量的所有操作，如OSSemPend(), OSSemPost(), OSSemAccept()和OSSemQuery()都是通过该指针完成的。因此，这个指针实际上就是该信号量的句柄。如果系统中没有可用的任务控制块，OSSemCreate()将返回一个NULL指针。
值得注意的是，在μC/OS-II中，信号量一旦建立就不能删除了，因此也就不可能将一个已分配的任务控制块再放回到空闲ECB链表中。如果有任务正在等待某个信号量，或者某任务的运行依赖于某信号量的出现时，删除该任务是很危险的。
3.2等待一个信号量, OSSemPend()
void OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
它首先检查指针pevent所指的任务控制块是否是由OSSemCreate()建立的。如果信号量当前是可用的（信号量的计数值大于0），将信号量 的计数值减1，然后函数将“无错”错误代码返回给它的调用函数。显然，如果正在等待信号量，这时的输出正是我们所希望的，也是运行OSSemPend() 函数最快的路径。
3.3发送一个信号量, OSSemPost()
INT8U OSSemPost (OS_EVENT *pevent)
它首先检查参数指针pevent指向的任务控制块是否是OSSemCreate()函数建立的，接着检查是否有任务在等待该信号量。如果该任务控制块 中的.OSEventGrp域不是0，说明有任务正在等待该信号量。这时，就要调用函数OSEventTaskRdy()，使一个任务进入就绪状态，把其 中的最高优先级任务从等待任务列表中删除并使它进入就绪状态。然后，调用OSSched()任务调度函数检查该任务是否是系统中的最高优先级的就绪任务。 如果是，这时就要进行任务切换[当OSSemPost()函数是在任务中调用的]，准备执行该就绪任务。如果不是，OSSched()直接返回，调用
 OSSemPost()的任务得以继续执行。如果这时没有任务在等待该信号量，该信号量的计数值就简单地加1。
上面是由任务调用OSSemPost()时的情况。当中断服务子程序调用该函数时，不会发生上面的任务切换。如果需要，任务切换要等到中断嵌套的最外层中断服务子程序调用OSIntExit()函数后才能进行。
3.4无等待地请求一个信号量, OSSemAccept()
INT16U OSSemAccept (OS_EVENT *pevent)
当一个任务请求一个信号量时，如果该信号量暂时无效，也可以让该任务简单地返回，而不是进入睡眠等待状态。这种情况下的操作是由 OSSemAccept()函数完成的。该函数在最开始也是检查参数指针pevent指向的事件控制块是否是由OSSemCreate()函数建立的，接 着从该信号量的事件控制块中取出当前计数值，并检查该信号量是否有效（计数值是否为非0值）。如果有效，则将信号量的计数值减1，然后将信号量的原有计数 值返回给调用函数。调用函数需要对该返回值进行检查。如果该值是0，说明该信号量无效。如果该值大于0，说明该信号量有效，同时该值也暗示着该信号量当前
 可用的资源数。应该注意的是，这些可用资源中，已经被该调用函数自身占用了一个（该计数值已经被减1）。中断服务子程序要请求信号量时，只能用 OSSemAccept()而不能用OSSemPend()，因为中断服务子程序是不允许等待的。
3.5查询一个信号量的当前状态, OSSemQuery()
INT8U OSSemQuery (OS_EVENT *pevent, OS_SEM_DATA *pdata)
在应用程序中，用户随时可以调用函数OSSemQuery()来查询一个信号量的当前状态。该函数有两个参数：一个是指向信号量对应事件控制块的指针 pevent。该指针是在生产信号量时，由OSSemCreate()函数返回的；另一个是指向用于记录信号量信息的数据结构OS_SEM_DATA（见 uCOS_II.H）的指针pdata。因此，调用该函数前，用户必须先定义该结构变量，用于存储信号量的有关信息。在这里，之所以使用一个新的数据结构 的原因在于，调用函数应该只关心那些和特定信号量有关的信息，而不是象OS_EVENT数据结构包含的很全面的信息。该数据结构只包含信号量计数
 值.OSCnt和等待任务列表.OSEventTbl[]、.OSEventGrp，而OS_EVENT中还包含了另外的两个域.OSEventType 和.OSEventPtr。
4． 时间类API的设计思路和实现机制
μC/OS-Ⅱ(其它内核也一样)要求用户提供定时中断来实现延时与超时控制等功能。这个定时中断叫做时钟节拍，它应该每秒发生10至100次。时钟节拍的实际频率是由用户的应用程序决定的。时钟节拍的频率越高，系统的负荷就越重。
下面主要讲述五个与时钟节拍有关的API函数。
4.1 任务延时函数，OSTimeDly()
void OSTimeDly (INT16U ticks)
这应该程序员们调用最多的一个函数了，这个函数完成功能很简单，就是先挂起当起当前任务，然后进行任务切换，在指定的时间到来之后，将当前任务恢复为 就绪状态，但是并不一定运行，如果恢复后是优先级最高就绪任务的话，那么运行之。简单点说，就是可以任务延时一定时间后再次执行它，或者说，暂时放弃 CPU的使用权。一个任务可以不显式的调用这些可以导致放弃CPU使用权的API，但那样多任务性能会大大降低，因为此时仅仅依靠时钟机制在进行任务切 换。一个好的任务应该在完成一些操作主动放弃使用权。
4.2 按时分秒延时函数 OSTimeDlyHMSM()
INT8U OSTimeDlyHMSM (INT8U hours, INT8U minutes, INT8U seconds, INT16U milli)
OSTimeDly()虽然是一个非常有用的函数，但用户的应用程序需要知道延时时间对应的时钟节拍的数目。增加了OSTimeDlyHMSM()函 数后，用户就可以按小时(H)、分(M)、秒(S)和毫秒(m)来定义时间了，这样会显得更自然些。与OSTimeDly()一样，调用 OSTimeDlyHMSM()函数也会使μC/OS-Ⅱ进行一次任务调度，并且执行下一个优先级最高的就绪态任务。任务调用 OSTimeDlyHMSM()后，一旦规定的时间期满或者有其它的任务通过调用OSTimeDlyResume()取消了延时，它就会马上处于就绪态。
 同样，只有当该任务在所有就绪态任务中具有最高的优先级时，它才会立即运行。
4.3 让处在延时期的任务结束延时，OSTimeDlyResume()
INT8U OSTimeDlyResume (INT8U prio)
μC/OS-Ⅱ允许用户结束延时正处于延时期的任务。延时的任务可以不等待延时期满，而是通过其它任务取消延时来使自己处于就绪态。这可以通过调用 OSTimeDlyResume()和指定要恢复的任务的优先级来完成。实际上，OSTimeDlyResume()也可以唤醒正在等待事件的任务，虽然 这一点并没有提到过。在这种情况下，等待事件发生的任务会考虑是否终止等待事件。
4.4 系统时间，OSTimeGet()和OSTimeSet()
INT32U OSTimeGet (void)
void OSTimeSet (INT32U ticks)
用户可以通过调用OSTimeGet()来获得该计数器的当前值。也可以通过调用OSTimeSet()来改变该计数器的值。注意，在访问 OSTime的时候中断是关掉的。这是因为在大多数8位处理器上增加和拷贝一个32位的数都需要数条指令，这些指令一般都需要一次执行完毕，而不能被中断 等因素打断。
5． 临界区类API的设计思路和实现机制
和其它内核一样，μC/OS-Ⅱ为了处理临界段代码需要关中断，处理完毕后再开中断。这使得μC/OS-Ⅱ能够避免同时有其它任务或中断服务进入临界段代码。
μC/OS-Ⅱ定义两个宏(macros)来关中断和开中断，以便避开不同C编译器厂商选择不同的方法来处理关中断和开中断。μC/OS-Ⅱ中的这两 个宏调用分别是：OS_ENTER_CRITICAL()和OS_EXIT_CRITICAL()。因为这两个宏的定义取决于所用的微处理器，故在文件 OS_CPU.H中可以找到相应宏定义。每种微处理器都有自己的OS_CPU.H文件。
5.1 OS_ENTER_CRITICAL宏
很多人都以为它是个函数，其实不然，仔细分析一下OS_CPU.H文件，它和下面马上要谈到的OS_EXIT_CRITICAL都是宏。他们都是涉及 特定CPU的实现。一般都被替换为一条或者几条嵌入式汇编代码。由于系统希望向上层程序员隐藏内部实现，故而一般都宣称执行此条指令后系统进入临界区。其 实，它就是关个中断而已。这样，只要任务不主动放弃CPU使用权，别的任务就没有占用CPU的机会了，相对这个任务而言，它就是独占了。所以说进入临界区 了。这个宏能少用还是少用，因为它会破坏系统的一些服务，尤其是时间服务。并使系统对外界响应性能降低。
5.2 OS_EXIT_CRITICAL宏
这个是和上面介绍的宏配套使用另一个宏，它在系统手册里的说明是退出临界区。其实它就是重新开中断。需要注意的是，它必须和上面的宏成对出现，否则会 带来意想不到的后果。最坏的情况下，系统会崩溃。我们推荐程序员们尽量少使用这两个宏调用，因为他们的确会破坏系统的多任务性能。
6． 结束语
通过对μC/OS-II中这几类API的简单分析，我们可以看出μC/OS-II作为一个多任务、抢占式嵌入式操作系统，其API都是为多任务及其多 任务之间的调度和通信的实现来设计的。μC/OS-II提供的API简洁而灵活，使用户在设计实际应用时能够很方便的利用系统提供的各种调用，充分发挥 μC/OS-II实时、高效的优点。

