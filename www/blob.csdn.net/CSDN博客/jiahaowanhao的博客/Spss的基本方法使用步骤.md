
# Spss的基本方法使用步骤 - jiahaowanhao的博客 - CSDN博客


2018年03月05日 21:58:03[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1381


[Spss的基本方法使用步骤](http://cda.pinggu.org/view/24907.html)
由于一次的调研工作，我们的数据分析采用spss的统计分析工具，然后我是一个新人，全都是一步一步从零开始操作的。在学习的过程中简单记录了一点笔记，既然写了，就觉得应该把它保存下来，所以来到了这里，为我的第一次spss操作做个马克。
因子分析方法：指标非常多，反映相同事情的进行聚合
设置的地方：
描述—— kmo
抽取 —— 主成分，碎石图
旋转——最大方差法
得分——保存为变量
选项——大小为变量、删除最小系数，特征值为0.6
kmo > 0.6 ——看是否有效，对原始数据的检验。
在SPSS软件统计结果中，不管是回归分析还是其它分析，都会看到“SIG”，SIG=significance，意为“显著性”，后面的值就是统计出的P值，如果P值0.01<P<0.05,则为差异显著，如果P<0.01,则差异极显著。
公因子方差——提取程度（损失的数据，如果损失低于40%即满意）
解释总方差：可以分成几类，然后提取主成分因子，累积方差贡献率，累积特征值大于等于85%（放宽70%）.(损失率低于15%)
碎石图：类似于解释总方差，特征值大于1的就是主成分，对解释方差的解释和完善
成分矩阵——一般不考虑，不够充分，只是中间步骤
旋转后成分矩阵——成分1，成分2中大于0.6的归为一类，载荷大于设置的值才会把得分显示在视图。

---
可靠性分析（问卷问题分类正确的前提下）
步骤：
分析→度量→可靠性分析→统计量→描述性（如果项已删除则进行度量）→继续（模型α）→确定
分析：可靠性统计量：0.7以上有效
可删除的分析：如果删除后信度变大，则可以考虑把这个因素删除

---

平均数：反应数量的中点
中位数：全体样本的中点
步骤：
均值：描述性统计分析→描述→导入变量→确定
中位数：比较均值→均值→导入变量→选项→导入中位数即可→确定

---
线性回归
步骤：
分析→回归→线性→因变量→自变量→
统计量：估计→模型拟合度→共线性诊断→DW
绘制：Y：ZRESID, X:ZPRED; 直方图，正态概率图
保存：不操作
选项： 默认
→确定
模型汇总表
DW统计量代表自相关
DW = 2不存在为伪回归
DW < 2 正自相关
DW > 2 负相关

---
多选题可以考虑使用多重响应
多重响应，多重响应数据本质上属于分类数据，但由于各选项均是对同一个问题的回答，之间存在一定的相关，将各选项单独进行分析并不恰当。因此对多选题最常见的分析方法是使用SPSS中的“多重响应”命令，通过定义变量集的方式，对选项进行简单的频数分析和交叉分析
作用1：进行简单的频数分析：可以直观明了的比较一道多选题的各个选项被选比例。
作用2：进行交叉分析：可以通过设置分层变量来进行某个选项控制下的分析。
> 步骤：

> 分析→多重响应→定义变量集（把多选题变成一个变量）→设置定义把多选题的选项放进集合中的变量→将变量编码设置为二分法，计数值为1→名称标签→添加 、

> 交叉表

> 行、列→定义范围→确定


