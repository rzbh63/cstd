
# 再谈最小熵原理：“飞象过河”之句模版和语言结构 | 附开源NLP库 - Paper weekly - CSDN博客


2018年05月31日 12:16:25[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：710



作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

在前一文[从无监督构建词库看「最小熵原理」，套路是如何炼成的](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)中，我们以最小熵原理为出发点进行了一系列的数学推导，最终得到 (2.15) 和 (2.17) 式，它告诉我们**两个互信息比较大的元素我们应该将它们合并起来，这有利于降低“学习难度”**。于是利用这一原理，我们通过邻字互信息来实现了词库的无监督生成。

由字到词、由词到词组，考察的是相邻的元素能不能合并成一个好“套路”。可是套路为什么非得要相邻的呢？

当然不一定相邻，我们学习语言的时候，不仅仅会学习到词语、词组，还要学习到“固定搭配”，也就是说词语怎么运用才是合理的，这是语法的体现，是本文所要探究的，希望最终能达到一定的无监督句法分析的效果。

由于这次我们考虑的是跨邻词的语言关联，因此我给它起个名字为“飞象过河”，正是：**“套路宝典”第二式——“飞象过河”**。

# 语言结构

对于大多数人来说，并不会真正知道什么是语法，他们脑海里就只有一些**“固定搭配”、“定式”**，或者更正式一点可以叫“模版”。大多数情况下，我们是根据模版来说出合理的话来。而不同的人的说话模版可能有所不同，这就是个人的说话风格，甚至是“口头禅”。

**句模版**

比如，**“X 的 Y 是什么”**就是一个简单的模版，它有一些明确的词语“的”、“是”、“什么”，还有一些占位符 X、Y，随便将 X 和 Y 用两个名词代进去，得到的就是合乎语法的句子（合不合事实，那是另外一回事了）。这类模版还可以举出很多，**“X和Y”、“X的Y”、“X可以Y吗”、“X有哪些Y”、“X是Y还是Z”**等等。

**▲**句模版及其相互嵌套示例

当然，虽然可以抽取尽可能多的模版，但有限的模版是无法覆盖千变万化的语言想象的，所以更重要的是基于模版的嵌套使用。比如“X 的 Y 是什么”这个模版，X 可以用模版“A 和 B”来代替，从而得到“（A 和 B）的 Y 是什么”。如此以来，模版相互嵌套，就可以得到相当多句子了。

**等价类**

接着，有了模版“X 的 Y 是什么？”之后，我们怎么知道 X 和 Y 分别可以填些什么呢？

刚才我们说“随便用两个名词”代进去，可是按照我们的思路，到现在为止我们也就只会构建词库，我们连什么是“名词”都不知道，更不知道应该把名词填进去。事实上，我们不需要预先知道什么，我们可以通过大料的语料来抽取每个候选位置的“等价类”，其中 X 的候选词组成一个词语等价类，Y 的候选词也组成一个词语等价类，等等。

**▲**句模版及等价类的概念

当然，**这样的设想是比较理想的，事实上目前我们能获取的生语料情况糟糕得多**，但不管怎样，万丈高楼平地起，我们先解决理想情况，实际使用时再去考虑一般情况。

下面我们来逐一探究如何从大量的原始语料获取句模版，并考虑如何识别句子中所用到的句模版，甚至挖掘出句子的层次结构。
# 生成模版

事实上，有了前一文的构建词库的经验，事实上就不难构思生成句子模版的算法了。

在构建词库那里，我们的统计对象是字，现在我们的统计对象是词，此外，词语是由相邻的字组成的，但句子模版却未必是由相邻的词组成的（否则就退化为词或词组），所以我们还要考虑跨词共现，也就是 Word2Vec 中的**Skip Gram 模型**。

**有向无环图**

有向无环图（Directed Acyclic Graph，DAG）其实是 NLP 中经常会遇到的一个图论模型。事实上，一元分词模型也可以直接抽象为有向无环图上的最短路径规划问题。而这里的候选模版集构建，也需要用到有向无环图。

因为考虑了 Skip Gram 模型，因此我们可以把句子内比较“紧凑”（互信息比较大）的“词对”连接起来，用图论的角度看，这就构成了一个“有向无环图”：


我们直接**将图上的所有路径都取出来**，如果跨过了相邻节点，那么就插入一个占位符（下面全部用 X 表示占位符），这样就可以得到候选模版集了。比如从上图中，抽取到的候选模版为：


**算法步骤**

**我们可以把上述流程具体描述如下：**

1. 将语料按句子切分，并分词；

2. 选定一个窗口大小 window，从语料中统计每个词的频率 (pa,pb)，以及在窗口大小内中任意两词的共现频率 (pab)；

3. 分别设定出现频率的阈值 min_prob 和互信息的阈值 min_pmi；

4. 遍历所有句子：

4.1. 对每个句子构建一个图，句子中的词当作图上的点；

4.2. 句子中窗口内的词对 (a,b)，如果满足 pab>min_prob和>mi_pmi，那么就给图上添加一条“a-->b”的有向边；

4.3. 找出图上所有的路径（孤立点也算路径），作为候选模版加入统计；

5. 统计各个“准模版”的频率，将“准模版”按频率降序排列，取前面部分即可。

**这个算法既可以用来句模版的抽取，也可以简单地用来做词组（短语）的抽取，只需要将 window 设为 1**。因此它也就基本上包含了前一文所说的词库构建了，所以**上述算法是一个一般化的抽取框架**。

**效果演示**

下面是从百度知道的问题集中抽取出来的一些句模版（数字是统计出来的频数，可以忽略）：


注意，事实上**“X 的 X”、“X 怎么 X”**这种**两个占位符夹住一个词**的模版是平凡的，它只不过是告诉我们这个词可以插入到句子中使用。因此，为了看出效果，我们排除掉这一类模版，得到：


从结果来看，我们的句模版生成算是确实是有效的。因为这些句模版就有助于我们发现语言的使用规律了。比如：

1.**“X 吗”**、**“X 了”**、**“X 怎么样”**这些模版的占位符出现在前面，说明这些词可以放在问句的末尾（我们用到的语料是问句）；

2.**“我 X”**、**“求 X”**、**“为什么 X”**、**“请问 X”**等模版的占位符出现在后面，说明这些词可以放到问句的开头；

3.**“谢谢”**、**“怎么办”**这类模版并没有出现占位符，表明它可以单独成句；

4.**“X 是 X 意思”**、**“X 有哪些 X”**等模版则反映了语言的一些固定搭配。

用通用的观点看，这些模版所描述的都是**句法级的语言现象**。当然，为了不至于跟目前主流的句法分析混淆，我们不妨就称为语言结构规律，或者直接就称为“句模版”。

# 结构解析

跟分词一样，**当构建好句子模版后，我们也需要有算法来识别句子中用到了哪些模版**，也只有做到了这一步，才有可能从语料中识别出词语的等价类出来。

回顾分词算法，分词只是一个句子的切分问题，切分出来的词是没有“洞”（占位符）的，而如果要识别句子中用了哪些模版，这些模版是有“洞”的，并且还可能相互嵌套，这就造成了识别上的困难。然而，一旦我们能够完成这个事情，我们就得到了句子的一个层次结构分解，这是非常有吸引力的目标。

**投射性假设**

为了实现对句子的层次分解，我们首先可以借鉴的是句法分析一般都会使用的“投射性（projective）假设”。

语言的投射性大概意思是指如果句子可以分为几个“语义块”，那么这些语义块是不交叉的。也就是说，假如第 1、2、3 个词组成一个语义块、第 4、5 个词组成一个语义块，这种情况是允许的，而第 1、2、4 个词组成一个语义块、第 3、5 个词组成一个语义块，这种情况是不可能的。大多数语言，包括汉语和英语，基本上都满足投射性。

**结构假设**

为了完成句子的层次结构分解，我们需要对句子的组成结构做更完整的假设。**受到投射性假设的启发**，笔者认为可以将句子的结构做如下假设：

1. 每个语义块是句子的一个连续子字符串，句子本身也算是一个语义块；

2. 每个语义块由一个主的句模版生成，其中句模版的占位符部分也是一个语义块；

3. 每个单独的词可以看成是一个平凡的句模版，也可以看成是一个最小粒度的语义块。

说白了，这三点假设可以归纳为一句话：**每个句子是由句模版相互嵌套生成的**。

咋看之下这个假设不够合理，但仔细思考就会发现，这个假设已经足够描述大多数句子的结构了。读者可能有疑虑的是“有没有可能并行地使用两个句模版，而不是嵌套”？答案是：应该不会。

因为如果出现这种情况，只需要将“并行”本身视为一个模版就行了，比如将“X 和 X”也视为一个模版，那么“X 和 X”这个模版中的两个语义块就是并行的了，甚至它可以与自身嵌套得到“X 和（X 和 X）”描述更多的并行现象。

也正因为我们对语言结构做了这种假设，所以**一旦我们识别出某个句子的最优句模版组合，我们就得到了句子的层次结构**——**因为根据假设，模版是按照嵌套的方式组合的，嵌套意味着递归，递归就是一种层次树的结构了**。

**分解算法**

有了对句子结构的假设，我们就可以描述句模版识别算法了。首先来重述一下分词算法，一元分词算法的思路为：**对句子切分成词，使得这些词的概率对数之和最大（信息量之和最小）**。

它还可以换一种表述：**找一系列的词来不重不漏地覆盖句子中的每个字，使得这些词的概率对数之和最大（信息量之和最小）**。

以往我们会认为分词是对句子进行切分，这种等价的表述则是反过来，要对句子进行覆盖。有了这个逆向思维，就可以提出模版识别算法了：

**找一系列的句模版来不重、不漏、不交叉地覆盖句子中的每个词，使得这些模版的概率对数之和最大（信息量之和最小）。**

当然，这只是思路，在实现过程中，主要难点是对占位符的处理，也就是说，句子中的**每个词既代表这个词本身，也可以代表占位符**，这种二重性使得扫描和识别都有困难。

而不幸中的万幸是，如果按照上面所假设的语言结构，我们可以转化为一个递归运算：**最优的结构分解方案中，主模版下的每个语义块的分解方案也是最优的**。

**▲**句子的层次结构解析，包含了句模版的嵌套调用

**因此我们可以得到算法：**

1. 扫描中句子中所有可能出现的模版（通过 Trie 树结构可以快速扫描）；

2. 每种分解方案的得分，等于句子的主模版得分，加上每个语料块的最优分解方案的得分。

**结果展示**

下面是一些简单例子的演示，是通过有限的几个模版进行的分析，可以看到，的确初步实现了句子的层次结构解析。

```python
+---> (鸡蛋)可以(吃)吗
```

```python
|     +---> 鸡蛋
```

```python
|     |     +---> 鸡蛋
```

```python
|     +---> 可以
```

```python
|     +---> 吃
```

```python
|     |     +---> 吃
```

```python
|     +---> 吗
```

```python
+---> (牛肉鸡蛋)可以(吃)吗
```

```python
|     +---> 牛肉鸡蛋
```

```python
|     |     +---> 牛肉
```

```python
|     |     +---> 鸡蛋
```

```python
|     +---> 可以
```

```python
|     +---> 吃
```

```python
|     |     +---> 吃
```

```python
|     +---> 吗
```

```python
+---> (苹果)的(颜色)是(什么)呢
```

```python
|     +---> 苹果
```

```python
|     |     +---> 苹果
```

```python
|     +---> 的
```

```python
|     +---> 颜色
```

```python
|     |     +---> 颜色
```

```python
|     +---> 是
```

```python
|     +---> 什么
```

```python
|     |     +---> 什么
```

```python
|     +---> 呢
```

```python
+---> (雪梨和苹果和香蕉)的(颜色)是(什么)呢
```

```python
|     +---> (雪梨和苹果)和(香蕉)
```

```python
|     |     +---> (雪梨)和(苹果)
```

```python
|     |     |     +---> 雪梨
```

```python
|     |     |     |     +---> 雪梨
```

```python
|     |     |     +---> 和
```

```python
|     |     |     +---> 苹果
```

```python
|     |     |     |     +---> 苹果
```

```python
|     |     +---> 和
```

```python
|     |     +---> 香蕉
```

```python
|     |     |     +---> 香蕉
```

```python
|     +---> 的
```

```python
|     +---> 颜色
```

```python
|     |     +---> 颜色
```

```python
|     +---> 是
```

```python
|     +---> 什么
```

```python
|     |     +---> 什么
```

```python
|     +---> 呢
```

当然，不能报喜不报忧，也有一些失败的例子：

```python
+---> (我的美味)的(苹果的颜色)是(什么)呢
```

```python
|     +---> (我)的(美味)
```

```python
|     |     +---> 我
```

```python
|     |     |     +---> 我
```

```python
|     |     +---> 的
```

```python
|     |     +---> 美味
```

```python
|     |     |     +---> 美味
```

```python
|     +---> 的
```

```python
|     +---> (苹果)的(颜色)
```

```python
|     |     +---> 苹果
```

```python
|     |     |     +---> 苹果
```

```python
|     |     +---> 的
```

```python
|     |     +---> 颜色
```

```python
|     |     |     +---> 颜色
```

```python
|     +---> 是
```

```python
|     +---> 什么
```

```python
|     |     +---> 什么
```

```python
|     +---> 呢
```

```python
+---> (苹果)的(颜色)是(什么的意思是什么)呢
```

```python
|     +---> 苹果
```

```python
|     |     +---> 苹果
```

```python
|     +---> 的
```

```python
|     +---> 颜色
```

```python
|     |     +---> 颜色
```

```python
|     +---> 是
```

```python
|     +---> (什么)的(意思)是(什么)
```

```python
|     |     +---> 什么
```

```python
|     |     |     +---> 什么
```

```python
|     |     +---> 的
```

```python
|     |     +---> 意思
```

```python
|     |     |     +---> 意思
```

```python
|     |     +---> 是
```

```python
|     |     +---> 什么
```

```python
|     |     |     +---> 什么
```

```python
|     +---> 呢
```

失败的例子我们后面再分析。

# 文章总结

看到一脸懵逼的，有各种话要吐槽的，还请先看到这一节。

**拼图游戏**

从词、词组都句模版，我们都像是在玩拼图：拼着拼着发现这两块合在一起效果还行，那么就将它合起来吧。因为将互信息大的项合起来，作为一个整体来看，就有助于降低整体的信息熵，也就能降低整体的学习难度。

对于句模版，如果在中文的世界里想不通，那么就回顾一下我们在小学、初中时学英语是怎么学过来的吧，那会我们应该学习了很多英语的句模版。

**有什么用**

“句模版”算是本文提出的新概念，用它来识别语言结果也算是一种新的尝试。读者不禁要问：这玩意有什么用？

我想，回答这个问题的最好方式，是引用牛顿的一段话：

> 我自己认为，我好像只是一个在海边玩耍的孩子，不时为捡到比通常更光滑的石子或更美丽的贝壳而欢欣，而展现在我面前的是完全未被探明的真理之海。

我引用这段话是想表明，**做这个探究的最根本原因，并不是出于某种实用目的，而是为了纯粹地探究自然语言的奥秘**。

当然，如果与此同时，研究出来的结果能具备一定的应用价值，那就更加完美了。从现在的结果来看，这种应用价值可能是存在的。

因为我们**在 NLP 中，面对的句子千变万化，但事实上“句式”却是有限的，这也意味着句模版也是有限的**，如果有必要，我们可以对各个句模板的占位符含义进行人工标注，这就能将句模板的结构跟常规的句法描述对应起来了。**通过有限的句模版来对句子进行（无限的）分解，能让 NLP 可面对的场景更加灵活多变一些**。

也许以往的传统自然语言处理中，也出现过类似的东西，但本文所描述的内容纯粹是无监督的结果，并且也有自洽的理论描述，算是一个比较完整的框架，初步的结果也差强人意，因此值得进一步去思考它的应用价值。

**艰难前进**

浏览完这篇文章，读者最大的感觉也许是“一脸懵逼”：**能再简化一点吗？**

要回答这个问题，就不得不提到：距离这个系列的上一篇文章已经过了一个多月，这篇文章才正式发出，这似乎有点久了？从形式上看，本文只不过是前文的简单推广：不就是将相邻关联推广到非相邻关联吗？

**的确，形式上确实如此。但为了将这个想法推广至同时具备理论和实用价值，却并不是那么简单和顺畅的事情**。比如，在句模版生成时，如何不遗漏地得到所有的候选模版，这便是一个难题；其次，在得到句模版（不管是自动生成还是人工录入）后，如何识别出句子中的句模版，这更加艰难了，不论在理论思考还是编程实现上，都具有相当多的障碍，需要对树结构、递归编程有清晰的把握。我也是陆陆续续调试了半个多月，才算是把整个流程调通了，但估计还不完备。

所以，你看得一脸懵逼是再正常不过了，我自己做完、写完这篇文章，还感觉很懵呢。

**改进思路**

在结果结果展示一节中，我们也呈现一些失败的例子。事实上，失败的例子可能还更多。

我们要从两个角度看待这个事情。一方面，我们有成功的例子，对应纯粹无监督挖掘的探索，我们哪怕只能得到一小部分成功的结果，也是值得高兴的；另外一方面，对于失败的例子，我们需要思考失败的原因，并且考虑解决方案。

笔者认为，整体的句模版思路是没有问题的，而问题在于我们没有达到真正的语义级别的理解。比如第一个失败的例子，结果是：**（我的美味）的（苹果的颜色）是（什么）呢**。

我们能说这个分解完全错吗？显然不是，严格来讲，这种分解在语法上并没有任何错误，只是它不符合语义，不符合我们的常识。因此，并非是句模版的错，而是**还不能充分地结合语义来构建句模版**。

回顾目前主流的句法分析工作，不管是有监督的还是无监督的，它们基本上都要结合“词性”来完成句法分析。所以这给我们提供了一个方向：最小熵系列下一步的工作就是要探究词语的聚类问题，以便更好地捕捉词义和语言共性。

# 基于最小熵原理的NLP库：NLP Zero

陆陆续续写了几篇最小熵原理的文章，致力于无监督做 NLP 的一些基础工作。为了方便大家实验，把文章中涉及到的一些算法封装为一个库，供有需要的读者测试使用。

由于面向的是无监督 NLP 场景，而且基本都是 NLP 任务的基础工作，因此命名为NLP Zero。

**地址**

> Github:
> https://github.com/bojone/nlp-zero

> Pypi:
> https://pypi.org/project/nlp-zero/

可以直接通过：

```python
pip install nlp-zero==
```
```python
0.1
```
```python
.6
```

进行安装。整个库纯 Python 实现，没有第三方调用，支持 Python 2.x 和 3.x。

**使用**

**默认分词**

库内带了一个词典，可以作为一个简单的分词工具用。

```python
from
```
```python
nlp_zero
```
```python
import
```
```python
*
```

```python
t = Tokenizer()
```

```python
t.tokenize(
```
```python
u'扫描二维码，关注公众号'
```
```python
)
```

自带的词典加入了一些通过新词发现挖掘出来的新词，并且经过笔者的人工优化，质量相对来说还是比较高的。

**词库构建**

通过大量的原始语料来构建词库。

首先我们需要写一个迭代容器，这样就不用一次性把所有语料加载到内存中了。迭代器的写法很灵活，比如我的数据存在 MongoDB 中，那就是：

```python
import
```
```python
pymongo
```

```python
db = pymongo.MongoClient().weixin.text_articles
```

```python
class
```
```python
D
```
```python
:
```

```python
def
```
```python
__iter__
```
```python
(self)
```
```python
:
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
db.find().limit(
```
```python
10000
```
```python
):
```

```python
yield
```
```python
i[
```
```python
'text'
```
```python
]
```

如果数据存在文本文件中，大概就是：

```python
class
```
```python
D
```
```python
:
```

```python
def
```
```python
__iter__
```
```python
(self)
```
```python
:
```

```python
with
```
```python
open(
```
```python
'text.txt'
```
```python
)
```
```python
as
```
```python
f:
```

```python
for
```
```python
l
```
```python
in
```
```python
f:
```

```python
yield
```
```python
l.strip()
```
```python
# python2.x还需要转编码
```

然后就可以执行了。

```python
from
```
```python
nlp_zero
```
```python
import
```
```python
*
```

```python
import
```
```python
logging
```

```python
logging.basicConfig(level = logging.INFO, format =
```
```python
'%(asctime)s - %(name)s - %(message)s'
```
```python
)
```

```python
f = Word_Finder(min_proba=
```
```python
1e-8
```
```python
)
```

```python
f.train(D())
```
```python
# 统计互信息
```

```python
f.find(D())
```
```python
# 构建词库
```

通过 Pandas 查看结果：

```python
import
```
```python
pandas
```
```python
as
```
```python
pd
```

```python
words = pd.Series(f.words).sort_values(ascending=
```
```python
False
```
```python
)
```

直接用统计出来的词库建立一个分词工具：

```python
t = f.export_tokenizer()
```

```python
t.tokenize(
```
```python
u'今天天气不错'
```
```python
)
```

**句模版构建**

跟前面一样，同样要写一个迭代器，这里不再重复。 因为构建句模版是基于词来统计的，因此还需要一个分词函数，可以用自带的分词器，也可以用外部的，比如结巴分词。

```python
from
```
```python
nlp_zero
```
```python
import
```
```python
*
```

```python
import
```
```python
logging
```

```python
logging.basicConfig(level = logging.INFO, format =
```
```python
'%(asctime)s - %(name)s - %(message)s'
```
```python
)
```

```python
tokenize = Tokenizer().tokenize
```
```python
# 使用自带的分词工具
```

```python
# 通过 tokenize = jieba.lcut 可以使用结巴分词
```

```python
f = Template_Finder(tokenize, window=
```
```python
3
```
```python
)
```

```python
f.train(D())
```

```python
f.find(D())
```

通过 Pandas 查看结果：

```python
import
```
```python
pandas
```
```python
as
```
```python
pd
```

```python
templates = pd.Series(f.templates).sort_values(ascending=
```
```python
False
```
```python
)
```

```python
idx = [i
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
templates.index
```
```python
if
```
```python
not
```
```python
i.is_trivial()]
```

```python
templates = templates[idx]
```
```python
# 筛选出非平凡的模版
```

每个模版已经被封装为一个类了。

**层次分解**

基于句模版来进行句子结构解析。

```python
from
```
```python
nlp_zero
```
```python
import
```
```python
*
```

```python
# 建立一个前缀树，并加入模版
```

```python
# 模版可以通过tuple来加入，
```

```python
# 也可以直接通过“tire[模版类]=10”这样来加入
```

```python
trie = XTrie()
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'呢'
```
```python
)] =
```
```python
10
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'可以'
```
```python
,
```
```python
None
```
```python
,
```
```python
u'吗'
```
```python
)] =
```
```python
9
```

```python
trie[(
```
```python
u'我'
```
```python
,
```
```python
None
```
```python
)] =
```
```python
8
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'的'
```
```python
,
```
```python
None
```
```python
,
```
```python
u'是'
```
```python
,
```
```python
None
```
```python
)] =
```
```python
7
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'的'
```
```python
,
```
```python
None
```
```python
,
```
```python
u'是'
```
```python
,
```
```python
None
```
```python
,
```
```python
u'呢'
```
```python
)] =
```
```python
7
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'的'
```
```python
,
```
```python
None
```
```python
)] =
```
```python
12
```

```python
trie[(
```
```python
None
```
```python
,
```
```python
u'和'
```
```python
,
```
```python
None
```
```python
)] =
```
```python
12
```

```python
tokenize = Tokenizer().tokenize
```
```python
# 使用自带的分词工具
```

```python
p = Parser(trie, tokenize)
```
```python
# 建立一个解析器
```

```python
p.parse(
```
```python
u'鸡蛋可以吃吗'
```
```python
)
```
```python
# 对句子进行解析
```

```python
"""输出：
```

```python
>>>
```
```python
p.parse(u'鸡蛋可以吃吗')
```

```python
+---> (鸡蛋)可以(吃)吗
```

```python
|     +---> 鸡蛋
```

```python
|     |     +---> 鸡蛋
```

```python
|     +---> 可以
```

```python
|     +---> 吃
```

```python
|     |     +---> 吃
```

```python
|     +---> 吗
```

```python
"""
```

为了方便对结果进行调用以及可视化，输出结果已经被封装为一个 SentTree 类。这个类有三个属性：template（当前主模版）、content（当前主模版覆盖的字符串）、modules（语义块的 list，每个语义块也是用 SentTree 来描述）。总的来说，就是按照本文对语言结构的假设来设计的。


**点击以下标题查看作者其他文章：**

[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)
[基于CNN的阅读理解式问答模型：DGCNN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)
[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)

[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

[简明条件随机场CRF介绍 | 附带纯Keras实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489378&idx=1&sn=0e0ed4424bb336022f36d8e2236f96cc&chksm=96e9c8e2a19e41f4d1fb67254ee3c057ce66a4eaa4084db89d53f314c833b73fb79b8ee3c0dd&scene=21#wechat_redirect)



**▲**戳我查看招募详情

**\#****作 者 招 募****\#**

**[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)**


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

▽ 点击 |阅读原文| 进入作者博客


