
# 量化交易研究———基础篇（1）技术指标KDJ说明 - leofionn的博客 - CSDN博客


2018年05月04日 17:11:58[leofionn](https://me.csdn.net/qq_36142114)阅读数：276


环境：Anaconda3
主要研究包：TuShare、Numpy、Pandas
基础研究方向：
技术指标：KDJ

KDJ概念
rsv =（收盘价– n日内最低价）/（n日内最高价– n日内最低价）×100
K = rsv的m天移动平均值
D = K的m1天的移动平均值
J = 3K - 2D
rsv:未成熟随机值

KDJ的计算比较复杂，首先要计算周期（n日、n周等）的RSV值，即未成熟随机指标值，然后再计算K值、D值、J值等。以n日KDJ数值的计算为例，其计算公式为
n日RSV=（Cn－Ln）/（Hn－Ln）×100
公式中，Cn为第n日收盘价；Ln为n日内的最低价；Hn为n日内的最高价。
其次，计算K值与D值：
当日K值=2/3×前一日K值+1/3×当日RSV
当日D值=2/3×前一日D值+1/3×当日K值
若无前一日K 值与D值，则可分别用50来代替。
J值=3*当日K值-2*当日D值
以9日为周期的KD线为例，即未成熟随机值，计算公式为
9日RSV=（C－L9）÷（H9－L9）×100
公式中，C为第9日的收盘价；L9为9日内的最低价；H9为9日内的最高价。
K值=2/3×第8日K值+1/3×第9日RSV
D值=2/3×第8日D值+1/3×第9日K值
J值=3*第9日K值-2*第9日D值
若无前一日K值与D值，则可以分别用50代替。

一般原则：
D%>80，市场超买；D%<20，市场超卖。
J%>100，市场超买；J%<10，市场超卖。
KD金叉：K%上穿D%，为买进信号。
KD死叉：K%下破D%，为卖出信号。
KDJ的基本应用方法
当Ｋ值（短期平均值）大于D值（长期平均值）时表明目前市场处于强势状态，因此在图形上K线向上突破D线时即为买进信号。
当D值大于K值时表明目前的趋势是向下跌落，因此在图形上K线向下跌破D线为卖出时机。
当D值跌至10-15时是最佳买入时机，若高至85-90时则是卖出信号。
K线与D线在高档二次交叉则行情将大跌，在低档二次交叉则行情将大涨。
与RSI指标相似，当价格与指标出现背离的时候说明市场即将出现反转。
J线的参考意义不大，但可作为市场反转的警示。应当注意的是，窄幅盘整行情不宜看KDJ指标，长期单边行情后也不宜看KDJ，因为此时指标已经钝化。另外，KDJ不适合作为研究长期走势的参考工具。
随机指标的优缺点：
KDJ指标比RSI准确率高，并且有明确的买卖信号出现，但K、D线交叉时须注意“骗线”出现，因为该指标过于敏感。
参数说明：
rsv天数默认值：9，K默认值：3，D默认值：3。

技术指标小结
许多投资者往往会在指标运用过程中产生这样的疑惑：有时指标严重超买，价格却继续上涨；有时指标在超卖区钝化十几周而价格仍未止跌企稳。实际上，投资者在这里混淆了指标与价格的关系。指标不能决定市场的走向，价格本身才决定指标的运行状况。价格是因，指标是果，由因可推出果，由果来溯因则是本末倒置。事实上，最能有效体现市场行为的是形态，投资者首先应当从技术形态中分析市场参与者的心理变化并服从市场。在涨跌趋势未改变之前，不要试图运用指标的超买、超卖或钝化等来盲目断定市场该反弹、该回调了。我们应当灵活地运用技术指标，充分发挥其辅助参考作用。

