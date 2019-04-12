# reStructuredText(.rst)语法规则快速入门

这几天写了个[Python](http://blog.useasp.net/tags/Python)的模块，用Markdown写个个README，传到GitHub，感觉效果还不错，就难抑冲动，打了个包，也想放到PyPI上，结果放上去，发现README变成了源代码。一查，才发现PyPI竟然不支持Markdown格式的README文件，好像支持的README要reStructuredText格式的，对菜鸟的我来说这是个坑啊，好不容易在Emacs下用Markdown用的有点熟路了，结果发现却不被支持。只好重新看看reStructuredText的语法了，因此，也就有了此篇reStructuredText语法快速入门。

先文绉绉的来一段[reStructuredText](http://blog.useasp.net/tags/reStructuredText)的介绍吧：

reStructuredText是一种轻量级的文本标记语言，直译为：重构建的文本，为Python中Docutils项目的一部分。其一般保存的文件以.rst为后缀。在必要的时候，.rst文件可以被转化成PDF或者HTML格式，也可以有Sphinx转化为LaTex,man等格式，现在被广泛的用于程序的文档撰写。

 好了，时间无多，直接正题：

reStructuredText大致分章节，段落，块和列表这几种内容。而在这其中reStructuredText最主要用得到的标记也就是：

- [标题](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#title)
- [段落](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#paragraph)
- [列表](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#list)
- [表格](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#table)
- [块（如：代码块）](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#block)
- [样式](http://blog.useasp.net/archive/2014/09/05/rst-file-restructuredtext-markup-syntax-quikstart.aspx#style)

下面一一介绍：

 

 

**标题（Title）**

来看看标题的实例：

```
`===================``这就是一个标题``===================` `----------------``这也是一个章节标题``----------------`
```

怎么样，看起来不难吧，你只要按这个写法，就能被reStructuredText认识，并被解释为章节标题。reStructuredText可用于作为标题修饰的字符有很多很多：

```
! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
```

只要你想，上面的任意一个都可以用来作为标题的修饰符，当然，reStructuredText也是有推荐的，它推荐下面这些字符：

```
= - ` : . ' " ~ ^ _ * + #
```

这些字符是上面一堆字符中稍微看起来不会那么奇怪的一部分，当然，个人建议不要那么花哨，尽量用这两个中的一个：

```
= -
```

上面实例的写法也许有点复杂，.rst文件中，你还可以只给出下半部分的字符即可：

```
`这个标题和上面的一样``===================`
```

TIPS：作为修饰的字符长度要大于等于文字长度。另外，标题是能够嵌套的。

 

**段落（Paragraphs）**

段落一般隶属于某个章节中，是一块左对齐并且没有其他元素体标记的块。在.rst文件中，段落和其他内容的分割是靠空行来完成，如果段落相较于其他的段落有缩进，reStructuredText会解析为引用段落，样式上有些不同。

```
`这里是一段reStructuredText的内容，它可以很长很长。。。。最后，末尾留出空行表示是本段落的结束即可。`  `这里是另外一段reStructuredText的内容，这段内容和上一段之间，乃至后面的其他内容之间都要留出空行进行分割。` `    ``这个也是段落，当时由于缩进了，会变成引用段落。显示和直接的段落有点不同`
```

 

**列表(List)**

列表在HTML中被分为两种，一个是有序列表（Enumerated Lists），一种是无序列表（Bullet Lists），在reStructuredText中，我们也能找到这两种列表，还有一种称为定义列表（Definition Lists），这和HTML中的DL一样，在.[rst文件](http://blog.useasp.net/tags/rst%E6%96%87%E4%BB%B6)中可以支持嵌套列表。

*无序列表*要求文本块是以下面这些字符开始，并且后面紧跟空格，而后跟列表项的内容，其中列表项比趋势左对齐并且有与列表对应的缩进。

```
* + - • ‣ ⁃
```

还是那句话，用最常用的几个字符就好，不用那么花哨。下面是示例：

```
`- 这里是列表的第一个列表项` `- 这是第二个列表项` `- 这是第三个列表项` `  ``- 这是缩进的第一个列表项``    ``注意，这里的缩进要和当前列表项的缩进同步。` `- 第一级的第四个列表项` `- 列表项之间要用个空格来分割。`
```

*有序列表*在格式上和无序列表差不多，但是在使用的前缀修饰符上，使用的不是无序列表那种字符，而是可排序的字符，可以识别的有下面这些：

```
arabic numerals: 1, 2, 3, ... (no upper limit).
uppercase alphabet characters: A, B, C, ..., Z.
lower-case alphabet characters: a, b, c, ..., z.
uppercase Roman numerals: I, II, III, IV, ..., MMMMCMXCIX (4999).
lowercase Roman numerals: i, ii, iii, iv, ..., mmmmcmxcix (4999).
```

如果你不想使用这些，在你标明第一个条目的序号字符后，第二个开始你还可以使用"#"号来让reStructuredText自动生成需要的序号（Docutils >= 0.3.8）。

```
`1. 第一项``    ``巴拉巴拉好多内容在这里。。。` `#. 第二项` `    ``a. 第二项的第一小项` `    ``#. 第二项的第二小项` `#. 第三项`
```

 *定义列表*：每个定义列表项里面包含术语（term），分类器（classifiers，可选）， 定义（definition）。术语是一行文字或者短语，分类器跟在术语后面，用“ ： ”(空格，冒号，空格）分隔。定义是相对于术语缩进后的一个块。定义中可以包含多个段落或者其他的内容元素。术语和定义之间可以没有空行，但是在定义列表前后必须要有空行的存在。下面是示例：

```
`术语1``    ``术语1的定义` `术语 2``    ``术语2的定义,这是第一段` `    ``术语2的定义，第二段` `术语 3 : 分类器``    ``术语3的定义`  `术语 4 : 分类器1 : 分类器2``    ``术语4的定义`
```

 TIPS：在reStructuredText中，还有两种列表，一种是字段列表（Field Lists），一种是选项列表（Option Lists）。由于是rst的[语法入门教程](http://blog.useasp.net/tags/%E8%AF%AD%E6%B3%95%E6%95%99%E7%A8%8B)，这里不做深入介绍

 

 **表格(Table)**

reStructuredText提供两种表格：网格表格（Grid Tables）， 简单表格（Simple Tables）。

 *网格表*中，共使用的符号有：

```
- = | +
```

“-” 用来分隔行， “=“ 用来分隔表头和表体行，"|" 用来分隔列，而"+"用来表示行和列相交的节点，如下面的例子：

```
`+------------------------+------------+----------+----------+``| Header row, column 1   | Header 2   | Header 3 | Header 4 |``| (header rows optional) |            |          |          |``+========================+============+==========+==========+``| body row 1, column 1   | column 2   | column 3 | column 4 |``+------------------------+------------+----------+----------+``| body row 2             | Cells may span columns.          |``+------------------------+------------+---------------------+``| body row 3             | Cells may  | - Table cells       |``+------------------------+ span rows. | - contain           |``| body row 4             |            | - body elements.    |``+------------------------+------------+---------------------+` `来自docutils的帮助文档.`
```

 TIPS：表头行是可选的，如果你不需要，就可以不用"="来分割了。

 

*简单表格*：这种表格比网格表来说简单许多，一般用于简单的数据展示。其用于修饰的字符也仅两个而已：

```
= -
```

一般用"="就能完成简单表格的绘制，如果有表头，同样需要用"="将它和表体(body)内容分开，否则会被视为无表头数据。

```
`基本形式``========` ``下面这种是最简单的表格形式，当然你也可以去掉表头展示。`` `=====  =====  =======``  ``A      B    A and B``=====  =====  =======``False  False  False``True   False  False``False  True   False``True   True   True``=====  =====  =======` `表内嵌入``========` ``下面这种简单表内有列表`` `=====  =====``col 1  col 2``=====  =====``1      Second column of row 1.``2      Second column of row 2.``       ``Second line of paragraph.``3      - Second column of row 3.` `       ``- Second item in bullet``         ``list (row 3, column 2).``\      Row 4; column 1 will be empty.``=====  =====` `表头合并``========` ``表头进行分类合并`` `=====  =====  ======``   ``Inputs     Output``------------  ------``  ``A      B    A or B``=====  =====  ======``False  False  False A``True   False  True``False  True   True``True   True   True``=====  =====  ======`
```

 TIPS：列需要和"="左对齐，不然可能会导致出错；如果碰到第一列为空时，需要使用"\"来转义，不然会被视为是上一行的延续；网格表和简单表中，简单表比较适合展现简单的数据，这些数据本身不需要太复杂的展现形式，而一旦碰到需要和并单元格这类的复杂操作，可能网格表会更加适合。

表格中还有更复杂的表格形式，比如：CSV表格，列表表格。这些复杂的格式就留给有兴趣的朋友深入吧。

 

**块（Blocks）**

块在reStructuredText中的表现方式也有好几种，但是最常见的是文字块(Literal Blocks)。这种块的表达非常简单，就是在前面内容结束之后，用两个冒号" :: "(空格[Optional]，冒号，冒号）来分割，并在之后紧接着插入空行，而后放入块的内容，块内容要相对之前的内容有缩进。

```
这里是块之前的的内容。。。::

   这里是块的内容。前面有缩进，空行，和::分隔符。
    此处内容会被一直视为块内容

    空行也不能阻断块内容。。

但是，当内容像这样，不再和块内容一样缩进时，块内容就自动的结束了。
```

这是块的最简单方式，一般我们编写的代码块就是用这种方式表现（如下）， 除此之外，.rst还有引用文字块(Quoted Literal Blocks)，行块（Line Blocks），块引用（Block Quotes）等。

```
`下面是我们的测试代码：` `::` `    ``for i in [1,2,3,4,5]:``        ``print i``    ``# 代码块测试` `很简单的代码块测试。`
```

 更多的块内容，请参阅官方帮助文档。

 

**样式(Style)**

reStructuredText中支持对文本进行样式控制，比如：粗体(Strong)，斜体(Italic)，等宽字体(Monospace)，引用( interpreted text)。

```
`.. Strong Emphasis` `This is **Strong Text**. HTML tag is strong.粗体` `.. Italic, Emphasis` `This is *Emphasis* Text.这个HTML使用em， 斜体` `.. Interpreted Text` `This is `Interpreted Text`. 注意，这个HTML一般用<cite>表示` `.. Inline Literals` `This is ``Inline Literals``. HTML tag is <tt>. 等宽字体.`
```

 

来点补充，如果你需要在文档中插入超链接，那么你可以像下面这样：

```
`我这里是一个 链接_.` `.. _链接: http://blog.useasp.net`
```

这种方式要求定义链接，而后引用链接。而且链接要有空格分隔前面的文字。这种方式略嫌麻烦，你可以用更加简化的方式——个人比较推荐：

```
`这里同样是一个 `链接<http://blog.useasp.net>`_，不需要特别设置。`
```

TIPS： 我们会发现，两个处理连接的时候，都需要在链接文字前面要空格与前面进行分割，这个在英文当中比较好处理，因为单个词之间有空格，而在中文中，字之间没有空格，如果加入空格，在显示时会有空格，影响观感，为此，如果在中文中使用，需要考虑好。

到此为止，reStructuredText这个[标记语言](http://blog.useasp.net/tags/%E6%A0%87%E8%AE%B0%E8%AF%AD%E8%A8%80)的基本用法已经展现完毕，进入实战吧，骚年！

 

参考文献：

\1. [reStructuredText Markup Specification](http://docutils.sourceforge.net/docs/ref/rst/restructuredtext.html) (本文有些例子是来源于此页面)

\2. 在线reStructuredText编辑器，[编辑器1](http://rst.ninjs.org/)，[编辑器2](https://www.notex.ch/)