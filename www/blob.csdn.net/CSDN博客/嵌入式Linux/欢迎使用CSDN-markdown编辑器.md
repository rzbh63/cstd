
# 欢迎使用CSDN-markdown编辑器 - 嵌入式Linux - CSDN博客

2015年07月07日 09:33:07[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：439个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



# 欢迎使用Markdown编辑器写博客
本Markdown编辑器使用[StackEdit](https://github.com/benweet/stackedit)修改而来，用它写博客，将会带来全新的体验哦：
Markdown和扩展Markdown简洁的语法
代码块高亮
图片链接和图片上传
LaTex数学公式
UML序列图和流程图
离线写博客
导入导出Markdown文件
丰富的快捷键
---

## 快捷键
加粗Ctrl + B
斜体Ctrl + I
引用Ctrl + Q
插入链接Ctrl + L
插入代码Ctrl + K
插入图片Ctrl + G
提升标题Ctrl + H
有序列表Ctrl + O
无序列表Ctrl + U
横线Ctrl + R
撤销Ctrl + Z
重做Ctrl + Y
## Markdown及扩展
> Markdown 是一种轻量级标记语言，它允许人们使用易读易写的纯文本格式编写文档，然后转换成格式丰富的HTML页面。    ——
> [ [ 维基百科 ]](https://zh.wikipedia.org/wiki/Markdown)
使用简单的符号标识不同的标题，将某些文字标记为**粗体**或者*斜体*，创建一个[链接](http://www.csdn.net)等，详细语法参考帮助？。
本编辑器支持**Markdown Extra**, 　扩展了很多好用的功能。具体请参考[Github](https://github.com/jmcmanus/pagedown-extra).
### 表格
**Markdown　Extra**表格语法：
|项目|价格|
|---|---|
|Computer|$1600|
|Phone|$12|
|Pipe|$1|
可以使用冒号来定义对齐方式：
|项目|价格|数量|
|---|---|---|
|Computer|1600 元|5|
|Phone|12 元|12|
|Pipe|1 元|234|
### 定义列表
**Markdown　Extra**定义列表语法：项目１项目２定义 A定义 B项目３定义 C定义 D
> 定义D内容

### 代码块
代码块语法遵循标准markdown代码，例如：
```python
@requires_authorization
```
```python
def
```
```python
somefunc
```
```python
(param1=
```
```python
''
```
```python
, param2=
```
```python
0
```
```python
)
```
```python
:
```
```python
'''A docstring'''
```
```python
if
```
```python
param1 > param2:
```
```python
# interesting
```
```python
print
```
```python
'Greater'
```
```python
return
```
```python
(param2 - param1 +
```
```python
1
```
```python
)
```
```python
or
```
```python
None
```
```python
class
```
```python
SomeClass
```
```python
:
```
```python
pass
```
```python
>>>
```
```python
message =
```
```python
'''interpreter
```
```python
...
```
```python
prompt'''
```
### 脚注
生成一个脚注[1](#fn:footnote).
### 目录
用`[TOC]`来生成目录：
[欢迎使用Markdown编辑器写博客](#欢迎使用markdown编辑器写博客)[快捷键](#快捷键)
[Markdown及扩展](#markdown及扩展)[表格](#表格)
[定义列表](#定义列表)
[代码块](#代码块)
[脚注](#脚注)
[目录](#目录)
[数学公式](#数学公式)
[UML 图](#uml-图)
[离线写博客](#离线写博客)
[浏览器兼容](#浏览器兼容)


### 数学公式
使用MathJax渲染*LaTex*数学公式，详见[math.stackexchange.com](http://math.stackexchange.com/).
行内公式，数学公式为：\Gamma(n) = (n-1)!\quad\forall n\in\mathbb N。
块级公式：

$$
x = \dfrac{-b \pm \sqrt{b^2 - 4ac}}{2a}
$$
更多LaTex语法请参考[这儿](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).
### UML 图:
可以渲染序列图：
Created with Raphaël 2.1.0张三张三李四李四嘿，小四儿, 写博客了没?李四愣了一下，说：忙得吐血，哪有时间写。
或者流程图：
Created with Raphaël 2.1.0开始我的操作确认？结束yesno
关于序列图语法，参考[这儿](http://bramp.github.io/js-sequence-diagrams/),
关于流程图语法，参考[这儿](http://adrai.github.io/flowchart.js/).
## 离线写博客
即使用户在没有网络的情况下，也可以通过本编辑器离线写博客（直接在曾经使用过的浏览器中输入[write.blog.csdn.net/mdeditor](http://write.blog.csdn.net/mdeditor)即可。**Markdown编辑器**使用浏览器离线存储将内容保存在本地。
用户写博客的过程中，内容实时保存在浏览器缓存中，在用户关闭浏览器或者其它异常情况下，内容不会丢失。用户再次打开浏览器时，会显示上次用户正在编辑的没有发表的内容。
博客发表后，本地缓存将被删除。
用户可以选择把正在写的博客保存到服务器草稿箱，即使换浏览器或者清除缓存，内容也不会丢失。
> 注意：
> 虽然浏览器存储大部分时候都比较可靠，但为了您的数据安全，在联网后，
> 请务必及时发表或者保存到服务器草稿箱
> 。

## 浏览器兼容
目前，本编辑器对Chrome浏览器支持最为完整。建议大家使用较新版本的Chrome。
IE９以下不支持
IE９，１０，１１存在以下问题
不支持离线功能
IE9不支持文件导入导出
IE10不支持拖拽文件导入

---

---
这里是脚注的内容.[↩](#fnref:footnote)


