
# IQA+不懂︱图像清洗：图像质量评估（评估指标、传统检测方法） - 素质云笔记-Recorder... - CSDN博客

2017年03月10日 12:19:30[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8341



> 深度学习技术如火如荼，但是训练的图像集都是标注好、质量高的，那么笔者对如何进行图像清洗表示好奇。难道只有让工人肉眼看吗？一些传统的IQA都是基于图像本身质量去评价，那么我想知道，之外的图像信息质量该如何评估？
.
# 一、IQA评估指标
现在还不知道除了肉眼之外的好办法，只能先来列举一下传统IQA的一些评估指标（主要参考论文：《无参考图像质量评价综述》）：
.
## 1、MOS、DMOS
图像质量评价可以分为主观评价方法和客观评价方法,
主观评价由观察者对图像质量进行主观评分, 一般采用平均主观得分(Mean opin-ion score, MOS) 或平均主观得分差异(Di®erential mean opinion score, DMOS) (即人眼对无失真图像和有失真图像评价得分的差异)
.
## 2、均方根误差(Root mean squared error, RMSE)
均方根误差比较算法评价值与人眼主观打分之间的绝对误差, 衡量算法预测的准确性
.
## 3、线性相关系数(Linear correlation coe±- cient, LCC), 也称为皮尔逊(Pearson) 线性相关 系数
线性相关系数描述算法评价值与人眼主观打分之间的相关性, 也衡量了算法预测的准确性.
.
## 4、Spearman 秩相关系数(Spearman0s rank ordered correlation coe±cient, SROCC)
Spearman 秩线性相关系数衡量算法预测的单调性(Monotonicity).
.
## 5、Kendall 秩相关系数(Kendall rank order correlation coe±cient, KROCC)
Kendall 秩线性相关系数也衡量了算法预测的单调性.
.
## 6、离出率(Outlier ratio, OR)
离出率表示超出主观得分§2 倍标准差(存在多个观察者的主观得分时) 的样本数百分比
.
---二、图像质量检测方式

---本章只是简单来说说传统的，现在并不知道如何对图像内容质量进行检测的无监督办法。

---.

---1、全、半参考方法

---图像的某些特征与原始图像的相同特征进行比较, 比如小波变换系数的概率分布、综合多尺度几何分析、对比度敏感函数和可觉察灰度差异特征 等. 其相应的应用领域包括视频传输中的数字水印验证、利用副通道进行视频质量监控与码流率控制等.

---.

---2、盲图像质量(Blind image quality, BIQ)

---评价方法, 则完全无需参考图像, 根据失真图像的自身特征来估计图像的质量. 有些方法是面向特定失真类型的, 如针对模糊、噪声、块状效应的严重程度进行评价; 有些方法先进行失真原因分类, 再进行定量评价; 而有些方法则试图同时评价不同失真类型的图像. 无参考方法最具实用价值, 有着非常广泛的应用范围.

---.

---3、机器学习的图像质量评价

---（1）SVM ＋ SVR

---算法则采用两步方案, 先用SVM 进行失真类型识别, 进而对特定失真类型建立SVR 回归分析模型,我们称之为SVM ＋ SVR 模型.

---（2）GGD

---Moorthy 和Bovik的盲图像质量指数(Blind image quality index, BIQI) 分两步对图像进行评价, 先采用小波分解系数经广义高斯分布(Generalized Gaussian distribution, GGD) 模型拟合得到的参数作为特征, 由SVM 分类得到当前图像属于每个类的概率, 再采用SVR 对各个退化类型计算图像质量指标值, 最后根据概率加权得到总的质量评价指标; 在后续的基于失真辨识的图像真

---实性和完整性评价。

---.

---4、基于概率模型的方法

---这类方法首先建立图像特征与图像质量之间的统计概率模型,

---大多采用多变量高斯分布描述概率分布.

---对待评价图像, 提取特征后根据概率模型计算最大后验概率的图像质量, 或根据与概率模型的匹配程度(如特征间的距离) 估计图像质量.

---在德克萨斯大学奥斯汀分校的Mittal 等 提出的

---自然图像质量评价(Natural image quality evaluator, NIQE)

---算法中, 无需利用人眼评分的失真图像进行训练, 在计算其局部MSCN 归一化图像后, 根据局部活性选择部分图像块作为训练数据, 以广义高斯模型拟合得到模型参数作为特征, 采用多变量高斯模型描述这些特征, 评价过程中利用待评价图像特征模型参数与预先建立的模型参数之间的距离来确定图像质量

---Abdalmajeed 和Jiao在对图像进行局部MSCN 归一化后,

---基于韦伯分布提取自然图像统计特征, 并以多变量高斯分布描述它的概率分布

---, 评时计算待评价图像特征与无失真图像统计模型的距离作为图像质量评价度量. 根据概率建模是一种基于大量样本的统计方法, 概率数学模型的选择和样本量的大小是影响性能的关键, 现有方法大都基于多变量高斯模型进行概率建模, 主要是为了方便建模. 考虑到表征图像质量的特征维度很高, 复杂的模型将需要更多的数据量, 这类方法只有当数据量较大时才可能取得较好的效果。

---.

---5、神经网络的方法

---这类方法先提取一定的图像变换域或空间特征, 再基于已知质量数据训练一个神经网络回归分析模型, 由图像特征预测图像质量.

---Kang 等采用

---卷积神经网络(Convolutionalneural networks, CNN)

---将特征提取和回归分析融入同一个网络中, 网络包括5 层, 图像经局部MSCN归一化后以32 £ 32 子块输入网络, 第一层卷积层由50 个滤波器提取特征, 第二层进行最大最小选择, 后面两层为800 节点的全连接网络, 最后一层为单个节点输出图像质量。

---Hou 等也采用具有5 层网络结构的深度学习算法进行图像质量评价,综合特征提取、分类、后验概率计算等功能为一体,由3 级小波变换细节特征为输入,

---训练过程先采用受限波尔兹曼机(Restricted Boltzmann machine,RBM) 进行层间学习

---, 再采用反向传递算法进行精细调整. 这两种算法的实验结果均明显优于其他无参考算法, 甚至在某些情况下优于全参考算法中较好的VIF


