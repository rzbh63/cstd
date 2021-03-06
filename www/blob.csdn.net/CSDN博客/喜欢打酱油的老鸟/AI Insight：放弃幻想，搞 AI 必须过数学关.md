
# AI Insight：放弃幻想，搞 AI 必须过数学关 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月09日 17:09:00[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：409标签：[AI																](https://so.csdn.net/so/search/s.do?q=AI&t=blog)[数学																](https://so.csdn.net/so/search/s.do?q=数学&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=AI&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


作者 | 子白
从2012年“大数据”概念兴起到2016年人工智能大热，已经四五年时间了，该看的热闹看到了，该爆炒的话题炒够了，该沉淀的也沉下来了。现在越来越多的人已经放下质疑，相信这一届 AI 确实靠谱，该找找门道了。最近好多关注 AI 的公众号都发表了文章谈数学学习的问题，这就真的上道了。如果你是技术人员，特别是程序员，想成为 AI 工程师，那么只要你稍微花点时间研究一下 AI 的门道，就会意识到，主要的拦路虎是数学。如果你看到有人说什么不懂数学也能搞 AI，那你可能要警惕，因为这很可能是一种误导。
一定要认识到一点，数据科学、机器学习、DT、AI，whatever，不管你叫什么，**它不是 IT 行业中又一项新技术，而是一个新行业。**马云说 IT 时代结束了，DT 时代开始了，有点标题党，但大意是对的。
我们要充分认识到，这不是一次 paradigm shift（范式转换）。游戏没变，规则变了，那是 paradigm shift，连游戏都变了，那就是 game shift 了。比如说，对于商业来说，互联网是一次 game shift，但对于 IT 行业来说，从企业网 C/S 架构走向互联网 Web 架构，平台变了，工具变了，协议变了，模式变了，赚钱的规则也变了，但事情的本质没变，还是在网络上构造商业管理和通讯系统。所以面向对象是 paradigm shift，敏捷是 paradigm shift，Web 是 paradigm shift，移动互联网开发是 paradigm shift。而 AI 不是 paradigm shift，是 game shift。
新行业有新玩法。AI 跟 IT 技术的主要差别是什么？**简单的说，就是 AI 对数学要求较高，对编程要求较低，而 IT 开发对于编程要求高，特别是对编程经验要求高，但对数学要求不高。**
![AI Insight：放弃幻想，搞 AI 必须过数学关](http://p1.pstatp.com/large/pgc-image/382371a7361340269d23c1e3e51c3dae)
“大学工科数学三板斧”
说一个秘密，全世界的程序员两千万，大多数手持大学学历，外行人以为他们的数学都得要好，但其实大部分人对于大学数学三板斧微积分、线性代数和概率统计的记忆已经基本清零。除了专业从事有关领域的底层技术开发人员，大多数程序员、架构师、软件工程师、项目经理甚至相当多的 CTO，对于什么方向导数和梯度、函数项级数的一致收敛、魏尔斯特拉斯定理、拉格朗日乘数法、矩阵特征值与特征向量、矩阵二次型、多维随机变量条件分布之类的大学数学基本概念，实际已经陌生如听天书了。这无可指责，因为这些东西在实际产品设计、系统开发和运维当中根本用不着。他们必须将主要精力投入到“广义计算机语言”的学习当中，适应所谓“热门驱动开发（Hype Driven-Development）”，包括一门又一门最新最火的编程语言，一个又一个网络协议，成百上千的 API 用法，各种框架、平台、数据库、操作系统和浏览器的方言、怪癖甚至陷阱，还有设计模式、架构模式、版本管理、build system、container、DevOps，等等等等，内容实在是太繁杂了，变化实在是太多了，风潮转换实在是太快了，耗费精力实在是太大了。哪有时间搞没用的东西？连算法都只是在准备跳槽刷题时才需要认真对待的，数学？一边凉快去吧。
![AI Insight：放弃幻想，搞 AI 必须过数学关](http://p9.pstatp.com/large/pgc-image/9ba81a8cac234ede98f9feab0f2e7153)
“三板斧之后”
然而搞 AI 就完全是另一回事，关键是数学。大学工科数学？那只是一个基础，想再搞懂 AI 的那些基本的算法和思想，你不但需要把这三板斧基础打扎实，而且还需要学习矩阵分析和凸优化。这只是一个入门，想要在这个领域有点建树，你可能还需要在随机过程、泛函分析、微分流形、数值分析和优化理论等领域进一步深造。想要搞机器人学或自动驾驶？还得研究微分方程、运动学、动力学。**毫不夸张的说，在 AI 学习的入门阶段，数学是主要的攻坚对象，任何胸有大志的 AI 学习者都不要幻想绕过数学。**
可能正是因为数学是一只拦路虎，最近我们看到了不少鸡汤文章出来安慰说数学不好也可以学 AI。这些文章，大多只是标题党，你以为他给你指了一条方便法门，其实打开一看，只不过是舒缓你的畏难情绪，推荐几本寓教于乐的数学书，给你做做心理按摩，**本质上还是一句话：想搞 AI，就得把数学拿下。**但是很多人现在看文章只看标题啊，所以就产生了幻想，是不是不学数学也能搞 AI呢？有没有可能把数学都交给封装好的框架和程序库，我只做调用和调试的工作呢？未来会不会达到 AI 工具化、AI 工具傻瓜化的程度呢？
**AI 工具化可能性是有的，而且是必然，趋势很明显。**比如著名的机器学习框架 scikit-learn，把多种机器学习算法包装成他们所说的“黑盒子”，使用时只需要选择算法，设置合适的参数进行初始化，然后灌数据训练模型，得到模型以后就可以进行分析和预测了，大多数复杂性都被包装在后面。而现在当红的深度学习，这个趋势更加明显。在张重生所著的《深度学习——原理与应用实践》一书中，作者援引周志华教授的一段话，吐槽当前深度学习现状说：
> “有点幽默，但很朴实，深度学习现在差不多就是民工活，调来调去，刷来刷去。文章发得飞快，貌似热闹，但有多少是能沉淀下来的实质真进展、真原理、真算法、真技术，又有多少是换个数据就不靠谱了的蒙事撞大运？既缺乏清澈干净的内在美感，又不致力于去伪存真、正本清源，只图热闹好看，迟早把 arXiv 变成废纸堆。”
通过这段吐槽，我们侧面了解到深度学习这个领域当前的局面，极少人做核心理论研究，一小部分人开发框架和程序库，大多数人只是把黑盒子拿来刷参数、攒论文。我们从正面的意义上来理解这件事情，这恰恰表明深度学习的工具化做得比较好，初步形成了一个金字塔模型。专家们批评的，只不过是现在很多金字塔底层的“深度学习民工”在利用 AI 的热潮和外界对这个领域的不了解沽名钓誉，导致某些 AI 人士所说的“深度学习泡沫”。批评归批评，这个人员分布结构是合理的，AI 工具化是有效的。
所以，是的，机器学习和 AI 领域的软件工具也会逐渐走向成熟，也会像黑盒子一样把很多复杂性包装起来，把很多今天需要较深数学理解才能做到的事情简化为调参。但是这是否就能让我们得出结论说，未来的 AI 工程师可以不懂数学呢？
当然不能。**首先，数学不过关的人在 AI 这个圈子里是无法参与交流的，**或者说得直白一点，是被人瞧不起的，根本无法被接纳成为AI 圈子里的人。**其次，数学不过关而只能调参数的人，实践当中并不好用。**我接触过几个 AI 创业企业，他们都疯狂的缺人，但是对于不懂数学只会使用工具的“人才”，他们却大门紧闭。什么原因？沟通效率低，出活慢，遇到问题无法解决，一句话，“不好用”。**第三，即便是调参这件事情本身，懂不懂数学也是有很大差别的。**因为调参是一个跟自己较劲的过程，不断地划分训练数据集和验证数据集，调整参数，追求更高的准确率，又要防范过拟合，一遍又一遍，本身就是非常繁琐和枯燥的工作。如果你数学不过关，对参数的意义和相关关系理解不深，方向感不强，只能半随机的去刷参数，那么工作的单调性和烦琐性还会大大上升，很难坚持到做出好的模型来。
就这三点，决定了不懂数学的“AI框架调参工”不会是一个令人向往的工作。我绝不相信一个年轻人满怀 AI 梦想走进这个行业，会满足于做一个“以其昏昏使人昭昭”的调参工。
**所以，结论已经很清楚，想搞 AI，数学必须过关。**一切与这个结论有冲突的说法，都是耍流氓。
当然，**请注意我在这里说的是“数学必须过关”，**只要求过关，并不是要求你为了搞 AI 而先要变成一个数学家，那既不现实也不必要。为什么只要求过关呢？为什么不是数学越棒越好呢？因为 AI 要在数学上形成重大的突破，可能是十年甚至几十年一遇的。今天机器学习中有效的数学方法，绝大多数都是几十年前做出来的成果。因此做 AI 工程，重点是充分理解、熟练掌握和运用这些成熟的数学工具，尊重分工，把数学领域的创新交给数学家和应用数学家。
机器学习学者张志华教授曾经说过：“搞好机器学习，关键是数学，但你又不能把机器学习变成搞数学，那样就漫无边际了。”数学浩瀚如海，神灵通天，我们还是要敬畏数学之神，不要毫无目标、不计成本的扩大数学的钻研，而是要把握好度。**对于绝大多数 AI 工程师来说，还是应该以机器学习为主导，对于其中涉及的数学知识形成理解，打牢基础，突出重点，适度拓宽，这就算过关了。**以后根据主攻方向，随用随学，急用先学，这样就可以了。
**数学过关以后，还得懂编程语言、系统架构、数据库、异构平台上的高性能计算，要成为一个优秀的 AI 工程师，这些都得通。**吴恩达就说过，深度学习的前沿是高性能计算。因为数学的进展是可遇而不可求的，而软硬件的进步是线性的甚至指数的，是一趟快车。你想冲在前沿，得搭上这趟快车。如果你只是数学好，编程和工程化能力不行，所能做的事情就比较有限，需要一个强有力的工程团队来支持你。实际上，我就知道一些学院派的 AI 人士，他们的数学背景很好，但是编程玩得不溜，下面的学生又不给力，只能封闭在“数学—Matlab—论文” 这个三角里，对他们来说，编程成了一个障碍。
**但是对于一个有一定经验的程序员而言，编程是优势。**前不久，我向一个在国内 AI 界有点名气的新锐 AI 科学家了解情况，他们手上的深度学习项目使用 Python 和 C++ 语言开发，都是基于开源的深度学习框架，但自己的工程师也要做相当程度的调整和改进。那么他们对于工程师编程语言的掌握程度有多高的要求呢？Python 语言不需要自己设计 class，就更不用提什么 decorator, metaclass, asyncio 了，C++ 只要求看懂框架代码当中核心的部分，只有很少的人需要去修改 C++ 代码。这里头真正有点挑战的，是要熟练掌握 Python 标准库和一些流行包中的 API 用法，快速完成数值计算、数据清洗、数据可视化等工作，这是需要时间去熟悉和掌握的。但有经验的人知道，这些可以边做边熟悉，不是什么硬约束。如果你在“城头变幻大王旗”的开发界混过几年，转行搞 AI，你的编程能力绝对是下山猛虎，完全不必担心。你真正要过的，就是数学关。只要你过了数学这关，后面将数学理解、工具框架、数据资源和应用结合起来形成正向循环，就可以一步一步的攀爬 AI 的高峰了。
[https://www.toutiao.com/a6662649835272798728/](https://www.toutiao.com/a6662649835272798728/)

