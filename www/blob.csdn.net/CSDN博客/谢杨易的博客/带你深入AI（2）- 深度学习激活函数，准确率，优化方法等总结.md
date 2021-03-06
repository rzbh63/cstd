
# 带你深入AI（2）- 深度学习激活函数，准确率，优化方法等总结 - 谢杨易的博客 - CSDN博客

2018年04月07日 21:42:11[谢杨易](https://me.csdn.net/u013510838)阅读数：1156


系列文章，请多关注
[Tensorflow源码解析1 – 内核架构和源码结构](https://blog.csdn.net/u013510838/article/details/84103503)
[自然语言处理1 – 分词](https://blog.csdn.net/u013510838/article/details/81673016)
[带你深入AI（1） - 深度学习模型训练痛点及解决方法](https://blog.csdn.net/u013510838/article/details/79835563)
[带你深入AI（2）- 深度学习激活函数，准确率，优化方法等总结](https://blog.csdn.net/u013510838/article/details/79845455)
[带你深入AI（3）- 物体分类领域：AlexNet VGG Inception ResNet mobileNet](https://blog.csdn.net/u013510838/article/details/79866740)
[带你深入AI（4）- 目标检测领域：R-CNN，faster R-CNN，yolo，SSD, yoloV2](https://blog.csdn.net/u013510838/article/details/79947553)
[带你深入AI（5）- 自然语言处理领域：RNN LSTM GRU](https://blog.csdn.net/u013510838/article/details/80024144)
[带你深入AI（6）- 详解bazel](https://blog.csdn.net/u013510838/article/details/80102438)
[带你深入AI（7）- 深度学习重要Python库](https://blog.csdn.net/u013510838/article/details/80412127)
## 1 引言
深度学习训练的基本步骤相信大家都很清楚，本节着重来讲解训练中使用到的激活函数，准确率函数，优化函数都有哪几种，以及它们之间的比较。
## 2 激活函数
根据奥卡姆剃刀法则，神经网络采用的是最简单的线性模型 f(x) = wx+b, 并且线性模型的组合仍然为线性模型。但我们要解决的大部分问题又不仅仅是一个线性问题，因此在线性模型中增加非线性元素就显得尤其重要了。激活函数正是增加非线性的一个重要手段。常见的激活函数有sigmoid relu tanh等
### 2.1 sigmoid
函数表达为
![](https://img.alicdn.com/tfs/TB1cRemj_tYBeNjy1XdXXXXyVXa-1464-622.png)
sigmoid是早期使用较多的激活函数，优点为
输出值为[0, 1], 符合概率分布
x靠近0时斜率很大，对应为神经元兴奋区，远离0的区域斜率很小，对应为神经元抑制区
但其缺点也较为明显
x稍微远离0, 导数就接近于0了，这样在反向传播优化w时，无论w取何值，梯度都很小，也就无法对w更新给出指导了，这种现象称为梯度弥散，在网络层级较深时尤其明显。
需要进行指数计算，比较耗时
### 2.2 tanh
函数表达为
![](https://img.alicdn.com/tfs/TB1ikokjKuSBuNjy1XcXXcYjFXa-1590-708.png)
tanh和sigmoid其实很像，只是y的分布以0为中心。其同样存在梯度弥散和计算耗时的问题。
### 2.3 relu
函数表达为
![](https://img.alicdn.com/tfs/TB15hmoj_tYBeNjy1XdXXXXyVXa-1592-614.png)
relu是当前使用最为频繁的激活函数，在几乎所有的CNN网络均使用relu作为激活函数，它解决了sigmoid和tanh的问题，优点为
x大于0时，导数为1，在反向传播计算梯度时，不存在梯度弥散问题。
x>0时，y=x, x<0时，y=0, 计算十分简单。
针对于relu的x<0部分，出现了一些变种函数，比如ELU和PRELU，均是针对x<0分支的改进，和relu大体相似。
## 3 损失函数
损失函数用来衡量预测值和真实值之间的差距，深度学习训练的目的，就是至少在训练集上，损失函数要达到一个较小的值。这个较小的值可以是局部最优解，也可以是全局最优解。但局部最优解往往不止一个，且不一定理想。全局最优解则很难求取，且往往是一个过拟合的结果。损失函数有如下几种。
### 3.1 平方差
L(Y,f(X))=(Y−f(X))2，其中Y为真实值，f(x)为预测值
描述了真实值和预测值之间的空间距离，线性回归中经常使用，分类问题中一般不使用。
### 3.2 交叉熵
![](https://img.alicdn.com/tfs/TB1QacqjKuSBuNjy1XcXXcYjFXa-1324-166.png)
它描述了两个概率分布之间的距离，在分类问题中使用较广。
TensorFlow支持自定义损失函数，如果对于现成的损失函数不满意，完全可以自己定义一个。
## 4 优化方法
优化方法用来指导如何调整参数w和b，来最有效率地使损失函数达到最小。优化方法最初需要手动设置学习率，来调整参数大小。这种方式比较麻烦，后来慢慢出现了各种自动优化学习率的方法。
### 4.1 BGD（batch gradient descent）
BGD每步迭代中，都需要使用训练集的所有内容来计算梯度和误差，来指导参数的更新。这种方式由于使用了训练集中的所有数据，故能够保证梯度是真实有效不含噪声的，故可以设置一个固定的学习率。但计算量太大。
### 4.2 SGD（stochastic gradient descent）
针对BGD计算量大的改进，随机梯度下降。每次随机抽取一定的样本计算梯度和误差，并更新参数。这种方式计算量不大，但由于采用的是随机样本，故梯度不一定是真实的梯度，带有一定的噪声。随着梯度的下降，我们需要逐渐减小学习率，来让其稳定的落入局部最优解。
### 4.3 Momentum
针对SGD梯度中含有噪声的改进。它将前几次的梯度也参与到本次梯度计算中来，也就是利用梯度之和来指导更新参数。它可以加速学习率，从而减少迭代次数，加快收敛速度。
### 4.4 AdaGrad
可以自动更新学习率，但需要设置一个全局学习率。其学习率的表达式如下
![](https://img.alicdn.com/tfs/TB167CCj_tYBeNjy1XdXXXXyVXa-1250-208.png)
其中，ϵ为全局学习率，我们需要设置一个固定的值。δ是一个很小的常量,大概在e-7, 主要是为了防止分母为0.gi为第i次迭代时的梯度。可以看到随着梯度累加和的增加，学习率会降低，从而使损失函数稳定落入局部最优解。
### 4.5 Adam
Adaptive Moment Estimation， 自适应矩估计，概率论中矩的定义是，如果一个随机变量服从某个分布，则其一阶矩为其平均值，即E(X)。二阶矩为其平方的平均值，即E(X^2)。Adam根据损失函数梯度的一阶矩估计和二阶矩估计来动态调整学习率。其计算过程如下
![](https://img.alicdn.com/tfs/TB11pCGj_tYBeNjy1XdXXXXyVXa-1502-632.png)
其中，g为迭代运算损失函数梯度的累加和，s为梯度的一阶矩估计，r为梯度的二阶矩估计。ρ1和ρ2为衰减系数，一般ρ1=0.9,ρ2=0.999。δ目的是防止分母为0，一般取e-8。ϵ为用户设置的全局学习率，一般可设为e-4。θ为学习率。
系列文章，请多关注
[Tensorflow源码解析1 – 内核架构和源码结构](https://blog.csdn.net/u013510838/article/details/84103503)
[自然语言处理1 – 分词](https://blog.csdn.net/u013510838/article/details/81673016)
[带你深入AI（1） - 深度学习模型训练痛点及解决方法](https://blog.csdn.net/u013510838/article/details/79835563)
[带你深入AI（2）- 深度学习激活函数，准确率，优化方法等总结](https://blog.csdn.net/u013510838/article/details/79845455)
[带你深入AI（3）- 物体分类领域：AlexNet VGG Inception ResNet mobileNet](https://blog.csdn.net/u013510838/article/details/79866740)
[带你深入AI（4）- 目标检测领域：R-CNN，faster R-CNN，yolo，SSD, yoloV2](https://blog.csdn.net/u013510838/article/details/79947553)
[带你深入AI（5）- 自然语言处理领域：RNN LSTM GRU](https://blog.csdn.net/u013510838/article/details/80024144)
[带你深入AI（6）- 详解bazel](https://blog.csdn.net/u013510838/article/details/80102438)
[带你深入AI（7）- 深度学习重要Python库](https://blog.csdn.net/u013510838/article/details/80412127)

