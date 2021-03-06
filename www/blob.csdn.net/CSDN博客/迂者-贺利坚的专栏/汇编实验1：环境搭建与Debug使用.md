
# 汇编实验1：环境搭建与Debug使用 - 迂者-贺利坚的专栏 - CSDN博客

2017年03月08日 22:52:17[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1567



### 1 实验目的
学会搭建汇编语言程序设计的软件平台
学会Debug实用程序的基本功能
对汇编指令、寄存器、内存空间产生直观的认识
### 2 实验内容
**任务0-搭建汇编语言实验环境**
参考视频“0105 汇编语言实践环境搭建”，搭建汇编语言实验环境，以便于下面的工作。
如果使用的winXP，可以不安装DOSBOX模拟器，而是用XP的MS-DOS方式运行masm文件夹中的命令。
参考文章：[搭建x86汇编语言学习环境（内含软件下载链接）](http://blog.csdn.net/sxhelijian/article/details/54845039)
**任务1-Debug程序的使用**
参考视频“0205 Debug的使用”和教材P35页对应的讲解，自行演练相关的Debug命令。
用R命令查看、改变CPU寄存器的内容
用D命令查看内存中的内容
用E命令改变内存中的内容
用U命令将内存中的机器指令翻译成汇编指令
用A命令以汇编指令的格式在内存中写入机器指令
用T命令执行机器指令
实验报告中，每条命令至少要截一个图展示。
**任务2-使用Debug运行程序**
使用Debug，将右面的程序段写入内存（用a命令）后单步执行（用t命令），观察每条指令执行后CPU中相关寄存器中内容的变化。
**任务3-查看内存中的内容**
在内存FFF00H~FFFFF间浏览（用d命令），找到一段记录日期值的内存，截屏并说明日期值。
注：这个日期代表主板的生产日期，参考第1章1.15节
**任务4-在屏幕上显示多彩符号**
向内存B8100H开始的空间中写入如下数据“01 01 02 02 03 03 04 04”（用e命令），观察并记录产生的现象，再修改写入的数据，如改写为“4C 17 6F 92 76 a3 65 84”，以及你想玩的其他数值，再观察和记录。
注：对实验结果的解释，见第1章1.15节和教材187页实验9。

