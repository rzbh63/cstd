
# Python使用re模块正则提取字符串中括号内的内容示例 - jiahaowanhao的博客 - CSDN博客


2018年06月24日 22:03:53[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：2225


[Python使用re模块正则提取字符串中括号内的内容示例](http://cda.pinggu.org/view/25912.html)
这篇文章主要介绍了Python使用re模块正则提取字符串中括号内的内容,结合实例形式分析了Python使用re模块进行针对括号内容的正则匹配操作,并简单解释了相关修正符与正则语句的用法,需要的朋友可以参考下
本文实例讲述了Python使用re模块正则提取字符串中括号内的内容操作。分享给大家供大家参考，具体如下：
直接上代码吧：
\# -*- coding:utf-8 -*-
\#! python2
import re
string = 'abe(ac)ad)'
p1 = re.compile(r'[(](.*?)[)]', re.S) \#最小匹配
p2 = re.compile(r'[(](.*)[)]', re.S)  \#贪婪匹配
print(re.findall(p1, string))
print(re.findall(p2, string))
输出：
['ac']
['ac)ad']
解释一下：
1.正则匹配串前加了r就是为了使得里面的特殊符号不用写反斜杠了。
2.[ ]具有去特殊符号的作用,也就是说[(]里的(只是平凡的括号
3.正则匹配串里的()是为了提取整个正则串中符合括号里的正则的内容
4. .是为了表示除了换行符的任一字符。*克林闭包，出现0次或无限次。
5. 加了？是最小匹配，不加是贪婪匹配。
6. re.S是为了让.表示除了换行符的任一字符。

