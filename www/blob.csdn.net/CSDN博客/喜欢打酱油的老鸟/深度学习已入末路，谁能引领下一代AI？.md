
# 深度学习已入末路，谁能引领下一代AI？ - 喜欢打酱油的老鸟 - CSDN博客


2019年04月01日 12:36:47[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：228


[https://www.toutiao.com/a6656350889747415556/](https://www.toutiao.com/a6656350889747415556/)
AI能将一根香蕉照片识别成一台烤面包机，尽管AI在训练阶段可能被展示了数千张香蕉、蛞蝓、蜗牛以及其他类似物体的照片，就像抽认卡一般，让AI反复练习，直到完全掌握分类为止。
但是，即便是成千上万次训练，这个先进的系统也很容易被搞糊涂——只需要将一张小小的贴纸放到图像的某个角落，就会改变识别结果。
上面这个被称为“对抗攻击”(adversarial attack)的例子是由Google Brain的深度学习研究人员发现的，这项研究突显了人工智能距离接近人类能力之前还有很长的路要走。
多伦多大学计算机科学家Geoffrey Hinton是深度学习的先驱之一，他说：“我最初认为对抗样本只是一个让人烦恼的问题，但我现在认为它们非常深奥。它们会告诉我们做错了什么。”
这也是AI从业者普遍的观点，研究人员中的任何一个人都能轻松说出一长串深度学习的弊端，除了易受欺骗之外，还存在严重的**低效率问题**。
比如说，让一个孩子学习认识奶牛，并不需要他的母亲说一万遍“奶牛”，人类通常只需要一两个例子就能学习新概念。但是10000遍是深度学习系统通常需要的数字。
然后是**不透明问题**。一旦深度学习系统被训练完，它是如何做出决定的就不总是那么清楚了。MIT- IBM 沃森人工智能实验室负责人、计算神经学家David Cox说：“在很多情况下，不透明问题是不可接受的，即使它得到了正确的答案。”
假设一家银行使用AI来评估你的信用等级，然后拒绝给你贷款，并且说不清楚拒绝的原因，你会觉得合理吗？
也许最重要的是，它们**缺乏常识**。深度学习系统可能是识别像素级模式的高手，但它们无法理解模式的含义，更不用说对它们进行推理了。DeepMind的AI研究员Greg Wayne表示：“我不清楚，目前的AI系统是否能够理解沙发和椅子是用来坐的。”
AI的这些弱点越来越多地引起人们的担忧，尤其是自动驾驶方面。自动驾驶汽车使用类似的深度学习系统进行导航，已经出现了几起广为人知的死亡事故。
![深度学习已入末路，谁能引领下一代AI？](http://p1.pstatp.com/large/pgc-image/fa119bdc2ea44d0a84aec5c605a3df95)
深度学习方法中的明显缺陷引起了研究人员和大众的担忧，因为自动驾驶等应用使用深度学习进行导航
“人们已经开始说，(深度学习)系统可能存在问题。”纽约大学认知科学家加里•马库斯(Gary Marcus)说，他是对深度学习最直言不讳的怀疑论者之一。他说：“在一年左右之前，一直存在一种感觉，即深度学习具有魔力。现在人们已经认识到深度学习不是魔法。”
不过，不可否认的是，深度学习是一种非常强大的工具——它使部署面部识别和语音识别等应用程序变得非常常见，而就在10年前，这些程序几乎是不可能的。
所以现在的研究人员很难想象，在这个时候深度学习会消失。
在过去的2018年，深度学习的进展似乎没有达到人们的预期，“深度学习至顶”、“深度学习已死”的论调频现。虽然目前深度学习依旧生命力旺盛，但更有可能的是，人们会修改或增强它，并培养出它的下一代接班人。
**起底深度学习：起源“大脑战争”**
今天的深度学习革命起源于上世纪80年代的“大脑战争”(brain wars)，当时两种不同人工智能方法的倡导者之间争论甚大。
一种是现在被称为“优秀的老式人工智能”(good old-fashioned AI)方法，自20世纪50年代以来一直占据这个领域的主导地位。
这种方法也被称为**符号AI**，使用数学符号来表示对象和对象之间的关系。再加上人类建立的大型知识库，这些系统被证明在医学等领域的推理和决策方面非常出色。
但到了20世纪80年代，人们也越来越清楚地认识到，符号AI在处理现实生活中的符号、概念和推理方面表现非常糟糕。
为了应对这些缺陷，一些研究人员开始提倡人工神经网络，或称**联结主义人工智能**(connectionist AI），即今天的深度学习系统的前身。
这类系统的想法是通过一个模拟节点的网络来处理信号，这个网络类似于人类大脑中的神经元。信号通过连接或链路从一个节点传递到另一个节点：类似于神经元之间的突触连接。而学习，就像在真实的大脑中一样，是一个调整“权重”问题，这个“权重”会放大或减弱每个连接所携带的信号。
在实践中，大多数网络将节点排列成一系列的层，大致类似于大脑皮层中的不同处理中心。因此，比如说，一个专门用于图像识别的网络会有一个输入节点的层，它对单个像素做出响应，就像杆状细胞和锥体细胞对照进视网膜的光线做出响应一样。一旦被激活，这些节点通过加权连接将其激活级别传播到下一级别的其他节点，这些节点组合输入信号并依次激活(或不激活)。
![深度学习已入末路，谁能引领下一代AI？](http://p3.pstatp.com/large/pgc-image/b1f462beb65041de9db410bb3a1fcb27)
这个过程一直持续到信号到达节点的输出层，其中的激活模式提供一个答案——一个断言，例如输入图像是数字“9”。如果答案错误了——假设输入的图像是“0”——那么“反向传播”算法就会沿着层反向运行，调整权重，以便下次得到更好的结果。
到20世纪80年代末，这类神经网络在处理噪声或模糊输入方面已经证明比符号AI好得多。
然而，这两种方法之间的对峙仍未得到解决，主要是因为当时能够装入计算机的AI系统非常有限。人们仍无法确切地知道这些系统能做什么。
**一切光荣得益计算力的爆发**
直到2000年，随着计算能力以数量级增大的计算机的出现，以及社交媒体网站提供大量图像、声音和其他训练数据，这种理解才开始得到发展。
首先抓住这波机会的是Hinton，他是反向传播算法的提出者之一，也是20世纪80年代联结主义的主导者。到2005年左右，他和他的学生们训练的网络不仅比以前的要大得多，而且深得多，层数从一层或两层增加到了大约六层。(如今的商业网络经常多达100层以上)
2009年，Hinton和他的两名研究生证明这种“深度学习”比任何其他已知的方法都能更好地识别语音。
![深度学习已入末路，谁能引领下一代AI？](http://p1.pstatp.com/large/pgc-image/348ec6b342a64974a2b341be8a5ae63c)
Hinton和他的学生在2009年的论文
2012年，Hinton和另外两名学生发表的实验表明，深度神经网络在识别图像方面可能比标准视觉系统要好得多，“几乎将错误率减半”。
在语音和图像识别的双重突破下，**深度学习应用的革命开始了，**研究人员改进这项技术的努力也开始了。
早期的优先任务之一是拓展深度学习系统的训练方式，DeepMind公司的Matthew Botvinick说，语音和图像识别系统都使用了所谓的监督学习，这意味着每幅图像都有一个正确的答案——比如“猫”——如果网络错了，你就要告诉它正确的答案是什么。然后，网络使用反向传播算法来改进下一个猜测。
实际上，对于大部分的现实世界任务，需要强化学习。例如，强化学习系统在玩电子游戏时能学会寻求奖励(找到宝藏)和避免惩罚(失去钱)。
强化学习深度神经网络的首次成功实现是在2015年，当时DeepMind的一个团队训练了一个网络玩经典雅达利2600街机游戏。
![深度学习已入末路，谁能引领下一代AI？](http://p3.pstatp.com/large/pgc-image/b56d83f34ebd42a48f3a8ceb728f6dcd)
2015年DeepMind发表在Nature的论文
网络在游戏过程中接收屏幕图像作为输入，在输出端是制定操作指令的层，比如如何移动操作杆。在这个游戏中，神经网络的表现与人类玩家相当，或超过了人类玩家。
然后在2016年，DeepMind的研究人员在AlphaGo中应用了同样方法的更精细版本，掌握了复杂的围棋游戏，并打败了人类围棋世界冠军李世乭。
接下来的几年，AI成为全民话题，此处不过多介绍。
**深度学习穷途末路，谁能引领下一代AI？**
不幸的是，这几个里程碑都没有解决深度学习的根本问题。例如，雅达利AI需要玩数千轮才能掌握一款大多数人类玩家可以在几分钟内学会的游戏。即便如此，网络也无法理解或解释屏幕上物体。
因此，Hinton提出的问题依然存在：**深度学习到底缺少了什么**?
也许什么都没有缺少，也许所需要的只是更多的连接，更多的层，以及更复杂的训练方法。毕竟，正如Botvinick指出的，**神经网络在数学上等同于一台通用计算机**，这意味着它们没有执行不了的计算——至少理论上是这样，只要你能找到合适的连接权重。
但是在实践中，这些可能是致命的——这也是为什么这个领域越来越多的人认为，为了解决深度学习的缺陷，需要有一些根本性的全新想法。
一种解决方案是简单地扩展训练数据的规模。例如，在2018年5月发表的一篇论文中，Botvinick所在的DeepMind小组研究了当一个网络被训练做不止一项任务时会发生什么。
他们发现，只要网络有足够的“循环”连接从后面的层向更早的层运行，它就能自动吸取它在前一个任务得到的教训，更快的学习下一个新任务。
这至少是一种人类式的“**元学习**”(meta-learning)的雏形，或者称为learning to learn，这是人类快速掌握事物能力的重要组成部分。
一种更为激进的方法是，放弃仅仅通过训练一个大型网络来解决手头的问题，而是让多个网络协同工作。
2018年6月，DeepMind团队发布了一个他们称为生成查询网络(Generative Query Network )的架构，利用两个不同的网络，在没有人工输入的复杂虚拟环境中学习。其中一个被称为“**表示网络**”(representation network)，它本质上是利用标准的图像识别学习来识别在任何给定时刻AI能看到的东西。
![深度学习已入末路，谁能引领下一代AI？](http://p1.pstatp.com/large/pgc-image/5d4a00b5529a42f4b9a279319879b228)
DeepMind在2018年6月发表在Science上的GQN研究
与此同时，**生成网络**(generation network)学习获取第一个网络的输出，并生成一种整个环境的3D模型——实际上是对AI看不到的对象和特征进行预测。例如，如果一张桌子只有三条可见的腿，那么模型将预测具有相同大小、形状和颜色的第四条腿。
Botvinick说，这些预测反过来又能让系统比使用标准的深度学习方法更快地学习。一个试图预测事物的agent会在每一个时间步上自动得到反馈，因为它可以看到自己的预测结果如何。因此，它可以不断更新自己的模型，让它们变得更好。
更让人兴奋的是，这种学习是自我监督的：研究人员不需要给环境中的任何东西贴上标签，就能让它起作用，甚至不需要提供奖励和惩罚。
**图网络前途无量，是类人智能的最佳选手吗？**
还有一种更为激进的方法是，不再要求网络对每一个问题都从头开始学习。
婴儿似乎生来就有许多固有的“**归纳偏见**”，这使他们能够以惊人的速度学会某些核心概念。到两个月大的时候，他们已经开始掌握直觉物理学的原理，包括物体存在的概念，物体倾向于沿着连续的路径移动，当两个物体接触时它们的移动路径会改变等等。
拥有这种内置的归纳偏见可能会有助于深度神经网络同样快速地学习，这就是为什么该领域的许多研究人员现在把这作为优先考虑的事情。
事实上，仅仅在过去的一两年里，这个领域就出现了一种被称为“**图网络**”(graph network)的强大潜在方法，引起了人们的极大兴趣。Botvinick说：“这些是内在地倾向于把事物表示为物体和关系的深度学习系统。”
![深度学习已入末路，谁能引领下一代AI？](http://p1.pstatp.com/large/pgc-image/daf1671d468949e58d96c82e1df969d8)
例如，某些对象，如爪子、尾巴和胡须，可能都属于一个较大的对象(猫)，并且它们之间的关系是“属于…的一部分”(a-part-of)。同样地，一个球A和一个方块B可能具有“在…旁边”(next-to)的相互关系，地球与太阳具有“围绕轨道转动”(in-orbit-around)的关系，以此类推，还有许许多多例子。这些关系都可以用一个抽象的图(graph)来表示，其中节点对应于“物体”，线对应于“关系”。
因此，**图网络是一种神经网络**，它将这样的图形作为输入(而不是原始像素或声波)，然后学习推理和预测对象及其关系如何随时间演变。
图网络方法已经证明了在一系列应用实现快速学习，以及人类水平的能力，包括复杂的视频游戏。如果它能像研究人员希望的那样继续发展，那么它就可以通过提高训练速度和效率来缓解深度学习的10000-cow 问题。
![深度学习已入末路，谁能引领下一代AI？](http://p3.pstatp.com/large/pgc-image/4338b20b159e443ebaf89b3bd91b29b6)
Box-World和星际争霸2的任务需要对实体及其关系进行推理
此外，图网络可以使网络不那么容易受到对抗性攻击，原因很简单，它是一个将事物表示为对象，而不是像素模式的系统，不会轻易被一点噪音或无关的贴纸所干扰。
Botvinick 承认，在这些领域中，任何一个都不会轻易、快速地取得根本性的进展。但即便如此，他仍然相信，图网络前途无量。“挑战是真实存在的，但它们并非死路一条。”
原文链接：https://www.pnas.org/content/116/4/1074

