
# 4.9 CAP和BASE理论 - 尹成的技术博客 - CSDN博客

2018年11月15日 14:34:54[尹成](https://me.csdn.net/yincheng01)阅读数：23个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



分布式数据库CAP理论
	CAP
	C:Consistency（强一致性）
	更新操作成功并返回客户端完成后，所有节点在同一时间的数据完全一致。
	A:Availability（可用性）
	服务一直可用，而且是正常响应时间。
	P:Partition tolerance（分区容错性）
	分布式系统在遇到某节点或网络分区故障的时候，仍然能够对外提供满足一致性和可用性的服务。
	CAP的3进2
	CAP理论就是说在分布式存储系统中，最多只能实现上面的两点。
	由于当前的网络硬件肯定会出现延迟丢包等问题，所以分区容忍性是我们必须需要实现的。
	所以我们只能在一致性和可用性之间进行权衡，没有NoSQL系统能同时保证这三点。
	CAP的基本应用
	CA 传统RDBMS数据库
	AP 大多数网站架构的选择
	CP Redis、Mongodb
	CAP的取舍（具体问题，具体分析）
	分布式架构的时候必须做出取舍。一致性和可用性之间取一个平衡。多余大多数web应用，其实并不需要强一致性。因此牺牲C换取P，这是目前分布式数据库产品的方向。
	很多web实时系统并不要求严格的数据库事务，对读一致性的要求很低， 有些场合对写一致性要求并不高。允许实现最终一致性。
	数据库的写实时性和读实时性需求：对关系数据库来说，插入一条数据之后立刻查询，是肯定可以读出来这条数据的，但是对于很多web应用来说，并不要求这么高的实时性，比方说发一条消息之后，过几秒乃至十几秒之后，我的订阅者才看到这条动态是完全可以接受的。
	对复杂的SQL查询，特别是多表关联查询的需求：任何大数据量的web系统，都非常忌讳多个大表的关联查询，以及复杂的数据分析类型的报表查询，特别是SNS类型的网站，从需求以及产品设计角 度，就避免了这种情况的产生。往往更多的只是单表的主键查询，以及单表的简单条件分页查询，SQL的功能被极大的弱化了。
	CAP总结：
	CAP理论的核心是：一个分布式系统不可能同时很好的满足一致性，可用性和分区容错性这三个需求，最多只能同时较好的满足两个。
	因此，根据 CAP 原理将 NoSQL 数据库分成了满足 CA 原则、满足 CP 原则和满足 AP 原则三大类：
	CA - 单点集群，满足一致性，可用性的系统，通常在可扩展性上不太强大。
	CP - 满足一致性，分区容忍必的系统，通常性能不是特别高。
	AP - 满足可用性，分区容忍性的系统，通常可能对一致性要求低一些。
	经典CAP图
分布式数据库 BASE理论
	BASE
	基本可用（Basically Available）
	分布式系统在出现故障的时候，允许损失部分可用性，即保证核心可用。
	软状态（Soft state）
	允许系统存在中间状态，而该中间状态不会影响系统整体可用性。分布式存储中一般一份数据至少会有三个副本，允许不同节点间副本同步的延时就是软状态的体现。
	最终一致（Eventually consistent）
	系统中的所有数据副本经过一定时间后，最终能够达到一致的状态。弱一致性和强一致性相反，最终一致性是弱一致性的一种特殊情况。
	BASE理论的思想：
	通过让系统放松对某一时刻数据一致性的要求来换取系统整体伸缩性和性能上改观。为什么这么说呢，缘由就在于大型系统往往由于地域分布和极高性能的要求，不可能采用分布式事务来完成这些指标，要想获得这些指标，我们必须采用另外一种方式来完成，这里BASE就是解决这个问题的办法。
	BASE与ACID（事务所具有的4个特性）的区别
	ACID是传统数据库常用的设计理念，追求强一致性模型。BASE支持的是大型分布式系统，提出通过牺牲强一致性获得高可用性。
	A：Atomicity（原子性）
	C：Consistency（一致性）
	I：Isolation（隔离性）
	D：Durability（持久性）
	ACID和BASE代表了两种截然相反的设计哲学，在分布式系统设计的场景中，系统组件对一致性要求是不同的，因此ACID和BASE又会结合使用。
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

