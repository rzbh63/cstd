# Java虚拟机内存区域划分

 

# Java程序运行时的数据区域

Java虚拟机在执行Java程序的过程中，会把它所管理的内存划分为若干个不同的数据区域，这些区域都有各自的用途，以及创建和销毁的时间，有的区域随着虚拟机进程的启动而存在，有些区域则依赖用户线程的启动和结束而建立和销毁。Java虚拟机所管理的内存主要包含下图所示的几个运行时数据区域。

![jvm Overview](https://img-blog.csdn.net/20180212093903709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzU5NTQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 程序计数器(PC Registers)

程序计数器是一块比较小的内存空间，它可以看作是当前线程所执行的字节码的行号指示器。在虚拟机的概念模型中，字节码解释器工作时就是通过改变这个计数器的值来选取下一条需要执行的字节码指令，分支、循环、跳转、异常处理、线程恢复等基础功能都需要依赖这个计数器来完成。

如果线程正在执行的是一个Java方法，这个计数器记录的是正在执行的虚拟机字节码指令的地址； 
 如果正在执行的是一个Native方法，这个计数器值则为空(Undefined)。

此内存区域是唯一一个在Java虚拟机规范中没有规定任何OutOfMemoryError情况的区域。

# Java虚拟机栈(Java Language Stacks)

与程序计数器一样，Java虚拟机栈也是线程似有的，它的生命周期与线程相同。

虚拟机栈描述的是Java方法执行的内存模型：每个方法在执行的同时都会创建一个栈帧(Stack Frame)用于存储局部变量表、操作数栈、动态链接、方法出口等信息。每一个方法从调用直至执行完成的过程，就对应着一个栈帧在虚拟机栈的入栈到出栈的过程。

经常有人把Java内存分为堆内存(Heap)和栈内存(Stack)，这种分法实际上是非常粗糙的，但是也突出表现出程序员实际上最为关注的两块内存。这里的“栈”则指的是虚拟机栈，或者说是虚拟机栈中局部变量表部分。

局部变量表存放了编译期可知的各种基本数据类型(boolean, byte, char, short, int, float, long, double)、对象引用(reference类型 *它不等同于对象本身，可能是一个指向对象起始地址的引用指针，也可能是指向了一个字节码指令的地址。*)

在Java虚拟机规范中，对这个区域规定了两种异常状况：`StackOverflowError`, `OutofMemoryError`

# 本地方法栈(Native Method Stacks)

本地方法栈与虚拟机栈所发挥的作用是非常类似的，它们的区别不过是虚拟机栈为虚拟机执行Java方法（也就是字节码）服务，而本地方法栈则为虚拟机使用到的Native方法服务。在Sun HotSpot虚拟机中，直接将本地方法栈和虚拟栈合二为一。

与虚拟机栈一样，本地方法栈区域也会抛出`StackOverflowError`和 `OutofMemoryError`异常。

# Java堆(Java Heap)

对于大多数应用来说，Java堆(Java Heap)是Java虚拟机所管理的内存中最大的一块。Java堆是被所有线程共享的一块内存区域，在虚拟机启动时创建。此内存区域的唯一目的就是存放对象的实例，**几乎**所有的对象实例都会在这里分配内存。

> 所有对象实例以及数组都要在堆上分配，但是随着JIT编译器的发展与逃逸分析技术逐渐成熟，栈上分配、标量替换优化技术导致一些微妙的变化发生，所有的对象都分配在堆上也渐渐不是那么“绝对“了。 
>    ——深入理解Java虚拟机：JVM高级特性与最佳实践

Java堆是垃圾收集器管理的主要区域，因此很多时候也称为**GC堆**(Garbage Collected Heap)。

根据Java虚拟机规范的规定，Java堆可以处于物理上不连续的内存空间中，只要逻辑上是连续的即可。因此在实现时，既可以实现成固定大小的，也可以是可扩展的，不过当前主流的虚拟机都是按照可扩展来实现的（通过-Xmx和-Xms控制）。如果在堆中没有内存完成实例分配，且堆也无法再扩展时，将会抛出`OutOfMemoryError`异常。

# 方法区(Method Area)

方法区与Java堆一样，是各个线程共享的内存区域，它用于存储已被虚拟机加载的类信息、常量、静态变量、即时编译器编译后的代码等数据。虽然Java虚拟机规范把方法区描述为堆的一个逻辑部分，但是它却有一个别名叫做Non-Heap，目的是与Java堆区分开来。

根据Java虚拟机规范的规定，当方法区无法满足内存分配需求时，将抛出`OutOfMemoryError`异常。

## 运行时常量池(Runtime Constant Pool)

运行时常量池是方法区的一部分。Class文件中除了有类的版本、字段、方法、接口等描述信息外，还有一项信息就是常量池，用于存放编译期生成的各种字面量和符号引用，这部分内容将在类加载后进入方法区的运行时常量池中存放。

# 直接内存(Direct Memory)

直接内存并不是虚拟机运行时数据区的一部分，也不是Java虚拟机规范中定义的内存区域。但是这部分内存也被频繁的使用，而且也可能导致OutOfMemoryError异常出现。