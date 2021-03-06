
# UC-OS-II内核调度分析 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:18:43[seven-soft](https://me.csdn.net/softn)阅读数：1199


一．内核概述：
多任务系统中，内核负责管理各个任务，或者说为每个任务分配CPU时间，并且负责任务之间的通讯。内核提供的基本服务是任务切换。之所以使用实时内 核可以大大简化应用系统的设计，是因为实时内核允许将应用分成若干个任务，由实时内核来管理它们。内核本身也增加了应用程序的额外负荷，代码空间增加 ROM的用量，内核本身的数据结构增加了RAM的用量。但更主要的是，每个任务要有自己的栈空间，这一块吃起内存来是相当厉害的。内核本身对CPU的占用 时间一般在2到5个百分点之间。
UC/OS-II有一个精巧的内核调度算法,实时内核精小，执行效率高，算法巧妙,代码空间很少。
二．UC/OS-II内核调度特点：
1．只支持基于优先级的抢占式调度算法，不支持时间片轮训；
2．64个优先级,只能创建64个任务,用户只能创建56个任务；
3．每个任务优先级都不相同。
4．不支持优先级逆转；
5．READY队列通过内存映射表实现快速查询。效率非常高；
6．支持时钟节拍；
7．支持信号量，消息队列，事件控制块，事件标志组，消息邮箱任务通讯机制；
8．支持中断嵌套，中断嵌套层数可达255层，中断使用当前任务的堆栈保存上下文；
9．每个任务有自己的堆栈，堆栈大小用户自己设定；
10．支持动态修改任务优先级；
11．任务TCB为静态数组，建立任务只是从中获得一个TCB，不用动态分配，释放内存；
12．任务堆栈为用户静态或者动态创建，在任务创建外完成，任务创建本身不进行动态内存分配；
13．任务的总个数（OS_MAX_TASKS）由用户决定；
14．0优先级最高，63优先级最低；
15．有一个优先级最低的空闲任务,在没有用户任务运行的时候运行.
三．任务控制块OS_TCB描述：
UC/OS-II的TCB数据结构简单，内容容易理解，保存最基本的任务信息，同时还支持裁减来减小内存消耗，TCB是事先根据用户配置，静态分配 内存的结构数组，通过优先级序号进行添加，查找，删除等功能。减少动态内存分配和释放。因为依靠优先级进行TCB分配，每个任务必须有自己的优先级，不能 和其他任务具有相同的优先级。
typedefstructos_tcb
{
OS_STK*OSTCBStkPtr;
\#ifOS_TASK_CREATE_EXT_EN
void*OSTCBExtPtr;
OS_STK*OSTCBStkBottom;
INT32UOSTCBStkSize;
INT16UOSTCBOpt;
INT16UOSTCBId;
\#endif
structos_tcb*OSTCBNext;
structos_tcb*OSTCBPrev;
\#if(OS_Q_EN&&(OS_MAX_QS>=2))||OS_MBOX_EN||OS_SEM_EN
OS_EVENT*OSTCBEventPtr;
\#endif
\#if(OS_Q_EN&&(OS_MAX_QS>=2))||OS_MBOX_EN
void*OSTCBMsg;
\#endif
INT16UOSTCBDly;
INT8UOSTCBStat;
INT8UOSTCBPrio;
INT8UOSTCBX;
INT8UOSTCBY;
INT8UOSTCBBitX;
INT8UOSTCBBitY;
\#ifOS_TASK_DEL_EN
BOOLEANOSTCBDelReq;
\#endif
}OS_TCB;
.OSTCBStkPtr是指向当前任务栈顶的指针。
.*OSTCBExtPtr;：任务扩展模块使用；
.*OSTCBStkBottom;
.OSTCBStkSize;.
.OSTCBOpt;
.OSTCBId;
.OSTCBNext和.OSTCBPrev用于任务控制块OS_TCBs的双重链接，
.OSTCBEventPtr是指向事件控制块的指针
.OSTCBMsg是指向传给任务的消息的指针。
.OSTCBDly当需要把任务延时若干时钟节拍时要用到这个变量，或者需要把任务挂起一段时间以等待某事件的发生，
.OSTCBStat是任务的状态字。
.OSTCBPrio是任务优先级。
.OSTCBX,.OSTCBY,.OSTCBBitX和.OSTCBBitY用于加速任务进入就绪态的过程或进入等待事件发生状态的过程
OSTCBY=priority>>3;
OSTCBBitY=OSMapTbl[priority>>3];
OSTCBX=priority&0x07;
OSTCBBitX=OSMapTbl[priority&0x07];
.OSTCBDelReq是一个布尔量，用于表示该任务是否需要删除
四．就绪表（ReadyList）：
UC/OS-II采用内存映射的方式来实现READY队列的加入，查找，删除功能，效率非常高。但是也因此只能支持64个任务，每个任务都有自己的优先级，不能和其他任务优先级向同。
每个任务的就绪态标志都放入就绪表中的，就绪表中有两个变量OSRdyGrp和OSRdyTbl[]。在OSRdyGrp中，任务按优先级分组，8 个任务为一组。OSRdyGrp中的每一位表示8组任务中每一组中是否有进入就绪态的任务。任务进入就绪态时，就绪表OSRdyTbl[]中的相应元素的 相应位也置位。就绪表OSRdyTbl[]数组的大小取决于OS_LOWEST_PRIO(见文件OS_CFG.H)。
为确定下次该哪个优先级的任务运行了，内核调度器总是将OS_LOWEST_PRIO在就绪表中相应字节的相应位置1。OSRdyGrp和OSRdyTbl[]的关系见图3.3，是按以下规则给出的：
当OSRdyTbl[i]中的任何一位是1时，OSRdyGrp的第i位置1。i从0到7。
使任务进入就绪态
OSRdyGrp|=OSMapTbl[prio>>3];
OSRdyTbl[prio>>3]|=OSMapTbl[prio&0x07];
任务优先级的低三位用于确定任务在总就绪表OSRdyTbl[]中的所在位。接下去的三位用于确定是在OSRdyTbl[]数组的第几个元素。 OSMapTbl[]是在ROM中的（见文件OS_CORE.C）屏蔽字，用于限制OSRdyTbl[]数组的元素下标在0到7之间，如果一个任务被删除 了，则用程序清单3.6中的代码做求反处理。
程序清单L3.6从就绪表中删除一个任务
if((OSRdyTbl[prio>>3]&=~OSMapTbl[prio&0x07])==0)
OSRdyGrp&=~OSMapTbl[prio>>3];
以上代码将就绪任务表数组OSRdyTbl[]中相应元素的相应位清零，而对于OSRdyGrp，只有当被删除任务所在任务组中全组任务一个都没有 进入就绪态时，才将相应位清零。也就是说OSRdyTbl[prio>>3]所有的位都是零时，OSRdyGrp的相应位才清零。为了找到那 个进入就绪态的优先级最高的任务，并不需要从OSRdyTbl[0]开始扫描整个就绪任务表，只需要查另外一张表，即优先级判定表 OSUnMapTbl([256])(见文件OS_CORE.C)。OSRdyTbl[]中每个字节的8位代表这一组的8个任务哪些进入就绪态了，低位的
 优先级高于高位。利用这个字节为下标来查OSUnMapTbl这张表，返回的字节就是该组任务中就绪态任务中优先级最高的那个任务所在的位置。这个返回值 在0到7之间。确定进入就绪态的优先级最高的任务是用以下代码完成的。
