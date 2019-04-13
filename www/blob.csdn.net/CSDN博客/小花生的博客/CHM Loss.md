
# CHM Loss - 小花生的博客 - CSDN博客


2019年03月18日 21:37:08[Peanut_范](https://me.csdn.net/u013841196)阅读数：34标签：[CHM loss																](https://so.csdn.net/so/search/s.do?q=CHM loss&t=blog)[样本不均衡																](https://so.csdn.net/so/search/s.do?q=样本不均衡&t=blog)[损失敏感																](https://so.csdn.net/so/search/s.do?q=损失敏感&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=样本不均衡&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=CHM loss&t=blog)个人分类：[损失函数																](https://blog.csdn.net/u013841196/article/category/8636876)
[
																								](https://so.csdn.net/so/search/s.do?q=CHM loss&t=blog)


## CHM Loss
《Gradient Harmonized Single-stage Detector》
2019，Buyu Li et al. CHM Loss
代码：[https://github.com/libuyu/GHM_Detection](https://github.com/libuyu/GHM_Detection)
1.引言：
one-stage的目标检测算法一直存在的问题是正负样本不均衡，简单和困难样本的不均衡。在one-stage算法中，负样本的数量要远远大于正样本，而且大多数负样本是简单样本（well-classified）。单个简单负样本的梯度虽然小，但是由于数量过大，会导致简单负样本主导模型的训练。在《focal loss》中通过大大降低简单样本的分类loss来平衡正负样本，但是设计的loss引入了两个需要通过实验来调整的超参数α和γ。
本篇论文从梯度的角度出发，提出gradient harmonizing mechanism（GHM）来解决样本不均衡的问题，GHM思想不仅可以应用于anchor的分类，同时也可以应用于坐标回归。
2.GHM（梯度均衡机制）
首先我们要定义统计对象——梯度模长（gradient norm）。考虑简单的二分类交叉熵损失函数（binary cross entropy loss）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318210808415.png)
其中 p=sigmoid(x) 为模型所预测的样本类别的概率，p* 是对应的监督。则其对 x 的梯度（导数）为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318210826996.png)
于是我们可以定义一个梯度模长，g=|p-p*|。
对一个交叉熵损失函数训练收敛的单阶段检测模型，样本梯度模长的分布统计如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318210857248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
于是某个样本的g值大小就可以表现这个样本是简单样本还是困难样本。从一个收敛的检测模型中统计样本梯度的分布情况如下上图所示。从图中我们可以看出，与之前所想一样，简单样本的数量要远远大于困难样本。但同时也看出，一个已经收敛的模型中还是有相当数量的非常困难的样本，我们把这些非常困难的样本当作异常值（outliers），论文指出如果一个好的模型去学习这些异常样本会导致模型准确度降低。我的理解是，这些异常值就像数据的噪声一样，比如一个长得非常像狗的蛋糕，模型学习这些异常值反而会导致模型误入歧途。
[
](https://img-blog.csdnimg.cn/20190318210857248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)基于以上现象与分析，研究者提出了梯度均衡机制，即根据样本梯度模长分布的比例，进行一个相应的标准化（normalization），使得各种类型的样本对模型参数更新有更均衡的贡献，进而让模型训练更加高效可靠。
由于梯度均衡本质上是对不同样本产生的梯度进行一个加权，进而改变它们的贡献量，而这个权重加在损失函数上也可以达到同样的效果，此研究中，梯度均衡机制便是通过重构损失函数来实现的。
为了清楚地描述新的损失函数，我们需要先定义梯度密度（gradient density）这一概念。仿照物理上对于密度的定义（单位体积内的质量），我们把梯度密度定义为单位取值区域内分布的样本数量。
首先定义梯度密度函数（Gradient density function）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318210924560.png)
其中
$$
g^{k}
$$
表示第k个样本的梯度，而且
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211028967.png)
所以梯度密度函数GD(g)就表示梯度落在区域:
![](https://img-blog.csdnimg.cn/20190318211352905.png)[的样本数量。再定义梯度密度协调参数（gradient density harmonizing parameter） β。](https://img-blog.csdnimg.cn/20190318211352905.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211435716.png)
这里乘样本数量 N，是为了保证均匀分布或只划分一个单位区域时，该权值为 1，即 loss 不变。
可以看出，梯度密度大的样本的权重会被降低，密度小的样本的权重会增加。于是把GHM的思想应用于分别应用于分类和回归上就形成了GHM-C和GHM-R。
3.GHM-C
把GHM应用于分类的loss上即为GHM-C，定义如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211455508.png)
根据GHM-C的loss计算方式，候选样本中的简单负样本和非常困难的异常样本的权重都会被降低，即loss会被降低，对于模型训练的影响也会被大大减小。正常困难样本的权重得到提升，这样模型就会更加专注于那些更为有效的正常困难样本，以提升模型的性能。GHM-C loss对模型梯度的修正效果如下图所示，横轴表示原始的梯度loss，纵轴表示修正后的。由于样本的极度不均衡，这篇论文中所有的图纵坐标都是取对数画的图。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211641194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
结合梯度密度的分布图，和上图单个样本的的梯度修正，我们可以得出整体样本对于模型训练梯度的贡献，如下图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211657134.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
4.GHM-R
GHM的思想同样适用于anchor的坐标回归。坐标回归loss常用smooth_l1,如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318211955390.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212002294.png)
其中
$$
t_{i}=(t_{x},t_{y},t_{w},t_{h})
$$
表示模型预测坐标偏移量,
$$
t_{i}=(t_{x}^{*},t_{y}^{*},t_{w}^{*},t_{h}^{*})
$$
表示anchor实际坐标偏移量，
$$
\delta
$$
表示
$$
SL_{1}
$$
的函数分界点，常取1/9。定义
$$
d=t_{i}-t_{i}^{*}
$$
，则
$$
SL_{1}
$$
的梯度求导为
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212549338.png)
其中sgn表示符号函数。可以看出对于
$$
|d|&gt;=\delta
$$
的所有样本梯度绝对值都为1，这使我们无法通过梯度来区分样本，同时d理论上可以到无穷大，这也使我们不能使用上面提到的RU。所以论文对
$$
SL_{1}
$$
进行变形，计算方法及梯度求导如下所示，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212733334.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212741465.png)
$$
ASL_{1}
$$
与
$$
SL_{1}
$$
的性质很相似，当d较大时都近似为L1 loss，d较小是都近似为L2 loss，而且  的范围在[0,1)，适合采用RU方法，在实际使用中，采用μ=0.02。
[
](https://img-blog.csdnimg.cn/20190318212741465.png)定义梯度的绝对值gr为:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212846450.png)
于是我们可以基于gr统计样本坐标回归偏移量的梯度分布情况如下图所示。由于坐标回归都是正样本，所以简单样本的数量相对并不是很多。而且不同于简单负样本的分类对模型起反作用，简单正样本的回归梯度对模型十分重要。但是同样也可以看出来，存在相当数量的异常样本的回归梯度值很大。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212906458.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
所以使用GHM的思想来修正loss函数，可以得到
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318212920466.png)
GHM-R loss对于回归梯度的修正如下图所示。可以看出，GHM-R loss加大了简单样本和正常困难样本的权重，大大降低了异常样本的权重，使模型的训练更加合理。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031821294110.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
摘录：
[https://zhuanlan.zhihu.com/p/55017036](https://zhuanlan.zhihu.com/p/55017036)
[https://zhuanlan.zhihu.com/p/50217821](https://zhuanlan.zhihu.com/p/50217821)
---
注：博众家之所长，集群英之荟萃。

