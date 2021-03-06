
# uC-OS-II的任务管理和调度 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 09:27:07[seven-soft](https://me.csdn.net/softn)阅读数：943



### 任务管理
一个任务时一个完整的功能程序。该程序可认为CPU完全属于自己。每个任务被赋予一定的优先级，并拥有自己的一套CPU寄存器和栈空间。任务处于非运行态时，该任务被保存到自己的栈空间。当被调度处于运行状态时，任务的现场从栈空间恢复到CPU寄存器中运行。
uC/OS-II最多可以管理64个任务。为了简化设计，uC/OS规定各任务的优先级必须不同。任务的优先级号就是任务编号，任务的优先级号越低，任务的优先级越高。uC/OS-II提供进行管理任务的各种函数包括创建、删除、改变任务优先级，挂起和恢复等。系统初始化时会自动运行两个系统任务：一个是空闲任务OSTaskIdle(),它的优先级最低，该任务在没有其他任务进入就绪态时运行；另外一个任务就是统计任务OSTaskStat（），它的优先级次低，负责计算当前CPU的利用率。
在uC/OS中有任务运行、就绪、等待或挂起、休眠这几个状态。休眠态指任务驻留在程序空间之中，还没有交给uC/OS-II管理。
uC/OS-II为每个任务分配了一个任务控制块（OS_TCB)，与大多数操作系统控制块类似，uC/OS-II的TCB中包含了任务运行和管理的信息。这样，当此任务处于挂起等状态时，内核就把该任务相关的状态信息保存到TCB中。当任务重新被调度执行时，该任务TCB中的信息就被调入CPU寄存器中，该任务得以继续运行。
uC/OS中所有的TCB都通过*OSTCBNext和*OSTCBPrev链接成一个用指针OSTCBFreeList指向的空闲的TCB链表。当任务创建的时候，就从该空闲链表中取出一个空闲TCB并初始化。

### 任务调度
uC/OS-II是 可占先式内核，采用的是最高优先级调度算法，该算法的关键是如何在最短的时间内找到具有最高优先级的任务，以便进行任务切换。对于最高优先级任务的查找， 最简单且易于实现的方法就是顺序检索：将所有任务按优先级从高到低进行排序，查找时从队列开始检索，遇到的第一个就绪的任务就是最高优先级的就绪任务。uC/OS利用哈希表来定位最高优先级的就绪任务。
uC/OS-II最多可管理64个任务，用一个8字节的数组OSRdyTB[8]来实现。每个任务对应于该数组中的一位，同时每一组的就绪状态又对应于一个8位变量
OSRdyGrp中的一位。在数组OSRdyTbl[]中，每个字节的8位表示对应的8个任务是否就绪，就绪就置1.同时，只要每组中有任意一个任务就绪，OSRdyGrp中的对应为也置1。数组OSRdyTbl[]和变量OSRdyGrp构成任务就绪表。内核主要是通过操作OSRdyTbl[]和OSRdyGrp来实现任务的调度。
OSPrioHighRdy就是处于就绪态的最高优先级任务的优先级号。为了找到那个进入就绪态的优先级最高的任务，并不需要从OSRdyTbl[0]开始扫描整个就绪任务表，只需要查另外一张表，即优先级判定表OSUnMapTbl[256].
我们可以把优先级看做是一种描述符，内核通过它来进行任务操作。但获得任务的优先级并不是最后目的，我们最终想得到的是任务的控制块TCB。uC/OS-II提供了一个任务控制块优先表OSTCBPrioTbl[]。TCBPrioTbl[]是个指针数组，根据数组的下标号来存放指向具有对应的优先级号TCB的指针。例如，指向任务优先级为6的ＴＣＢ的指针就存放在OSTCBPrioTbl[6]里面。最高优先级任务的找寻可以如下实现：
OSTCBHighRdy=OSTCBPrioTbl[OSPrioHighrdy]
最高优先级任务的找寻是通过建立就绪任务表来实现的。uC/OS-II中的每一个任务都有独立的堆栈空间和任务控制块TCB，任务控制块的第一个成员变量就是保存的任务堆栈指针。任务调度模块首先用变量OSTCBHighRdy来记录当前最高优先级就绪任务的TCB地址，然后调用OS_TASK_SW()函数来进行任务切换。

