
# GDI画图的几种方式(小游戏不闪烁) - .NET博文收藏 - CSDN博客


2008年06月10日 12:48:00[hejishan](https://me.csdn.net/hejishan)阅读数：1058


1 画图方式:
1.1 使用临时DC,先将要画的界面全部画到临时DC,到最后才全部帖回原DC,这样就不会出现闪烁.
1.2 创建的GDI对象要删除,不然会造成泄露.
将新对象选进DC,一般要保留旧的对象,然后用完新对象，把老对象选回DC.
一般Create*** 函数多要记得Delete***
GetDC         <-->    ReleaseDC
还有就是CPaintDC dc(this),获取的DC,不需要你去删除,并且CPaintDC已经封装了
BeginPaint和EndPaint组合
2 更新方式:
2.1 全部画图全部更新
这种方式主要用在控件上(小范围),画的东西并不多,可以采用本方式
2.2 全部画图局部更新
此方式用的比较多，主要还是方便，但是CPU占用相对来说还是高了一点
我的一个麻将采用这种方式，结果占用CPU一般保持在5%(5%-10%)[测试环境:笔记本无显卡]
2.3 局部画图局部更新
这个方法是最好的，不过代码相对复杂一点,如果类封装的好，也没什么问题.
我的纸牌和一些飞机小游戏游戏采用这类方式,CPU占用一般只有1-3%[测试环境:笔记本无显卡]