找出进入就绪态的优先级最高的任务
y=OSUnMapTbl[OSRdyGrp];
x=OSUnMapTbl[OSRdyTbl[y]];
prio=(y<<3)+x;
例如，如果OSRdyGrp的值为二进制01101000，查OSUnMapTbl[OSRdyGrp]得到的值是3，它相应于OSRdyGrp中 的第3位bit3，这里假设最右边的一位是第0位bit0。类似地，如果OSRdyTbl[3]的值是二进制11100100,则 OSUnMapTbl[OSRdyTbc[3]]的值是2，即第2位。于是任务的优先级Prio就等于26（3*8+2）。利用这个优先级的值。查任务控 制块优先级表OSTCBPrioTbl[]，得到指向相应任务的任务控制块OS_TCB的工作就完成了。
五．任务状态：
UC/OS-II主要有五种任务状态，睡眠态就是挂起态，阻塞态和延时态这里统一为等待状态。增加了一个被中断状态。UC/OS-Ⅱ总是建立一个空 闲任务，这个任务在没有其它任务进入就绪态时投入运行。这个空闲任务[OSTaskIdle()]永远设为最低优先级空闲任务OSTaskIdle()什 么也不做，只是在不停地给一个32位的名叫OSIdleCtr的计数器加1，统计任务使用这个计数器以确定现行应用软件实际消耗的CPU时间。空闲任务不 可能被应用软件删除。
睡眠态（DORMANT）指任务驻留在程序空间之中，还没有交给μC/OS-Ⅱ管理，把任务交给μC/OS-Ⅱ是通过调用下述两个函数之 一：OSTaskCreate()或OSTaskCreateExt()。当任务一旦建立，这个任务就进入就绪态准备运行。任务的建立可以是在多任务运行 开始之前，也可以是动态地被一个运行着的任务建立。如果一个任务是被另一个任务建立的，而这个任务的优先级高于建立它的那个任务，则这个刚刚建立的任务将 立即得到CPU的控制权。一个任务可以通过调用OSTaskDel()返回到睡眠态，或通过调用该函数让另一个任务进入睡眠态。
调用OSStart()可以启动多任务。OSStart()函数运行进入就绪态的优先级最高的任务。就绪的任务只有当所有优先级高于这个任务的任务转为等待状态，或者是被删除了，才能进入运行态。
正在运行的任务可以通过调用两个函数之一将自身延迟一段时间，这两个函数是OSTimeDly()或OSTimeDlyHMSM()。这个任务于是 进入等待状态，等待这段时间过去，下一个优先级最高的、并进入了就绪态的任务立刻被赋予了CPU的控制权。等待的时间过去以后，系统服务函数 OSTimeTick()使延迟了的任务进入就绪态（见3.10节，时钟节拍）。
正在运行的任务期待某一事件的发生时也要等待，手段是调用以下3个函数之一：OSSemPend()，OSMboxPend()，或 OSQPend()。调用后任务进入了等待状态（WAITING）。当任务因等待事件被挂起（Pend），下一个优先级最高的任务立即得到了CPU的控制 权。当事件发生了，被挂起的任务进入就绪态。事件发生的报告可能来自另一个任务，也可能来自中断服务子程序。
正在运行的任务是可以被中断的，除非该任务将中断关了，或者μC/OS-Ⅱ将中断关了。被中断了的任务就进入了中断服务态（ISR）。响应中断时， 正在执行的任务被挂起，中断服务子程序控制了CPU的使用权。中断服务子程序可能会报告一个或多个事件的发生，而使一个或多个任务进入就绪态。在这种情况 下，从中断服务子程序返回之前，μC/OS-Ⅱ要判定，被中断的任务是否还是就绪态任务中优先级最高的。如果中断服务子程序使一个优先级更高的任务进入了 就绪态，则新进入就绪态的这个优先级更高的任务将得以运行，否则原来被中断了的任务才能继续运行。
当所有的任务都在等待事件发生或等待延迟时间结束，μC/OS-Ⅱ执行空闲任务（idletask），执行OSTaskIdle()函数。
六．任务切换：
ContextSwitch在有的书中翻译成上下文切换，实际含义是任务切换，或CPU寄存器内容切换。当多任务内核决定运行另外的任务时，它保存 正在运行任务的当前状态（Context），即CPU寄存器中的全部内容。这些内容保存在任务的当前状况保存区 （Task’sContextStoragearea），也就是任务自己的栈区之中。（见图2.2）。入栈工作完成以后，就是把下一个将要运行的任务的当 前状况从该任务的栈中重新装入CPU的寄存器，并开始下一个任务的运行。这个过程叫做任务切换。任务切换过程增加了应用程序的额外负荷。CPU的内部寄存
 器越多，额外负荷就越重。做任务切换所需要的时间取决于CPU有多少寄存器要入栈。实时内核的性能不应该以每秒钟能做多少次任务切换来评价。
