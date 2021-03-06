
# JConsole 使用总结 - 阳光岛主 - CSDN博客

2013年08月24日 00:00:23[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3862



JVM调优工具：Jconsole，jProfile，VisualVM
**1） Jconsole  ：**jdk自带，功能简单，可以在系统有一定负荷的情况下使用；对垃圾回收算法有很详细的跟踪。
**2） JProfiler**：商业软件，需要付费；功能强大。
**3） VisualVM**：JDK自带，功能强大，与JProfiler类似；推荐。
**如何调优**观察内存释放情况、集合类检查、对象树，上面这些调优工具都提供了强大的功能，但是总的来说一般分为以下几类功能
**堆信息查看**
![](http://dl.iteye.com/upload/picture/pic/51401/7b7ece1a-1596-3240-a37d-c3a7d06e2c01.png)
可查看堆空间大小分配（年轻代、年老代、持久代分配）
提供即时的垃圾回收功能
垃圾监控（长时间监控回收情况）
![](http://dl.iteye.com/upload/picture/pic/51403/48059c43-43ff-3d91-8699-78e6ea8af8a6.png)
查看堆内类、对象信息查看：数量、类型等

![](http://dl.iteye.com/upload/picture/pic/51405/dc26b52b-62d5-320d-a627-6a88e6b57d8f.png)
对象引用情况查看

有了堆信息查看方面的功能，我们一般可以顺利解决以下问题：
1）  --年老代年轻代大小划分是否合理
2）--内存泄漏
3）--垃圾回收算法设置是否合理

**线程监控**
![](http://dl.iteye.com/upload/picture/pic/51407/4e7705f6-75f6-3549-8976-dce68396bbc8.png)

线程信息监控：系统线程数量。
线程状态监控：各个线程都处在什么样的状态下
![](http://dl.iteye.com/upload/picture/pic/51409/c9486ed8-90b4-3a46-96f3-7aaa97ace11f.png)

Dump线程详细信息：查看线程内部运行情况
死锁检查
**热点分析**
![](http://dl.iteye.com/upload/picture/pic/51413/ece5e1f6-d7a6-3aa6-96d0-5f9d43f69808.png)
CPU热点：检查系统哪些方法占用的大量CPU时间
内存热点：检查哪些对象在系统中数量最大（一定时间内存活对象和销毁对象一起统计）
这两个东西对于系统优化很有帮助。我们可以根据找到的热点，有针对性的进行系统的瓶颈查找和进行系统优化，而不是漫无目的的进行所有代码的优化。
**快照**
快照是系统运行到某一时刻的一个定格。在我们进行调优的时候，不可能用眼睛去跟踪所有系统变化，依赖快照功能，我们就可以进行系统两个不同运行时刻，对象（或类、线程等）的不同，以便快速找到问题
举例说，我要检查系统进行垃圾回收以后，是否还有该收回的对象被遗漏下来的了。那么，我可以在进行垃圾回收前后，分别进行一次堆情况的快照，然后对比两次快照的对象情况。
**内存泄漏检查**内存泄漏是比较常见的问题，而且解决方法也比较通用，这里可以重点说一下，而线程、热点方面的问题则是具体问题具体分析了。
内存泄漏一般可以理解为系统资源（各方面的资源，堆、栈、线程等）在错误使用的情况下，导致使用完毕的资源无法回收（或没有回收），从而导致新的资源分配请求无法完成，引起系统错误。
内存泄漏对系统危害比较大，因为他可以直接导致系统的崩溃。
需要区别一下，内存泄漏和系统超负荷两者是有区别的，虽然可能导致的最终结果是一样的。内存泄漏是用完的资源没有回收引起错误，而系统超负荷则是系统确实没有那么多资源可以分配了（其他的资源都在使用）。
**年老代堆空间被占满**
**异常：**java.lang.OutOfMemoryError: Java heap space
**说明：**
![](http://dl.iteye.com/upload/picture/pic/51415/49464252-97ea-3ce2-b433-d9088bafb70a.png)
这是最典型的内存泄漏方式，简单说就是所有堆空间都被无法回收的垃圾对象占满，虚拟机无法再在分配新空间。
如上图所示，这是非常典型的内存泄漏的垃圾回收情况图。所有峰值部分都是一次垃圾回收点，所有谷底部分表示是一次垃圾回收后剩余的内存。连接所有谷底的点，可以发现一条由底到高的线，这说明，随时间的推移，系统的堆空间被不断占满，最终会占满整个堆空间。因此可以初步认为系统内部可能有内存泄漏。（上面的图仅供示例，在实际情况下收集数据的时间需要更长，比如几个小时或者几天）
**解决：**
这种方式解决起来也比较容易，一般就是根据垃圾回收前后情况对比，同时根据对象引用情况（常见的集合对象引用）分析，基本都可以找到泄漏点。
**持久代被占满**
**异常：**java.lang.OutOfMemoryError: PermGen space
**说明：**
Perm空间被占满。无法为新的class分配存储空间而引发的异常。这个异常以前是没有的，但是在Java反射大量使用的今天这个异常比较常见了。主要原因就是大量动态反射生成的类不断被加载，最终导致Perm区被占满。
更可怕的是，不同的classLoader即便使用了相同的类，但是都会对其进行加载，相当于同一个东西，如果有N个classLoader那么他将会被加载N次。因此，某些情况下，这个问题基本视为无解。当然，存在大量classLoader和大量反射类的情况其实也不多。
**解决：**
1. -XX:MaxPermSize=16m
2. 换用JDK。比如JRocket。
**堆栈溢出**
**异常：**java.lang.StackOverflowError
**说明：**这个就不多说了，一般就是递归没返回，或者循环调用造成
**线程堆栈满**
**异常**：Fatal: Stack size too small
**说明**：java中一个线程的空间大小是有限制的。JDK5.0以后这个值是1M。与这个线程相关的数据将会保存在其中。但是当线程空间满了以后，将会出现上面异常。
**解决**：增加线程栈大小。-Xss2m。但这个配置无法解决根本问题，还要看代码部分是否有造成泄漏的部分。
**系统内存被占满**
**异常**：java.lang.OutOfMemoryError: unable to create new native thread
**说明**：
这个异常是由于操作系统没有足够的资源来产生这个线程造成的。系统创建线程时，除了要在Java堆中分配内存外，操作系统本身也需要分配资源来创建线程。因此，当线程数量大到一定程度以后，堆中或许还有空间，但是操作系统分配不出资源来了，就出现这个异常了。
分配给Java虚拟机的内存愈多，系统剩余的资源就越少，因此，当系统内存固定时，分配给Java虚拟机的内存越多，那么，系统总共能够产生的线程也就越少，两者成反比的关系。同时，可以通过修改-Xss来减少分配给单个线程的空间，也可以增加系统总共内生产的线程数。
**解决：**
1. 重新设计系统减少线程数量。
2. 线程数量不能减少的情况下，通过-Xss减小单个线程大小。以便能生产更多的线程。

**垃圾回收的悖论**
所谓“成也萧何败萧何”。Java的垃圾回收确实带来了很多好处，为开发带来了便利。但是在一些高性能、高并发的情况下，垃圾回收确成为了制约Java应用的瓶颈。目前JDK的垃圾回收算法，始终无法解决垃圾回收时的暂停问题，因为这个暂停严重影响了程序的相应时间，造成拥塞或堆积。这也是后续JDK增加G1算法的一个重要原因。
当然，上面是从技术角度出发解决垃圾回收带来的问题，但是从系统设计方面我们就需要问一下了：
<p style=">我们需要分配如此大的内存空间给应用吗？
我们是否能够通过有效使用内存而不是通过扩大内存的方式来设计我们的系统呢？

**我们的内存中都放了什么**
内存中需要放什么呢？个人认为，**内存中需要放的是你的应用需要在不久的将来再次用到到的东西**。想想看，如果你在将来不用这些东西，何必放内存呢？放文件、数据库不是更好？这些东西一般包括：
1. 系统运行时业务相关的数据。比如web应用中的session、即时消息的session等。这些数据一般在一个用户访问周期或者一个使用过程中都需要存在。
2. 缓存。缓存就比较多了，你所要快速访问的都可以放这里面。其实上面的业务数据也可以理解为一种缓存。
3.  线程。
因此，我们是不是可以这么认为，如果我们不把业务数据和缓存放在JVM中，或者把他们独立出来，那么Java应用使用时所需的内存将会大大减少，同时垃圾回收时间也会相应减少。
我认为这是可能的。
**解决之道****数据库、文件系统**
把所有数据都放入数据库或者文件系统，这是一种最为简单的方式。在这种方式下，Java应用的内存基本上等于处理一次峰值并发请求所需的内存。数据的获取都在每次请求时从数据库和文件系统中获取。也可以理解为，一次业务访问以后，所有对象都可以进行回收了。
这是一种内存使用最有效的方式，但是从应用角度来说，这种方式很低效。
**内存-硬盘映射**
上面的问题是因为我们使用了文件系统带来了低效。但是如果我们不是读写硬盘，而是写内存的话效率将会提高很多。
数据库和文件系统都是实实在在进行了持久化，但是当我们并不需要这样持久化的时候，我们可以做一些变通——把内存当硬盘使。
内存-硬盘映射很好很强大，既用了缓存又对Java应用的内存使用又没有影响。Java应用还是Java应用，他只知道读写的还是文件，但是实际上是内存。
这种方式兼得的Java应用与缓存两方面的好处。memcached的广泛使用也正是这一类的代表。
**同一机器部署多个JVM**
这也是一种很好的方式，可以分为纵拆和横拆。纵拆可以理解为把Java应用划分为不同模块，各个模块使用一个独立的Java进程。而横拆则是同样功能的应用部署多个JVM。
通过部署多个JVM，可以把每个JVM的内存控制一个垃圾回收可以忍受的范围内即可。但是这相当于进行了分布式的处理，其额外带来的复杂性也是需要评估的。另外，也有支持分布式的这种JVM可以考虑，不要要钱哦：）
**程序控制的对象生命周期**
这种方式是理想当中的方式，目前的虚拟机还没有，纯属假设。即：考虑由编程方式配置哪些对象在垃圾收集过程中可以直接跳过，减少垃圾回收线程遍历标记的时间。
这种方式相当于在编程的时候告诉虚拟机某些对象你可以在*时间后在进行收集或者由代码标识可以收集了（类似C、C++），在这之前你即便去遍历他也是没有效果的，他肯定是还在被引用的。
这种方式如果JVM可以实现，个人认为将是一个飞跃，Java即有了垃圾回收的优势，又有了C、C++对内存的可控性。
**线程分配**
Java的阻塞式的线程模型基本上可以抛弃了，目前成熟的NIO框架也比较多了。阻塞式IO带来的问题是线程数量的线性增长，而NIO则可以转换成为常数线程。因此，对于服务端的应用而言，NIO还是唯一选择。不过，JDK7中为我们带来的AIO是否能让人眼前一亮呢？我们拭目以待。
**其他的JDK**
本文说的都是Sun的JDK，目前常见的JDK还有JRocket和IBM的JDK。其中JRocket在IO方面比Sun的高很多，不过Sun JDK6.0以后提高也很大。而且JRocket在垃圾回收方面，也具有优势，其可设置垃圾回收的最大暂停时间也是很吸引人的。不过，系统Sun的G1实现以后，在这方面会有一个质的飞跃。


[Using jconsole](http://docs.oracle.com/javase/1.5.0/docs/guide/management/jconsole.html)
[JVM调优工具](http://pengjiaheng.iteye.com/blog/552456)



