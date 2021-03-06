
# linux shell 实现 四则运算（整数及浮点） 简单方法 - Augusdi的专栏 - CSDN博客


2015年04月26日 00:06:04[Augusdi](https://me.csdn.net/Augusdi)阅读数：867


﻿﻿
在刚刚学习写shell 批处理时候，进行逻辑运算中，少不了需要进行基础的：四则运算，这里说说在linux shell 里面简单的实现方法。
**1.简单方法**
[chengmo@centos5 ~]$ b=$((5*5+5-3/2))
[chengmo@centos5 ~]$ echo $b
29
在linux shell中，我们可以使用**$(())**将表达式放在括号中，即可达到运算的功能。
**2.其它方法：**
用：expr 实现运算
[chengmo@centos5 ~]$ expr 5 - 4
1
注意：将需要运算的表达式写入在expr 后面即可，保证 参数与运算符号中间有空格隔开。
|类别|语法|说明|
|条件判断|expr1 \| expr2|如果 expr1 不是零或 null 则传回 expr1，否则传回 expr2。|
|expr1 \& expr2|如果 expr1 及 expr2 都不为零或 null，则传回 expr1，否则传回 0。|
|四则运算|expr1 + expr2|传回 expr1 加 expr2 后的值。|
|expr1 - expr2|传回 expr1 减 expr2 后的值。|
|expr1\* expr2|传回 expr1 乘 expr2 后的值。|
|expr1 / expr2|传回 expr1 除 expr2 后的值。|
|expr1 % expr2|传回 expr1 除 expr2 的余数。|
|大小判断|expr1 \> expr2|如果 expr1 大于 expr2 则传回 1，否则传回 0。如果 expr1 及 expr2 都是数字，则是以数字大小判断，否则是以文字判断。以下皆同。|
|expr1 \< expr2|如果 expr1 小于 expr2 则传回 1，否则传回 0。|
|expr1 = expr2|如果 expr1 等于 expr2 则传回 1，否则传回 0。|
|expr1 != expr2|如果 expr1 不等于 expr2 则传回 1，否则传回 0。|
|expr1 \>= expr2|如果 expr1 大于或等于 expr2 则传回 1，否则传回 0。|
|expr1 \<= expr2|如果 expr1 小于或等于 expr2 则传回 1，否则传回 0。|
|文字处理|expr1 : expr2|比较一固定字符串，即 regular expression。可以使用下列字符来辅助：|. 匹配一个字符。
|$ 找字符串的结尾。
|[list] 找符合 list 中的任何字符串。
|* 找寻 0 个或一个以上在 * 之前的字。
|\( \) 传回括号中所匹配的字符串。
|

**3.浮点运算：**
[chengmo@centos5 ~]$ expr 5.0 - 4
expr: 非法参数
[chengmo@centos5 ~]$ echo $((5.0-4))
-bash: 5.0-4: syntax error in expression (error token is ".0-4")
从上面运算结果，看来上面表达式，不足以支持浮点运算了。查阅资料才发现：bash 不支持浮点运算，如果需要进行浮点运算，需要借助bc,awk 处理。
方法一：
[chengmo@centos5 ~]$ c=$(echo "5.01-4*2.0"|bc)
[chengmo@centos5 ~]$ echo $c
-2.99
方法二：
[chengmo@centos5 ~]$ c=$(awk 'BEGIN{print 7.01*5-4.01 }')
[chengmo@centos5 ~]$ echo $c
31.04
注：在shell 中$() 与 ``等效。 中间包含命令语句执行，返回执行结果。

[http://www.cnblogs.com/chengmo/archive/2010/09/30/1839556.html](http://www.cnblogs.com/chengmo/archive/2010/09/30/1839556.html)

