
# Python3.x中set()集合的使用方法 - jerrygaoling的博客 - CSDN博客


2018年07月16日 18:39:02[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：933


# Python中set()集合的使用方法
python中的set()是一个无序的不重复的元素集。
sets 支持 x in set, len(set),和 for x in set。
1.初始化方法：
```python
x = set()
x.add(
```
```python
'str'
```
```python
)
......
```
2.基本使用方法：
```python
x = set(
```
```python
'class'
```
```python
)
y = set([
```
```python
'c'
```
```python
,
```
```python
'a'
```
```python
,
```
```python
'm'
```
```python
])
print(x, y)
>>>(set([
```
```python
'l'
```
```python
,
```
```python
'c'
```
```python
,
```
```python
'a'
```
```python
,
```
```python
's'
```
```python
]), set([
```
```python
'c'
```
```python
,
```
```python
'a'
```
```python
,
```
```python
'm'
```
```python
]))
```
储存方式是==无序的==，==不重复的==。
3.交、并、差集
```python
#交集
```
```python
print(x&y)
>>>set([
```
```python
'c'
```
```python
,
```
```python
'a'
```
```python
])
```
```python
#并集
```
```python
print(x | y)
>>>set([
```
```python
'l'
```
```python
,
```
```python
'c'
```
```python
,
```
```python
'a'
```
```python
,
```
```python
's'
```
```python
,
```
```python
'm'
```
```python
])
```
```python
#差集
```
```python
print
```
```python
（x - y）
>>>set([
```
```python
'l'
```
```python
,
```
```python
's'
```
```python
])
```
4.去除list列表中重复的数据
在某些长列表中，需要获取列表中的元素类型时，可以使用set()方法去除重复的元素。
```python
a = [
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
3
```
```python
,
```
```python
1
```
```python
]
b = set(a)
print(b)
>>>set([
```
```python
2
```
```python
,
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
4
```
```python
])
```
与列表和元组不同，集合是无序的，也无法通过数字进行索引。此外，集合中的元素不能重复。
5.基本操作方式：
```python
# 添加一项
```
```python
a.add(
```
```python
'x'
```
```python
)
```
```python
# 在a中添加多项
```
```python
a.update([
```
```python
10
```
```python
,
```
```python
37
```
```python
,
```
```python
42
```
```python
])
```
```python
#使用remove()可以删除一项,如果不存在则引发 KeyError
```
```python
a.remove(
```
```python
'c'
```
```python
)
```
```python
#获取集合的长度(元素个数)
```
```python
len(a)
```
```python
#测试是否为成员项
```
```python
'2'
```
```python
in
```
```python
a
```
```python
#测试是否不为成员项
```
```python
'2'
```
```python
not
```
```python
in
```
```python
a
```
```python
#测试是否 s 中的每一个元素都在 t 中
```
```python
s.issubset(t)
s <= t
```
```python
#测试是否 t 中的每一个元素都在 s 中
```
```python
s.issuperset(t)
s >= t
```
```python
#返回一个新的 set 包含 s 和 t 中的每一个元素
```
```python
s.union(t)
s | t
```
```python
#返回一个新的 set 包含 s 和 t 中的公共元素
```
```python
s.intersection(t)
s & t
```
```python
#返回一个新的 set 包含 s 中有但是 t 中没有的元素
```
```python
s.difference(t)
s - t
```
```python
#返回一个新的 set 包含 s 和 t 中不重复的元素
```
```python
s.symmetric_difference(t)
s ^ t
```
```python
#返回 set “s”的一个浅复制
```
```python
s.copy()
```
```python
#删除并且返回 set “s”中的一个不确定的元素, 如果为空则引KeyError
```
```python
s.pop()
```
```python
#删除 set “s”中的所有元素
```
```python
s.clear()
```
```python
#如果在 set “s”中存在元素 x, 则删除
```
```python
s.discard(x)
```

