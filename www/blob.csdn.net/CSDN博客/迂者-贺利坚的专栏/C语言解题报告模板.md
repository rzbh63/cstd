
# C语言解题报告模板 - 迂者-贺利坚的专栏 - CSDN博客

2016年09月17日 11:10:44[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1011


问题及代码：

```python
/* 
Copyright (c) 2014, 烟台大学计算机学院 
 All rights reserved. 
文件名称：test.cpp 
作    者：贺利坚  
完成日期：2016年 10 月 x 日 
版 本 号：v1.0 

题目描述
　计算并输出它们串联电阻值
输入
　两个电阻r1和r2的阻值
输出
　一个整数，代表串联电阻的值
样例输入
　1 2
样例输出
　3
 */ 
#include <stdio.h>
int main()
{
    int r,r1,r2;
    printf("输入两个电阻的阻值： ");
    scanf("%d %d", &r1, &r2);
    r = r1 + r2;
    printf("并联后的电阻值为：%d\n", r);
    return 0;
}
```

运行结果：
![](https://img-blog.csdn.net/20160917103138185)
知识点总结：
从这个程序中，进一步巩固了对简单程序结构的认识，也学会了利用输入的变量，构造表达式求出想要的结果的方法。
数据类型选择中，有过一点思考，其实用成float也行，不过程序中输入输出的地方需要改一改。
学习心得：
一开始写程序，某句结束没写分号、少了一个花括号，于是有了一番纠结。
把这个问题改过来了，明白了这就是积累。遇到错误不慌张，在错误中成长，我能行！
从现在开始，就算是以专业人员的身份在CSDN安营扎寨了，我期望自己能做得更好。少年，努力吧！




