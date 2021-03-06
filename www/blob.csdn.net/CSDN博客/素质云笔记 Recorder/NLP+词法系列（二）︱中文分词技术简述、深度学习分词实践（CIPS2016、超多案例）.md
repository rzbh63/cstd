
# NLP+词法系列（二）︱中文分词技术简述、深度学习分词实践（CIPS2016、超多案例） - 素质云笔记-Recorder... - CSDN博客

2017年02月18日 22:12:52[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：18686


![这里写图片描述](https://img-blog.csdn.net/20170218230317137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 摘录自：CIPS2016 中文信息处理报告《第一章 词法和句法分析研究进展、现状及趋势》P4 CIPS2016

> 中文信息处理报告下载链接：
> [http://cips-upload.bj.bcebos.com/cips2016.pdf](http://cips-upload.bj.bcebos.com/cips2016.pdf)
![这里写图片描述](https://img-blog.csdn.net/20170218215422402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 之前写过一篇中文分词总结，那么在那篇基础上，通过在CIPS2016的摘录进行一些拓展。可参考上篇：
> [NLP+词法系列（一）︱中文分词技术小结、几大分词引擎的介绍与比较](http://blog.csdn.net/sinat_26917383/article/details/52275328)

> NLP词法、句法、语义、语篇综合系列：

> [NLP+词法系列（一）︱中文分词技术小结、几大分词引擎的介绍与比较](http://blog.csdn.net/sinat_26917383/article/details/52275328)

> [NLP+词法系列（二）︱中文分词技术及词性标注研究现状（CIPS2016）](http://blog.csdn.net/sinat_26917383/article/details/55682577)

> [NLP+句法结构（三）︱中文句法结构研究现状（CIPS2016）](http://blog.csdn.net/sinat_26917383/article/details/55682996)

> [NLP+语义分析（四）︱中文语义分析研究现状（CIPS2016）](http://blog.csdn.net/sinat_26917383/article/details/55683599)

> [NLP+语篇分析（五）︱中文语篇分析研究现状（CIPS2016）](http://blog.csdn.net/sinat_26917383/article/details/55683843)

> 词法分析是将输入句子从字序列转化为词和词性序列， 句法分析将输入句子从词序列形式转化为树状结构，从而刻画句子的词法和句法结构。

> 一、词法分析的难题

> 1、词的定义和生词问题、未登录词（新词）

> 特别是在互联网时代，外来语、新词、热词不断出现，事实上，也不存在一个绝对统一的构词标准和分词规范。

> 未登录词（新词）识别错误对分词效果有着很大的影响。一般的专有名词还有一定的构词规律，如前缀后缀有迹可循。而新词则五花八门，如新术语、新缩略语、新商品名、绰号、笔名等。

> 尤其是在领域移植的情境下，当测试文本与训练数据的领域存在较大差异的时候，未登录词的数量增多，导致分词效果变差。

> 解决办法：
> 交互建模：如上所述，未登录词识别，尤其是新词识别，对分词效果的影响很大。如果一个句子中出现了一个新词时，人在理解句子时会尝试多种分词结果，甚至会综合句法结构、语义结构是否合理，从而判断出这个新词是否应该是一个词

> 2、错别字、谐音字规范化、非规范词

> 不规范文本（如网络文本和语音转录文本） 时，输入的句子中不可避免会存在一些错别字或者刻意的谐音词（如“香菇” ->“想哭”；“蓝瘦” ->“难受”；“蓝菇” ->“难过”等等）。这些错别字或谐音字对于分词系统造成了很大的困扰。

> 当不规范文本时，分析准确率急剧下降。

> 3、分词歧义消解

> [cips2016+学习笔记︱NLP中的消岐方法总结（词典、有监督、半监督）](http://blog.csdn.net/sinat_26917383/article/details/54882754)

> 4、词性定义和词性兼类问题

> 词性类别远比词的个数要小，但词性的定义也不完全存在一个统一的信息处理用的国内和国际标准。词性兼类问题是词性标注面临的主要问题，需要更高层次的上下文信息来解决。

> 5、分词粒度问题

> 受到个人的知识结构和所处环境的很大影响(黄昌宁、赵海, 2007)。这样就导致多人标注的语料存在大量不一致现象，即表达相同意思的同一字串，在语料中存在不同的切分方式，如“我”和“我/国”。

> 细粒度

> 可以缓解数据稀疏问题，解决粗粒度词语在训练数据中没有出现的问题。
> 存在两个问题：

> 1、标注人员对于分词规范的理解存在差异，因此会影响人工标注数据的质量

> 2、不同的上层应用对于分词粒度的需求不同，有些应用甚至需要不同粒度的分词结果

> 多粒度

> 数据标注时，便可以弱化分词规范，可以让标注者根据自己对词语定义的理解进行标注，模棱两可的地方允许提供多个标注结果。多粒度分词规范下，如何进行词性标注、句法分析，也是一个很有意思的问题

> .

> .

> 二、分词实践

> 1、基于词典的特征

> 分词过程中，可以把“当前字开始的三个字构成的字串是否在词典中出现”这样的信息作为特征，加入到统计模型中，这种信息称为基于词典的特征。

> 2、基于无标注数据的半指导特征

> 很多有效的基于无标注数据的半指导特征，如两个字串之间的互信息（ mutual information），一个字串左右邻接字的多样性（ accessor variety） ,一个字串左右邻接标点符号的频率，字串在篇章中出现频率，汉字的左右边界熵，两个汉字的卡方统计量等 (Weiwei Sun and Jia Xu, 2011; 韩东煦、常宝宝, 2015)。

> 研究表明，这些半指导特征可以显著提高分词准确率，尤其在领域移植的场景中处理有别于训练数据的文本时。

> 相关词-词之间的距离标注可见：

> [NLP︱句子级、词语级以及句子-词语之间相似性（相关名称：文档特征、词特征、词权重）](http://blog.csdn.net/sinat_26917383/article/details/52174672)

> [R+NLP︱text2vec包——四类文本挖掘相似性指标 RWMD、cosine、Jaccard 、Euclidean （三,相似距离）](http://blog.csdn.net/sinat_26917383/article/details/53286009)

> 3、基于自然标注数据的学习方法

> 网页源文本中包含了大量的 html 标记，指定了文字在网页中的角色、超链接、显示位置或显示格式，而这些标记无形中也隐含了分词边界信息。研究者们将这种隐含的分词边界信息称为自然标注，将包含自然标注信息的文本转化为局部标注数据，加入到模型训练数据中，显著提高了分词效果 (Wenbin Jiang et al., 2013;Yijia Liu et al., 2014)

> 4、基于深度学习的分词方法

> 近几年，深度学习方法为分词技术带来了新的思路，直接以最基本的向量化原子特征作为输入，经过多层非线性变换，输出层就可以很好的预测当前字的标记或下一个动作。在深度学习的框架下，仍然可以采用基于子序列标注的方式，或基于转移的方式，以及半马尔科夫条件随机场。深度学习主要有两点优势:

> 1) 深度学习可以通过优化最终目标，有效学习原子特征和上下文的表示；

> 2）基于深层网络如 CNN、 NN、 LSTM等，深度学习可以更有效的刻画长距离句子信息。

> 5、词法句法一体化建模

> 随着计算资源的飞速发展和对机器学习模型的理解更加深入，研究者们提出了很有效的统计模型，直接从字开始对句子进行分析，输出分词、词性、句法的结果。

> 多年前也有研究者提出词法句法一体化分析，如最有代表性的 NLPWin**，但是均采用基于规则的方法。**基于统计模型的一体化建模可以让词法句法分析互相影响，互相提高，显著提高了词法和句法的分析效果。尤其值得提出的是，研究者们提出进一步分析词语内部结构，有效缓解了数据稀疏问题。

> 6、分词开源软件开放

> 影响较大、使用人数较多的几个分词系统包括中科院计算所的 ICTLAS 分词系统、哈工大语言技术平台 LTP、清华大学自然语言处理工具包、海量云分词等。

> 常用的中文分词工具包括jieba分词、哈工大分词、Stanford分词等。

> 关于中文分词的测评，可见上一篇博客：
> [NLP+词法系列（一）︱中文分词技术小结、几大分词引擎的介绍与比较](http://blog.csdn.net/sinat_26917383/article/details/52275328)

> 最近还看到一个比较全面的项目：
> [HanLP](https://github.com/hankcs/HanLP)
![这里写图片描述](https://img-blog.csdn.net/20170219112613976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219112613976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> github：
> [https://github.com/hankcs/HanLP](https://github.com/hankcs/HanLP)

> [
](https://img-blog.csdn.net/20170219112613976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> .

> 三、词性标注

> 给定一个切好词的句子，词性标注的目的是为每一个词赋予一个类别，这个类别称为词性标记（ part-of-speech tag），比如，名词（> noun）、动词（ verb）、形容词（ adjective）等。
![这里写图片描述](https://img-blog.csdn.net/20170218220937735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 词性标注人工标注数据目前也存在一些问题，可能会影响面向不规范文本的词性标注研究：

> 数据中往往会包含存在一些标注不一致。现象，如 CTB 中“新华社”被大量标为两种词性“ NR”（专有名词）和“ NN”（普通

> 名词）

> 词性标注标签集合如何设计？这个问题取决于词性标注服务于什么任务。

> 互联网文本分析和领域自适应问题：句法分析面临的一个重要问题是面对和训练数据很接近的新闻领域规范文本时性能较好，但用于其它领域或类型的文本（比如口语化文本）时，准确率则急剧下降。

> 1、异构数据融合

> 汉语数据目前存在多个人工标注数据，然而不同数据遵守不同的标注规范，因此称为多源异构数据。近年来，学者们就如何利用多源异构数据提高模型准确率，提出了很多有效的方法，如基于指导特征的方法、基于双序列标注的方法、以及基于神经网络

> 共享表示的方法。

> 2、基于深度学习的方法

> 转化为字标注的方式，进行分析。传统词性标注方法的特征抽取过程主要是将固定上下文窗口的词进行人工组合，而深度学习方法能够自动利用非线性激活函数完成这一目标。进一步，如果结合循环神经网络如双向 LSTM，则抽取到的信息不再受到固定窗口的约束，而是考虑整个句子。除此之外，深度学习的另一个优势是初始词向量输入本身已经刻画了词语之间的相似度信息，这对词性标注非常重要。

> .

> .

> 公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 四、深度学习分词实践案例

> 1、动态规划的方法——转移概率

> 本文来源于苏剑林，泰迪大数据挖掘，原创作品名称《【OCR技术浅探】6. 语言模型》

> 转移概率：
> 从概率的角度来看，就是对于第一个字的区域的识别结果s1，我们前面的卷积神经网络给出了“电”、“宙”两个候选字(仅仅选了前两个，后面的概率太小)，每个候选字的概率W(s1)分别为0.99996、0.00004；第二个字的区域的识别结果s2，我们前面的卷积神经网络给出了“柳”、“视”、“规”(仅仅选了前三个，后面的概率太小)，每个候选字的概率W(s2)分别为0.87838、0.12148、0.00012，因此，它们事实上有六种组合：“电柳”、“电视”、“电规”、“宙柳”、“宙视”、“宙规”。

> 下面考虑它们的迁移概率。
> 所谓迁移概率，其实就是条件概率P(s1|s2)，即当s1出现时后面接s2的概率。通过10万微信文本，我们统计出，“电”字出现的次数为145001，而“电柳”、“电视“、”电规“出现的次数为0、12426、7；“宙”字出现的次数为1980次，而“宙柳”、“宙视”、“宙规”出现的次数为0、0、18，因此，可以算出
![这里写图片描述](https://img-blog.csdn.net/20170219102333087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219102333087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 结果如下图：
![这里写图片描述](https://img-blog.csdn.net/20170219102325758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219102325758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 从统计的角度来看，最优的s1,s2组合，应该使得式(14)取最大值：
![这里写图片描述](https://img-blog.csdn.net/20170219102403774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219102403774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 因此，可以算得s1,s2的最佳组合应该是“电视”而不是“电柳”。这时我们成功地通过统计的方法得到了正确结果，从而提高了正确率。

> [
](https://img-blog.csdn.net/20170219102403774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 对于第一个问题，只需要从大的语料库中统计si的出现次数\#si，以及si,si+1相接地出现的次数\#(si,si+1)，然后认为
![这里写图片描述](https://img-blog.csdn.net/20170219102515775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219102515775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 即可，本质上没有什么困难。本文的识别对象有3062个，理论上来说，应该生成一个3062×3062的矩阵，这是非常庞大的。当然，这个矩阵是非常稀疏的，我们可以只保存那些有价值的元素。

> [
](https://img-blog.csdn.net/20170219102515775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 现在要着重考虑当\#(si,si+1)=0的情况。在前一节我们就直接当P(si|si+1)=0，但事实上是不合理的。没有出现不能说明不会出现，只能说明概率很小，因此，即便是对于\#(si,si+1)=0，也应该赋予一个小概率而不是0。这在统计上称为数据的平滑问题。

> [
](https://img-blog.csdn.net/20170219102515775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 一个简单的平滑方法是在所有项的频数(包括频数为0的项)后面都加上一个正的小常数α(比如1)，然后重新统计总数并计算频率，这样每个项目都得到了一个正的概率。这种思路有可能降低高频数的项的概率，但由于这里的概率只具有相对意义，因此这个影响是不明显的(一个更合理的思路是当频数小于某个阈值T时才加上常数，其他不加。)。按照这种思路，从数十万微信文章中，我们计算得到了160万的邻接汉字的转移概率矩阵。

> .

> 2、动态规划的方法——Viterbi算法

> 本文来源于泰迪大数据挖掘，原创作品名称《【OCR技术浅探】6. 语言模型》

> 求解最优组合是属于动态规划中求最优路径的问题，其中最有效的方法是Viterbi算法。

> Viterbi算法是一个简单高效的算法，用Python实现也就十来行的代码。它的核心思想是：如果最终的最优路径经过某个si−1，那么从初始节点到si−1点的路径必然也是一个最优路径——因为每一个节点si只会影响前后两个P(si−1|si)和P(si|si+1)。

> 根据这个思想，可以通过递推的方法，在考虑每个si时只需要求出所有经过各si−1的候选点的最优路径，然后再与当前的si结合比较。这样每步只需要算不超过 次，就可以逐步找出最优路径。Viterbi算法的效率是θ，l 是候选数目最多的节点si的候选数目，它正比于n，这是非常高效率的。

> 由于Viterbi算法的简单高效，这是一个性价比很高的步骤。

> 3、无监督分词——基于切分的新词发现

> 本节内容也可以参考 苏剑林
> [《2. ](http://mp.weixin.qq.com/s?__biz=MzAwOTMzNjY3NQ==&mid=2653339311&idx=3&sn=15238cf582d06364d02d63bb2c1012b4&chksm=80b33ca3b7c4b5b58ca068aaebe188ff6fb1ad08ec49e9f1b30c57413ef7988acbfb18a0cd4b&mpshare=1&scene=1&srcid=01117q6Y50g0cXstCLv3xJFB#rd)

> 基于切分的新词发现》

> 我们说片段的凝固度大于一定程度时，片段可能成词（接下来要去考虑它的边界熵）。那这不就是说，如果片段的凝固度低于一定程度时，这个片段就不可能成词了吗？那么我们就可以在原来的语料中把它断开了。

> 我们可以做适当的简化，如果a,ba,b是语料中相邻两字，那么可以统计(a,b)(a,b)成对出现的次数\#(a,b)\#(a,b)，继而估计它的频率P(a,b)P(a,b)，然后我们分别统计a,ba,b出现的次数\#a,\#b\#a,\#b，然后估计它们的频率P(a)，P(b)P(a)，P(b)，如果

> P(a,b) / P(a)P(b) < α(α是给定的大于1的阈值)

> 那么就应该在原来的语料中把这两个字断开。
> 这个操作本质上就是——我们根据这个指标，对原始语料进行初步的分词！
> 在完成初步分词后，我们就可以统计词频了，然后根据词频来筛选。

> 我们现在只用了两个指标：
> 频数和凝固度
> ，去掉了计算量最大的边界熵，而且，在计算凝固度时，我们只需要计算二字片段的凝固度，省掉了更多字片段的凝固度计算，但是，由于我们是基于切分的方式做的，因此我们少了很多计算量，但理论上却能够得任意长度的词语！

> 下面给出实现代码，很短，纯Python，并且不用第三方库的支持，而且内存非常友好，这里的texts可以是一个列表，也可以是一个迭代器（每次返回一篇文章），配合tqdm库，可以方便地显示进度。最后，在统计时，用到了加γ平滑法，以缓解出现不合理的词。以前做这些统计计算的时候，不用想就用Pandas了，最近尝试了一下Python原生的一些库，发现也相当好用呢～

> from
> collections
> import
> defaultdict
> from
> itertools
> import
> tee
> import
> re
> def
> count_words
> (texts, min_count=
> 10
> , min_proba=
> 1
> , gamma=
> 0.5
> )
> :
> segments = [defaultdict(int), defaultdict(int)]
> for
> text
> in
> texts:
> for
> i
> in
> range(
> 2
> ):
> for
> j
> in
> range(len(text)-i):
                segments[i][text[j: j+i+
> 1
> ]] +=
> 1
> segments[
> 0
> ] = {i:j+gamma
> for
> i,j
> in
> segments[
> 0
> ].iteritems()}
    segments[
> 1
> ] = {i:j+gamma
> for
> i,j
> in
> segments[
> 1
> ].iteritems()}
    nb_words = sum(segments[
> 0
> ].values())**
> 2
> /sum(segments[
> 1
> ].values())
    strong_segments = {i: nb_words*j/(segments[
> 0
> ][i[
> 0
> ]]*segments[
> 0
> ][i[
> 1
> ]])
> for
> i,j
> in
> segments[
> 1
> ].iteritems()
> if
> j >= min_count}
    strong_segments = {i:j
> for
> i,j
> in
> strong_segments.iteritems()
> if
> j >= min_proba}
> return
> strong_segments
> def
> filter_words
> (texts)
> :
> for
> text
> in
> texts:
> for
> t
> in
> re.split(
> u'[^\u4e00-\u9fa50-9a-zA-Z]+'
> , text):
> yield
> t
> def
> find_words
> (texts, min_count=
> 10
> , min_proba=
> 1
> )
> :
> texts_for_count, texts_for_cut = tee(filter_words(texts))
    strong_segments = count_words(texts_for_count, min_count, min_proba)
    words = defaultdict(int)
> for
> text
> in
> texts_for_cut:
> if
> text:
            s = text[
> 0
> ]
> for
> i
> in
> range(len(text)-
> 1
> ):
> if
> text[i:i+
> 2
> ]
> in
> strong_segments:
                    s += text[i+
> 1
> ]
> else
> :
                    words[s] +=
> 1
> s = text[i+
> 1
> ]
> return
> {i:j
> for
> i,j
> in
> words.iteritems()
> if
> j >= min_count}
> 一般情况下，为了得到更细粒度的词语（避免分出太多无效的长词），我们可以选择较大的α，比如α=10，但是这带来一个问题：一个词语中相邻两个字的凝固度不一定很大。

> 一个典型的例子是“共和国”，“和”跟“国”都是很频繁的字，“和国”两个字的凝固度并不高（在微信文本中大概为3左右），如果α太大就会导致切错了这个词语（事实上，是“共和”跟“国”的凝固度高），这些例子还有很多，比如“林心如”的“心如”凝固度就不大（当然，如果语料来源于娱乐圈，那又另当别论）。而如果设置α=1，则需要更大的语料库才能使得词库完备起来。这是在使用本算法时需要仔细考虑的。

> 4、基于HMM的字标注

> 本节内容也可以参考 苏剑林
> [《3. 字标注法与HMM模型》](http://mp.weixin.qq.com/s?__biz=MzAwOTMzNjY3NQ==&mid=2653339344&idx=2&sn=a4faed4005537a9d995592c9116d6058&chksm=80b33cdcb7c4b5ca92834032b00b976e175cfca7f16015a7bbe9f4c34d32719abe1dbfef249c&mpshare=1&scene=1&srcid=0120VYHd6SLgm7dPRijeQIl2#rd)

> 通过字标注法来进行分词的模型有隐马尔科夫模型（HMM）、最大熵模型（ME）、条件随机场模型（CRF），它们在精度上都是递增的，据说目前公开评测中分词效果最好的是4标注的CRF。然而，在本文中，我们要讲解的是最不精确的HMM。因为在笔者看来，它并非一个特定的模型，而是解决一大类问题的通用思想，一种简化问题的学问。

> HMM模型一般都是用来解决“在查词典方法的过程中不能解决的部分”（就好比结巴分词所做的）。当然，你可以把马尔可夫假设加强——比如假设每个状态跟前面两个状态有关，那样肯定会得到更精确的模型，但是模型的参数就更难估计了。

> 文中也有的python实现

> .

> 5、★无监督分词典范——基于凝聚度和自由度的非监督词库生成

> 本节来自于张宏伦工作室，同时
> 开源了代码在自己网站
> 上，可以计算频率、凝聚度、自由度的
> [corpus.py](http://zhanghonglun.cn/blog/project/%E5%9F%BA%E4%BA%8E%E5%87%9D%E8%81%9A%E5%BA%A6%E5%92%8C%E8%87%AA%E7%94%B1%E5%BA%A6%E7%9A%84%E9%9D%9E%E7%9B%91%E7%9D%A3%E8%AF%8D%E5%BA%93%E7%94%9F%E6%88%90/)

> 主流的分词模型比较固定，而好的语料词库往往很难获得，并且大多需要人工标注。这里介绍一种基于
> 词频、凝聚度和自由度的非监督词库
> 生成方法

> 第一步：获取所有的备选词语

> 例如《西游记》的全文，使用的是utf-8版本，我关注的最大词语长度为5，因此可以使用正则匹配出全部的单个汉字、双汉字、三汉字、四汉字、五汉字，作为可能的备选词语。

> 所以我用的是python的regex模块，可以进行多汉字的重叠匹配。

> 第二步：统计词频

> 真正的词语应该至少出现一定次数，而那些局部切割出来的碎片出现次数则较少，因此可以统计一下以上全部可能备选词语的词频

> 对于《西游记》而言，一共出现了4459个汉字，而长度不超过5个汉字的全部可能备选词语共824567个。为了得到这些词语的词频，我写了一个循环，挨个在《西游记》中查找每一个词的词频。但是事实证明这样相当浪费时间，因为这个循环需要进行824567次！所以更好的方法是，同样还是使用regex匹配单汉字、双汉字、三汉字、四汉字和五汉字词语，只不过不进行set、list的去重操作，这样返回的匹配结果中便包含了全部备选词语的词频，而且一共只需执行五次正则匹配，所需时间会少很多。

> 第三步：计算聚合度

> 聚合度的概念很好理解，例如“齐天”会和“大圣”出现在一起，因为“齐天大圣”这个词的聚合度很高。

> 假设该词语为S，首先计算该词语出现的概率P(S)，然后尝试S的所有可能的二切分，即分为左半部分sl和右半部分sr并计算P(sl)和P(sr)，例如双汉字词语存在一种二切分、三汉字词语存在两种二切分。接下来计算所有二切分方案中，P(S)/(P(sl)×P(sr))的最小值，取对数之后即可作为聚合度的衡量。

> 以双汉字词语为例，可以想象到，如果该词语的聚合度很低，说明其第一个字和第二个字的相关性很弱，甚至是不相关的，那么P(S)和P(sl)×P(sr)将处于同一个数量级。相反，如果该词语的聚合度很高，“齐天”、“大圣”和“齐天大圣”三者的概率都很接近，因此P(S)/(P(sl)×P(sr))将是一个远大于1的数值。

> 有意思的是，《西游记》的824567个备选词中，对应的聚合度范围为-6至20。为什么会出现负数呢？说明P(S)比P(sl)×P(sr)更小，即sl和sr同时出现的可能性更低，因此别提聚合，可能还存在某些排斥。

> 第四步：计算自由度

> 有了聚合度的概念，我们可以自动识别出类似“齐天大圣”这样的词语。那么问题来了，“天大圣”是一个有效词语吗？“齐天大”呢？可以推测到，这两个词语的聚合度也很高，但是它们却不应该成为有效的词语。

> 一个有效的词语，应该能够被灵活地运用到各种语句中，其上下文搭配应当是丰富的，这便是自由度的概念。“天大圣”的左边绝大多数都是“齐”，“齐天大”的右边绝大多数都是“圣”，因此它们的自由度很低。可以用熵来衡量一个词语的自由度。假设一个词语一共出现了N次，其左边共出现过n个汉字，每个汉字依次出现N1，N2，……，Nn次，则满足N = N1 + N2 + …… + Nn，因此可以计算该词语左边各个汉字出现的概率，并根据熵公式计算左邻熵。熵越小则自由度越低，例如“天大圣”的左邻熵接近于0，因为“齐”字的概率几乎为1；熵越大则自由度越高，表示用词搭配越混乱、越自由、越多样。因为“天大圣”的左邻熵很小，而右邻熵则相对较大，因此我们将一个词语左邻熵和右邻熵中较小者作为最终的自由度。

> .

> 6、
> [反作弊基于左右信息熵和互信息的新词挖掘](https://zhuanlan.zhihu.com/p/25499358?winzoom=1)

> 标准一：通常我们认为两个片段可以成词的一个条件就是这个词语会在很多的语境中被提到。熵就是一个用来衡量这个维度的指标。

> 标准二：内部聚合程度 - 互信息 (mutual information) & 点间互信息 (pointwise mutual information)

> .

> 7、semi-CRF算法与神经网络的中文分词方法

> 作者： 哈工大SCIR博士生 刘一佳，文章为《赛尔原创 | 基于词的神经网络中文分词方法》 在ICLR2016上，Kong等人发表了一篇名为Segmental Recurrent Neural Networks (SRNN)的工作[Kong et al.2015]。这篇工作的核心思想是将semi-CRF算法与神经网络进行结合。

> CRF基于马尔科夫过程建模，算法在随机过程的每步对输入序列的一个元素进行标注。
> 而semi-CRF则是基于半-马尔科夫过程建模，算法在每步给序列中的连续元素标注成相同的标签。semi-CRF算法的这一性质使得它可以直接应用于中文分词任务
> 。标注连续元素的行为可以看做从字序列中识别出词来。形式化地讲，semi-CRF建模的是整句分割的概率。其函数形式如下：
![这里写图片描述](https://img-blog.csdn.net/20170219105513544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219105513544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 其中，x对应输入字序列，s对应分词序列。G(x,s)将x和s表示为特征向量。G是一种泛化的函数，它的输出可以是稀疏的0-1向量，也可以是稠密的词表示。

> 他们使用一个双向RNN作为G函数。这个双向RNN将成词的若干个字的向量组合为这个词的向量。他们的模型如图1所示。由于这种网络具有将字向量组合成词向量的功能，本文称其为组合网络。
![这里写图片描述](https://img-blog.csdn.net/20170219105619243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219105619243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 在IJCAI2016发表的工作 Exploring Segment Representations for Neural Segmentation Models 中尝试了两种双向RNN的替代网络：CNN以及直接拼接。
> 实验结果如表1所示。结果显示，使用CNN (SCNN)或拼接 (SCONCATE)都无法取得与基线结果类似的性能，准确率甚至低于SRNN。

> [
](https://img-blog.csdn.net/20170219105619243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们首先使用一个不依赖词向量的模型(SRNN或SCONCATE)对大规模文本进行切分，然后从自动切分结果中使用word2vec学习词向量。这种方法解决了词向量的获取问题。
![这里写图片描述](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 引入词向量能够带来分词准确率的显著提升。

> [
](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> [

](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 8、中文分词：字嵌入+Bi-LSTM+CRF

> [
](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 本文来源于公众号待字闺中，原创作品名称《97.5%准确率的深度学习中文分词（字嵌入+Bi-LSTM+CRF）》

> [
](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 基于传统机器学习的方法 ，以CRF为主，也有用svm，nn的实现，这类都是基于模型的，跟本文一样，都有个缺陷，不方便增加用户词典（但可以结合，比如解码的时候force-decode）。 速度上会有损耗。 另外都需要提取特征。传统CRF一般是定义特征模板，方便性上有所提高。另外传统CRF训练算法(LBFGS)较慢，也有使用sgd的，但多线程都支持的不好。代表有crf++, crfsuite, crfsgd, wapiti等。

> [
](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 本文实现的第一步也是对语料进行处理，使用word2vec对语料的字进行嵌入，每个字特征为50维。

> 得到字嵌入后，用字嵌入特征喂给双向LSTM， 对输出的隐层加一个线性层，然后加一个CRF就得到本文实现的模型。

> [
](https://img-blog.csdn.net/20170219105829627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170219101740726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们首先使用word2vec对字进行嵌入，具体就是把每一句按字符切割，空格隔开，喂给word2vec,指定维度50

> 然后我们把每一句处理成 :

> 字索引1 字索引2 … 字索引N 标注1 标注2 … 标注N

> 对于标注，我们按字分词的典型套路：

> 对于单独字符，不跟前后构成词的，我们标注为S (0)

> 跟后面字符构成词且自身是第一个字符的，我们标注为B (1)

> 在成词的中间的字符，标注为M (2)

> 在词尾的字符，标注为E (3)

> 这样处理后使用前面描述模型训练。

> .

> 9、基于双向LSTM的seq2seq字标注

> 本节来自于苏剑林，《【中文分词系列】 4.
> [基于双向LSTM的seq2seq字标注](http://spaces.ac.cn/archives/3924/)
> 》

> 关于标注，还有一个值得讨论的内容，就是标注的数目。常用的是4tag，事实上还有6tag和2tag，而标记分词结果最简单的方法应该是2tag。

> 用4tag标注，我们能总结出哪些字单字成词、哪些字经常用作开头、哪些字用作末尾，但仅仅用2tag，就只能总结出哪些字经常用作开头，从归纳的角度来看，是不够全面的。

> RNN的意思是，为了预测最后的结果，我先用第一个词预测，当然，只用第一个预测的预测结果肯定不精确，我把这个结果作为特征，跟第二词一起，来预测结果；接着，我用这个新的预测结果结合第三词，来作新的预测；然后重复这个过程；直到最后一个词。这样，如果输入有n个词，那么我们事实上对结果作了n次预测，给出了n个预测序列。整个过程中，模型共享一组参数。因此，RNN降低了模型的参数数目，防止了过拟合，同时，它生来就是为处理序列问题而设计的，因此，特别适合处理序列问题。

> LSTM对RNN做了改进，使得能够捕捉更长距离的信息。但是不管是LSTM还是RNN，都有一个问题，它是从左往右推进的，因此后面的词会比前面的词更重要，但是对于分词这个任务来说是不妥的，因为句子各个字应该是平权的。因此出现了双向LSTM，它从左到右做一次LSTM，然后从右到左做一次LSTM，然后把两次结果组合起来。

> LSTM可以根据输入序列输出一个序列，这个序列考虑了上下文的联系，因此，可以给每个输出序列接一个softmax分类器，来预测每个标签的概率。基于这个序列到序列的思路，我们就可以直接预测句子的标签。

> Keras实现代码可见其博客。同类文章有：

> [深度学习将会变革NLP中的中文分词，陈圳,keras实现](http://www.leiphone.com/news/201608/IWvc75oJglAIsDvJ.html)

> [基于深度学习的中文分词尝试，gensim库和keras库](https://github.com/xccds/chinese_wordseg_keras)

> [Deep Learning 在中文分词和词性标注任务中的应用](http://blog.csdn.net/itplus/article/details/13616045)

> 简单解释seq2seq：

> 从一个Sequence做某些工作映射到（to）另外一个Sequence的任务 。
> 具体结合实际应用来说，如下的连个任务都可以看做是Seq2Seq的任务：

> 1、SMT翻译任务（源语言的语句 -> 目标语言的语句）

> 2、对话任务（上下文语句->应答语句）

> RNN Encoder-Decoder 框架：

> Encoder，是将输入的Sequence编码成为一个固定长度的向量

> Decoder，是根据背景Vector（即Encoder最后输出的向量）生成一个Token序列，作为输出的Sequence

> 这两部分Sequence的长度可以不一致。

![这里写图片描述](https://img-blog.csdn.net/20170219113814794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170219113814794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 最经典的Encoder-Decoder框架，用的就是RNN，即Encoder是一个RNN，而Decoder也是一个RNN。

> [
](https://img-blog.csdn.net/20170219113814794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 以上参考：
> [深度学习（BOT方向） 学习笔记（1） Sequence2Sequence 学习](http://blog.csdn.net/MebiuW/article/details/52832847)

> .

> 10、基于全卷积网络的中文分词

> 苏剑林，
> [基于全卷积网络的中文分词](http://spaces.ac.cn/archives/4195/)

> 放到语言任务中看，（一维）卷积其实就是ngram模型，从这个角度来看其实CNN远比RNN来得自然，RNN好像就是为序列任务精心设计的，而CNN则是传统ngram模型的一个延伸。另外不管CNN和RNN都有权值共享，看上去只是为了降低运算量的一个折中选择，但事实上里边大有道理。CNN中的权值共享是平移不变性的必然结果，而不是仅仅是降低运算量的一个选择，试想一下，将一幅图像平移一点点，或者在一个句子前插入一个无意义的空格（导致后面所有字都向后平移了一位），这样应该给出一个相似甚至相同的结果，而这要求卷积必然是权值共享的，即权值不能跟位置有关系。

> RNN类模型，尤其是LSTM，一直语言任务的霸主，
> 但最近引入门机制的卷积GCNN据说在语言模型上已经超过了LSTM（一点点），这说明哪怕在语言任务中CNN还是很有潜力的。

> LSTM的优势就是能够捕捉长距离的信息，但事实上语言任务中真正长距离的任务不多
> ，哪怕是语言模型，事实上后一个字的概率只取决于前面几个字罢了，不用取决于前面的全文，而CNN只要层数多一点，卷积核大一点，其实也能达到这个效果了。但CNN还有一个特别的优势：CNN比RNN快多了。用显卡加速的话，显卡最擅长的就是作卷积了，因为显卡本身就是用来处理图像的，GPU对CNN的加速要比对RNN的加速明显多了…

> 解码阶段加入硬解码（人工干预解码）用viterbi算法得到最优路径
> ，但是在viterbi之前，可以利用词表对各个标签的概率进行调整。这里的做法是：添加一个add_dict.txt文件，每一行是一个词，包括词语和倍数，这个倍数就是要将相应的标签概率扩大的倍数，比如词表中指定词语“科学空间,10”，而对“科学空间挺好”进行分词时，先用模型得到这六个字的标签概率，然后查找发现“科学空间”这个词在这个句子里边，所以将第一个字为s的概率乘以10，将第二、三个字为m的概率乘以10，将第4个字为e的概率乘以10（不用归一化，因为只看相对值就行了），同样地，如果某些地方切漏了（该切的没有切），也可以加入到词表中，然后设置小于1的倍数就行了。

> .

> 11、词典+LSTM的深度学习的结合（keras）

> 参考博客：
> [【中文分词系列】 7. 深度学习分词？只需一个词典！](http://spaces.ac.cn/archives/4245/)

> 仅用一个词典，就完成了一个深度学习分词器的训练，居然效果还不错！这种方案可以称得上是半监督的，甚至是无监督的。

> 做法很简单，既然深度学习需要语料，那我就自己生成一批语料。怎么生成？我把词典中的词随机组合就行了。不对不对，随机组合生成的不是自然语言呀？我开始也怀疑这一点，但实验之后发现，这样做出来的效果特别好，甚至有胜于标注语料的结果的现象。

> 事不宜迟，我们来动手。首先得准备一个带词频的词表，词频是必须的，如果没有词频，则效果会大打折扣。然后写一个函数，以正比于词频的概率，随机挑选词表中的词语，组合成“句子”。

> 首先得准备一个带词频的词表，词频是必须的，如果没有词频，则效果会大打折扣。然后写一个函数，以正

> 比于词频的概率，随机挑选词表中的词语，组合成“句子”。

> .

> 12、【中文分词系列】 8. 更好的新词发现算法

> 苏剑林：
> [http://spaces.ac.cn/archives/4256/](http://spaces.ac.cn/archives/4256/)

> 注意到，当特征之间明显不独立的时候，可以考虑将特征组合之后，使得特征之间的相关性减弱，再用朴素贝叶斯。比如，对于文本，如果以字为特征，则朴素假设显然不成立，如“我喜欢数学”中的“喜”和“欢”、“数”和“学”都明显相关，这时候我们可以考虑将特征进行组合，得到“我/喜欢/数学”，这样三个片段之间的相关性就没有那么强了，因此可以考虑用上述结果。可以发现，这个过程很像分词，或者反过来说，分词的主要目的之一，就是将句子分为若干个相关性比较弱的部分，便于进一步处理。从这个角度来看，分的可能不一定是“词”，也可能是短语、常用搭配等。

> 说白了，分词就是为了削弱相关性，降低对词序的依赖，这一点，哪怕在深度学习模型中，都是相当重要的。有些模型，不分词但是用CNN，也就是把若干个字组合作为特征来看，这也是通过字的组合来减弱特征间的相关性的体现。

> 既然分词是为了削弱相关性，那么我们分词，就是在相关性弱的地方切断了。文章《【中文分词系列】 2. 基于切分的新词发现》其实就是这个意思，只是那里认为，文本的相关性仅由相邻两字（2grams）来决定，这在很多时候都是不合理的，比如“林心如”中的“心如”、“共和国”中的“和国”，凝固度（相关性）都不是很强，容易错切。因此，本文就是在前文的基础上改进，那里只考虑了相邻字的凝固度，这里同时考虑多字的内部的凝固度（ngrams），比如，定义三字的字符串内部凝固度为：
![这里写图片描述](https://img-blog.csdn.net/20170319162153787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 这个定义其实也就是说，要枚举所有可能的切法，因为一个词应该是处处都很“结实”的，4字或以上的字符串凝固度类似定义。一般地，我们只需要考虑到4字（4grams）就好（但是注意，我们依旧是可以切出4字以上的词来的）。

> 考虑了多字后，我们可以设置比较高的凝固度阈值，同时防止诸如“共和国”之类的词不会被切错，因为考虑三字凝固度，“共和国”就显得相当结实了，所以，这一步就是“宁放过，勿切错”的原则。

> 但是，“各项”和“项目”这两个词，它们的内部凝固度都很大，因为前面一步是“宁放过，勿切错”，因此这样会导致“各项目”也成词，类似的例子还有“支撑着”、“球队员”、“珠海港”等很多例子。但这些案例在3grams中来看，凝固度是很低的，所以，我们要有一个“回溯”的过程，在前述步骤得到词表后，再过滤一遍词表，过滤的规则就是，如果里边的n字词，不在原来的高凝固度的ngrams中，那么就得“出局”。

> 所以，考虑ngrams的好处就是，可以较大的互信息阈值情况下，不错切词，同时又排除模凌两可的词。就比如“共和国”，三字互信息很强，两字就很弱了（主要还是因为“和国”不够结实），但是又能保证像“的情况”这种不会被切出来，因为阈值大一点，“的情”和“的情况”都不结实了。

> 算法可见BLOG：
> [http://spaces.ac.cn/archives/4256/](http://spaces.ac.cn/archives/4256/)


