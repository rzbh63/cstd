
# CSDN-markdown的格式 - 小花生的博客 - CSDN博客


2018年09月19日 22:50:07[Peanut_范](https://me.csdn.net/u013841196)阅读数：47



### 这里写自定义目录标题
[欢迎使用Markdown编辑器](#Markdown_2)
[新的改变](#_6)
[功能快捷键](#_20)
[合理的创建标题，有助于目录的生成](#_35)
[如何改变文本的样式](#_43)
[插入链接与图片](#_61)
[如何插入一段漂亮的代码片](#_72)
[生成一个适合你的列表](#_81)
[创建一个表格](#_95)
[设定内容居中、居左、居右](#_103)
[SmartyPants](#SmartyPants_112)
[创建一个自定义列表](#_121)
[如何创建一个注脚](#_130)
[注释也是必不可少的](#_137)
[KaTeX数学公式](#KaTeX_144)
[新的甘特图功能，丰富你的文章](#_158)
[UML 图表](#UML__172)
[FLowchart流程图](#FLowchart_200)
[导出与导入](#_218)
[导出](#_220)
[导入](#_223)

# 欢迎使用Markdown编辑器
你好！ 这是你第一次使用**Markdown编辑器**所展示的欢迎页。如果你想学习如何使用Markdown编辑器, 可以仔细阅读这篇文章，了解一下Markdown的基本语法知识。
## 新的改变
我们对Markdown编辑器进行了一些功能拓展与语法支持，除了标准的Markdown编辑器功能，我们增加了如下几点新功能，帮助你用它写博客：
全新的界面设计，将会带来全新的写作体验；
在创作中心设置你喜爱的代码高亮样式，Markdown将代码片显示选择的高亮样式进行展示；
增加了图片拖拽功能，你可以将本地的图片直接拖拽到编辑区域直接展示；
全新的KaTeX数学公式语法；
增加了支持甘特图的mermaid语法[1](#fn1)功能；
增加了多屏幕编辑Markdown文章功能；
增加了焦点写作模式、预览模式、简洁写作模式、左右区域同步滚轮设置等功能，功能按钮位于编辑区域与预览区域中间；
增加了检查列表功能。
## 功能快捷键
撤销：Ctrl/Command+Z
重做：Ctrl/Command+Y
加粗：Ctrl/Command+Shift+B
斜体：Ctrl/Command+Shift+I
标题：Ctrl/Command+Shift+H
无序列表：Ctrl/Command+Shift+U
有序列表：Ctrl/Command+Shift+O
检查列表：Ctrl/Command+Shift+C
插入代码：Ctrl/Command+Shift+K
插入链接：Ctrl/Command+Shift+L
插入图片：Ctrl/Command+Shift+G
## 合理的创建标题，有助于目录的生成
直接输入1次\#，并按下space后，将生成1级标题。
输入2次\#，并按下space后，将生成2级标题。
以此类推，我们支持6级标题。有助于使用`TOC`语法后生成一个完美的目录。
## 如何改变文本的样式
*强调文本**强调文本*
**加粗文本****加粗文本**
标记文本
删除文本
> 引用文本
H2O is是液体。
210运算结果是 1024.
## 插入链接与图片
链接:[link](https://mp.csdn.net).
图片:![Alt](https://avatar.csdn.net/7/7/B/1_ralf_hx163com.jpg)
带尺寸的图片:![Alt](https://avatar.csdn.net/7/7/B/1_ralf_hx163com.jpg)
当然，我们为了让用户更加便捷，我们增加了图片拖拽功能。
## 如何插入一段漂亮的代码片
去[博客设置](https://mp.csdn.net/configure)页面，选择一款你喜欢的代码片高亮样式，下面展示同样高亮的`代码片`.
```python
// An highlighted block
```
```python
var
```
```python
foo
```
```python
=
```
```python
'bar'
```
```python
;
```
## 生成一个适合你的列表
项目项目项目

项目1
项目2
项目3
计划任务
完成任务
创建一个表格
一个简单的表格是这么创建的：
项目Value|
|---|---|
电脑$1600|
手机$12|
导管$1|
设定内容居中、居左、居右
使用:---------:居中
使用:----------居左
使用----------:居右
第一列第二列第三列|
|---|---|---|
第一列文本居中第二列文本居右第三列文本居左|
SmartyPants
SmartyPants将ASCII标点字符转换为“智能”印刷标点HTML实体。例如：
TYPEASCIIHTML|
|---|---|---|
Single backticks'Isn't this fun?'‘Isn’t this fun?’|
Quotes"Isn't this fun?"“Isn’t this fun?”|
Dashes-- is en-dash, --- is em-dash– is en-dash, — is em-dash|
创建一个自定义列表
MarkdownText-to-HTMLconversion toolAuthorsJohnLuke如何创建一个注脚
一个具有注脚的文本。[2](#fn2)
注释也是必不可少的
Markdown将文本转换为HTML。
KaTeX数学公式
您可以使用渲染LaTeX数学表达式[KaTeX](https://khan.github.io/KaTeX/):
Gamma公式展示
$$
\Gamma(n) = (n-1)!\quad\forall
n\in\mathbb N
$$
是通过欧拉积分
$$
\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,.
$$
你可以找到更多关于的信息LaTeX数学表达式[here](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).
新的甘特图功能，丰富你的文章
Mon 06Mon 13Mon 20已完成进行中计划一计划二现有任务Adding GANTT diagram functionality to mermaid
关于甘特图语法，参考[这儿](https://mermaidjs.github.io/),
UML 图表
可以使用UML图表进行渲染。[Mermaid](https://mermaidjs.github.io/). 例如下面产生的一个序列图：:
张三李四王五你好！李四, 最近怎么样?你最近怎么样，王五？我很好，谢谢!我很好，谢谢!李四想了很长时间,文字太长了不适合放在一行.打量着王五...很好... 王五, 你怎么样?张三李四王五
这将产生一个流程图。:
链接

长方形
圆
圆角长方形
菱形
关于Mermaid语法，参考[这儿](https://mermaidjs.github.io/),
FLowchart流程图
我们依旧会支持flowchart的流程图：
Created with Raphaël 2.2.0开始我的操作确认？结束yesno
关于Flowchart流程图语法，参考[这儿](http://adrai.github.io/flowchart.js/).
导出与导入
导出
如果你想尝试使用此编辑器, 你可以在此篇文章任意编辑。当你完成了一篇文章的写作, 在上方工具栏找到文章导出，生成一个.md文件或者.html文件进行本地保存。
导入
如果你想加载一篇你写过的.md文件或者.html文件，在上方工具栏可以选择导入功能进行对应扩展名的文件导入，
继续你的创作。
---
[mermaid语法说明](https://mermaidjs.github.io/)[↩︎](#fnref1)
注脚的解释[↩︎](#fnref2)


