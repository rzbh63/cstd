
# Python3.x伪随机，元素重排 - jerrygaoling的博客 - CSDN博客


2018年04月11日 21:07:42[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：54


## 非控制随机种子列表随机洗牌
特点：
- 随机的产生一个随机数`p`，当做列表`lis`的索引位置,随机数产生范围为列表长度范围`(len(lis))`内
- 取出当前索引`p`位置的元素，依次向后放置于新列表`new_lis`中，之后删除掉原列表中对应位置元素
- 循环，直至全部取出、
此种洗牌方式，是最为直观的洗牌方式，随机的从原列表中抽出一张牌，生成一个新的排列
```python
import
```
```python
random
```
```python
def
```
```python
glyz_shuffle
```
```python
(lis)
```
```python
:
```
```python
new_lis = []
```
```python
while
```
```python
lis:
        p = random.randrange(
```
```python
0
```
```python
, len(lis))
        new_lis.append(lis[p])
        lis.pop(p)
```
```python
return
```
```python
new_lis
lis = glyz_shuffle([
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
2
```
```python
,
```
```python
1
```
```python
])
print(lis)
```
![这里写图片描述](https://img-blog.csdn.net/20180411210550242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以使用`pop()`方法来对列表中的元素进行删除，也可以使用全局函数`del`来删除指定位置
没有添加随机种子的随机排序，将会存在不可控排序，即每次执行的结果都是随机的。
再次执行程序，重排结果将不同：
![这里写图片描述](https://img-blog.csdn.net/20180411210602677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 使用随机种子控制伪随机列表洗牌
```python
import
```
```python
random
```
```python
def
```
```python
glyz_shuffle
```
```python
(lis)
```
```python
:
```
```python
new_lis = []
    randseed =
```
```python
3
```
```python
random.seed(randseed)
```
```python
while
```
```python
lis:
        p = random.randrange(
```
```python
0
```
```python
, len(lis))
        new_lis.append(lis[p])
        lis.pop(p)
```
```python
return
```
```python
new_lis
lis = glyz_shuffle([
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
2
```
```python
,
```
```python
1
```
```python
])
print(lis)
```
![这里写图片描述](https://img-blog.csdn.net/20180411210616420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
每次执行的重排结果将相同。
如果想增加洗牌的随机性，可将每一次得到的最后一个随机数当做下次洗牌的随机种子。
```python
def
```
```python
glyz_shuffle
```
```python
(lis,randseed)
```
```python
:
```
```python
new_lis = []
    return_p =
```
```python
0
```
```python
return_all = []
    random.seed(randseed)
```
```python
while
```
```python
lis:
        p = random.randrange(
```
```python
0
```
```python
, len(lis))
        new_lis.append(lis[p])
        lis.pop(p)
        return_p = p
    return_all.append(new_lis)
    return_all.append(return_p)
```
```python
return
```
```python
return_all
randseed =
```
```python
3
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
2
```
```python
):
    lis = glyz_shuffle([
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
2
```
```python
,
```
```python
1
```
```python
],randseed)
    randseed = lis[
```
```python
1
```
```python
]
    print(lis)
```
![这里写图片描述](https://img-blog.csdn.net/20180411210629218?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

