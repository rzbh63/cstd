
# Python3.x Numpy中的array数组_矩阵操作 - jerrygaoling的博客 - CSDN博客


2018年03月21日 08:54:31[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：296


## - 将array数组转换为矩阵
```python
import
```
```python
math
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
C_zd=np.array([
```
```python
7
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
7
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
42
```
```python
,
```
```python
54
```
```python
,
```
```python
45
```
```python
,
```
```python
43
```
```python
,
```
```python
10
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
15
```
```python
,
```
```python
14
```
```python
]).reshape((
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
))
```
```python
#将array数组转换为3行4列的矩阵
```
## - 矩阵强制修改
```python
C_zd
```
```python
.shape
```
```python
(
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
)
```
```python
#将原（3,4）矩阵，强制转换为（4,3）矩阵
```
## - 计算均值：
```python
np
```
```python
.mean
```
```python
(C_zd)
```
```python
#将所有的元素求均值
```
```python
np
```
```python
.mean
```
```python
(C_zd, axis=
```
```python
0
```
```python
)
```
```python
# axis=0，计算每一列的均值
```
```python
np
```
```python
.mean
```
```python
(C_zd, axis=
```
```python
1
```
```python
)
```
```python
# 计算每一行的均值
```
```python
矩阵：
```
```python
[[ 7  5  7  2]
     [42 54 45 43]
     [10  8 15 14]]
```
```python
按行均值：[
```
```python
5.25
```
```python
46.
```
```python
11.75
```
```python
]
```
## - 计算方差：
```python
np.
```
```python
var
```
```python
(C_zd)
```
```python
#将所有的元素求样本方差
```
```python
np.
```
```python
var
```
```python
(C_zd,axis=
```
```python
1
```
```python
,ddof=
```
```python
1
```
```python
)
```
```python
#axis=0，计算每一列的均值,样本数N-ddof
```
```python
np.
```
```python
var
```
```python
(C_zd, axis=
```
```python
0
```
```python
,ddof=
```
```python
1
```
```python
)
```
```python
# axis=0，计算每一列的均值
```
```python
np.
```
```python
var
```
```python
(C_zd, axis=
```
```python
1
```
```python
,ddof=
```
```python
1
```
```python
)
```
```python
# 计算每一行的均值
```
```python
按行方差（样本数
```
```python
N-1
```
```python
）：
```
```python
[ 5.58333333, 30. ,10.91666667]
```
其中，ddof确定样本数量，默认为N-ddof
详细参数设置：[numpy.var](https://docs.scipy.org/doc/numpy/reference/generated/numpy.var.html)
## - 矩阵的转置
在Python中，np.array产生的一维矩阵（行矩阵），会被认定为向量。
例如：
```python
a
```
```python
=np.array([
```
```python
7
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
7
```
```python
,
```
```python
2
```
```python
])
```
```python
#看似行矩阵
```
```python
a
```
```python
.shape
```
```python
#a.shape为（4，）是一个向量形式
```
此时，用np.transpose(a)进行转置，不会产生变化，因为a为隐藏矩阵（1,1）所以转置后还是（1,1）的矩阵，显示不存在变化。
需要将其强制转换为（4,1）的矩阵，才能进行转置
```python
a
```
```python
.shape(
```
```python
4
```
```python
,
```
```python
1
```
```python
)
```
```python
#此时为（4,1）的矩阵
```
如果是二维或高维矩阵，可直接用np.transpose(a)来进行转置。
## - 生成指定行列范围的零矩阵
```python
a
```
```python
= np.zeros(
```
```python
4
```
```python
,
```
```python
4
```
```python
)
```
```python
#一个4*4的全零矩阵
```
## - 将np.array数组转换为list列表
```python
np
```
```python
.zeros
```
```python
(
```
```python
4
```
```python
,
```
```python
4
```
```python
)
```
```python
.tolist
```
```python
()
```
```python
#转换为list列表
```
```python
#转换后，可进行list方法计算
```
## - 将list列表转换为np.array数组
```python
a
```
```python
= [
```
```python
5
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
9
```
```python
,
```
```python
8
```
```python
]
```
```python
#转换为list列表
```
```python
np.array(
```
```python
a
```
```python
)
```
```python
#转换为为array数组
```
```python
#转换后，可进行np.array的方法计算
```

