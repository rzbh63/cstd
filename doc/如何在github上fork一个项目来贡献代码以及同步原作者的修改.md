# 如何在github上fork一个项目来贡献代码以及同步原作者的修改

作为一个IT人，通过github进行学习是最快的成长手段。我们可以浏览别人的优秀代码。但只看不动手还是成长得很慢，因此为别人贡献代码才是明智之举。比如我们看下片看，许多大片都是由字幕组免费翻译压制的。为什么他们要这样做呢？因为他们都是聪明的大学生，为了提高听力水平，提高笔译水平才这样干的！中国人都是非常实务的！因此贡献代码，参与开源项目都是有益无害的！好了，让我们开始吧！

# 如何贡献自己的力量

首先你总得有**自己的github帐号**吧，注册一个，非常简单，只需用户名，邮箱，密码，邮箱只是用来找回密码的，不做验证。因此注册后立即能用！比如我现在新注册一个叫JsLouvre的示范帐号。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/1.jpg)

然后搜索我的项目——[mass Framework](https://github.com/RubyLouvre/mass-Framework)

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/2.jpg)
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/3.jpg)

 



第一个就是，点击进入项目，然后点上方的Fork按钮，这就拷贝一份[我的项目](https://github.com/RubyLouvre/mass-Framework)的副本作为**你自己的项目**

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/4.jpg)
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/5.jpg)

 



创建成功！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/6.jpg)
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/7.jpg)

 



接着就是修改代码了，这要在自己发现真的存在漏洞或有什么改进之处才要动手啊！不能想改就改。要通读你要改的那一部分，必要时通读全框架。因此新手们最好找国内高手的框架进行学习， 一来中文注释比较亲切，二来也方便接下来的交流。通常我们在clone git到本地进行修改的，这又涉及另外一些工具与命令的学习。不过，github完全允许你在线上进行修改，提交，合并。

比如你发现data模块的注释与mass模块的不一样，要统一合并，将函数外的注释移到里头。（更有意义的方式是，打开<http://www.jshint.com/>，就能发现许多小问题。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/8.jpg) 
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/9.jpg)

 



变成编辑状态，不过不太好用。大家有条件一定要学学如何使用TortoiseGit或Sublime Text 2下载github项目到本地，进行修改，提交，pull request啊！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/10.jpg) 
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/11.jpg)

 



请认真写下你的修改日志，方便原框架作者查阅。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/12.jpg)

修改成功后的样子：

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/13.jpg)

接着下来一步非常重要，就是提交你的修改给原作者。点击上方的pull request按钮！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/14.jpg) 
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/15.jpg)

 



红色区域为原作者的项目，你要贡献的目标；亮蓝色区域为你的项目；黑色处填写标题与必须描述；点击右下方绿区域的按扭进行提交！

------

------

接着下来就是框架作者的事儿了，我会在自己的项目看到你们的提交。万一以后你们有幸被别人贡献代码，也做这活儿。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/16.jpg)

点击中间那个大大的pull request(2)的按钮到另一页面，中间有个被鲜绿色高亮的merge pull request按钮。作者查看你的修改，觉得可以就点它进行合并。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/17.jpg)
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/18.jpg)

 



最后你们就会在原项目上看到自己贡献的代码！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/19.jpg)

------

# 如何让自己的项目与原作者的项目保持同步！

我经常看到许多人，只会fork一次，提交过一次修改就不知怎么办了！因此原框架作者是非常勤奋的，一天会提交N次，一个星期后许多文件都改动过了，而那些代码贡献者不可能一个个跟着修改。 因此我们还是用到上方的pull request按钮。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/20.jpg) 
![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/21.jpg)

 



这次是把自己的项目放到左边，原框架作者放到右边，在选择过程中，你会发现原框架作者有许多贡献者的。这里我希望大家一定要浏览*Commits与Files Changed*进行学习!这也是github最大的价值所在！把握别人对代码的改进，最能提高我们编码水平。这里面会涉及大量的编码技巧！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/22.jpg)

补上必要说明，然后点击下方send pull request按钮。

很快你就看到评论区最下方有个鲜绿色高亮的按钮，继续点就是。

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/23.jpg)

继续让你确认，没问题就继续点！

![img](https://raw.github.com/RubyLouvre/mass-Framework/master/course/24.jpg)

这样就同步成功！！！！！！