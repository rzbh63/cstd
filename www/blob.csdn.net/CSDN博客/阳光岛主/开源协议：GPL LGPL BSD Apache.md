
# 开源协议：GPL-LGPL-BSD-Apache - 阳光岛主 - CSDN博客

2013年09月01日 07:38:01[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3145个人分类：[Learn																](https://blog.csdn.net/sunboy_2050/article/category/632423)



越来越多的开发者与设计者希望将自己的产品开源，以便其他人可以在他们的代码基础上做更多事，开源社区也因此充满生机。在我们所能想到的应用领域，都有开源软件存在（例如开源CMS[WordPress](http://wordpress.org/)、[Drupal](https://drupal.org/)）。然而很多人对开源许可并不了解，本文介绍开源领域常用的几种许可协议以及它们之间的区别。
**什么是许可协议？**
什么是许可，当你为你的产品签发许可，你是在出让自己的权利，不过，你仍然拥有版权和专利（如果申请了的话），许可的目的是，向使用你产品的人提供一定的权限。
不管产品是免费向公众分发，还是出售，制定一份许可协议非常有用，否则，对于前者，你相当于放弃了自己所有的权利，任何人都没有义务表明你的原始作者身份，对于后者，你将不得不花费比开发更多的精力用来逐个处理用户的授权问题。
而开源许可协议（[open-source license](http://en.wikipedia.org/wiki/Open-source_license)）使这些事情变得简单，开发者很容易向一个项目贡献自己的代码，它还可以保护你原始作者的身份，使你至少获得认可，开源许可协议还可以阻止其它人将某个产品据为 己有。以下是开源界的 5 大许可协议。

**GNU GPL**
GNU General Public Licence([GPL](http://opensource.org/licenses/gpl-2.0.php)) 有可能是开源界最常用的许可模式。GPL保证了所有开发者的权利，同时为使用者提供了足够的复制，分发，修改的权利：
1）可自由复制
你可以将软件复制到你的电脑，你客户的电脑，或者任何地方。复制份数没有任何限制。
2）可自由分发
在你的网站提供下载，拷贝到U盘送人，或者将源代码打印出来从窗户扔出去（环保起见，请别这样做）。
3）可以用来盈利
你可以在分发软件的时候收费，但你必须在收费前向你的客户提供该软件的 GNU GPL 许可协议，以便让他们知道，他们可以从别的渠道免费得到这份软件，以及你收费的理由。
4）可自由修改
如果你想添加或删除某个功能，没问题，如果你想在别的项目中使用部分代码，也没问题，唯一的要求是，使用了这段代码的项目也必须使用 GPL 协议。
需要注意的是，分发的时候，需要明确提供源代码和二进制文件，另外，用于某些程序的某些协议有一些问题和限制，你可以看一下[@PierreJoye ](http://www.twitter.com/PierreJoye)写的[Practical Guide to GPL Compliance ](http://www.softwarefreedom.org/resources/2008/compliance-guide.html)一文。使用 GPL 协议，你必须在源代码代码中包含相应信息，以及协议本身。

**GNU LGPL**
GNU 还有另外一种协议，叫做 LGPL （[Lesser General Public Licence ](http://www.opensource.org/licenses/lgpl-2.1.php)），它对产品所保留的权利比 GPL 少，总的来说，LGPL 适合那些用于非 GPL 或非开源产品的开源类库或框架。因为 GPL 要求，使用了 GPL 代码的产品必须也使用 GPL 协议，开发者不允许将 GPL 代码用于商业产品。LGPL 绕过了这一限制。
**BSD**
BSD 在软件分发方面的限制比别的开源协议（如 GNU GPL）要少。该协议有多种版本，最主要的版本有两个，新 BSD 协议与简单 BSD 协议，这两种协议经过修正，都和 GPL 兼容，并为开源组织所认可。
新 BSD 协议（3条款协议）在软件分发方面，除需要包含一份版权提示和免责声明之外，没有任何限制。另外，该协议还禁止拿开发者的名义为衍生产品背书，但简单 BSD 协议删除了这一条款。

**MIT**
[MIT 协议](http://www.opensource.org/licenses/mit-license.php)可能是几大开源协议中最宽松的一个，核心条款是：
该软件及其相关文档对所有人免费，可以任意处置，包括使用，复制，修改，合并，发表，分发，再授权，或者销售。唯一的限制是，软件中必须包含上述版权和许 可提示。
这意味着：
1）你可以自由使用，复制，修改，可以用于自己的项目。
2）可以免费分发或用来盈利。
3）唯一的限制是必须包含许可声明。
4）MIT 协议是所有开源许可中最宽松的一个，除了必须包含许可声明外，再无任何限制。

**Apache**
Apache 协议 2.0 和别的开源协议相比，除了为用户提供版权许可之外，还有专利许可，对于那些涉及专利内容的开发者而言，该协议最适合（[这里有一篇文章阐述这个问题 ](http://www.howstuffworks.com/question492.htm)）。
Apache 协议还有以下需要说明的地方:
1）永久权利：一旦被授权，永久拥有。
2）全球范围的权利：在一个国家获得授权，适用于所有国家。假如你在美国，许可是从印度授权的，也没有问题。
3）授权免费，且无版税：前期，后期均无任何费用。
4）授权无排他性：任何人都可以获得授权
5）授权不可撤消：一旦获得授权，没有任何人可以取消。比如，你基于该产品代码开发了衍生产品，你不用担心会在某一天被禁止使用该代码。

分发代码方面包含一些要求，主要是，要在声明中对参与开发的人给予认可并包含一份许可协议原文。
Creative Commons
Creative Commons (CC) 并非严格意义上的开源许可，它主要用于设计。Creative Commons 有多种协议，每种都提供了相应授权模式，CC 协议主要包含 4 种基本形式：
署名权
必须为原始作者署名，然后才可以修改，分发，复制。
保持一致
作品同样可以在 CC 协议基础上修改，分发，复制。
非商业
作品可以被修改，分发，复制，但不能用于商业用途。但商业的定义有些模糊，比如，有的人认为非商业用途指的是不能销售，有的认为是甚至不能放在有广告的网 站，也有人认为非商业的意思是非盈利。
不能衍生新作品
你可以复制，分发，但不能修改，也不能以此为基础创作自己的作品。
**总结**
这些许可形式可以结合起来用，其中最严厉的组合是“署名，非商用，不能衍生新作品”，意味着，你可以分享作品，但不能改动或以此盈利，而且必须为原作者署名。在这种许可模式下，原始作者对作品还拥有完全的控制权，而最宽松的组合是“署名”，意味着，只要为原始作者署名了，就可以自由处置。
延伸资源
[Creative Commons Licenses ](http://creativecommons.org/about/licenses)
Creative Common 许可的官方条款与解释
[Open Source Initiative ](http://www.opensource.org/licenses/alphabetical)
包含各种开源协议资源
[Open Source Licenses Comparison ](http://developer.kde.org/documentation/licensing/licenses_summary.html)
对主流开源协议的对比
[Understanding Open-Source Licensing ](http://openacs.org/about/licensing/open-source-licensing)
开源协议详解
[HOW TO: Pick an Open Source License ](http://blogs.zdnet.com/Burnette/?p=130)
如何选择不同的开源协议

之前对开源软件的使用在授权上存有顾虑，现在比较清楚了~~~

**参考推荐：**
[开源许可协议：GPL/LGPL/BSD/MIT/Apache/CC](http://hi.baidu.com/sunboy_2050/item/7e603731472187fea98428f2)



