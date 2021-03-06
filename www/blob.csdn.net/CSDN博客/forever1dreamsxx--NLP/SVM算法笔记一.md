
# SVM算法笔记一 - forever1dreamsxx--NLP - CSDN博客


2012年12月04日 14:33:57[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：481个人分类：[算法																](https://blog.csdn.net/forever1dreamsxx/article/category/1226549)[SVM																](https://blog.csdn.net/forever1dreamsxx/article/category/1295874)[
							](https://blog.csdn.net/forever1dreamsxx/article/category/1226549)


转载地址：[http://blog.csdn.net/caohao2008/article/details/1654537](http://blog.csdn.net/caohao2008/article/details/1654537)
SVM里面涉及的数学要求太高了.我一时半会也没看懂.看了看一些概念性的东西.做的笔记贴上来.
SVM算法学习笔记：
SVM算法是用于机器学习和机器训练的一个有效算法。
Support Vector Machine
第一章:学习方法
1.1 监督学习
学到的概念有:
监督学习:当样例是由输入/输出对给出时,成为监督学习.
有关输入输出关系的样例称为训练数据.
输入/输出对通常反映了把输入映射到输出的一种函数关系.
当输入到输出存在内在函数时,该函数称为目标函数.
由学习算法输出的对目标函数的估计成为学习的解.
对于分类问题,该函数有时称为决策函数.
在学习区分赛车的例子中,输出为简单的是/否,它就可看作是二元输出值.对于识别蛋白质类型的问题,输出值为有限数量的类别之一;对于化学反应的问题输出值为实数值的表示的反应化合物的浓度.
有二元输出的问题成为二类问题.有多个类别的问题成为多类问题.而实数值输出的问题称为回归问题.
非监督学习问题:数据不包含输出值,学习的任务是理解数据产生的过程.这种类型的学习包括密度估计,分布类型的学习和聚类等.
本书着重于在批量学习的设置下,根据有输出值的数据来学习输入/输出映射,即应用监督学习方法到批量训练数据上.
1.2 学习和泛化性
而当前情形下,学习的目标是输出一个假设以正确分类训练数据,早期的学习算法目标也是寻找对数据的精确拟合,这样寻找到的假设称为一致假设.
然而生成 可验证的一致性假设这一目标存在两个问题:
一个问题是待学习的目标函数可能简单的表示,因此不能很容易地加以验证,例如在DNA序列中定位基因.某些子序列是基因,某些不是.但没有一种简单的方法来区分两者.
第二个问题是,通常训练数据是有噪音的.因此不能保证存在一个目标函数能够正确地映射训练数据.很明显信用检测是其中一例,因为偿债能力可以取决于其他一些系统无法获知的因素.另一个例子是网页分类的问题,这也是一个不精确的科学问题.

还有一个更基为基本的问题在于,及时能够找到与训练数据一致的假设,他也可能无法对未见数据进行分类.一个假设正确分类训练集之外数据的能力称为泛化性,这正是要优化的属性.
如果一个假设能给出正确的输出,它就满足泛化性准则.
1.3 提高泛化性
过拟合:
许多经典的机器学习算法能够表示任意函数,并且对于困难的训练数据集会得到一个类似机械式学习器的假设.所谓机械式学习器是指能够正确分类训练数据,但对于所有未见数据会做出根本无关联性的预测.例如,决策树有可能过度增长直至针对每个训练样例有一叶子节点.为了得到一致假设变得过度复杂成为过拟合.
控制此问题的一种方法是限制假设的规模,例如对于决策树可进行修剪操作.奥卡姆(Ockham)剃刀是该类方法的准则之一,它建议如无必要,不必增加复杂性,或者说更精细的复杂性必须有利于显著提高训练数据的分类正确性.
1.5 用于学习的支持向量机
支持向量机(SVM,Support Vector Machine)是在高维特征空间使用线性函数假设空间的学习系统,它由一个来自最优化理论的学习算法训练,该算法实现了一个由统计学习理论到处的学习偏置.此学习策略由Vapnik和他的合作者提出,是一个准则性的 并且强有力的方法.在它提出来的若干年来,在范围广大的应用中,SVM的性能胜过其他大多数的学习系统.
第二章 线性学习器
对监督学习来说，学习器会输入一个带有标记（或者输出值）的样例（或输入）的训练集。
样例通常是以属性向量的形式给出，因此输入空间是R^n的子集。一旦给定输入向量，就可以为问题选择一定数目的假设函数集。其中，线性函数最容易理解并且应用最简单。
学习方法意味着一定要从数据中学习到这些参数。
在从样例中研究监督学习问题前，首先要介绍一些再本书中都要用到的符号，比如输入、输出、训练集，等等。
20世纪60年代就已经提出了几个简单的迭代算法来优化代价函数，这些代价函数使用超平面把点分为两类。下面将回顾一些最著名的算法。感知机是有趣的，这不仅是因为历史原因，还因为在如此简单的一个系统内可以找到研究SVM理论所需要的绝大多数核心概念。注意有一些算法，比如最小二乘，既可以做回归也可用来作分类。

2.1.1 Rosenblatt感知机
它是一个“在线”和“错误驱动“的程序，从一个初始权重向量w0(通常w0=0,一个全零的向量)开始，每次当一个训练点被现在的权重误分的时候都调整权重。算法见下，这个算法直接更新权重向量和偏置。
如果存在一个超平面能够正确分类训练数据，并且这个程序保证收敛，这种情况称为线性可分。如果这样的超平面不存在，则数据称为不可分。
2.2       线性回归
线性回归的问题就是求线性函数：f(x) = <w*x> +b使其能够最好地拟合一个给定标记为的训练点集Ｓ。从几何角度讲是寻找一个立定给定点的超平面。
这个问题最著名的解是最小二乘法，它能在线性目标被高斯噪音干扰的情况下获得最优结果

