
# 如何写出复杂业务查询的sql语句 - jiahaowanhao的博客 - CSDN博客


2018年03月16日 15:01:41[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：256


[如何写出复杂业务查询的sql语句](http://cda.pinggu.org/view/25003.html)
如何写出复杂的sql语句：
首先要建立一个概念，复杂的sql语句也是最基本的sql语句连接而成，所以最重要的是先要理清思路和逻辑，弄清自己要查哪几张表，要用哪几个字段，表之间如何关联，将这些弄清，然后由简单到复杂，从最基本的sql写起，通过找共同点，实现表关联等。
select后是自己需要的字段
from后是自己需要查询的多张表或者自己子查询得出的结果集
where后是条件 是对from后的结果集进行筛选
多张表关联 最重要的是找共同点 比如通过userid 第一种方式就是通过join管理 第二种方式就是通过where条件子句 比如几个表的userid相等来筛
选结果集
在处理复杂的业务查询时，先从逻辑层面理清几张表之间的关系以及自己需要的字段和数据 然后逐步拆分 从最简单的局部sql出发 一步步迭代出复
杂的sql语句 这可以看做是写复杂脚本的原则：
由简单到复杂 逐步迭代 得出结果
最重要的还是在工作实践中多加总结 主动接触

