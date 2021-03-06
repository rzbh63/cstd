
# 高效使用Google - saltriver的专栏 - CSDN博客


2016年12月03日 22:47:36[saltriver](https://me.csdn.net/saltriver)阅读数：573


如果说互联网是人类有史以来最伟大的发明，那么搜索引擎就是第二伟大的发明。互联网不单是一项发明，更是一项伟大的成就，最大程度的消灭了人与人之间信息的不平等。
要知道，书籍是古代传递信息和知识的主要方式，拥有书籍是古代有钱人才有的特权。印刷术出现之前，抄书非常盛行，仅纸价就不能为一般贫寒人士所接受，所以南宋叶梦得《石林燕语》对唐以前的书价总结了一个“贵”字：“唐以前，凡书籍写本，未有模印之法，人以藏书为贵”。一卷（4000-6000字）的价格在1000文左右（即一两银子，折合米价约现代2000-4000元）。即使在宋代印刷术出现后，书籍价格下降到原来的1/10，也有100文/卷。
![古代书籍](https://img-blog.csdn.net/20161203221945309)
有了互联网，人人都可以接触到几乎无限多的信息，但又带来了另一个问题，如何高效找到需要的信息，这就需要善用搜索引擎。
搜索引擎人人都会用，但用不用的好则是另一回事。从某种程度上说，很多人（据观察，这个比例不低）都没有真正掌握搜索引擎的使用方法。要有效、高效的使用搜索引擎，如下几个核心方法必须掌握：
#### 一、
#### 完整匹配
在Google的输入框中，如果你输入的是：python AI machine learning。那么返回的搜索结果中也许这4个单词都存在，也许有其中3个存在，也许其中2个。
搜索输入： python AI machine learning
![无全部匹配](https://img-blog.csdn.net/20161203222223048)
---
如果你只想要包含所有4个单词的结果，你需要在搜索词前后加上引号“”，从而返回完整匹配的结果。
搜索输入： “python AI machine learning”
![全部匹配](https://img-blog.csdn.net/20161203222306064)
大家注意看上面2张图片上标黄部分的搜索结果数量，一个是1930，一个是852000，差了400多倍。
#### 二、
#### 筛选匹配
当我们搜索的结果在某个领域方向应用的较多，搜索结果中肯定有大量体现，但我们实际上不想要这方面的内容，我们可以使用筛选符号“-”：A -B，搜索A，但不存在B的信息，**注意：这里“-”之前一定要有空格**
搜索输入： “python AI” -machine learning
包含python AI但不包含machine learning的网页。
![筛选匹配正确](https://img-blog.csdn.net/20161203222712007)
#### 三、
#### 站内搜索
当我们觉得某个网站具有权威性，可信度也最高的情况下，可以指定只返回该网站下的内容。如stackoverflow。
搜索输入： “python AI” site:stackoverflow.com
返回的结果都是该网站下的链接。
![站内搜索](https://img-blog.csdn.net/20161203223005482)
#### 四、
#### 文档搜索
当我们需要搜索特定格式的文件时，使用“filetype:pdf/doc/ppt…”。例如，我们想找关于scikit-learn的pdf文档资料。当然想要其他格式doc/ppt的都可以。
搜索输入： “scikit-learn” filetype:pdf
![filetype](https://img-blog.csdn.net/20161203223139450)
#### 五、
#### 最后，Google还是百度？
2个都可以用。但从搜索内容和质量上看，Google都比百度要强大很多。不客气的说，对于搞技术的人，如果连Google都不用，算不上一个合格的职业人士。
当然，国内由于某些原因，是不能直接访问Google的。很多人尝试了各种方法，费了很多功夫，折腾很久但效果经常不好。个人建议是花点钱找个代理，毕竟免费的东西其实是最贵的，耗费了大量时间精力，影响了学习工作的注意力，得不偿失。

