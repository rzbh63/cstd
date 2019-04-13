
# spark中的广播变量与累加器 - bitcarmanlee的博客 - CSDN博客


2017年11月25日 22:09:01[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：870



## 1.累加器(accumulator)
累加器是仅仅被相关操作累加的变量，因此可以在并行中被有效地支持。它可以被用来实现计数器和总和。
累加器通过对一个初始化了的变量v调用SparkContext.accumulator(v)来创建。在集群上运行的任务可以通过add或者”+=”方法在累加器上进行累加操作。但是，它们不能读取它的值。只有驱动程序能够读取它的值，通过累加器的value方法。
看看在spark shell中展示如何使用accumulator。
```python
//创建一个accumulator变量
scala> val acc = sc
```
```python
.accumulator
```
```python
(
```
```python
0
```
```python
,
```
```python
"Accumulator"
```
```python
)
```
```python
acc:
```
```python
org
```
```python
.apache
```
```python
.spark
```
```python
.Accumulator
```
```python
[Int] =
```
```python
0
```
```python
//
```
```python
add
```
```python
方法可以相加
scala> sc
```
```python
.parallelize
```
```python
(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
))
```
```python
.foreach
```
```python
(
```
```python
x
```
```python
=> acc
```
```python
.add
```
```python
(
```
```python
x
```
```python
))
scala> acc
```
```python
res4:
```
```python
org
```
```python
.apache
```
```python
.spark
```
```python
.Accumulator
```
```python
[Int] =
```
```python
15
```
```python
scala> acc
```
```python
.value
```
```python
res5:
```
```python
Int =
```
```python
15
```
```python
//+=也可以相加
scala> sc
```
```python
.parallelize
```
```python
(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
))
```
```python
.foreach
```
```python
(
```
```python
x
```
```python
=> acc +=
```
```python
x
```
```python
)
scala> acc
```
```python
.value
```
```python
res9:
```
```python
Int =
```
```python
30
```
累加器并没有改变Spark的lazy求值的模型。如果它们被RDD上的操作更新，它们的值只有当RDD因为动作操作被计算时才被更新。因此，当执行一个惰性的转换操作,比如map时，不能保证对累加器值的更新被实际执行了。下面的代码可以清晰地看到此特点。
```python
scala> val data = sc
```
```python
.parallelize
```
```python
(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
))
```
```python
data:
```
```python
org
```
```python
.apache
```
```python
.spark
```
```python
.rdd
```
```python
.RDD
```
```python
[Int] = ParallelCollectionRDD[
```
```python
3
```
```python
] at parallelize at <console>:
```
```python
27
```
```python
scala> data
```
```python
.map
```
```python
(
```
```python
x
```
```python
=> acc +=
```
```python
x
```
```python
)
```
```python
res10:
```
```python
org
```
```python
.apache
```
```python
.spark
```
```python
.rdd
```
```python
.RDD
```
```python
[Unit] = MapPartitionsRDD[
```
```python
4
```
```python
] at map at <console>:
```
```python
32
```
```python
//由此可见，此时acc的值并未改变
scala> acc
```
```python
res11:
```
```python
org
```
```python
.apache
```
```python
.spark
```
```python
.Accumulator
```
```python
[Int] =
```
```python
30
```
需要注意的地方是：
1.累加器的值只有在驱动器程序中访问，所以检查也应当在驱动器程序中完成。
2.对于行动操作中使用的累加器，Spark只会把每个任务对各累加器的修改应用一次。因此如果想要一个无论在失败还是在重新计算时候都绝对可靠的累加器，必须把它放在foreach（）这样的行动操作中。
## 2.广播变量
Spark提供的Broadcast Variable，是只读的。并且在每个节点上只会有一份副本，而不会为每个task都拷贝一份副本。因此其最大作用，就是减少变量到各个节点的网络传输消耗，以及在各个节点上的内存消耗。此外，spark自己内部也使用了高效的广播算法来减少网络消耗。
调用SparkContext的broadcast()方法，来针对某个变量创建广播变量。然后在算子的函数内，使用到广播变量时，每个节点只会拷贝一份副本了。每个节点可以使用广播变量的value()方法获取值。Broadcast是只读的。
使用Broadcast变量的步骤：
1.调用SparkContext.broadcast方法创建一个Broadcast[T]对象。
任何序列化的类型都可以这么实现。
2.通过value属性访问改对象的值(Java之中为value()方法)
3.变量只会被发送到各个节点一次，应作为只读值处理（修改这个值不会影响到别的节点）
```python
val cast = sc.broadcast(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
))
```
```python
...
```
```python
cast: org.apache.spark.broadcast.Broadcast[Array[Int]] = Broadcast(
```
```python
3
```
```python
)
scala> cast.value
res12: Array[Int] = Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
)
```
## 3.mapPartitions
mapPartitions与map类似，只不过映射函数的参数由RDD中的每一个元素变成了RDD中每一个分区的迭代器。如果在映射的过程中需要频繁创建额外的对象（如数据库连接对象），使用mapPartitions要比map高效的多。
比如，将RDD中的所有数据通过JDBC连接写入数据库，如果使用map函数，可能要为每一个元素都创建一个connection，这样开销很大，如果使用mapPartitions，那么只需要针对每一个分区建立一个connection。
mapPartitions的源码如下
```python
/**
   * Return a new RDD by applying a function to each partition of this RDD.
   *
   * `preservesPartitioning` indicates whether the input function preserves the partitioner, which
   * should be `false` unless this is a pair RDD and the input function doesn't modify the keys.
   */
```
```python
def
```
```python
mapPartitions[U: ClassTag](
      f: Iterator[T] => Iterator[U],
      preservesPartitioning: Boolean =
```
```python
false
```
```python
): RDD[U] = withScope {
```
```python
val
```
```python
cleanedF = sc.clean(f)
```
```python
new
```
```python
MapPartitionsRDD(
```
```python
this
```
```python
,
      (context: TaskContext, index: Int, iter: Iterator[T]) => cleanedF(iter),
      preservesPartitioning)
  }
```
spark-shell中的mapPartitions的例子
```python
scala> val ardd= sc.parallelize(1 to 9, 3)
ardd: org.apache.spark.rdd.RDD[Int] = ParallelCollectionRDD[5] at parallelize at
```
```python
<
```
```python
console
```
```python
>
```
```python
:27
scala> def doubleFunc(iter: Iterator[
```
```python
Int
```
```python
]) : Iterator[
```
```python
(Int,Int)
```
```python
] = {var res = List[
```
```python
(Int,Int)
```
```python
](
```
```python
);while (iter.hasNext) { val cur = iter.next;res .::= (cur,cur*2)} ; res.iterator }
doubleFunc: (iter: Iterator[Int])Iterator[(Int, Int)]
scala> val result = ardd.mapPartitions(doubleFunc)
result: org.apache.spark.rdd.RDD[(Int, Int)] = MapPartitionsRDD[6] at mapPartitions at
```
```python
<
```
```python
console
```
```python
>
```
```python
:31
scala> result.collect().mkString
...
res14: String = (3,6)(2,4)(1,2)(6,12)(5,10)(4,8)(9,18)(8,16)(7,14)
```

