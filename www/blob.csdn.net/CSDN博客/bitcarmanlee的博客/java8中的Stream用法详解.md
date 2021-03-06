
# java8中的Stream用法详解 - bitcarmanlee的博客 - CSDN博客


2017年11月09日 17:37:10[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：10220



## 1.为什么java8中加入Stream
Stream 作为 Java 8 的一大亮点，它与 java.io 包里的 InputStream 和 OutputStream 是完全不同的概念。Java 8 中的 Stream 是对集合（Collection）对象功能的增强，它专注于对集合对象进行各种非常便利、高效的聚合操作（aggregate operation），或者大批量数据操作 (bulk data operation)。尤其是对于数据从业人员来说，对数据做各种操作转换是再正常不过的需求，基本每天都会用到。例如下面这么一个简单的小需求：求一个集合中字符串长度小于5的数量。
在java8之前，我们一般这么做：
```python
@Test
```
```python
public
```
```python
void
```
```python
lenIter
```
```python
() {
        List<String> list = Arrays.asList(
```
```python
"java"
```
```python
,
```
```python
"scala"
```
```python
,
```
```python
"python"
```
```python
,
```
```python
"shell"
```
```python
,
```
```python
"ruby"
```
```python
);
```
```python
int
```
```python
num =
```
```python
0
```
```python
;
```
```python
for
```
```python
(String lan: list) {
```
```python
if
```
```python
(lan.length() <
```
```python
5
```
```python
) {
                num++;
            }
        }
        System.
```
```python
out
```
```python
.println(num);
    }
```
这段代码逻辑很简单，但是显得很冗长，可读性嘛也就呵呵了。如果用Stream，我们可以这样：
```python
@Test
    public void lenStream() {
        List<String> list = Arrays
```
```python
.asList
```
```python
(
```
```python
"java"
```
```python
,
```
```python
"scala"
```
```python
,
```
```python
"python"
```
```python
,
```
```python
"shell"
```
```python
,
```
```python
"ruby"
```
```python
)
```
```python
;
```
```python
long num = list
```
```python
.parallelStream
```
```python
()
```
```python
.filter
```
```python
(
```
```python
x
```
```python
->
```
```python
x
```
```python
.length
```
```python
() <
```
```python
5
```
```python
)
```
```python
.count
```
```python
()
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(num)
```
```python
;
```
```python
}
```
代码量明显减少而且逻辑特别清楚，即使不懂代码的人看到也能猜出来是什么意思。如果大家了解过函数式编程，就会觉得特别亲切自然。
## 2.什么是Stream
Stream 不是集合元素，它不是数据结构并不保存数据，它是有关算法和计算的，它更像一个高级版本的 Iterator。原始版本的 Iterator，用户只能显式地一个一个遍历元素并对其执行某些操作；高级版本的 Stream，用户只要给出需要对其包含的元素执行什么操作，比如 “过滤掉长度大于 10 的字符串”、“获取每个字符串的首字母”等，Stream 会隐式地在内部进行遍历，做出相应的数据转换。
Stream 就如同一个迭代器（Iterator），单向，不可往复，数据只能遍历一次，遍历过一次后即用尽了，就好比流水从面前流过，一去不复返。
而和迭代器又不同的是，Stream 可以并行化操作，迭代器只能命令式地、串行化操作。顾名思义，当使用串行方式去遍历时，每个 item 读完后再读下一个 item。而使用并行去遍历时，数据会被分成多个段，其中每一个都在不同的线程中处理，然后将结果一起输出。Stream 的并行操作依赖于 Java7 中引入的 Fork/Join 框架（JSR166y）来拆分任务和加速处理过程。
Stream和Collection的区别主要有：
1.stream本身并不存储数据，数据是存储在对应的collection里，或者在需要的时候才生成的；
2.stream不会修改数据源，总是返回新的stream；
3.stream的操作是懒执行(lazy)的：仅当最终的结果需要的时候才会执行，比如上面的例子中，结果仅需要前3个长度大于7的字符串，那么在找到前3个长度符合要求的字符串后，filter()将停止执行；
使用stream的步骤如下：
1.创建stream；
2.通过一个或多个中间操作(intermediate operations)将初始stream转换为另一个stream；
3.通过中止操作(terminal operation)获取结果；该操作触发之前的懒操作的执行，中止操作后，该stream关闭，不能再使用了；
## 3.创建Stream的方法
最常用的为使用静态方法创建
```python
@Test
```
```python
public
```
```python
void
```
```python
numberStreamConstruct
```
```python
() {
        IntStream.of(
```
```python
new
```
```python
int
```
```python
[] {
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
}).forEach(System.
```
```python
out
```
```python
::println);
        IntStream.range(
```
```python
1
```
```python
,
```
```python
3
```
```python
).forEach(System.
```
```python
out
```
```python
::println);
        IntStream.rangeClosed(
```
```python
1
```
```python
,
```
```python
3
```
```python
).forEach(System.
```
```python
out
```
```python
::println);
    }
```
## 4.Stream的转换
Stream最大的用途就是各种转换了。跟Spark中的Rdd类似，Rdd里面也是各种transfer操作。
1.filter操作。即使原stream中满足条件的元素构成新的stream：
```python
@Test
    public void lenStream() {
        List<String> list = Arrays
```
```python
.asList
```
```python
(
```
```python
"java"
```
```python
,
```
```python
"scala"
```
```python
,
```
```python
"python"
```
```python
,
```
```python
"shell"
```
```python
,
```
```python
"ruby"
```
```python
)
```
```python
;
```
```python
long num = list
```
```python
.parallelStream
```
```python
()
```
```python
.filter
```
```python
(
```
```python
x
```
```python
->
```
```python
x
```
```python
.length
```
```python
() <
```
```python
5
```
```python
)
```
```python
.count
```
```python
()
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(num)
```
```python
;
```
```python
}
```
结果：
```python
2
```
得到长度小于5的单词个数
2.map操作。map算是最常用的一种操作了，遍历原stream中的元素，转换后构成新的stream：
```python
@Test
```
```python
public
```
```python
void turnUpperCase() {
```
```python
List
```
```python
<String>
```
```python
list
```
```python
= Arrays.asList(
```
```python
new
```
```python
String[] {
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
});
```
```python
List
```
```python
<String> result =
```
```python
list
```
```python
.stream().map(String::toUpperCase).collect(Collectors.toList());
        result.
```
```python
forEach
```
```python
(x -> System.out.
```
```python
print
```
```python
(x +
```
```python
" "
```
```python
));
    }
```
3.distinct操作。distinct也是常用的操作之一。
```python
@Test
    public void distinctStream() {
        Stream<String> distinctStream = Stream
```
```python
.of
```
```python
(
```
```python
"bj"
```
```python
,
```
```python
"shanghai"
```
```python
,
```
```python
"tianjin"
```
```python
,
```
```python
"bj"
```
```python
,
```
```python
"shanghai"
```
```python
)
```
```python
.distinct
```
```python
()
```
```python
;
```
```python
Stream<String> sortedStream = distinctStream
```
```python
.sorted
```
```python
(Comparator
```
```python
.comparing
```
```python
(String::length))
```
```python
;
```
```python
sortedStream
```
```python
.forEach
```
```python
(
```
```python
x
```
```python
-> System
```
```python
.out
```
```python
.print
```
```python
(
```
```python
x
```
```python
+
```
```python
" "
```
```python
))
```
```python
;
```
```python
}
```
结果如下
```python
bj tianjin shanghai
```
4.排序操作。
```python
@Test
```
```python
public
```
```python
void
```
```python
sortStream() {
        Stream<Integer> sortedStream = Stream.
```
```python
of
```
```python
(
```
```python
1
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
7
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
,
```
```python
8
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
).sorted();
        sortedStream.collect(Collectors.toList()).forEach(x
```
```python
->
```
```python
System.out.
```
```python
print
```
```python
(x +
```
```python
" "
```
```python
));
        System.out.println();
        Stream<Integer> sortedReverseStream = Stream.
```
```python
of
```
```python
(
```
```python
1
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
7
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
,
```
```python
8
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
).sorted(
```
```python
new
```
```python
Comparator<Integer>() {
```
```python
@Override
```
```python
public int compare(Integer o1, Integer o2) {
```
```python
return
```
```python
o1 - o2;
            }
        });
        Stream<Integer> sortedReverseStreamV2 = Stream.
```
```python
of
```
```python
(
```
```python
1
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
7
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
,
```
```python
8
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
)
```
```python
.
```
```python
sorted
```
```python
((Integer o1, Integer o2) -> o2 - o1)
```
```python
;
```
```python
sortedReverseStreamV2
```
```python
.
```
```python
collect
```
```python
(Collectors.toList())
```
```python
.
```
```python
forEach
```
```python
(x -> System.out.
```
```python
print
```
```python
(x +
```
```python
" "
```
```python
))
```
```python
;
    }
```
最终的结果：
```python
1 2 3 4 5 6 7 8 
8 7 6 5 4 3 2 1
```
## 5.reduction操作
1.reduction就是从stream中取出结果，是terminal operation，因此经过reduction后的stream不能再使用了。主要包含以下操作： findFirst()/findAny()/allMatch/anyMatch()/noneMatch等等
```python
@Test
    public void reductionStream() {
        Stream<String> wordList = Stream
```
```python
.of
```
```python
(
```
```python
"bj"
```
```python
,
```
```python
"tj"
```
```python
,
```
```python
"sh"
```
```python
,
```
```python
"yy"
```
```python
,
```
```python
"yq"
```
```python
)
```
```python
.distinct
```
```python
()
```
```python
;
```
```python
Optional<String> firstWord = wordList
```
```python
.filter
```
```python
(word -> word
```
```python
.startsWith
```
```python
(
```
```python
"y"
```
```python
))
```
```python
.findFirst
```
```python
()
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(firstWord
```
```python
.orElse
```
```python
(
```
```python
"unknown"
```
```python
))
```
```python
;
```
```python
}
```
结果：
```python
yy
```
reduce方法。与其他语言里的reduce方法一样的逻辑。
```python
@Test
```
```python
public
```
```python
void
```
```python
reduceTest() {
        Stream<Integer> list = Stream.
```
```python
of
```
```python
(
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
);
        Optional<Integer> result = list.reduce
```
```python
((x, y) -> x + y)
```
```python
;
```
```python
System
```
```python
.
```
```python
out
```
```python
.
```
```python
println
```
```python
(result)
```
```python
;
    }
```
结果如下：
```python
Optional
```
```python
[15]
```
## 6.collect
collect()方法可以对stream中的元素进行各种处理后，得到stream中元素的值。并且Collectors接口提供了很方便的创建Collector对象的工厂方法。
```python
@Test
    public void collectTest() {
        List<String> list = Stream
```
```python
.of
```
```python
(
```
```python
"hello"
```
```python
,
```
```python
"world"
```
```python
,
```
```python
"hello"
```
```python
,
```
```python
"java"
```
```python
)
```
```python
.collect
```
```python
(Collectors
```
```python
.toList
```
```python
())
```
```python
;
```
```python
list
```
```python
.forEach
```
```python
(
```
```python
x
```
```python
-> System
```
```python
.out
```
```python
.print
```
```python
(
```
```python
x
```
```python
+
```
```python
" "
```
```python
))
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
()
```
```python
;
```
```python
Set
```
```python
<String>
```
```python
set
```
```python
= Stream
```
```python
.of
```
```python
(
```
```python
"hello"
```
```python
,
```
```python
"world"
```
```python
,
```
```python
"hello"
```
```python
,
```
```python
"java"
```
```python
)
```
```python
.collect
```
```python
(Collectors
```
```python
.toSet
```
```python
())
```
```python
;
```
```python
set
```
```python
.forEach
```
```python
(
```
```python
x
```
```python
-> System
```
```python
.out
```
```python
.print
```
```python
(
```
```python
x
```
```python
+
```
```python
" "
```
```python
))
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
()
```
```python
;
```
```python
Set
```
```python
<String> treeset = Stream
```
```python
.of
```
```python
(
```
```python
"hello"
```
```python
,
```
```python
"world"
```
```python
,
```
```python
"hello"
```
```python
,
```
```python
"java"
```
```python
)
```
```python
.collect
```
```python
(Collectors
```
```python
.toCollection
```
```python
(TreeSet::new))
```
```python
;
```
```python
treeset
```
```python
.forEach
```
```python
(
```
```python
x
```
```python
-> System
```
```python
.out
```
```python
.print
```
```python
(
```
```python
x
```
```python
+
```
```python
" "
```
```python
))
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
()
```
```python
;
```
```python
String resultStr = Stream
```
```python
.of
```
```python
(
```
```python
"hello"
```
```python
,
```
```python
"world"
```
```python
,
```
```python
"hello"
```
```python
,
```
```python
"java"
```
```python
)
```
```python
.collect
```
```python
(Collectors
```
```python
.joining
```
```python
(
```
```python
","
```
```python
))
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(resultStr)
```
```python
;
```
```python
}
```
最后的输出结果为：
```python
hello world hello java 
world java hello 
hello java world 
hello,world,hello,java
```

