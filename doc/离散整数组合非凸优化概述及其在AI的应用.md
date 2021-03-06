# 离散/整数/组合/非凸优化概述及其在AI的应用

[![留德华叫兽](https://pic4.zhimg.com/v2-9f62cb7cf5fd6abb06893e96e631eb10_xs.jpg)](https://www.zhihu.com/people/ruobingshen)

[留德华叫兽](https://www.zhihu.com/people/ruobingshen)



海德堡大学 交叉学科计算中心离散与组合优化实验室研究员

238 人赞同了该文章

> 作者
>
>  
>
> [@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)
>
> 系美国克莱姆森大学运筹学硕士，Ph.D. Candidate，后跳槽至欧盟玛丽居里博士项目，期间前往意大利IBM Cplex实习半年，现任德国海德堡大学交叉学科计算中心、组合优化实验室助理研究员，主攻图像处理。
>
>  
>
> 欢迎原链接转发，付费转载请前往
>
>  
>
> [@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)
>
>  
>
> 的主页获取信息，盗版必究。
>
> 敬请关注和扩散本专栏及同名公众号，定期邀请
>
> 全球知名学者
>
> 发布运筹学、人工智能中优化理论等相关干货、
>
> 知乎Live
>
> 及行业动态：
>
> 『运筹OR帷幄』大数据人工智能时代的运筹学--知乎专栏

## 前言：

**运筹学**在国内，远没有新兴的人工智能以及传统学科统计来的普及。人工智能、统计最后几乎都能化简成求解一个**能量/损失函数的优化问题**。但相信很多人不知道，**运筹学正是研究优化理论的学科**。而因此，我把**运筹学**称为人工智能、大数据的“**引擎**”，**正本清源**其在人工智能中重要性。

本文提纲：

1，运筹学、线性规划回顾 2，整数规划问题 3，什么是组合优化

4，非凸优化 5，整数规划与非凸优化的关系

6，整数规划、非凸优化为何重要 7，整数规划在工业界的应用

8，整数规划在AI的应用和展望

注：以下文中黑体字代表其在学术界的术语



首先，对运筹学（**O.R.**）还比较陌生的童鞋，请戳本专栏的开篇之作：

[运筹学--一门建模、优化、决策的科学 - 知乎专栏](https://zhuanlan.zhihu.com/p/25579864?refer=operations-research)



## 1. 运筹学、线性规划（**Linear Programming**）回顾

运筹学的初学者，欢迎查看我在下面的回答：

[运筹学如何入门？ - 知乎](https://www.zhihu.com/question/22686770/answer/113176244)



![img](https://pic1.zhimg.com/80/v2-dfbc131790ca5e8f748a3280f110d02c_hd.jpg)

运筹学、数学规划（**Math Programming**）问题的数学表达式，由自变量（**Variables**）、目标函数（**Objective Function**）和约束条件（**Constraints**）组成，所有优化问题本质上都可以化简为由它们组成的数学表达式，然后求解满足约束条件下使得目标函数最大/小的变量的值。



如上图，当自变量是连续的，目标函数和不等式是线性的时候，该问题被称为线性规划问题。线性规划因其具有的良好性质（例如，最优解必定出现在极点），可以用单纯型法（**Simplex Method**）或内点算法（**Interior Method**）高效地求解，熟悉算法复杂度的童鞋，知道它是多项式时间可解的（**Polynomial Time Solvable--O（n^k）**）。这里n表示自变量个数。

可行域（**Feasible Set**）：可行解的集合。如下图，阴影区域（多面体、**Polyhedron**）即为三个线性不等式（半平面）组成的可行域。是不是很眼熟？其实高中代数课大家就已接触过线性规划了。

![img](https://pic3.zhimg.com/80/v2-8c30a29735cd18890f2421a1d1ebb646_hd.jpg)





## 2. 整数规划（**Integer Programming**）问题

整数规划，或者离散优化（**Discrete Optimization**），是指数学规划问题中自变量存在整数。与线性规划连续的可行域不同，整数规划的可行域是离散的。



![img](https://pic1.zhimg.com/80/v2-9196250c67b0526bbe74b18ab47e2fa8_hd.jpg)

如上图，蓝线依旧代表线性不等式，但是这里x,y被约束成整数变量，因此可行域变成了红线区域内的9个离散的点。

凸包（**Convex Hull**）:整数规划所有可行点的凸包围，即图中红线组成的多面体（想象多维的情况）。凸包是未知的，已知的是蓝线的不等式，并且凸包是非常难求解的，或者形成凸包需要指数数量级的线性不等式（图中红线）。如果知道了凸包的所有线性表示，那么整数规划问题就可以等价为求解一个凸包表示的线性规划问题。

另外，除了整数规划，还有混合整数规划（**Mixed Integer Programming, MIP**），即自变量既包含整数也有连续变量。如下图：





![img](https://pic2.zhimg.com/80/v2-195662df7c065a494db4d1847f6944c1_hd.jpg)

x是连续的，y被约束成整数变量，这时候可行域变成了4条离散的橘黄色线段+4处的红色整数点（0,4）。课后作业，求图中的凸包。

整数规划的精确算法通常需要用到分支定界法（**Branch and Bound Method**）,以及增加分支定界效率的各种技巧，例如割平面方法（**Cutting Planes Method**）。总的来说，求解整数规划的精确解是NP难的，也就是指数级算法复杂度（**Exponential Time Solvable**）。

怎么来理解指数级复杂度呢？假设这里的整数是0,1变量，那么我们可以简单地理解为算法复杂度是2^n（需要解2^n个线性规划问题）。也就是说，每增加一个0，1变量，求解的速度就有可能要增加一倍！例如求解n=100的整数规划问题需要1小时，那么求解n=101的规模可能会需要2小时，n=102需要4小时，n=105需要32小时。。这就是**指数爆炸**！

因此，整数规划问题被看作数学规划里、甚至是**世界上最难的问题**之一，被很多其他领域（例如机器学习）认为是不可追踪（**Intractable**）的问题，也就是他们直接放弃治疗了。

作为研究世界上最难问题的学者，想出了解决整数规划问题的各种其他途径，例如近似算法（**Approximation Algorithms**），启发式算法（**Heuristic Algorithms**），遗传算法（**Evolution Algorithms, Meta-Heuristic**）等等。它们虽然不能求得整数规划的最优解，但是却能在短时间（通常多项式时间）内给出一个较好的可行解。

> 篇幅限制，我将在下一篇专栏着重探讨整数规划精确解的算法、整数规划求解器、近似算法以及启发式算法，敬请期待。



## 3. 什么是组合优化(**Combinatorial Optimization**)

通俗的讲，我把组合优化理解为，在组合优化种可能性里找出最优的方案。假设自变量为n，用强力搜索法（B**rute**-**force Algorithm**）来解组合优化的算法复杂度最坏需要**n的阶乘**！什么概念？这比指数爆炸还要可怕！

从这个意义上讲，组合优化是整数规划的子集。的确，绝大多数组合优化问题都可以被建模成（混合）整数规划模型来求解。但是似乎学术圈更多地把组合优化与图（**Graph**）优化以及网络流（**Network Flow**）优化联系在一起，并且最终目标不在精确解，而是近似解。（这点可以从整数规划的国际会议上看出）

Anyway，这里开始，我将混淆整数规划、离散优化、组合优化。

下面给出一个经典的组合优化例子-最大流问题（**Max Flow Problem**）：



![img](https://pic4.zhimg.com/80/v2-d3e155a097be2558402055553e91af0b_hd.jpg)

给定一张图G（V，E），V是点（**Node**）的集合，E是边（**Edge**）的集合。该问题试图从点0到5导流最大的流量，边上的数字代表该条边的最大流量，因此形成了约束条件--每条边的流量不得超过该条边的限额。自然而然地，该问题可以被建模成一个整数规划问题。

我们跳过模型和算法，直观的判断该问题的算法复杂度大概为多少。设想从0出发，有俩种可能线路，0到1以及0到2；从1和2出发，有分别有俩种可能的线路。因此，可以初步判断，改问题如果用强力算法（穷举法），算法复杂度将为指数级！

但是聪明的组合优化学家，把这个看似指数级算法复杂度的问题，用巧妙的算法多项式时间便可求解出最优解。This is the beauty of Mathematics!

> 时间关系，该问题的具体模型和近似算法，会放在下一篇专栏展开，有兴趣的可以搜索“Max Flow/Min Cut”。



## 4. 非凸优化 （**Non-Convex Optimization**）

凸（**Convex**） VS 非凸的概念，数学定义就不写了，介绍个直观判断一个集合是否为Convex的方法，如下图：

![img](https://pic4.zhimg.com/80/v2-1963486a0165957f800e7191c05d08d7_hd.jpg)

简单的测试一个集合是不是凸的，只要任意取集合中的俩个点并连线，如果说连线段完全被包含在此集合中，那么这个集合就是凸集，例如左图所示。

凸优化有个非常重要的定理，即任何局部最优解即为全局最优解。由于这个性质，只要设计一个较为简单的局部算法，例如贪婪算法（**Greedy Algorithm**）或梯度下降法（**Gradient Decent**），收敛求得的局部最优解即为全局最优。因此求解凸优化问题相对来说是比较高效的。这也是为什么机器学习中凸优化的模型非常多，毕竟机器学习处理大数据，需要高效的算法。

而非凸优化问题被认为是非常难求解的，因为可行域集合可能存在无数个局部最优点，通常求解全局最优的算法复杂度是指数级的（NP难）。如下图：

![img](https://pic4.zhimg.com/80/v2-15fbe3739bda767c0ffe2efecb5877ab_hd.jpg)

最经典的算法要算蒙特卡罗投点法(M**onte Carlo Algorithm**)了，大概思想便是随便投个点，然后在附近区域（可以假设convex）用2中方法的进行搜索，得到局部最优值。然后随机再投个点，再找到局部最优点。如此反复，直到满足终止条件。

假设有1w个局部最优点，你至少要投点1w次吧？并且你还要假设每次投点都投到了不同的区域，不然你只会搜索到以前搜索过的局部最优点。



## 5. 整数规划与非凸优化的关系

大家或许不知道，（混合）整数规划被称为极度非凸问题（**highly nonconvex problem**），如下图：

![img](https://pic1.zhimg.com/80/v2-9196250c67b0526bbe74b18ab47e2fa8_hd.jpg)

实心黑点组成的集合，是一个离散集，按照4中判断一个集合是否为凸集的技巧，我们很容易验证这个离散集是非凸的，并且相比4中的非凸集更甚。因此整数规划问题也是一个非凸优化问题。



## 6. 整数规划、非凸优化为何重要

虽然时间是连续的，但是社会时间却是离散的。例如时刻表，通常都是几时几分，即使精确到几秒，它还是离散的（整数）。没见过小数计数的时刻表吧？

同样，对现实社会各行各业问题数学建模的时候，整数变量有时是不可避免的。例如：x辆车，y个人。x，y这里便是整数变量，小数是没有意义的。

决策变量(**Decision Varible**): x={0,1}.

0/1变量被广泛地应用在商业和决策领域。我们假设变量x={0,1}，当x=1的时候，我们便可以建模执行x这个决策；x=0，则表示不执行。这样引入决策变量x的建模技巧，在工业界案例中屡见不鲜。

从这些案例，**社会是由一个个离散变量组成的**。

关于非凸优化，现实生活中，万物的本质是非凸的，就像万物是趋于混乱（**Chaos**）的，规则化需要代价。如果把4中的图看作山川盆地，你在现实中有见过左图那么“光滑”的地形么？右图才是Reality！

说到这里，当然不能否定了凸优化和连续优化的作用，科学的本质便是由简到难，先把简单问题研究透彻，然后把复杂问题简化为求解一个个的简单问题。求解整数规划便是利用分支定界法求解一个个线性规划问题，非凸优化同样如此。



## 7. 整数规划在工业界的应用

路径优化问题（**Routing Problem**）--交通领域（GPS导航）；

仓储、运输等物流（**Logistics**）以及供应链(**Supply chain**)领域；

制造业里的生产流程优化（**Process Optimization**）；

电力领域的电网的布局以及分配(**Power Grid**)；

电子工程里的设施部件分配问题(**Facility Layout Problem**)；

能源领域的优化，如：如何铺设输油管道；

火车、课程、飞机时刻表安排问题等调度问题 (**Scheduling Problem**);

资产配置 （**Asset Allocation**）、风险控制 (**risk managemen**t)等经济金融领域的应用；

以上工业界的应用，频繁使用着决策变量，以及整数变量，建模成（混合）整数规划模型，而机器学习（ML），特别是深度学习（DL），至今没有怎么渗透进来。也希望有志青年探索ML、DL在这些领域的应用。

[国内(全球)TOP互联网公司、学术界超高薪的揽才计划有哪些？](https://www.zhihu.com/question/56552107/answer/149452103)



## 8. 整数规划在AI的应用和展望

如果你是AI初学者，请戳：

[大话“人工智能、数据科学、机器学习”--综述 - 知乎专栏](https://zhuanlan.zhihu.com/p/26645993)

这里我举一个统计和机器学习的例子，这个模型也可以和统计里经典的**Lasso**问题联系起来，以及可以给出**L0范数问题**的精确解。

如下图，是一个分段常数回归问题。统计中大家都知道线性回归和常数回归，其中常数回归即求一组数的平均值。但是这里，我们想对数据分段，并且不知道分段的节点在哪里。如下例所示，假设n个点，要分成三段作常数回归，节点有2个。那么节点的选择有n选2种可能性，从这个意义上理解，这个问题是一个组合优化的问题。

![img](https://pic4.zhimg.com/80/v2-c6827d1a1b3b7ec15da2dea59a3524e7_hd.jpg)

自变量有实数变量w和整数变量x，y_i是常数，即点i的值，w_i是回归值，上半部分的表达式可以看作是伪表达式（pseudo formulation），L0函数即向量中非零的个数（可能需要一些高等的数学知识）。我们直接看下半部分的混合整数非线性规划模型。

我们引进了一个0/1决策变量，X_{ei},这个变量是作用在边上的。我们希望当它是处于节点位置，即俩段常数回归的临界处，就等于1；但它处于某一段常数回归中间时，就等于0.

因此目标函数前半部分是回归方程，希望回归的误差越小越好，后半部分即为规则化项（**regularization term**），用来约束分段的个数，来惩罚过多的分段以防止过拟合（**over-fitting**）。大家经常可以在信号处理、图像处理中看到这样的目标函数。

约束条件第一个不等式保证了同一个分段回归中，w_i和w_{i+1}的值相等，因为x_{ei}=0；而在节点处，他们可以不相等，M是一个很大的常数，以保证节点处x_{ei}=1时，不等式总是成立。

写出了这个整数规划模型，我们就可以编程并调用整数规划的**优化求解器**来求解这个问题的最优解。例如IBM Cplex。虽然整数规划通常的算法复杂度是指数级的，但是比起强力搜索，还是会高效很多很多。这样我们就可以得到每个点的回归值w以及分段的节点，即哪些点x_e=1。



## **展望：**

深度学习的优化问题在运筹学看来是“小儿科”，这句话可能会打脸大部分观众。虽然目标函数非常复杂，但是它没有约束条件阿！

深度学习里的损失函数，是一个高度复合的函数。例如h(x)=f(g(x))就是一个f和g复合函数。深度学习里用到的函数，Logistic, ReLU等等，都是非线性 ，并且非常多。把他们复合起来形成的函数h,便是非凸的。但是深度学习训练参数的优化问题，本质是一个无约束的非凸优化问题。求解这个非凸函数的最优解，类似于求凸优化中某点的gradient，然后按照梯度最陡的方向搜索。不同的是，复合函数无法求gradient，于是这里用方向传播法（**Back Propagation**）求解一个类似梯度的东西，反馈能量，然后更新。

机器学习、数据科学因为处理数据量庞大，因此研究问题主要的方法还是凸优化模型（包括线性规划、锥优化），原因正是求解高效，问题可以scale。

[想学数据分析（人工智能）需要学哪些课程？ - 知乎](https://www.zhihu.com/question/50623000/answer/121833512?group_id=823702071478992896)

虽然目前还很小众，但是随着计算机硬件能力的提高，以及GPU并行计算的流行，以及非凸优化算法、模型的进化，想必非凸优化，（混合）整数规划会是未来的研究热点。

敬请关注我老板，Gerhard Reinelt 教授，以及我们的合作教授之一，法国Rouen的Stephane Canu教授，最近便投身于整数规划在机器学习的应用。（当然还有我：）以及蒙特利尔的Andrea Lodi教授，目前与Yoshua探索着MIP与DL的交叉。当然还有运筹学界泰斗之一，MIT ORC的Dimitris Bertsimas，近十年都在统计、数据学界推崇整数规划。