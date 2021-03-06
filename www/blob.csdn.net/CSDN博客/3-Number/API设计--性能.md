
# API设计--性能 - 3-Number - CSDN博客


2017年11月08日 10:16:18[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：230


# 0x01 缘由
前面学习了一些API编写的原则，作为后期的指导作用。今天关注一些API性能细节和分析方法。
# 0x02 API考虑的性能指标和原则
1.编译时速度。
2.运行时速度。
3.运行时内存开销。
4.库的大小。
5.启动时间。
一个重要的性能优化经验是：绝不应该相信你的“哪些部分会比较慢”直觉，而是应该在实际环境中测量API的真实性能概况，然后把优化精力集中在影响最大的部分。进一步说是，没必要一开始就实现最高效的API：先用一种最简单的方式实现，在一切正常工作后，再找出需要优化的部分。
为优化API，应使用工具手机代码在真实运行实例中的性能数据，然后把优化精力集中在实际的瓶颈上，不要猜测性能瓶颈的位置。
# 0x03 编程时一些注意事项
# 1.通过const 应用传递输入参数（对自定义对象）
主要是复制构造函数和析构函数的开销。应该通过const应用而非传值的方式传递不会改变的对象。这样可以避免创建和销毁对象的临时副本，及副本中所有的成员和继承对象的内存与性能开销。
防止切割问题：基类指针指向一个派生类对象 ，然后这个指针被函数传值调用，那么拷贝构造只复制了该对象的基类部分。
## 2.最小化\#include依赖
头文件A包含另一个投文件B，是为了引入在头文件A中使用到的类、函数、结构体、枚举或其他实体的声明。在面向对象程序中，最常见的情况是头文件A想要引入头文件B中一个或多个类的声明。不过，很多情况下头文件A实际上不必包含头文件B，而只需要提供所需类的前置声即可。
前置声明，什么情况下使用前置声明：
（1）不需要知道类的大小。如果包含的类要作为程序变量或打算从包含类派生子类，那么编译器需要知道类的大小。
（2）没有引用类的任何成员变量和陈源方法。引用类的成员方法需要知道方法原型，即参数和返回值类型。
## 3.声明常量
应使用extern声明全局作用域的常量，或者在类中以静态const方式声明常量，然后再.cpp文件中定义常量值。这样减少了包含这些头文件的模块的目标文件大小。更可取的方法是将这些常量隐藏在函数调用背后。
新标准constexpr ，标识已知为恒定不变的函数活变量，以便编译器执行更好的优化。
## 4.初始化列表
使用构造函数初始化列表，从而为每个数据成员减少一次调用构造函数的开销。这些应在.cpp文件中声明，以便隐藏实现细节。成员变量在构造函数的函数体调用之前构造。
初始化列表注意事项：
（1）初始化列表中的变量顺序必须与类中指定的顺序一致。
（2）不能在初始化列表汇总指定数组，但是可以指定std::verctor。
（3）如果把成员变量声明为应用或者const，那么就必须通过初始化列表来初始化他们，从而避免默认构造函数定义其恒定不变的唯一初始值。
## 5.写时复制
节省内存最好的办法之一是到确实需要时再分配。这是写时复制技巧的本质目标。其方法是允许所有客户共享一份唯一的资源，直到他们中的一个需要修改这份资源为止。只有在那个时间点才会构造副本。
## 6.迭代元素
迭代器尽可能使用前自增/减，以避免临时对象的构造和析构。
采用迭代器模型遍历简单的线性数据结构。对于链表或树形数据结构，如果迭代性能很重要，那么应该考虑使用数组引用。
# 0x04 性能分析
考虑性能的几个不同方面：时效性、内存开销、多线程竞争。
## 1.时效性分析
性能最传统的解释是代码执行各项操作所需花费的时间。
（1）内嵌测试量。在代码关键位置插入快速计时器类，手机精确的计时信息。
（2）二进制测量。查看每次函数调用的细节。
（3）采样。借助工具对应用程序CPU花费采样。gpreftools等工具。
（4）监控计时器。Callgrind等工具。
## 2.基于内存的分析
频繁分配并释放内存的算法，以及内存分配模式不能很好适应现代处理器缓存的算法，最终执行效果会比预期慢得多。同事，内存编程缺陷，比如二次释放或访问未分配内存，可能破坏数据并导致程序奔溃；而内存泄露问题会随着时间推移愈发严重，直至耗尽所有可用内存。
工具：valgrind
## 3.多线程分析
借助工具可以帮助你找到代码中的逻辑线程错误，比如静态条件和死锁。他们还可以分析多线程代码的性能，以便发现并发瓶颈。
工具：valgrind
# 0x05 总结
在产品过程也做过一些系统性能优化，但是没有深入到每个API接口，仅仅通过工具分析出了性能热点，然后针对热点进行了一些性能优化。如果还进一步就得深入热点的每个API。