七．任务调度分析：
μC/OS-Ⅱ提供最简单的实时内核任务调度，算法简单，因此也只支持优先级抢占任务调度，不支持时间片轮训调度算法,不支持优先级逆转。
μC/OS-Ⅱ总是运行进入就绪态任务中优先级最高的那一个。确定哪个任务优先级最高，下面该哪个任务运行了的工作是由调度器 （Scheduler）完成的。任务级的调度是由函数OSSched()完成的。中断级的调度是由另一个函数OSIntExt()完成的，这个函数将在以 后描述。
μC/OS-Ⅱ任务调度所花的时间是常数，与应用程序中建立的任务数无关。
在μC/OS中曾经是先得到OSTCBHighRdy然后和OSTCBCur做比较。因为这个比较是两个指针型变量的比较，在8位和一些16位微处 理器中这种比较相对较慢。而在μC/OS-Ⅱ中是两个整数的比较。并且，除非用户实际需要做任务切换，在查任务控制块优先级表 OSTCBPrioTbl[]时，不需要用指针变量来查OSTCBHighRdy。综合这两项改进，即用整数比较代替指针的比较和当需要任务切换时再查 表，使得μC/OS-Ⅱ比μC/OS在8位和一些16位微处理器上要更快一些。
为实现任务切换，OSTCBHighRdy必须指向优先级最高的那个任务控制块OS_TCB，这是通过将以OSPrioHighRdy为下标的 OSTCBPrioTbl[]数组中的那个元素赋给OSTCBHighRdy来实现的[L3.8(4)]。最后宏调用OS_TASK_SW()来完成实际 上的任务切换[L3.8(6)]。
任务切换很简单，由以下两步完成，将被挂起任务的微处理器寄存器推入堆栈，然后将较高优先级的任务的寄存器值从栈中恢复到寄存器中。在μC/OS- Ⅱ中，就绪任务的栈结构总是看起来跟刚刚发生过中断一样，所有微处理器的寄存器都保存在栈中。换句话说，μC/OS-Ⅱ运行就绪态的任务所要做的一切，只 是恢复所有的CPU寄存器并运行中断返回指令。为了做任务切换，运行OS_TASK_SW(),人为模仿了一次中断。多数微处理器有软中断指令或者陷阱指 令TRAP来实现上述操作。中断服务子程序或陷阱处理（Traphardler），也称作事故处理（exceptionhandler），必须提供中断向
 量给汇编语言函数OSCtxSw()。OSCtxSw()除了需要OS_TCBHighRdy指向即将被挂起的任务，还需要让当前任务控制块 OSTCBCur指向即将被挂起的任务。
