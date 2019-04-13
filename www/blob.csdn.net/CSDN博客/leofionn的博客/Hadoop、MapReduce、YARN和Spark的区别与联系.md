
# Hadoop、MapReduce、YARN和Spark的区别与联系 - leofionn的博客 - CSDN博客


2018年05月11日 08:27:16[leofionn](https://me.csdn.net/qq_36142114)阅读数：709


（1） Hadoop 1.0
第一代Hadoop，由分布式存储系统HDFS和分布式计算框架MapReduce组成，其中，HDFS由一个NameNode和多个DataNode组成，MapReduce由一个JobTracker和多个TaskTracker组成，对应Hadoop版本为Hadoop 1.x和0.21.X，0.22.x。
（2）  Hadoop 2.0
第二代Hadoop，为克服Hadoop 1.0中HDFS和MapReduce存在的各种问题而提出的。针对Hadoop 1.0中的单NameNode制约HDFS的扩展性问题，提出了HDFS Federation，它让多个NameNode分管不同的目录进而实现访问隔离和横向扩展；针对Hadoop 1.0中的MapReduce在扩展性和多框架支持方面的不足，提出了全新的资源管理框架YARN(Yet Another Resource Negotiator)，它将JobTracker中的资源管理和作业控制功能分开，分别由组件ResourceManager和ApplicationMaster实现，其中，ResourceManager负责所有应用程序的资源分配，而ApplicationMaster仅负责管理一个应用程序。对应Hadoop版本为Hadoop 0.23.x和2.x。
（3） MapReduce 1.0或者MRv1（MapReduceversion 1）
第一代MapReduce计算框架，它由两部分组成：编程模型（programming model）和运行时环境（runtime environment）。它的基本编程模型是将问题抽象成Map和Reduce两个阶段，其中Map阶段将输入数据解析成key/value，迭代调用map()函数处理后，再以key/value的形式输出到本地目录，而Reduce阶段则将key相同的value进行规约处理，并将最终结果写到HDFS上。它的运行时环境由两类服务组成：JobTracker和TaskTracker，其中，JobTracker负责资源管理和所有作业的控制，而TaskTracker负责接收来自JobTracker的命令并执行它。
（4）MapReduce 2.0或者MRv2（MapReduce version 2）或者NextGen MapReduc
MapReduce 2.0或者MRv2具有与MRv1相同的编程模型，唯一不同的是运行时环境。MRv2是在MRv1基础上经加工之后，运行于资源管理框架YARN之上的MRv1，它不再由JobTracker和TaskTracker组成，而是变为一个作业控制进程ApplicationMaster，且ApplicationMaster仅负责一个作业的管理，至于资源的管理，则由YARN完成。
简而言之，MRv1是一个独立的离线计算框架，而MRv2则是运行于YARN之上的MRv1。
（5）Hadoop-MapReduce（一个离线计算框架）
Hadoop是google分布式计算框架MapReduce与分布式存储系统GFS的开源实现，由分布式计算框架MapReduce和分布式存储系统HDFS（Hadoop Distributed File System）组成，具有高容错性，高扩展性和编程接口简单等特点，现已被大部分互联网公司采用。
（6）Hadoop-YARN(Hadoop 2.0的一个分支，实际上是一个资源管理系统)
YARN是Hadoop的一个子项目（与MapReduce并列），它实际上是一个资源统一管理系统，可以在上面运行各种计算框架（包括MapReduce、Spark、Storm、MPI等）。
当前Hadoop版本比较混乱，让很多用户不知所措。实际上，当前Hadoop只有两个版本：Hadoop 1.0和Hadoop 2.0，其中，Hadoop 1.0由一个分布式文件系统HDFS和一个离线计算框架MapReduce组成，而Hadoop 2.0则包含一个支持NameNode横向扩展的HDFS，一个资源管理系统YARN和一个运行在YARN上的离线计算框架MapReduce。相比于Hadoop 1.0，Hadoop 2.0功能更加强大，且具有更好的扩展性、性能，并支持多种计算框架。
Borg/YARN/Mesos/Torca/Corona一类系统可以为公司构建一个内部的生态系统，所有应用程序和服务可以“和平而友好”地运行在该生态系统上。有了这类系统之后，你不必忧愁使用Hadoop的哪个版本，是Hadoop 0.20.2还是 Hadoop 1.0，你也不必为选择何种计算模型而苦恼，因此各种软件版本，各种计算模型可以一起运行在一台“超级计算机”上了。
从开源角度看，YARN的提出，从一定程度上弱化了多计算框架的优劣之争。YARN是在Hadoop MapReduce基础上演化而来的，在MapReduce时代，很多人批评MapReduce不适合迭代计算和流失计算，于是出现了Spark和Storm等计算框架，而这些系统的开发者则在自己的网站上或者论文里与MapReduce对比，鼓吹自己的系统多么先进高效，而出现了YARN之后，则形势变得明朗：MapReduce只是运行在YARN之上的一类应用程序抽象，Spark和Storm本质上也是，他们只是针对不同类型的应用开发的，没有优劣之别，各有所长，合并共处，而且，今后所有计算框架的开发，不出意外的话，也应是在YARN之上。这样，一个以YARN为底层资源管理平台，多种计算框架运行于其上的生态系统诞生了。
目前[spark](http://dongxicheng.org/category/framework-on-yarn/)是一个非常流行的内存计算（或者迭代式计算，DAG计算）框架，在MapReduce因效率低下而被广为诟病的今天，spark的出现不禁让大家眼前一亮。
从架构和应用角度上看，[spark](http://dongxicheng.org/category/framework-on-yarn/)是一个仅包含计算逻辑的开发库（尽管它提供个独立运行的master/slave服务，但考虑到稳定后以及与其他类型作业的继承性，通常不会被采用），而不包含任何资源管理和调度相关的实现，这使得spark可以灵活运行在目前比较主流的资源管理系统上，典型的代表是mesos和yarn，我们称之为“spark on mesos”和“spark on yarn”。将spark运行在资源管理系统上将带来非常多的收益，包括：与其他计算框架共享集群资源；资源按需分配，进而提高集群资源利用率等。
FrameWork On YARN
运行在YARN上的框架，包括MapReduce-On-YARN, Spark-On-YARN, Storm-On-YARN和Tez-On-YARN。
（1）[MapReduce-On-YARN](https://issues.apache.org/jira/browse/MAPREDUCE-279)：YARN上的离线计算；
（2）[Spark-On-YARN](http://spark.incubator.apache.org/docs/0.6.0/running-on-yarn.html)：YARN上的内存计算；
（3）[Storm-On-YARN](https://github.com/yahoo/storm-yarn)：YARN上的实时/流式计算；
（4）[Tez-On-YARN](http://tez.incubator.apache.org/)：YARN上的DAG计算


转载。	https://blog.csdn.net/cuiyaonan2000/article/details/80241948


