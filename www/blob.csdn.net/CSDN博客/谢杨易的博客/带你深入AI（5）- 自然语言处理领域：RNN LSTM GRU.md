
# 带你深入AI（5）- 自然语言处理领域：RNN LSTM GRU - 谢杨易的博客 - CSDN博客

2018年04月20日 21:14:32[谢杨易](https://me.csdn.net/u013510838)阅读数：940


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
# 1 引言
深度学习算法模型大致分为三类，物体分类，目标检测和自然语言处理。前面两章我们分析了物体分类算法和目标检测算法，着重讲解了算法执行流程，优缺点，以及他们的优化技巧。本文分析最后一个大类，即自然语言处理领域。
与物体分类和目标检测不同，自然语言处理中，前一个时刻和后一个时刻会对我们当前的输出结果产生影响，也就是网络模型是与时序相关的。比如“我是法国人，我会说“这个句子中，我们要预测最后的词语，需要结合前面的词语来分析。针对这个问题，业界提出了循环神经网络RNN，以及它的变种LSTM，GRU等。
# 2 RNN
普通的CNN网络，没有时序的概念。输入层经过隐藏层后，达到输出层，从而得到结果。而RNN中，输入层和上一时刻的隐藏层，会一起进入到当前时刻的隐藏层，最后到达输出层。如下图所示
![](https://img.alicdn.com/tfs/TB1Ze7MnhGYBuNjy0FnXXX5lpXa-1600-654.png)
Xt时刻的输入和前一时刻的隐藏层St-1，共同进入到当前时刻的隐藏层St中，经过运算得到输出层Ot。当前时刻的隐藏层不仅仅和当前时刻的输入有关，还和上一时刻，以及上上时刻的隐藏层有关。所以称之为循环神经网络。为了减少计算量，借鉴卷积神经网络中的权值共享的理念，每个时刻的网络结构以及参数，如U V W都是相同的。这样，理论上我们可以构建无限长的RNN网络。
## 2.1 RNN结构分类
根据网络结构，RNN的变种有双向RNN和deep RNN
### 2.1.1 双向RNN
单向RNN中，当前时刻的隐藏层只和之前时刻相关。但对于很多问题，当前时刻与之前和未来都有关系。因此提出了双向RNN。当前时刻的隐藏层和之前时刻和之后时刻都相关。如下图
![](https://img.alicdn.com/tfs/TB13sqpnDtYBeNjy1XdXXXXyVXa-1562-532.png)
### 2.1.2 deep RNN
为了抽取更为高阶的特征，我们可以将每一个隐藏层变为多层的结构，从而提高泛化性。这样网络具有更强大的学习和表达能力。但复杂度也会大大提高。如下图
![](https://img.alicdn.com/tfs/TB10XEHnamWBuNjy1XaXXXCbXXa-1490-654.png)
## 2.2 RNN使用场景分类
根据输入和输出的数据对比，可以分为N对1， 1对N，N对N，N对M等结构。
![](https://img.alicdn.com/tfs/TB11NSqnDtYBeNjy1XdXXXXyVXa-1380-992.png)
N对1的网络如上图，我们并不需要得到每个时刻网络的输出，仅仅需要得到最后的输出而已。比如预测”我是中国人，我会说“ 这句话中的最后一个词语。
![](https://img.alicdn.com/tfs/TB1ZNSqnDtYBeNjy1XdXXXXyVXa-1504-1120.png)
1对N如上图所示，针对同一个输入得到不同时刻的输出。比如针对一幅图，我们预测出它对应的一句话（看图说话）。
![](https://img.alicdn.com/tfs/TB1TxSqnDtYBeNjy1XdXXXXyVXa-1460-786.png)
N对M如上图所示，这个场景比较复杂，比如机器翻译。将一句中文翻译为一句英文。这种场景称为seq2seq，我们一般会建立一个encoder和一个decoder。先将N个输入建模，得到encoder，然后decode出M个输出。
## 2.3 RNN训练难点
RNN有个最大的问题，就是对于距离当前时刻比较远的隐藏层，容易出现梯度爆炸或者梯度弥散。这是由于在反向传播求解梯度过程中，各个隐藏层的参数会累乘，当特征值小于1时，不断相乘导致特征值向0衰减。特征值大于1时，则向正无穷扩增。导致无法利用梯度来对距离比较远的时刻进行调节，称之为长距离依赖。
因为长距离依赖问题，我们无法训练比较长的序列，导致RNN的应用领域大大受限。为了解决这个问题，LSTM网络应运而生。
# 3 LSTM
LSTM全称为Long short Term Memory，它通过门控的方式，使得不断相乘的梯度的积保持在接近1的数值，从而使得长序列仍然可以训练。门控，也就是gates，是一个（0,1) 的数值，它控制信息的流动量。如果为0表示信息不通过，为1则表示信息全部通过。gate也是由神经网络训练得到的，通过它可以动态的控制当前时刻的输入量以及上一时刻隐藏层的输入量。
## 3.1 网络结构
LSTM的网络结构如下所示
![](https://img.alicdn.com/tfs/TB1BZkXnkyWBuNjy0FpXXassXXa-1692-670.png)
网络结构中包含三部分，输入层Xt, Xt-1, Xt+1, 输出隐藏层ht ht-1 ht+1, 记忆单元Ct Ct-1 Ct+1。我们的目标是通过输入层和记忆单元得到隐藏层。输入层和隐藏层很容易理解，记忆单元是LSTM中提出的，它用以保存当前时刻网络信息，并作用到下一个时刻。
LSTM中包含三个门，遗忘门，输入门和输出门，对应到上图中的三个sigmoid方框。回顾一下sigmoid函数，它输出(0, 1)区间的数值，用于决定多少信息可以通过。0表示全部不通过，1表示全部通过。LSTM的三个门的作用我们分别讲解
### 3.1.1 遗忘门
最左边的是遗忘门，它用于决定有多少上一时刻的记忆单元信息Ct-1可以作用到当前时刻。也就是说有多少上一时刻的信息需要遗忘，或者叫舍弃。它的结构图如下
![](https://img.alicdn.com/tfs/TB1KrMcnkyWBuNjy0FpXXassXXa-1300-460.png)
遗忘门的输出ft会作用到前一时刻记忆单元Ct-1上，从而决定有多少上一时刻信息需要遗忘掉。ft与当前输入层Xt和前一时刻隐藏层ht-1相关，计算公式如上所示。
### 3.1.2 输入门
中间一个是输入门，它用于决定当前多少输入信息可以作用到最终的结果中。结构图如下
![](https://img.alicdn.com/tfs/TB1ThnmngmTBuNjy1XbXXaMrVXa-1354-546.png)
通过输入门控制后的输入信息it, 会作用到当前输入和上一时刻隐藏层上（当前输入和上一时刻隐藏层在RNN中都是输入信息），最终和经过遗忘门作用后的前一时刻记忆单元Ct-1，累加后得到当前时刻的记忆单元Ct。这个门主要是处理输入信息（当前输入层和上一时刻隐藏层）， 故称之为输入门。其计算公式如上图。
### 3.1.3 输出门
最后，我们需要决定当前记忆单元中有多少东西需要输出到隐藏层中，输出门结构如下图所示
![](https://img.alicdn.com/tfs/TB1Fn3dnkyWBuNjy0FpXXassXXa-1298-540.png)
经过上图中的输出门后的信息Ot, 会作用到当前记忆单元Ct上，从而控制有多少当前记忆单元信息可以传递到隐藏层中。故称之为输出门。
上面可能会有个问题，为啥采用tanh作为激活函数，而不是relu呢。因为relu在大于0时，输出为y=x, 可能会导致非常大的输出值。而tanh输出范围永远在(-1, 1)之间。
# 4 GRU
GRU是LSTM的变种，它将LSTM中的遗忘门和输入门合并成了一个更新门，也就是说你需要输入多少新内容时，就需要遗忘掉一定量的上一时刻信息。另外，为了简化结构，它将记忆单元和隐藏层合并，直接用隐藏层来表示记忆单元。结构如下
![](https://img.alicdn.com/tfs/TB1IlqznDtYBeNjy1XdXXXXyVXa-1294-566.png)
其中包含两个gate，为reset gate rt和update gate Zt。更新门的一路类似于LSTM中的遗忘门，表示多少上一时刻的隐藏层信息（其实也是记忆单元）会作用到这个时刻中。更新门的另一路则类似于LSTM的输入门，表示多少输入信息（当前时刻输入Xt和上一时刻隐藏层ht-1）会作用到这个时刻中。
而重置门则作用在上一时刻隐藏层上，并和当前时刻输入值一起，经过激活函数后，作用到当前时刻输出中。如果重置门为0，则之前隐藏层信息会丢弃，这样允许模型去掉一些和当前无关的信息。
GRU相比LSTM结构更简单，参数更少，因此应用也比较广泛。
# 5 总结
RNN的网络模型不像CNN那样种类繁多，现在使用得比较多的就是LSTM和它的变种GRU。它们都是为了解决RNN中的长距离梯度弥散问题的，也就是长程依赖问题。
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