OSSched()的所有代码都属临界段代码。在寻找进入就绪态的优先级最高的任务过程中，为防止中断服务子程序把一个或几个任务的就绪位置位，中 断是被关掉的。为缩短切换时间，OSSched()全部代码都可以用汇编语言写。为增加可读性，可移植性和将汇编语言代码最少化，OSSched()是用 C写的。
任务切换的相关函数:与CPU体系相关，汇编完成。
1.OSStartHighRdy()执行优先级最高的任务
2.OSCtxSw()完成任务的上下文切换
3.OSIntCtxSw()中断后的上下文切换
4.OSTickISR()中断服务程序启动
八．UC/OS-II的初始化：
OSInit()建立空闲任务idletask，这个任务总是处于就绪态的。空闲任务OSTaskIdle（）的优先级总是设成最低。
这两个任务的任务控制块（OS_TCBs）是用双向链表链接在一起的。OSTCBList指向这个链表的起始处。当建立一个任务时，这个任务总是被放在这个链表的起始处。换句话说，OSTCBList总是指向最后建立的那个任务。链的终点指向空字符NULL(也就是零)。
因为这两个任务都处在就绪态，在就绪任务表OSRdyTbl[]中的相应位是设为1的。还有，因为这两个任务的相应位是在OSRdyTbl[]的同一行上，即属同一组，故OSRdyGrp中只有1位是设为1的。
μC/OS-Ⅱ还初始化了4个空数据结构缓冲区，如图F3.8所示。每个缓冲区都是单向链表，允许μC/OS-Ⅱ从缓冲区中迅速得到或释放一个缓冲区中的元素。控制块OS_TCB的数目也就自动确定了。当然，包括足够的任务控制块分配给统计任务和空闲任务。
UC/OS-II内核调度分析vxWorks内核调度分析
1．只支持基于优先级的抢占式调度算法，不支持时间片轮训；2．64个优先级,只能创建64个任务,用户只能创建56个任务；3．每个任务优先级都 不相同。4．不支持优先级逆转；5．READY队列通过内存映射表实现快速查询。效率非常高；6．支持时钟节拍；7．支持信号量，消息队列，事件控制块， 事件标志组，消息邮箱任务通讯机制；8．支持中断嵌套，中断嵌套层数可达255层，中断使用当前任务的堆栈保存上下文；9．每个任务有自己的堆栈，堆栈大 小用户自己设定；10．支持动态修改任务优先级；11．任务TCB为静态数组，建立任务只是从中获得一个TCB，不用动态分配，释放内存；12．任务堆栈
 为用户静态或者动态创建，在任务创建外完成，任务创建本身不进行动态内存分配；13．任务的总个数（OS_MAX_TASKS）由用户决定；14．0优先 级最高，63优先级最低；15．有一个优先级最低的空闲任务,在没有用户任务运行的时候运行.1．根据用户指定，动态分配堆栈，可以创建任意多个任 务；2．任务的优先级从0——255，0优先级最高，允许多个任务相同优先级；3．支持优先级逆转，TCB保存两个优先级；4．支持抢占与时间片轮训的任 务调度方式；5．支持中断嵌套，中断使用专用的堆栈保存上下文；6．队列采用FIFO或者优先级的双向链表实现；7．系统没有空闲任务执行；8．采用工作
 队列workQword的方式调度；9．任务是基于类，对象的管理方式；10．任务的TCB保存在任务的堆栈里；11．每个任务有自己的堆栈，堆栈大小用 户自己设定；12．支持动态修改任务优先级；13．通过编译开关实现对多cpu体系结构的支持。

