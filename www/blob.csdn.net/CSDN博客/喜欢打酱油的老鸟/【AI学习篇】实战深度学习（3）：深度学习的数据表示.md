
# 【AI学习篇】实战深度学习（3）：深度学习的数据表示 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月10日 08:31:12[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：3


[https://www.toutiao.com/a6677188362316546564/](https://www.toutiao.com/a6677188362316546564/)
**医工荟萃，不是萝卜开会，融合创新才是硬道理！**
**预计阅读时间： 4 分钟**
上一讲中荟荟用keras演示了使用全连接神经网络如何实现手写数字的识别，当然这个神经网络还是相当的"浅"。在正式开始进入深度学习前，荟荟认为作为初学者，很有必要先了解一下神经网络里的数据表示方法，即TensorFlow这个词里的**Tensor(张量)**，以及张量的基本操作和运算方法。
上一讲发出后，被吐槽太长了，看得累。。。所以这讲荟荟先来说说啥是张量，下一讲再说说它的基本操作及运算，再下一讲正式进入深度学习。
啥叫张量(Tensor)?
前面的例子中，我们的数据都是存储在Numpy数组中的，在深度学习中，我们对这种多维数组有个专门的名字叫做**张量“Tensor”**, 携带信息的张量(Tensor)在神经网络中不断流动（Flow），所以知名的深度学习框架叫做TensorFlow。在这里我们无需理解张量在数学中的含义----多重线性映射（听起来就晕死），只需理解**张量是矩阵向任意维度的推广**。下面用实际的例子来解释一下，就一清二楚了：
标量(0维张量)
在Numpy中使用只有一个float32或float64数构成的数组，叫做0维张量，它的维数为0**(张量的维数有时也称之为轴，故标量张量的轴为0)**。**代码中的x.ndim给出数组x的轴数**。
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p1.pstatp.com/large/pgc-image/fa63af090c3e40e2a207645d39063bd6)
标量(0维张量)
向量(1维张量)
一系列数字构成的向量，也称为1维（1D）张量，有1个轴。
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p3.pstatp.com/large/pgc-image/8e5a670131584305a6b96801d8dd48fc)
向量(1维张量)
矩阵（2维张量）
多个向量组成的矩阵，称为2维(2D)张量，有两个轴，**第一个轴叫做行(row)，第二个轴叫做列(column)**。
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p1.pstatp.com/large/pgc-image/20c398ef729641028abb5d4ec8fd6937)
矩阵（2维张量）
3维张量或更高维张量
将多个矩阵组成数组，则构成3维(3D)张量，有三个轴，第一个轴叫做**批量轴(batch-axis)**，可以理解为：**如果某个矩阵代表一个样本比如一个灰度图像，批量轴就是有多少个样本。第二个轴是行，第三个轴是列。通常来说深度学习中的数据的第一个轴（0轴，索引从0开始）都是批量轴。**
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p1.pstatp.com/large/pgc-image/f5507ef0fbc446c98f604198515a48f1)
3维张量或更高维张量
本例中除了使用x.ndim看有多少个轴外，还可以使用**x.shape**看一下这个3D张量x的形状，可以发现x是由三个3*5的矩阵构成的（3, 3 ,5）。**x.shape返回的是一个python元组tuple（ 就是一个常量列表，**python语言的基础入门在这里就不再赘述，一点基础没有的朋友，**推荐大家搜索公众号“crossin的编程教室”**，里面有非常简洁明了的入门教程哦！）
至于4维张量，就是由多个3维张量构成的数组，比如做图像识别的彩色图像张量就是4维张量，在TensorFlow里的形状就是(样本数/批量，高，宽，颜色通道)。**深度学习里最常用的张量也就是0D-4D的张量了**。
0D-6D张量的形象理解如下图所示。
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p1.pstatp.com/large/pgc-image/e466ddd049984f01b32d0867fabb3390)
0D-6D张量
除了上述介绍的轴的个数(ndim),张量形状(shape), 张量同学还有一个重要属性是**数据类型(dtype)**,通常包括float32、float64、uint8等。以上讲中mnist数据集为例（见下图），训练集是一个3维张量，形状是(60000, 28, 28)即60000个28*28的灰度图像，图像像素的数据类型是uint8，也就是取值范围为0-255的整形数。
![【AI学习篇】实战深度学习（3）：深度学习的数据表示](http://p1.pstatp.com/large/pgc-image/3eff36de469b406bb9f22c0a549b5dd8)
mnist数据集的张量属性
总结：本讲应该了解啥是张量，0D-4D的张量代表了什么，张量的主要属性**ndim、shape、dtype**。下一讲荟荟来说说张量的基本操作和运算。

