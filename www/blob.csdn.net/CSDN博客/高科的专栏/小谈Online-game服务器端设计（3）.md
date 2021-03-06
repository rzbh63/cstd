
# 小谈Online-game服务器端设计（3） - 高科的专栏 - CSDN博客

2013年01月05日 22:43:47[高科](https://me.csdn.net/pbymw8iwm)阅读数：5878个人分类：[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[架构管理																](https://blog.csdn.net/pbymw8iwm/article/category/1219853)[游戏																](https://blog.csdn.net/pbymw8iwm/article/category/1252249)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1219853)
[
																								](https://blog.csdn.net/pbymw8iwm/article/category/910215)


下面我想来谈谈关于服务器上NPC的设计以及NPC智能等一些方面涉及到的问题。首先，我们需要知道什么是NPC，NPC需要做什么。NPC的全称是（Non-Player Character），很显然，他是一个character，但不是玩家，那么从这点上可以知道，NPC的某些行为是和玩家类似的，他可以行走，可以战斗，可以呼吸（这点将在后面的NPC智能里面提到），另外一点和玩家物件不同的是，NPC可以复生（即NPC被打死以后在一定时间内可以重新出来）。其实还有最重要的一点，就是玩家物件的所有决策都是玩家做出来的，而NPC的决策则是由计算机做出来的，所以在对NPC做何种决策的时候，需要所谓的NPC智能来进行决策。
下面我将分两个部分来谈谈NPC，首先是NPC智能，其次是服务器如何对NPC进行组织。之所以要先谈NPC智能是因为只有当我们了解清楚我们需要NPC做什么之后，才好开始设计服务器来对NPC进行组织。
**NPC智能**
NPC智能分为两种，一种是被动触发的事件，一种是主动触发的事件。对于被动触发的事件，处理起来相对来说简单一些，可以由事件本身来呼叫NPC身上的函数，比如说NPC的死亡，实际上是在NPC的HP小于一定值的时候，来主动呼叫NPC身上的OnDie() 函数，这种由事件来触发NPC行为的NPC智能，我称为被动触发。这种类型的触发往往分为两种：
一种是由别的物件导致的NPC的属性变化，然后属性变化的同时会导致NPC产生一些行为。由此一来，NPC物件里面至少包含以下几种函数：
class NPC {
public:
// 是谁在什么地方导致了我哪项属性改变了多少。
OnChangeAttribute(object_t *who, int which, int how, int where);
Private:
OnDie();
OnEscape();
OnFollow();
OnSleep();
// 一系列的事件。
}
这是一个基本的NPC的结构，这种被动的触发NPC的事件，我称它为NPC的反射。但是，这样的结构只能让NPC被动的接收一些信息来做出决策，这样的NPC是愚蠢的。那么，怎么样让一个NPC能够主动的做出一些决策呢？这里有一种方法：呼吸。那么怎么样让NPC有呼吸呢？
一种很简单的方法，用一个计时器，定时的触发所有NPC的呼吸，这样就可以让一个NPC有呼吸起来。这样的话会有一个问题，当NPC太多的时候，上一次NPC的呼吸还没有呼吸完，下一次呼吸又来了，那么怎么解决这个问题呢。这里有一种方法，让NPC异步的进行呼吸，即每个NPC的呼吸周期是根据NPC出生的时间来定的，这个时候计时器需要做的就是隔一段时间检查一下，哪些NPC到时间该呼吸了，就来触发这些NPC的呼吸。
上面提到的是系统如何来触发NPC的呼吸，那么NPC本身的呼吸频率该如何设定呢？这个就好象现实中的人一样，睡觉的时候和进行激烈运动的时候，呼吸频率是不一样的。同样，NPC在战斗的时候，和平常的时候，呼吸频率也不一样。那么就需要一个Breath_Ticker来设置NPC当前的呼吸频率。
那么在NPC的呼吸事件里面，我们怎么样来设置NPC的智能呢？大体可以概括为检查环境和做出决策两个部分。首先，需要对当前环境进行数字上的统计，比如说是否在战斗中，战斗有几个敌人，自己的HP还剩多少，以及附近有没有敌人等等之类的统计。统计出来的数据传入本身的决策模块，决策模块则根据NPC自身的性格取向来做出一些决策，比如说野蛮型的NPC会在HP比较少的时候仍然猛扑猛打，又比如说智慧型的NPC则会在HP比较少的时候选择逃跑。等等之类的。
至此，一个可以呼吸，反射的NPC的结构已经基本构成了，那么接下来我们就来谈谈系统如何组织让一个NPC出现在世界里面。
**NPC的组织**
这里有两种方案可供选择，其一：NPC的位置信息保存在场景里面，载入场景的时候载入NPC。其二，NPC的位置信息保存在NPC身上，有专门的事件让所有的NPC登陆场景。这两种方法有什么区别呢？又各有什么好坏呢？
前一种方法好处在于场景载入的时候同时载入了NPC，场景就可以对NPC进行管理，不需要多余的处理，而弊端则在于在刷新的时候是同步刷新的，也就是说一个场景里面的NPC可能会在同一时间内长出来。而对于第二种方法呢，设计起来会稍微麻烦一些，需要一个统一的机制让NPC登陆到场景，还需要一些比较麻烦的设计，但是这种方案可以实现NPC异步的刷新，是目前网络游戏普遍采用的方法，下面我们就来着重谈谈这种方法的实现：
首先我们要引入一个“灵魂”的概念，即一个NPC在死后，消失的只是他的肉体，他的灵魂仍然在世界中存在着，没有呼吸，在死亡的附近漂浮，等着到时间投胎，投胎的时候把之前的所有属性清零，重新在场景上构建其肉体。那么，我们怎么来设计这样一个结构呢？首先把一个场景里面要出现的NPC制作成图量表，给每个NPC一个独一无二的标识符，在载入场景之后，根据图量表来载入属于该场景的NPC。在NPC的OnDie() 事件里面不直接把该物件destroy 掉，而是关闭NPC的呼吸，然后打开一个重生的计时器，最后把该物件设置为invisable。这样的设计，可以实现NPC的异步刷新，在节省服务器资源的同时也让玩家觉得更加的真实。
（这一章节已经牵扯到一些服务器脚本相关的东西，所以下一章节将谈谈服务器脚本相关的一些设计）
补充的谈谈启发式搜索（heuristic searching）在NPC智能中的应用。
其主要思路是在广度优先搜索的同时，将下一层的所有节点经过一个启发函数进行过滤，一定范围内缩小搜索范围。众所周知的寻路A*算法就是典型的启发式搜索的应用，其原理是一开始设计一个Judge(point_t* point)函数，来获得point这个一点的代价，然后每次搜索的时候把下一步可能到达的所有点都经过Judge()函数评价一下，获取两到三个代价比较小的点，继续搜索，那些没被选上的点就不会在继续搜索下去了，这样带来的后果的是可能求出来的不是最优路径，这也是为什么A*算法在寻路的时候会走到障碍物前面再绕过去，而不是预先就走斜线来绕过该障碍物。如果要寻出最优化的路径的话，是不能用A*算法的，而是要用动态规划的方法，其消耗是远大于A*的。
那么，除了在寻路之外，还有哪些地方可以应用到启发式搜索呢？其实说得大一点，NPC的任何决策都可以用启发式搜索来做，比如说逃跑吧，如果是一个2D的网络游戏，有八个方向，NPC选择哪个方向逃跑呢？就可以设置一个Judge(int direction)来给定每个点的代价，在Judge里面算上该点的敌人的强弱，或者该敌人的敏捷如何等等，最后选择代价最小的地方逃跑。下面，我们就来谈谈对于几种NPC常见的智能的启发式搜索法的设计：
Target select （选择目标）：
首先获得地图上离该NPC附近的敌人列表。设计Judge() 函数，根据敌人的强弱，敌人的远近，算出代价。然后选择代价最小的敌人进行主动攻击。
Escape（逃跑）：
在呼吸事件里面检查自己的HP，如果HP低于某个值的时候，或者如果你是远程兵种，而敌人近身的话，则触发逃跑函数，在逃跑函数里面也是对周围的所有的敌人组织成列表，然后设计Judge() 函数，先选择出对你构成威胁最大的敌人，该Judge() 函数需要判断敌人的速度，战斗力强弱，最后得出一个主要敌人，然后针对该主要敌人进行路径的Judge() 的函数的设计，搜索的范围只可能是和主要敌人相反的方向，然后再根据该几个方向的敌人的强弱来计算代价，做出最后的选择。
Random walk（随机走路）：
这个我并不推荐用A*算法，因为NPC一旦多起来，那么这个对CPU的消耗是很恐怖的，而且NPC大多不需要长距离的寻路，只需要在附近走走即可，那么，就在附近随机的给几个点，然后让NPC走过去，如果碰到障碍物就停下来，这样几乎无任何负担。
Follow Target（追随目标）：
这里有两种方法，一种方法NPC看上去比较愚蠢，一种方法看上去NPC比较聪明，第一种方法就是让NPC跟着目标的路点走即可，几乎没有资源消耗。而后一种则是让NPC在跟随的时候，在呼吸事件里面判断对方的当前位置，然后走直线，碰上障碍物了用A*绕过去，该种设计会消耗一定量的系统资源，所以不推荐NPC大量的追随目标，如果需要大量的NPC追随目标的话，还有一个比较简单的方法：让NPC和目标同步移动，即让他们的速度统一，移动的时候走同样的路点，当然，这种设计只适合NPC所跟随的目标不是追杀的关系，只是跟随着玩家走而已了。

