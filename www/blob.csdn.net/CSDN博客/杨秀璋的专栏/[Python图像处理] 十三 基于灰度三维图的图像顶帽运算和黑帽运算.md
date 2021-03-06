
# [Python图像处理] 十三.基于灰度三维图的图像顶帽运算和黑帽运算 - 杨秀璋的专栏 - CSDN博客

2019年03月21日 16:33:15[Eastmount](https://me.csdn.net/Eastmount)阅读数：419标签：[Python图像处理																](https://so.csdn.net/so/search/s.do?q=Python图像处理&t=blog)[顶帽运算																](https://so.csdn.net/so/search/s.do?q=顶帽运算&t=blog)[黑帽运算																](https://so.csdn.net/so/search/s.do?q=黑帽运算&t=blog)[形态学变换																](https://so.csdn.net/so/search/s.do?q=形态学变换&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=黑帽运算&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=顶帽运算&t=blog)个人分类：[Python图像处理																](https://blog.csdn.net/Eastmount/article/category/7912787)[数字图像处理																](https://blog.csdn.net/Eastmount/article/category/1858075)[
							](https://blog.csdn.net/Eastmount/article/category/7912787)
所属专栏：[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)
[
																	](https://so.csdn.net/so/search/s.do?q=顶帽运算&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Python图像处理&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Python图像处理&t=blog)

该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~
同时推荐作者的C++图像系列知识：
[[数字图像处理] 一.MFC详解显示BMP格式图片](https://blog.csdn.net/eastmount/article/details/18238863)
[[数字图像处理] 二.MFC单文档分割窗口显示图片](https://blog.csdn.net/eastmount/article/details/18987539)
[[数字图像处理] 三.MFC实现图像灰度、采样和量化功能详解](https://blog.csdn.net/eastmount/article/details/46010637)
[[数字图像处理] 四.MFC对话框绘制灰度直方图](https://blog.csdn.net/eastmount/article/details/46237463)
[[数字图像处理] 五.MFC图像点运算之灰度线性变化、灰度非线性变化、阈值化和均衡化处理详解](https://blog.csdn.net/eastmount/article/details/46312145)
[[数字图像处理] 六.MFC空间几何变换之图像平移、镜像、旋转、缩放详解](https://blog.csdn.net/eastmount/article/details/46345299)
[[数字图像处理] 七.MFC图像增强之图像普通平滑、高斯平滑、Laplacian、Sobel、Prewitt锐化详解](https://blog.csdn.net/eastmount/article/details/46378783)
前文参考：
[[Python图像处理] 一.图像处理基础知识及OpenCV入门函数](https://blog.csdn.net/Eastmount/article/details/81748802)
[[Python图像处理] 二.OpenCV+Numpy库读取与修改像素](https://blog.csdn.net/eastmount/article/details/82120114)
[[Python图像处理] 三.获取图像属性、兴趣ROI区域及通道处理](https://blog.csdn.net/eastmount/article/details/82177300)
[[Python图像处理] 四.图像平滑之均值滤波、方框滤波、高斯滤波及中值滤波](https://blog.csdn.net/Eastmount/article/details/82216380)
[[Python图像处理] 五.图像融合、加法运算及图像类型转换](https://blog.csdn.net/Eastmount/article/details/82347501)
[[Python图像处理] 六.图像缩放、图像旋转、图像翻转与图像平移](https://blog.csdn.net/Eastmount/article/details/82454335)
[[Python图像处理] 七.图像阈值化处理及算法对比](https://blog.csdn.net/Eastmount/article/details/83548652)
[[Python图像处理] 八.图像腐蚀与图像膨胀](https://blog.csdn.net/Eastmount/article/details/83581277)
[[Python图像处理] 九.形态学之图像开运算、闭运算、梯度运算](https://blog.csdn.net/Eastmount/article/details/83651172)
[[Python图像处理] 十.形态学之图像顶帽运算和黑帽运算](https://blog.csdn.net/Eastmount/article/details/83692456)
[[Python图像处理] 十一.灰度直方图概念及OpenCV绘制直方图](https://blog.csdn.net/Eastmount/article/details/83758402)
[[Python图像处理] 十二.图像几何变换之图像仿射变换、图像透视变换和图像校正](https://blog.csdn.net/Eastmount/article/details/88679772)
前面的第十篇文章讲解过图形形态学变换——顶帽运算和黑帽运算，本篇文章继续深入，结合灰度三维图像讲解图像顶帽运算和图像黑猫运算，通过Python调用OpenCV函数实。基础性知识希望对您有所帮助。
**1.图像顶帽运算**
**2.图像黑帽运算**
**3.基于灰度三维图的顶帽黑帽运算**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时，本篇文章涉及到《计算机图形学》基础知识，请大家下来补充。
参考文献：
《数字图像处理》（第3版），冈萨雷斯著，阮秋琦译，电子工业出版社，2013年
[光照不均匀图像分割技巧2——顶帽变换和底帽变换](https://blog.csdn.net/kk55guang2/article/details/78490069)
[基于opencv绘制图片的三维空间显示图（python）](https://blog.csdn.net/yefcion/article/details/80883605)
---

# 一.图像顶帽运算
图像顶帽运算（top-hat transformation）又称为图像礼帽运算，它是用原始图像减去图像开运算后的结果，常用于解决由于光照不均匀图像分割出错的问题。其公式定义如下：
![](https://img-blog.csdnimg.cn/20190321155840897.png)[
](https://img-blog.csdnimg.cn/20190321155840897.png)图像顶帽运算是用一个结构元通过开运算从一幅图像中删除物体，校正不均匀光照的影响，其效果图如下图所示。
![](https://img-blog.csdnimg.cn/20190321160009595.png)[
](https://img-blog.csdnimg.cn/20190321160009595.png)在Python中，图像顶帽运算主要调用morphologyEx()实现，其中参数cv2.MORPH_TOPHAT表示顶帽处理，函数原型如下：
**dst = cv2.morphologyEx(src, cv2.MORPH_TOPHAT, kernel)**
src表示原始图像
cv2.MORPH_TOPHAT表示图像顶帽运算
kernel表示卷积核，可以用numpy.ones()函数构建
假设存在一张光照不均匀的米粒图像，如图所示，我们需要调用图像顶帽运算解决光照不均匀的问题。其Python代码如下所示：
![](https://img-blog.csdnimg.cn/20190321160147409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321160147409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
```python
#encoding:utf-8
```
```python
import
```
```python
cv2
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
#读取图片
```
```python
src
```
```python
=
```
```python
cv2
```
```python
.
```
```python
imread
```
```python
(
```
```python
'test06.png'
```
```python
,
```
```python
cv2
```
```python
.
```
```python
IMREAD_UNCHANGED
```
```python
)
```
```python
#设置卷积核
```
```python
kernel
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
ones
```
```python
(
```
```python
(
```
```python
10
```
```python
,
```
```python
10
```
```python
)
```
```python
,
```
```python
np
```
```python
.
```
```python
uint8
```
```python
)
```
```python
#图像顶帽运算
```
```python
result
```
```python
=
```
```python
cv2
```
```python
.
```
```python
morphologyEx
```
```python
(
```
```python
src
```
```python
,
```
```python
cv2
```
```python
.
```
```python
MORPH_TOPHAT
```
```python
,
```
```python
kernel
```
```python
)
```
```python
#显示图像
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"src"
```
```python
,
```
```python
src
```
```python
)
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"result"
```
```python
,
```
```python
result
```
```python
)
```
```python
#等待显示
```
```python
cv2
```
```python
.
```
```python
waitKey
```
```python
(
```
```python
0
```
```python
)
```
```python
cv2
```
```python
.
```
```python
destroyAllWindows
```
```python
(
```
```python
)
```
其运行结果如下，它有效地将米粒与背景分离开来。
![](https://img-blog.csdnimg.cn/20190321160457293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321160457293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
---

# 二.图像黑帽运算
图像底帽运算（bottom-hat transformation）又称为图像黑帽运算，它是用图像闭运算操作减去原始图像后的结果，从而获取图像内部的小孔或前景色中黑点，也常用于解决由于光照不均匀图像分割出错的问题。其公式定义如下：
![](https://img-blog.csdnimg.cn/20190321160555501.png)[
](https://img-blog.csdnimg.cn/20190321160555501.png)图像底帽运算是用一个结构元通过闭运算从一幅图像中删除物体，常用于校正不均匀光照的影响。其效果图如下图所示。
![](https://img-blog.csdnimg.cn/20190321160645596.png)[
](https://img-blog.csdnimg.cn/20190321160645596.png)在Python中，图像底帽运算主要调用morphologyEx()实现，其中参数cv2.MORPH_BLACKHAT表示底帽或黑帽处理，函数原型如下：
**dst = cv2.morphologyEx(src, cv2.MORPH_BLACKHAT, kernel)**
src表示原始图像
cv2.MORPH_BLACKHAT表示图像底帽或黑帽运算
kernel表示卷积核，可以用numpy.ones()函数构建
Python实现图像底帽运算的代码如下所示：
```python
#encoding:utf-8
```
```python
import
```
```python
cv2
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
#读取图片
```
```python
src
```
```python
=
```
```python
cv2
```
```python
.
```
```python
imread
```
```python
(
```
```python
'test06.png'
```
```python
,
```
```python
cv2
```
```python
.
```
```python
IMREAD_UNCHANGED
```
```python
)
```
```python
#设置卷积核
```
```python
kernel
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
ones
```
```python
(
```
```python
(
```
```python
10
```
```python
,
```
```python
10
```
```python
)
```
```python
,
```
```python
np
```
```python
.
```
```python
uint8
```
```python
)
```
```python
#图像黑帽运算
```
```python
result
```
```python
=
```
```python
cv2
```
```python
.
```
```python
morphologyEx
```
```python
(
```
```python
src
```
```python
,
```
```python
cv2
```
```python
.
```
```python
MORPH_BLACKHAT
```
```python
,
```
```python
kernel
```
```python
)
```
```python
#显示图像
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"src"
```
```python
,
```
```python
src
```
```python
)
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"result"
```
```python
,
```
```python
result
```
```python
)
```
```python
#等待显示
```
```python
cv2
```
```python
.
```
```python
waitKey
```
```python
(
```
```python
0
```
```python
)
```
```python
cv2
```
```python
.
```
```python
destroyAllWindows
```
```python
(
```
```python
)
```
其运行结果如图所示：
![](https://img-blog.csdnimg.cn/20190321160830441.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321160830441.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
---

# 三.基于灰度三维图的顶帽黑帽运算
![](https://img-blog.csdnimg.cn/20190321161317453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321161317453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)为什么图像顶帽运算会消除光照不均匀的效果呢？通常可以利用灰度三维图来进行解释该算法。灰度三维图主要调用Axes3D包实现，对原图绘制灰度三维图的代码如下：
```python
# -*- coding: utf-8 -*-
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
import
```
```python
cv2
```
```python
as
```
```python
cv
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
from
```
```python
mpl_toolkits
```
```python
.
```
```python
mplot3d
```
```python
import
```
```python
Axes3D
```
```python
from
```
```python
matplotlib
```
```python
import
```
```python
cm
```
```python
from
```
```python
matplotlib
```
```python
.
```
```python
ticker
```
```python
import
```
```python
LinearLocator
```
```python
,
```
```python
FormatStrFormatter
```
```python
#读取图像
```
```python
img
```
```python
=
```
```python
cv
```
```python
.
```
```python
imread
```
```python
(
```
```python
"test06.png"
```
```python
)
```
```python
img
```
```python
=
```
```python
cv
```
```python
.
```
```python
cvtColor
```
```python
(
```
```python
img
```
```python
,
```
```python
cv
```
```python
.
```
```python
COLOR_BGR2GRAY
```
```python
)
```
```python
imgd
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
img
```
```python
)
```
```python
#image类转numpy
```
```python
#准备数据
```
```python
sp
```
```python
=
```
```python
img
```
```python
.
```
```python
shape
h
```
```python
=
```
```python
int
```
```python
(
```
```python
sp
```
```python
[
```
```python
0
```
```python
]
```
```python
)
```
```python
#图像高度(rows)
```
```python
w
```
```python
=
```
```python
int
```
```python
(
```
```python
sp
```
```python
[
```
```python
1
```
```python
]
```
```python
)
```
```python
#图像宽度(colums) of image
```
```python
#绘图初始处理
```
```python
fig
```
```python
=
```
```python
plt
```
```python
.
```
```python
figure
```
```python
(
```
```python
figsize
```
```python
=
```
```python
(
```
```python
16
```
```python
,
```
```python
12
```
```python
)
```
```python
)
```
```python
ax
```
```python
=
```
```python
fig
```
```python
.
```
```python
gca
```
```python
(
```
```python
projection
```
```python
=
```
```python
"3d"
```
```python
)
```
```python
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
arange
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
w
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
y
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
arange
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
h
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
x
```
```python
,
```
```python
y
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
meshgrid
```
```python
(
```
```python
x
```
```python
,
```
```python
y
```
```python
)
```
```python
z
```
```python
=
```
```python
imgd
surf
```
```python
=
```
```python
ax
```
```python
.
```
```python
plot_surface
```
```python
(
```
```python
x
```
```python
,
```
```python
y
```
```python
,
```
```python
z
```
```python
,
```
```python
cmap
```
```python
=
```
```python
cm
```
```python
.
```
```python
coolwarm
```
```python
)
```
```python
#自定义z轴
```
```python
ax
```
```python
.
```
```python
set_zlim
```
```python
(
```
```python
-
```
```python
10
```
```python
,
```
```python
255
```
```python
)
```
```python
ax
```
```python
.
```
```python
zaxis
```
```python
.
```
```python
set_major_locator
```
```python
(
```
```python
LinearLocator
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
)
```
```python
#设置z轴网格线的疏密
```
```python
#将z的value字符串转为float并保留2位小数
```
```python
ax
```
```python
.
```
```python
zaxis
```
```python
.
```
```python
set_major_formatter
```
```python
(
```
```python
FormatStrFormatter
```
```python
(
```
```python
'%.02f'
```
```python
)
```
```python
)
```
```python
# 设置坐标轴的label和标题
```
```python
ax
```
```python
.
```
```python
set_xlabel
```
```python
(
```
```python
'x'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_ylabel
```
```python
(
```
```python
'y'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_zlabel
```
```python
(
```
```python
'z'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_title
```
```python
(
```
```python
"surface plot"
```
```python
,
```
```python
weight
```
```python
=
```
```python
'bold'
```
```python
,
```
```python
size
```
```python
=
```
```python
20
```
```python
)
```
```python
#添加右侧的色卡条
```
```python
fig
```
```python
.
```
```python
colorbar
```
```python
(
```
```python
surf
```
```python
,
```
```python
shrink
```
```python
=
```
```python
0.6
```
```python
,
```
```python
aspect
```
```python
=
```
```python
8
```
```python
)
```
```python
plt
```
```python
.
```
```python
show
```
```python
(
```
```python
)
```
运行结果如下图所示：
![](https://img-blog.csdnimg.cn/20190321160956433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321160956433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)从图像中的像素走势显示了该图受各部分光照不均匀的影响，从而造成背景灰度不均现象，其中凹陷对应图像中灰度值比较小的区域。而通过图像白帽运算后的图像灰度三维图的代码如下：
```python
# -*- coding: utf-8 -*-
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
import
```
```python
cv2
```
```python
as
```
```python
cv
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
from
```
```python
mpl_toolkits
```
```python
.
```
```python
mplot3d
```
```python
import
```
```python
Axes3D
```
```python
from
```
```python
matplotlib
```
```python
import
```
```python
cm
```
```python
from
```
```python
matplotlib
```
```python
.
```
```python
ticker
```
```python
import
```
```python
LinearLocator
```
```python
,
```
```python
FormatStrFormatter
```
```python
#读取图像
```
```python
img
```
```python
=
```
```python
cv
```
```python
.
```
```python
imread
```
```python
(
```
```python
"test06.png"
```
```python
)
```
```python
img
```
```python
=
```
```python
cv
```
```python
.
```
```python
cvtColor
```
```python
(
```
```python
img
```
```python
,
```
```python
cv
```
```python
.
```
```python
COLOR_BGR2GRAY
```
```python
)
```
```python
#图像黑帽运算
```
```python
kernel
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
ones
```
```python
(
```
```python
(
```
```python
10
```
```python
,
```
```python
10
```
```python
)
```
```python
,
```
```python
np
```
```python
.
```
```python
uint8
```
```python
)
```
```python
result
```
```python
=
```
```python
cv
```
```python
.
```
```python
morphologyEx
```
```python
(
```
```python
img
```
```python
,
```
```python
cv
```
```python
.
```
```python
MORPH_BLACKHAT
```
```python
,
```
```python
kernel
```
```python
)
```
```python
#image类转numpy
```
```python
imgd
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
result
```
```python
)
```
```python
#准备数据
```
```python
sp
```
```python
=
```
```python
result
```
```python
.
```
```python
shape
h
```
```python
=
```
```python
int
```
```python
(
```
```python
sp
```
```python
[
```
```python
0
```
```python
]
```
```python
)
```
```python
#图像高度(rows)
```
```python
w
```
```python
=
```
```python
int
```
```python
(
```
```python
sp
```
```python
[
```
```python
1
```
```python
]
```
```python
)
```
```python
#图像宽度(colums) of image
```
```python
#绘图初始处理
```
```python
fig
```
```python
=
```
```python
plt
```
```python
.
```
```python
figure
```
```python
(
```
```python
figsize
```
```python
=
```
```python
(
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
)
```
```python
)
```
```python
ax
```
```python
=
```
```python
fig
```
```python
.
```
```python
gca
```
```python
(
```
```python
projection
```
```python
=
```
```python
"3d"
```
```python
)
```
```python
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
arange
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
w
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
y
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
arange
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
h
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
x
```
```python
,
```
```python
y
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
meshgrid
```
```python
(
```
```python
x
```
```python
,
```
```python
y
```
```python
)
```
```python
z
```
```python
=
```
```python
imgd
surf
```
```python
=
```
```python
ax
```
```python
.
```
```python
plot_surface
```
```python
(
```
```python
x
```
```python
,
```
```python
y
```
```python
,
```
```python
z
```
```python
,
```
```python
cmap
```
```python
=
```
```python
cm
```
```python
.
```
```python
coolwarm
```
```python
)
```
```python
#自定义z轴
```
```python
ax
```
```python
.
```
```python
set_zlim
```
```python
(
```
```python
-
```
```python
10
```
```python
,
```
```python
255
```
```python
)
```
```python
ax
```
```python
.
```
```python
zaxis
```
```python
.
```
```python
set_major_locator
```
```python
(
```
```python
LinearLocator
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
)
```
```python
#设置z轴网格线的疏密
```
```python
#将z的value字符串转为float并保留2位小数
```
```python
ax
```
```python
.
```
```python
zaxis
```
```python
.
```
```python
set_major_formatter
```
```python
(
```
```python
FormatStrFormatter
```
```python
(
```
```python
'%.02f'
```
```python
)
```
```python
)
```
```python
# 设置坐标轴的label和标题
```
```python
ax
```
```python
.
```
```python
set_xlabel
```
```python
(
```
```python
'x'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_ylabel
```
```python
(
```
```python
'y'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_zlabel
```
```python
(
```
```python
'z'
```
```python
,
```
```python
size
```
```python
=
```
```python
15
```
```python
)
```
```python
ax
```
```python
.
```
```python
set_title
```
```python
(
```
```python
"surface plot"
```
```python
,
```
```python
weight
```
```python
=
```
```python
'bold'
```
```python
,
```
```python
size
```
```python
=
```
```python
20
```
```python
)
```
```python
#添加右侧的色卡条
```
```python
fig
```
```python
.
```
```python
colorbar
```
```python
(
```
```python
surf
```
```python
,
```
```python
shrink
```
```python
=
```
```python
0.6
```
```python
,
```
```python
aspect
```
```python
=
```
```python
8
```
```python
)
```
```python
plt
```
```python
.
```
```python
show
```
```python
(
```
```python
)
```
效果图如下所示，对应的灰度更集中于10至100区间，由此证明了不均匀的背景被大致消除了，有利于后续的阈值分割或图像分割。
![](https://img-blog.csdnimg.cn/20190321161207505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190321161207505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
---
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近连续奔波考博，经历的事情太多，有喜有悲，需要改变自己好好对家人，也希望读者与我一起加油。
（By：Eastmount 2019-03-21 下午6点[https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)

