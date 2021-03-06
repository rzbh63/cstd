
# Learning中的代数结构的建立 - zouxy09的专栏 - CSDN博客


2012年10月18日 10:45:36[zouxy09](https://me.csdn.net/zouxy09)阅读数：5449


**Learning****中的****代数结构****的建立**
转自：http://blog.sina.com.cn/s/blog_6833a4df0100nazk.html
Learning是一个融会多种数学于一体的领域。说起与此有关的数学学科，我们可能会迅速联想到线性代数以及建立在向量空间基础上的统计模型——事实上，主流的论文中确实在很大程度上基于它们。
R^n (n-维实向量空间)是我们在paper中见到最多的空间，它确实非常重要和实用，但是，仅仅依靠它来描述我们的世界并不足够。事实上，数学家们给我们提供了丰富得多的工具。
“空间”(space)，这是一个很有意思的名词，几乎出现在所有的数学分支的基础定义之中。归纳起来，所谓空间就是指一个集合以及在上面定义的某种数学结构。关于这个数学结构的定义或者公理，就成为这个数学分支的基础，一切由此而展开。
还是从我们最熟悉的空间——R^n说起吧。大家平常使用这个空间的时候，除了线性运算，其实还用到了别的数学结构，包括度量结构和内积结构。
第一，它是一个**拓扑空间(Topological space)**。而且从拓扑学的角度看，具有**非常优良的性质**：**Normal****(implying
 Hausdorff and Regular)**,**Locally Compact**,**Paracompact**,**with Countable basis**,**Simply connected (implying connected and path connected)**,**Metrizable**.
第二，它是一个**度量空间(Metric space)**。我们可以**计算上面任意两点的距离**。
第三，它是一个**有限维向量空间(Finite dimensional space)**。因此，我们可以对里面的元素进行代数运算（加法和数乘），我们还可以赋予它一组有限的基，从而可以用有限维坐标表达每个元素。
第四，基于度量结构和线性运算结构，可以建立起**分析(Analysis)**体系。我们可以对连续函数进行微分，积分，建立和求解微分方程，以及进行傅立叶变换和小波分析。
第五，它是一个**希尔伯特空间**（也就是**完备的内积空间**）(**Hilbert space, Complete inner product space**)。它有一套很方便计算的内积(inner product)结构——这个空间的度量结构其实就是从其内积结构诱导出来。更重要的，它是完备的(Complete)——代表任何一个柯西序列(Cauchy sequence)都有极限——很多人有意无意中其实用到了这个特性，不过习惯性地认为是理所当然了。
第六，它上面的**线性映射构成的算子空间**仍旧是**有限维**的——一个非常重要的好处就是，**所有的线性映射都可以用矩阵唯一表示**。特别的，因为它是有限维完备空间，它的泛函空间和它本身是同构的，也是R^n。因而，它们的**谱结构**，也就可以通过**矩阵的特征值和特征向量获得**。
第七，它是一个**测度空间——**可以计算**子集的大小（面积/****体积）**。正因为此，我们才可能在上面建立**概率分布(distribution)**——这是我们接触的**绝大多数连续统计模型**的基础。
我们可以看到，这是一个非常完美的空间，为我们的应用在数学上提供了一切的方便，在上面，我们可以理所当然地认为它具有我们希望的各种良好性质，而无须特别的证明；我们可以直接使用它的各种运算结构，而不需要从头建立；而且很多本来不一样的概念在这里变成等价的了，我们因此不再需要辨明它们的区别。
**以此为界，Learning****的主要工作分成两个大的范畴：**
1、建立一种表达形式，让它处于上面讨论的R^n空间里面。
2、获得了有限维向量表达后，建立各种代数算法或者统计模型进行分析和处理。
这里只讨论第一个范畴。先看看，目前用得比较广泛的一些方法：
1、**直接基于原始数据建立表达**。
我们关心的最终目标是一个个现实世界中的对象：一幅图片，一段语音，一篇文章，一条交易记录，等等。这些东西大部分本身没有附着一个数值向量的。为了构造一个向量表达，我们可以把传感器中记录的数值，或者别的什么方式收集的数值数据按照一定的顺序罗列出来，就形成一个向量了。如果有n个数字，就认为它们在R^n里面。
不过，这在数学上有一点小问题，在大部分情况下，根据数据产生的物理原理，这些向量的值域并不能充满整个空间。比如图像的像素值一般是正值，而且在一个有界闭集之中。这带来的问题是，对它们进行线性运算很可能得到的结果会溢出正常的范围——在大部分paper中，可能只是采用某些heuristics的手段进行简单处理，或者根本不管，很少见到在数学上对此进行深入探讨的——不过如果能解决实际问题，这也是无可厚非的，毕竟不是所有的工作都需要像纯数学那样追求严谨。
2、**量化(quantization)**。
这是在处理连续信号时被广泛采用的方式。只是习以为常，一般不提名字而已。比如一个空间信号（Vision中的image）或者时间信号，它们的domain中的值是不可数无限大的(uncountably infinite)，不要说表示为有限维向量，即使表达为无限序列也是不可能的。在这种情况下，一般在有限域内，按照一定顺序每隔一定距离取一个点来代表其周围的点，从而形成有限维的表达。这就是信号在时域或空域的量化。
这样做不可避免要丢失信息。但是，由于小邻域内信号的高度相关，信息丢失的程度往往并不显著。而且，从理论上说，这相当于在频域中的低通过率。对于有限能量的连续信号，不可能在无限高的频域中依然保持足够的强度，只要采样密度足够，丢失的东西可以任意的少。
除了表示信号，对于几何形体的表达也经常使用量化，比如表示curve和surface。
3、**找出有限个数充分表达一个对象也许不是最困难的**。
不过,在其上面建立数学结构却未必了。一般来说，我们要对其进行处理，首先需要一个拓扑结构用以描述空间上的点是如何联系在一起。直接建立拓扑结构在数学上往往非常困难，也未必实用。因此，绝大部分工作采取的方式是首先建立度量结构。一个度量空间，其度量会自然地诱导出一个拓扑结构——不过，很多情况下我们似乎会无视它的存在。
最简单的情况，就是使用原始向量表达的欧氏距离(Euclidean distance)作为metric。不过，由于原始表达数值的不同特性，这种方式效果一般不是特别好，未必能有效表达实际对象的相似性（或者不相似性）。因此，很多工作会有再此基础上进行度量的二次建立。方式是多种多样的，一种是寻求一个映射，把原空间的元素变换到一个新的空间，在那里欧氏距离变得更加合适。这个映射发挥的作用包括对信息进行筛选，整合，对某些部分进行加强或者抑制。这就是大部分关于feature
 selection，feature extraction，或者subspace learning的文章所要做的。另外一种方式，就是直接调节距离的计算方式（有些文章称之为metric learning）。
这两种方式未必是不同的。如果映射是单射，那么它相当于在原空间建立了一个不同的度量。反过来，通过改变距离计算方式建立的度量在特定的条件下对应于某种映射。
**4****、代数运算**。
上面提到的度量建立方法，比如欧氏距离，它需要对元素进行代数运算。对于普通的向量空间，线性运算是天然赋予的，我们无须专门建立，所以可以直接进行度量的构造——这也是大部分工作的基础。可是，有些事物其原始表达不是一个n-tuple，它可能是一个set，一个graph，或者别的什么特别的object。怎么建立代数运算呢？
一种方法是直接建立。就是给这些东西定义自己的加法和数乘。这往往不是那么直接（能很容易建立的线性运算结构早已经被建立好并广泛应用了），可能需要涉及很深的数学知识，并且要有对问题本身的深入了解和数学上的洞察力。不过，一个新的代数结构一旦建立起来，其它的数学结构，包括拓扑，度量，分析，以及内积结构也随之能被自然地诱导出来，我们也就具有了对这个对象空间进行各种数学运算和操作的基础。加法和数乘看上去简单，但是如果我们对于本来不知道如何进行加法和数乘的空间建立了这两样东西，其理论上的贡献是非常大的。
（一个小问题：大家常用各种graphical model，但是，每次这些model都是分别formulate，然后推导出estimation和evaluation的步骤方法。是否可能对"the space of graphical model"或者它的某个特定子集建立某种代数结构呢？（不一定是线性空间，比如群，环，广群，
 etc）从而使得它们在代数意义上统一起来，而相应的estimation或者evaluation也可以用过代数运算derive。这不是我的研究范围，也超出了我目前的能力和知识水平，只是我相信它在理论上的重要意义，留作一个远景的问题。事实上，数学中确实有一个分支叫做Algebraic statistics可能在探讨类似的问题，不过我现在对此了解非常有限。）
**5、嵌入**
回到我们的正题，除了**直接建立运算定义**，另外一种方式就是**嵌入(embedding)****到某个向量空间**，从而继承其运算结构为我所用。当然这种**嵌入**也不是乱来，它**需要保持原来这些对象的某种关系**。最常见的就是**保距嵌入(isometric
 embedding)**，我们**首先**建立**度量结构**（**绕过向量表达，直接对两个对象的距离通过某种方法进行计算**），**然后**把这个**空间嵌入到目标空间**，通常是**有限维向量空间，要求保持度量不变。**
**“****嵌入”**是一种在数学上应用广泛的手段，其**主要目标**就是**通过嵌入到一个属性良好，结构丰富的空间，从而利用其某种结构或者运算体系**。在拓扑学中，**嵌入到metric
 space**是**对某个拓扑空间建立度量的重要手段**。而在这里，我们是已有度量的情况下，通过嵌入获取线性运算的结构。除此以来，还有一种就是前些年比较热的manifold embedding，这个是通过保持局部结构的嵌入，获取全局结构，后面还会提到。
**6****、内积(inner product)****结构。**
接下来的一个重要的代数结构，就是**内积(inner product)**结构。内积结构一旦建立，会直接诱导出一种性质良好的度量，就是范数(norm)，并且进而诱导出拓扑结构。一般来说，内积需要建立在线性空间的基础上，否则连一个二元运算是否是内积都无法验证。不过，kernel理论指出，对于一个空间，只要定义一个正定核(positive
 kernel)——一个符合正定条件的二元运算，就必然存在一个希尔伯特空间，其内积运算等效于核运算。这个结论的重要意义在于，我们可以绕开线性空间，通过首先定义kernel的方式，诱导出一个线性空间(叫做再生核希尔伯特空间Reproducing Kernel Hilbert Space)，从而我们就自然获得我们所需要的度量结构和线性运算结构。这是kernel
 theory的基础。
在很多教科书中，以二次核为例子，把二维空间变成三维，然后告诉大家kernel用于升维。对于这种说法，我一直认为在一定程度上是误导的。事实上，kernel的最首要意义是内积的建立（或者改造），从而诱导出更利于表达的度量和运算结构。对于一个问题而言，选择一个切合问题的kernel比起关注“升维”来得更为重要。
kernel被视为非线性化的重要手段，用于处理非高斯的数据分布。这是有道理的。通过nonlinear kernel改造的内积空间，其结构和原空间的结构确实不是线性关联，从这个意义上说，它实施了非线性化。不过，我们还应该明白，它的最终目标还是要回到线性空间，新的内积空间仍旧是一个线性空间，它一旦建立，其后的运算都是线性的，因此，kernel的使用就是为了寻求一个新的线性空间，使得线性运算更加合理——非线性化的改造最终仍旧是要为线性运算服务。
值得一提的是，kernelization本质上说还是一种嵌入过程：对于一个空间先建立内积结构，并且以保持内积结构不变的方式嵌入到一个高维的线性空间，从而继承其线性运算体系。
**7、流形**
上面说到的都是从全局的方式建立代数结构的过程，但是那必须以某种全局结构为基础（无论预先定义的是运算，度量还是内积，都必须适用于全空间。）但是，全局结构未必存在或者适合，而局部结构往往简单方便得多。这里就形成一种策略，**以局部而达全局**——这就是**流形(manifold)****的思想**，而其则根源于拓扑学。
从拓扑学的角度说，流形就是一个非常优良的拓扑空间：符合**Hausdorff**分离公理（**任何不同的两点都可以通过不相交的邻域分离**），**符合第二可数公理（****具有可数的拓扑基****）**，并且更重要的是，局部同胚于R^n。因此，一个正则(Regular)流形基本就具有了各种最良好的拓扑特性。而局部同胚于R^n，代表了它至少在局部上可以继承R^n的各种结构，比如线性运算和内积，从而建立分析体系。事实上，拓扑流形继承这些结构后形成的体系，正是现代流形理论研究的重点。继承了分析体系的流形，就形成了微分流形(Differential
 manifold)，这是现代微分几何的核心。而微分流形各点上的切空间(Tangent Space)，则获得了线性运算的体系。而进一步继承了局部内积结构的流形，则形成黎曼流形(Riemann manifold)，而流形的全局度量体系——测地距离(geodesics)正是通过对局部度量的延伸来获得。进一步的，当流行本身的拓扑结构和切空间上的线性结构发生关系——也就获得一簇拓扑关联的线性空间——向量丛(Vector bundle)。
虽然**manifold theory**作为现代几何学的核心，是一个博大精深的领域，但是它在learning中的应用则显得非常狭窄。事实上，对于manifold，很多做learning的朋友首先反应的是**ISOMAP,
 LLE, eigenmap****之类的算法**。这些都属于**embedding**。当然，这确实是流形理论的一个重要方面。严格来说，这要求是**从原空间到其映像的微分同胚映射**，因此，嵌入后的空间在局部上具有相同的分析结构，同时也获得了各种好处——全局的线性运算和度量。不过，这个概念在learning的应用中被相当程度的放宽了——微分同胚并不能被完全保证，而整个分析结构也不能被完全保持。大家更关注的是保持局部结构中的某个方面——不过这在实际应用中的折衷方案也是可以理解的。事实表明，当原空间中的数据足够密集的情况下，这些算法工作良好。
Learning中流形应用的真正问题在于它被过滥地运用于**稀疏空间(Sparse space)**，事实上在高维空间中撒进去几千乃至几十万点，即使最相邻的几点也难称为局部了，局部的范围和全局的范围其实已经没有了根本差别，连局部的概念都立不住脚的时候，后面基于其展开的一切工作也都没有太大的意义。事实上，**稀疏空间有其本身的规律和法则**，**通过局部形成全局的流形思想**从本质上是不适合于此的。虽然，流形是一种非常美的理论，但是再漂亮的理论也需要用得其所——它**应该用于解决具有密集数据分布的低维空间**。至于，一些paper所报告的在高维空间（比如人脸）运用流形方法获得性能提升，其实未必是因为“流形”本身所起的作用，而很可能是其它方面的因素。
**8****、流形在实际应用中起重要作用**
流形在实际应用中起重要作用的还有两个方面：一个是**研究几何形体的性质**（我们暂且不谈这个），还有就是**它和代数结构的结合形成的李群(Lie
 group)****和李代数(Lie algebra)**。当我们研究的对象是变换本身的时候，它们构成的空间是有其**特殊性**的，比如**所有子空间投影形成了Grassmann**流形，所有的可逆线性算子，或者仿射算子，也形成各自的流形。对他们的**最重要操作是变换的结合**，而不是加法数乘，因此，它们上面定义的更合适的代数结构应该是**群**和不是线性空间。而**群和微分流形的结合体——****李群**则成为它们最合适的描述体系——而**其切空间**则构成了**一种加强的线性空间**：**李代数，用于描述其局部变化特性。**
李代数和李群的关系是非常漂亮的。它**把****变换的微变化****转换成了****线性空间的代数运算**，使得移植传统的基于**线性空间的模型和算法到李空间变得可能**。而且李代数中的矩阵比起变换本身的矩阵甚至更能反映变换的特性。**几何变换的李代数矩阵的谱结构**就能非常方便地用于**分析变换的几何特性**。
最后，回头总结一下关于嵌入这个应用广泛的策略，在learning中的isometry, kernel和manifold embedding都属于此范畴，它们分别**通过保持原空间的****度量结构，内积结构和局部结构**来获得到目标（通常是向量空间）的嵌入，从而获得全局的坐标表达，线性运算和度量，进而能被各种线性算法和模型所应用。
在获得这一系列好处的同时，也有值得我们注意的地方。**首先**，**嵌入只是一种数学手段**，并不能取代**对问题本身的研究和分析**。一种不恰当的原始结构或者嵌入策略，很多时候甚至适得其反——比如稀疏空间的流形嵌入，或者选取不恰当的kernel。另外，**嵌入适合于分析，而未必适合于重建或者合成**。这是因为**嵌入是一个****单射****(injection)**，目标空间不是每一个点都和原空间能有效对应的。嵌入之后的运算往往就打破了原空间施加的限制。比如**两个元素即使都是从原空间映射过来，它们的和却未必有原像，这时就不能直接地回到原空间了**。当然可以考虑在原空间找一个点它的映射与之最近，不过这在实际中的有效性是值得商榷的。
和Learning有关的数学世界是非常广博的，我随着学习和研究的深入，越来越发现在一些我平常不注意的数学分支中有着适合于问题的结构和方法。比如，**广群(groupoid)****和广代数(algebroid)****能克服李群和李代数在****表示连续变换过程****中的一些困难**——这些困难困扰了我很长时间。**解决问题和建立数学模型是相辅相成的**，一方面，一个**清晰的问题将使我们有明确的目标去寻求合适的数学结构**，另一方面，**对数学结构的深入理解对于指导问题的解决也是有重要作用的。**对于解决一个问题来说，数学工具的选择最重要的是**适合**，而不是高深，但是如果在现有数学方法陷入困难的时候，**寻求更高级别的数学的帮助，往往能柳暗花明**。数学家长时间的努力解决的很多问题，并不都是理论游戏，他们的解决方案中很多时候蕴含着我们需要的东西，而且可能导致对更多问题的解决——但是我们需要时间去学习和发现它们。


