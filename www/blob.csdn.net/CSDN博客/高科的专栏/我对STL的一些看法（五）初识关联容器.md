
# 我对STL的一些看法（五）初识关联容器 - 高科的专栏 - CSDN博客

2013年12月09日 18:33:23[高科](https://me.csdn.net/pbymw8iwm)阅读数：5795个人分类：[STL																](https://blog.csdn.net/pbymw8iwm/article/category/1793843)[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1793843)



# 3关联容器
## pair类型
这个是一个简单的标准库类型，该类型在utility头文件中定义，我们来看看他主要的操作：
pair<T1 ,T2> p1;     创建一个空的pair对象
pair<T1,T2> p1(v1,v2);创建一个pair对象，他的两个元素分别为T1类型的v1，T2类型的v2
make_pair(v1,v2);    以v1,v2创建一份新的pair对象,相信在map这一块我们将会见面的
p1<p2,p1==p2
p1.first         返回p中名为first的数据成员
p1,second      返回p中名为second的数据成员
## 3.1特点
set, multiset, map,multimap 是一种非线性的树结构，具体的说采用的是一种比较高效的特殊的平衡检索二叉树——红黑树结构。（至于什么是红黑树，可以看看数据结构）
因为关联容器的这四种容器类都使用同一原理，所以他们核心的算法是一致的，但是它们在应用上又有一些差别，先描述一下它们之间的差别。
set 又称集合，实际上就是一组元素的集合，但其中所包含的元素的值是唯一的，且是按一定顺序排列的，集合中的每个元素被称作集合中的实例。因为其内部是通过链表的方式来组织，所以在插入的时候比vector 快，但在查找和末尾添加上比vector 慢。
multiset 是多重集合，其实现方式和set 是相似的，只是它不要求集合中的元素是唯一的，也就是说集合中的同一个元素可以出现多次。
map 提供一种“键- 值”关系的一对一的数据存储能力。其“键”在容器中不可重复，且按一定顺序排列（其实我们可以将set 也看成是一种键- 值关系的存储，只是它只有键没有值。它是map 的一种特殊形式）。由于其是按链表的方式存储，它也继承了链表的优缺点。
multimap 和map 的原理基本相似，它允许“键”在容器中可以不唯一。
关联容器的特点是明显的，相对于顺序容器，有以下几个主要特点：
1、其内部实现是采用非线性的二叉树结构，具体的说是红黑树的结构原理实现的；
2、set 和map 保证了元素的唯一性，mulset 和mulmap 扩展了这一属性，可以允许元素不唯一；
3、元素是有序的集合，默认在插入的时候按升序排列。
基于以上特点，
1、关联容器对元素的插入和删除操作比vector 要快，因为vector 是顺序存储，而关联容器是链式存储；比list 要慢，是因为即使它们同是链式结构，但list 是线性的，而关联容器是二叉树结构，其改变一个元素涉及到其它元素的变动比list 要多，并且它是排序的，每次插入和删除都需要对元素重新排序；
2、关联容器对元素的检索操作比vector 慢，但是比list 要快很多。vector 是顺序的连续存储，当然是比不上的，但相对链式的list 要快很多是因为list 是逐个搜索，它搜索的时间是跟容器的大小成正比，而关联容器查找的复杂度基本是Log(N) ，比如如果有1000 个记录，最多查找10 次，1,000,000 个记录，最多查找20 次。容器越大，关联容器相对list 的优越性就越能体现；
3、在使用上set 区别于vector,deque,list 的最大特点就是set 是内部排序的，这在查询上虽然逊色于vector ，但是却大大的强于list 。
4、在使用上map 的功能是不可取代的，它保存了“键- 值”关系的数据，而这种键值关系采用了类数组的方式。数组是用数字类型的下标来索引元素的位置，而map 是用字符型关键字来索引元素的位置。在使用上map 也提供了一种类数组操作的方式，即它可以通过下标来检索数据，这是其他容器做不到的，当然也包括set 。（STL 中只有vector 和map 可以通过类数组的方式操作元素，即如同ele[1] 方式）

