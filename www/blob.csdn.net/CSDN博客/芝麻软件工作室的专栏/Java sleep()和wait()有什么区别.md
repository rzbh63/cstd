
# Java sleep()和wait()有什么区别 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:45:15[seven-soft](https://me.csdn.net/softn)阅读数：191个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



sleep()方法是使线程停止一段时间的方法。在sleep 时间间隔期满后，线程不一定立即恢复执行。这是因为在那个时刻，其它线程可能正在运行而且没有被调度为放弃执行，除非(a)“醒来”的线程具有更高的优先级，(b)正在运行的线程因为其它原因而阻塞。
wait()是线程交互时，如果线程对一个同步对象x 发出一个wait()调用，该线程会暂停执行，被调对象进入等待状态，直到被唤醒或等待时间到。

