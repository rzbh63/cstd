
# Razor视图中的 @helper - 追求卓越,做到专业 - CSDN博客


2016年04月24日 21:53:55[Waldenz](https://me.csdn.net/enter89)阅读数：669


ASP.NET MVC 3支持一项名为“Razor”的新视图引擎选项（除了继续支持/加强现有的.aspx视图引擎外）。当编写一个视图模板时，Razor将所需的字符和击键数减少到最小，并保证一个快速、通畅的编码工作流。
与大部分模板的语法不同，在Razor的帮助下，您不需要中断代码编写，仅仅为了在HTML中标注服务器端代码块的开始和结束。代码分析器足够聪明，它能够从你的代码里推断出是否为服务器端代码。这种更加简洁、富有表现力的语法更加干净，输入也更快速，有趣。
今天的博文涵盖了Razor的一项很多人都不知道的功能——利用@helper语法定义可重用的帮助器方法。
简单的 @helper 方法应用场景
Razor中的@helper语法让您能够轻松创建可重用的帮助器方法，此方法可以在您的视图模板中封装输出功能。他们使代码能更好地重用，也使代码更具有可读性。让我们看一个超级简单的应用场景，它展示了@helper语法是怎样被使用的。
在我们定义@helper方法之前的代码
让我们看一个简单的产品列表应用场景。在此场景中，我们列出产品明细并输出产品的价格或是单词“免费！”——如果这个产品不花费任何成本的话：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516462143.png)
以上代码非常直截了当，而且Razor的语法使得在HTML里能简单地集成服务器端C\#代码。
然而，一个有点混乱的地方是价格的if/else逻辑。我们可能在站点的其他位置输出价格（或者在同一页面上），而复制以上逻辑很容易出错且难以维护。类似的应用场景是使用@helper语法提取和重构成为帮助器方法的首选考虑。
使用@helper语法重构以上样例
让我们提取价格输出逻辑，并将其封装在一个我们将命名为“DisplayPrice”的帮助器方法内。我们可以通过重写以下代码样例来实现此操作：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516463932.png)
我们已经使用上述@helper语法来定义名为“DisplayPrice”的可重用帮助器方法。就像标准C\#/VB方法一样，它可以包含任意数量的参数（您也可以定义参数为空或可选参数）。不过，与标准C\#/VB方法不同的是，@helper方法可以同时包含内容和代码并支持其中的完整Razor语法——这使得定义和封装呈现/格式化帮助器方法变得非常简单。
![](http://pic001.cnblogs.com/images/2011/24634/2011052516464996.png)
您可以像调用一个标准的C\#或VB方法一样，调用@helper方法：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516465841.png)
当调用该方法时，Visual Studio会提供智能感知代码：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516470754.png)
在多视图模式中重用@helper
在上面的实例中，我们在相同的视图模板中将@helper方法定义为调用它的代码。或者，我们可以将@helper方法定义在视图模板外，并保证其在项目的所有视图模板中可重复使用。
您可以在.cshtml/.vbhtml保存我们的@helper方法，并把这个文件放在项目根目录下创建的\App_Code目录下例如，我在\App_Code文件夹中创建了一个“ScottGu.cshtml”文件，并且在文件中定义了2个单独的帮助器方法（在每个文件中您可以有任意数量的帮助器方法）：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516472951.png)
一旦我们的帮助器定义在应用程序级别，我们就可以在应用程序的任何视图模板中使用它们。
在上面的\App_Code文件夹中的ScottGu.cshtml会逻辑编译为一个称为“ScottGu”的类。这个类中包含了“DisplayPrice” 和 “AnotherHelper”的静态成员。我们可以使用以下代码重写前面的示例来调用它：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516474325.png)
当像如下方法调用应用程序级别帮助器时，Visual Studio将会提供智能感知代码：
![](http://pic001.cnblogs.com/images/2011/24634/2011052516475246.png)
5月15日更新：有一些人指出的一个问题是，当一个@helper保存在\app_code目录中时，默认情况下您不能访问其中的ASP.NET MVC Html帮助器方法。（例如Html.ActionLink(), Html.TextBox()等等)。而当它们定义在与视图相同的文件夹中，您是可以访问内置HTML帮助器方法的。当帮助器位于\app_code目录下时，确实当下是不支持内置HTML帮助器方法的访问的——我们将在下次发布中添加此功能。Paul Stovall有一个很好的帮助器类，您可以同时访问和使用它和您在\app_code目录下定义的@helper方法中的内置Html方法。请从[这里](http://stackoverflow.com/questions/4710853/using-mvc-htmlhelper-extensions-from-razor-declarative-views)了解更多关于如何使用的信息。
总结
Razor的@helper语法提供了一种简便的方法来将呈现功能封装到帮助方法中去。您可以在单个视图模板或整个项目的所有视图模板中重用它。
您可以使用此功能来编写更加干净、更易维护的代码。

