
# 吐槽版︱MRO-Microsoft R Open快捷键+界面识别+功能设置 - 素质云笔记-Recorder... - CSDN博客

2016年08月01日 17:24:33[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2318


下载了之后，发现连运行（RUN键）在哪都不知道，蒙逼的在哪倒弄半天，都执行不了。。。问了别人，都说”ctrl+enter“，但是我的电脑执行不了，于是今天就狠狠的一个一个按钮的点一遍...（真是费劲...）于是有了以下的一些总结。
中文界面的更改可以看：新工具︱微软Microsoft
 Visual Studio的R语言模块下载试用Ing...（尝鲜）

**——————————————————————————————————————————**

# 一、执行快捷键

因为用的是中文界面，发现翻译还是有点误差的。一般“交互执行”才能run出来。点击是可以的，但是快捷键在哪呢？

![](https://img-blog.csdn.net/20160801163824520)

于是，我点击了 R Tools的OPtions,只能说，这个界面简直弱爆了...（有可能是因为我自己换成了中文版，看不懂其中的意思）

![](https://img-blog.csdn.net/20160801164401872)
我一直在interactive window 的“send to R console”shotcut折腾，弄了半天，根本更改不了设置，
而且是control+E，其中control应该就是ctrl才对，可是就是执行不出来。


终于在visual studio选项界面找到了设置方法：
**选项-键盘-Rtools交互执行**
![](https://img-blog.csdn.net/20160801165724574)

**注意：**
我设置的是跟RStudio一样的键位，ctrl+enter，但是这个键位在Visual Studio已经有默认，所以看客们不仅需要设置，还要把VS中默认的给删除，不然有时候会出现执行的时候，执行了默认设置去了。
我的电脑里面，CTRL+ENTER默认设置是编辑.上开新行，如果没有删除这个默认，按了快捷键，就会在代码上面多了一行，而不是run代码。

——————————————————————————————————————————


# 二、R Tools设置 与 visual studio设置

RTools的设置简直是渣渣，Visual studio自身的设定才靠谱。

RTOOLS中可以进行以下几个设置：如第一张图，看客需要简单设置一下general的一些设置：
1、cran mirror的路径，设置到china来；
2、R engine的路径，就是Microsoft R Open MRO的路径

**VS的设置（第二张图）：**
比较关键的设置是在环境、文本编辑器

1、环境-键盘
笔者设置了run键（ctrl+enter），设置了R的reset键（F5）、R执行时退出（esc）
其中看客有自己的看法欢迎留言。

2、文本编辑器-R
文本编辑器有一个R选项，你可以设置行号，因为RSutio（RS）中每一个都有号码，VS中反而没有了，所以如果需要，可以设置一下。

——————————————————————————————————————————


# 三、help 查找功能

RS中的查找功能比VS要好一些，有主动查找的地方。

![](https://img-blog.csdn.net/20160801171720615)

VS好像调不出来，于是乎，只能用??来查找一些不知道的函数。
除了有查找框之外，显示内容貌似没有太大的差异。

![](https://img-blog.csdn.net/20160801171835694)


-----------------------------------------------
发现国内没有很好的关于VS R应用的讨论群体或社团，如果有请看客留言，谢谢~~~


