
# LoadRunner监控数据库服务 - anlcy - 博客园






# [LoadRunner监控数据库服务](https://www.cnblogs.com/camilla/p/10338150.html)
一、LR监控SQL Server
SQL Server自身提供的性能计数器指标有：
|指标名称
|指标描述
|指标范围
|指标单位
|
|1．SQL Server中访问方法（Access Methods）对象包含的性能计数器
|
|全表扫描/秒
|(Full Scans/sec)
|指每秒全表扫描的数量。全表扫描可以是基本表扫描或全索引扫描。由于全表扫描需要耗费大量时间，因此全表扫描的频率过高的话，会影响性能。
|如果该指标的值比1或2高，应该分析设计的查询以确定是否确实需要全表扫描，以及SQL查询是否可以被优化。
|次数/秒
|
|2．SQL Server中缓冲器管理器（Buffer Manager）对象包含的性能计数器
|
|缓冲区高速缓存命中率(BufferCache
|Hit Ratio%）
|指在缓冲区高速缓存中找到而不需要从磁盘中读取的页的百分比。该比率是缓存命中总次数与缓存查找总次数之比。经过很长时间后，该比率的变化很小。由于从缓存中读取数据比从磁盘中读取数据的开销小得多，一般希望该比率高一些。
|该指标的值最好为90%或更高。通常可以通过增加SQL Server可用的内存数量来提高该指标的值。增加内存直到这指标的值持续高于90%，表示90%以上的数据请求可以从数据缓冲区中获得所需数据。
|
|读的页/秒
|(Page Reads/sec)
|指每秒发出的物理数据库页读取数。该指标主要考察数据库从磁盘读取数据的频率。因为物理I/O会耗费大量时间，所以应尽可能地减少物理I/O以提高性能。
|该指标的值应尽可能的小。可以通过使用更大的数据高速缓存、智能索引、更高效的查询或者改变数据库设计等方法，以降低该指标的值。
|个数/秒
|
|写的页/秒
|(Page Writes/sec)
|指每秒执行的物理数据库写的页数。该指标主要考察数据库向磁盘写入数据的频率。因为物理I/O会耗费大量时间，所以应尽可能地减少物理I/O以提高性能。
|该指标的值应尽可能的小。可以通过使用更大的数据高速缓存、智能索引、更高效的查询或者改变数据库设计等方法，以降低该指标的值。
|个数/秒
|
|惰性写/秒
|(Lazy Writes/sec)
|指每秒被缓冲区管理器的惰性编写器写入的缓冲区数。惰性编写器是一个系统进程，用于成批刷新脏的老化的缓冲区（包含更改的缓冲区，必须将这些更改写回磁盘，才能将缓冲区重用于其他页），并使它们可用于用户进程。
|该指标的值最好为0。
|个数/秒
|
|3．SQL Server中高速缓存管理器（Cache Manager）对象包含的性能计数器
|
|高速缓存命中率(Cache Hit Ratio%）
|指高速缓存命中次数和查找次数的比率。在SQL Server中，Cache包括Log Cache，Buffer Cache以及Procedure Cache，该指标是指所有Cache的命中率，是一个总体的比率。
|该指标的值越高越好。如果该指标的值持续低于80%，就需要增加更多的内存。
|%
|
|4．SQL Server中闩（Latches）对象包含的性能计数器
|
|平均闩等待
|时间(毫秒)
|(Average Latch
|Wait Time(ms))
|指一个SQL Server线程必须等待一个闩的平均时间。
|如果该指标的值很高，则系统可能正经历严重的资源竞争问题。
|毫秒
|
|闩等待/秒
|(Latch Waits/sec)
|指在一个闩上每秒的平均等待数量。
|如果该指标的值很高，则系统可能正经历严重的资源竞争问题。
|个数/秒
|
|5．SQL Server中锁（Locks）对象包含的性能计数器
|
|死锁的数量/秒
|(Number of Deadlocks/sec)
|指每秒导致死锁的锁请求数。
|锁加在SQL Server资源上（如在一个事务中进行的行读取或修改），以防止多个事务并发使用资源。应尽可能少使用锁以提高事务的并发性，从而改善性能。
|个数/秒
|
|平均等待时间(毫秒)
|(Average Wait
|Time(ms))
|指线程等待某种类型的锁的平均等待时间。
|同上
|毫秒
|
|锁请求/秒
|(Lock Requests/sec)
|指每秒钟某种类型的锁请求的数量。
|同上
|个数/秒
|
一般需要监控以下指标：
1） SQLServer资源监控中指标缓存点击率（Cache Hit Ratio），该值越高越好。如果持续低于80%，应考虑增加内存。
2） 如果Full Scans/sec（全表扫描/秒）计数器显示的值比1或2高，则应分析你的查询以确定是否确实需要全表扫描，以及SQL查询是否可以被优化。
3） Number of Deadlocks/sec(死锁的数量/秒)：死锁对应用程序的可伸缩性非常有害，并且会导致恶劣的用户体验。该计数器的值必须为0。
4） Lock Requests/sec(锁请求/秒)，通过优化查询来减少读取次数，可以减少该计数器的值。





