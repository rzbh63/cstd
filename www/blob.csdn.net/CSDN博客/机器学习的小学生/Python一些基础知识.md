
# Python一些基础知识 - 机器学习的小学生 - CSDN博客


置顶2017年05月09日 17:04:44[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：462个人分类：[Python																](https://blog.csdn.net/xuluhui123/article/category/6679259)



\#\#\#python 中[…]
```python
# coding=gbk
```
```python
'''
Created on 2017年5月9日
'''
```
```python
from
```
```python
scipy
```
```python
.
```
```python
misc
```
```python
.
```
```python
pilutil
```
```python
import
```
```python
*
```
```python
# read image ，read 会提示错误，但是不影响使用
```
```python
import
```
```python
matplotlib
```
```python
.
```
```python
pyplot
```
```python
as
```
```python
plt
```
```python
# show image
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
```
```python
# 两个方法都用
```
```python
from
```
```python
numpy
```
```python
import
```
```python
*
```
```python
A
```
```python
=
```
```python
np
```
```python
.
```
```python
zeros
```
```python
(
```
```python
(
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
```python
,
```
```python
dtype
```
```python
=
```
```python
'float'
```
```python
)
```
```python
C
```
```python
=
```
```python
np
```
```python
.
```
```python
zeros
```
```python
(
```
```python
(
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
```python
,
```
```python
dtype
```
```python
=
```
```python
'float'
```
```python
)
```
```python
B
```
```python
=
```
```python
np
```
```python
.
```
```python
array
```
```python
(
```
```python
[
```
```python
[
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
]
```
```python
,
```
```python
[
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
6
```
```python
]
```
```python
]
```
```python
)
```
```python
A3
```
```python
=
```
```python
np
```
```python
.
```
```python
zeros
```
```python
(
```
```python
(
```
```python
2
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
```python
,
```
```python
dtype
```
```python
=
```
```python
'float'
```
```python
)
```
```python
C3
```
```python
=
```
```python
np
```
```python
.
```
```python
zeros
```
```python
(
```
```python
(
```
```python
2
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
```python
,
```
```python
dtype
```
```python
=
```
```python
'float'
```
```python
)
```
```python
B3
```
```python
=
```
```python
np
```
```python
.
```
```python
array
```
```python
(
```
```python
[
```
```python
[
```
```python
[
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
]
```
```python
,
```
```python
[
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
6
```
```python
]
```
```python
]
```
```python
,
```
```python
[
```
```python
[
```
```python
7
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
9
```
```python
]
```
```python
,
```
```python
[
```
```python
10
```
```python
,
```
```python
11
```
```python
,
```
```python
12
```
```python
]
```
```python
]
```
```python
]
```
```python
)
```
```python
A
```
```python
[
```
```python
0
```
```python
,
```
```python
.
```
```python
.
```
```python
.
```
```python
]
```
```python
=
```
```python
B
```
```python
[
```
```python
0
```
```python
,
```
```python
.
```
```python
.
```
```python
.
```
```python
]
```
```python
C
```
```python
[
```
```python
0
```
```python
,
```
```python
:
```
```python
]
```
```python
=
```
```python
B
```
```python
[
```
```python
0
```
```python
,
```
```python
:
```
```python
]
```
```python
A3
```
```python
[
```
```python
0
```
```python
,
```
```python
.
```
```python
.
```
```python
.
```
```python
]
```
```python
=
```
```python
B3
```
```python
[
```
```python
0
```
```python
,
```
```python
:
```
```python
]
```
```python
C3
```
```python
[
```
```python
:
```
```python
]
```
```python
=
```
```python
B3
```
```python
[
```
```python
.
```
```python
.
```
```python
.
```
```python
]
```
```python
print
```
```python
A
```
```python
print
```
```python
C
```
```python
print
```
```python
"############################"
```
```python
print
```
```python
A3
```
```python
print
```
```python
"############################"
```
```python
print
```
```python
C3
```
**result:**
`[[ 1.  2.  3.]
 [ 0.  0.  0.]]
[[ 1.  2.  3.]
 [ 0.  0.  0.]]
############################
[[[ 1.  2.  3.]
  [ 4.  5.  6.]]
 [[ 0.  0.  0.]
  [ 0.  0.  0.]]]
############################
[[[  1.   2.   3.]
  [  4.   5.   6.]]
 [[  7.   8.   9.]
  [ 10.  11.  12.]]]`结果[…]和[:]几乎具有同样的效果，其他情况需要待测。
### arr.flat
`arr1 = np.array([1,2,3])
b = np.zeros(arr1.shape)
b.flat = arr1.flat
print b  # [1,2,3]`
### 词典dict的get函数
词典可以通过键值对访问，也可以通过get访问，区别是：如果元素不存在，通过键值对访问会提示错误，但是通过get返回none，当然也可以通过制定参数的形式，来返回你想要的结果。如下:
`print dic1.get('xlh','你查找的内容不存在!')  #存在的时候，返回值
print dic1.get('gyl')                  #默认返回none，
print dic1.get('gyl','你查找的内容不存在!')  #不存在，返回指定的参数
temp = dic1.get('gyl')
if temp is not None:
    pass # do something`
### pickle模块
`# coding=gbk

from pickle import load,dump
list1 = ['xlh',20,'gyl','21']

with open('text.txt','w') as f:
    dump(list1,f)
    print 'save list1 to text1 file over ....'
    
with open('text2.txt','wb') as f2:
    dump(list1,f2)
    print 'save list1 to text2 file over ...'`
### python清理变量内存
清理变量所占的内存，有两种方法：
```python
var
```
```python
=
```
```python
None
```
```python
;
```
```python
# method 1
```
```python
del
```
```python
var
```
```python
;
```
```python
# method 2
```
方法1虽然会留一个变量名，单几乎不占内存，因此可以忽略。
### dlib for python2.7安装
`conda install -c menpo dlib=18.18`
### copy文件
`import shutil 
filename = 'helloworld.mat'
dstname  = 'D:/helloworld.mat'
shutil.copy(filename,dstname)
print 'copy successfully!'`
### flatten
`from compiler.ast import flatten # 在python3.5下,废弃
from funcy import flatten, isa # 替代，去pypi下载funcy安装即可。`
### 同样的代码在两个不同的环境有不同的结果时
当同样的代码在两个不同的环境有不同的结果时，请仔细检查两个环境中变量的定义的类型是否相同。
### python执行字符串中的表达式和语句
eval:计算字符串中的表达式
exec:执行字符串中的语句
execfile:用来执行一个文件
```python
# 执行表达式
```
```python
x
```
```python
=
```
```python
1
```
```python
print
```
```python
eval
```
```python
(
```
```python
"x+1"
```
```python
)
```
```python
# 执行语句
```
```python
exec
```
```python
(
```
```python
'pose = np.zeros(shape=[10,2],dtype=np.float32)'
```
```python
)
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
range
```
```python
(
```
```python
npose
```
```python
)
```
```python
:
```
```python
exec
```
```python
(
```
```python
'pose'
```
```python
+
```
```python
str
```
```python
(
```
```python
i
```
```python
)
```
```python
+
```
```python
'np.zeros(shape=[10,2],dtype=np.float32)'
```
```python
)
```
### hdf5数据的items()
访问hdf5里面有哪些属性字段
`train_data = h5py.File(train_path)
print(train_data.items())`
### 一维数组(64,)到二维矩阵(64,1)
`>>> import numpy as np
>>> a = np.arange(5)
>>> a.shape
(5L,)
# 方式一：利用 np.expand_dims
>>> np.expand_dims(a, axis=1).shape
(5L, 1L)
# 方式二：利用 np.reshape
>>> np.reshape(a, (-1, 1)).shape
(5L, 1L)
# 方式三：利用 np.newaxis
>>> a[:, np.newaxis].shape
(5L, 1L)
# 方式四：直接操作 shape 属性
>>> b = a.copy()
>>> b.shape = (-1, 1)
>>> b.shape
(5L, 1L)
>>> a.shape
(5L,)
作者：采石工
链接：https://www.zhihu.com/question/59563149/answer/168674704
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。`
### print 格式化矩阵
利用set_printoptions()设置输出矩阵的格式化形式
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
x
```
```python
=
```
```python
np
```
```python
.
```
```python
random
```
```python
.
```
```python
random
```
```python
(
```
```python
10
```
```python
)
```
```python
print
```
```python
(
```
```python
x
```
```python
)
```
```python
# [ 0.07837821  0.48002108  0.41274116  0.82993414  0.77610352  0.1023732
```
```python
#   0.51303098  0.4617183   0.33487207  0.71162095]
```
```python
np
```
```python
.
```
```python
set_printoptions
```
```python
(
```
```python
precision
```
```python
=
```
```python
3
```
```python
)
```
```python
print
```
```python
(
```
```python
x
```
```python
)
```
```python
# [ 0.078  0.48   0.413  0.83   0.776  0.102  0.513  0.462  0.335  0.712]
```
### 获得当前程序的运行的路径
`import os
path_dir = os.getcwd()`
### 在assert后加入说明，
`a = 100
assert a == 1000 , 'please make sure a == 1000'
assert mode in ['training', 'inference']`
### window 版本的 pythonlib
> [https://www.lfd.uci.edu/~gohlke/pythonlibs](https://www.lfd.uci.edu/~gohlke/pythonlibs)

### python装饰器
详细参考知乎：
[https://www.zhihu.com/question/26930016](https://www.zhihu.com/question/26930016)
### *号的使用
参考文献：
[https://www.cnblogs.com/jony7/p/8035376.html](https://www.cnblogs.com/jony7/p/8035376.html)
**1.表示乘号。**
**2.表示倍数**。
```python
def
```
```python
times
```
```python
(
```
```python
msg
```
```python
,
```
```python
time
```
```python
=
```
```python
1
```
```python
)
```
```python
:
```
```python
print
```
```python
(
```
```python
(
```
```python
msg
```
```python
+
```
```python
' '
```
```python
)
```
```python
*
```
```python
time
```
```python
)
```
```python
times
```
```python
(
```
```python
'hello'
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
结果：`hello hello hello`
**3.单个*星号。**
**情况一**：出现在函数定义时的形参上，即*parameter是用来接受任意多个参数并将其放在一个元组中。
```python
def
```
```python
demo
```
```python
(
```
```python
*
```
```python
p
```
```python
)
```
```python
:
```
```python
print
```
```python
(
```
```python
p
```
```python
)
```
```python
demo
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
)
```
结果：`(1, 2, 3)`
**情况二**：函数在调用多个参数时（调用时），在列表、元组、集合、字典及其他可迭代对象作为实参，并在前面加 *。
如   *（1,2,3）解释器将自动进行解包然后传递给多个单变量参数（参数个数要对应相等）。
```python
def
```
```python
d
```
```python
(
```
```python
a
```
```python
,
```
```python
b
```
```python
,
```
```python
c
```
```python
)
```
```python
:
```
```python
print
```
```python
(
```
```python
a
```
```python
,
```
```python
b
```
```python
,
```
```python
c
```
```python
)
```
```python
a
```
```python
=
```
```python
[
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
]
```
```python
d
```
```python
(
```
```python
*
```
```python
a
```
```python
)
```
结果:`1 2 3`
**4. 两个 ** 的情况**
如:    **parameter用于接收类似于关键参数一样赋值的形式的多个实参放入字典中（即把该函数的参数转换为字典）
`def demo(**p):
    for i in p.items():
        print(i)
demo(x=1,y=2)`结果:
`('x', 1)
('y', 2)`
### dir(obj)
dir() 是一个内置函数，用于列出对象的所有属性及方法。
### python中与matlab dir函数相对应的函数为：
`frames = glob.glob('%s%s/*.jpg' %(self.path_to_images, video))`
### 利用路径导入模块
```python
caffe_root
```
```python
=
```
```python
'E:/caffe-ssd-windows-master/python/'
```
```python
# this file is expected to be in {sfd_root}/sfd_test_code/AFW
```
```python
#caffe_root = '/python'
```
```python
#import os
```
```python
#os.chdir(caffe_root)
```
```python
import
```
```python
sys
```
```python
#sys.path.insert(0, 'python')
```
```python
sys
```
```python
.
```
```python
path
```
```python
.
```
```python
insert
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
caffe_root
```
```python
)
```
```python
import
```
```python
caffe
```
```python
#help(caffe) #输出路径
```
### 移除所有的变量
```python
import
```
```python
aflw_version2_angle_radians_correct
```
```python
import
```
```python
sys
sys
```
```python
.
```
```python
modules
```
```python
[
```
```python
__name__
```
```python
]
```
```python
.
```
```python
__dict__
```
```python
.
```
```python
clear
```
```python
(
```
```python
)
```
```python
print
```
```python
(
```
```python
'have removed !'
```
```python
)
```
### call
### 函数
参考文献：[https://www.cnblogs.com/superxuezhazha/p/5793536.html](https://www.cnblogs.com/superxuezhazha/p/5793536.html)

