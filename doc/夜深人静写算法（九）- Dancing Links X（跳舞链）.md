# 夜深人静写算法（九）- Dancing Links X（跳舞链）

  

## **一、引例**

###   **1、买点彩票压压惊**

  【**例题1**】**有这样一种彩票，规则如下：总共八个数字，范围是[1,8]。八选五，如果五个都中，则为特等奖；如果中了其中四个，则为一等奖。作者觉得连年会都抽不到奖的人来说特等奖的概率太小了，所以对特等奖基本不抱希望。但是想尝试下一等奖，于是他想知道至少要买多少张彩票才能使得他中一等奖的概率为100%****。**

  如果这个问题问的是让特等奖概率为100%，会变得简单许多。可以这么考虑，C(8,5)种情况下，每种情况都有可能中奖，所以每张彩票都必须买才能保证没有漏网之鱼。所以只要买下C(8,5)=56张彩票，就能保证一定有一张能中特等奖。

  但是，如果五个里面中四个，情况就不一样了。假设我买了两张彩票，一张为{1,2,3,4,5}，一张为**{4,5,6,7,8}，但是中一等奖的四个数字为{1,2,3,6}。虽然两张彩票覆盖了所有数字，但是第一张只中了三个数字{1,2,3}；第二张之中了一个数字{6}，因而都不算中奖。**

![img](https://img-blog.csdn.net/20180131192656803)

图一-1-1

  为了应对任何一种中奖情况，我们需要做这样一件事情：从所有的C(8,5)种组合，也就是彩票中挑选出K种彩票，使得这K种彩票里能够找到任意的C(8,4)的组合（{1,2,3,4},{1,2,3,5},...{4,5,6,7}...等等），并且使得这个K最小。为了使问题更加通俗易懂，我们减小数据量，考虑“四选三中二”的情况（四个数字选三个，中其中二个才算中奖）。


  **如图一-1-2，这是一个矩阵，矩阵的行代表所有的彩票组合（四选三），矩阵的列代表所有中奖组合（四中二）。对于每一行，如果这种组合包含对应的中奖组合，那么将它对应矩阵的位置图上颜色。为了看起来不混淆，第一种组合方案采用红色，第二种橙色，以此类推...**

![img](https://img-blog.csdn.net/20180131192811050)

图一-1-2

  ***\*然后我们把这个矩阵数字化，有颜色的地方置为1，没有颜色的置为0，得到了如下矩阵：\**** 

![img](https://img-blog.csdn.net/20180131192941049)

图一-1-3

  **这个矩阵有一个特点就是：“每行三个1”，这是肯定的，因为对于每一行来说，要从三个数字中挑出所有中了两个数字的情况，即C(3,2)=3。但是这不是我们关心的重点，我们关心的是如何选择一些行集合，使得所有列都能被选到（或者说覆盖到）。**

**更加官方的描述是：在这个矩阵上找到一些行集合，使得集合中每一列至少一个“1”，并且保证选出的行数最少。**

  **这就是经典的重复覆盖问题。接下来我们通过从精确覆盖入手，引入DancingLinks结构，逐步抽丝剥茧，揭晓如何高效的求解重复覆盖问题。**

 

## **二、精确覆盖**

###   **1、精确覆盖的定义**

  **【例题2】给定一个R×C（R, C <= 15）的01矩阵，问是否存在这样一个行集合，使得集合中每一列恰有一个“1”。**

  如图二-1-1，表示的是一个6×7的01矩阵。我们可以通过选择第1、5、6行使得这些行集合中每列恰有一个“1”（“恰有”的意思是有且仅有）。

![img](https://img-blog.csdn.net/20180131193047070)

图二-1-1

  这类问题就是经典的精确覆盖问题，没有多项式算法，属于NP完全问题。通用解法是穷举法。

###   **2、穷举法**

  穷举的意思就是枚举所有状态，每一行的状态有“选”和“不选”两种，那么R行的状态数就是2^R。所以穷举的复杂度是指数级的。穷举的常用实现就是深度优先搜索：对于一个R×C的矩阵，枚举每一行r的“选”与“不选”，第r行“选”的条件是它和已经选择的行集合都没有冲突（两行冲突的定义是两行中至少存在一列上的两个数字均为“1”）。当已经选择的行集合中所有的列都恰有一个“1”时算法终止。

  那么，枚举每行选与不选的时间复杂度为O(2^R)，每次选行时需要进行冲突判定，需要遍历之前选择的行集合的所有列，冲突判定的最坏时间复杂度为O(R*C)，所以整个算法的最坏复杂度O(R*C*2^R)。

  这里可以加入一个很明显的优化：由于问题的特殊性，即选中的行集合的每列只能有一个“1”。所以可以利用一个全局的哈希数组H[]标记当前状态下列的选择情况（H[c]=1表示第c列已经有一个“1”了）。这样每次进行冲突判定的时候就不需要遍历之前所有的行，而只需要遍历这个哈希数组H，遍历的最坏复杂度为O(C)。选中一个没有冲突的行之后，用该行里有“1”的列去更新哈希数组H，复杂度也是O(C)（需要注意的是：深搜在回溯的时候，需要将哈希数组标记回来）。所以整个算法的最坏复杂度为O(C*2^R)。对于R和C都在15以内的情况，时间复杂度的数量级大概在10^6，已经可以接受了。

  **【例题3】给定一个R×C（R <= 20, C <= 50）的01矩阵，问是否存在这样一个行集合，使得集合中每一列恰有一个“1”。**

  这题和上一题的区别在于R和C的数据量，总数据规模是之前的四倍多，观察数据量，如果利用穷举+哈希，那么时间复杂度是10^8的数量级，已经无法满足我们的需求。这时可以采用状态压缩。

###   **3、状态压缩**

  状态压缩一般用在动态规划中，这里我们可以将它进行扩展，运用到搜索里。

  考虑上述矩阵的某一行，这一行中的每个元素的值域是[0, 1]，所以可以把每个元素想象成二进制数字的某一位，那么我们可以将一个二维矩阵的每一行压缩成一个二进制数，使得一个二维矩阵变成一个一维数组（降维）。

![img](https://img-blog.csdn.net/20180131193106849)

图二-3-1

  由于列数C的上限是50，所以可以把每一行用一个64位的整型来表示。

  然后我们可以把问题的求解相应做一个转化，变成了求一个一维数组的子集，子集中的数满足两个条件：

   1) 任意两个数的“位与”(C++中的'&')等于0;

   2) 所有数的“位或”(C++的'|')等于2^C - 1;

  第1)条很容易理解，倘若存在某两个数的“位与”不等于0，那么在这两个数的二进制表示中势必存在某一位都为1，即一列上至少有两个“1”，不满足题目要求；第2)条可以这么理解，所有数的“位或”等于2^C - 1，代表选出的数中所有位都至少有一个“1”，结合第1)条，代表选出的数中所有位至多有一个“1”。与之前的矩阵精确覆盖问题等价转化。

  那么我们依旧采用穷举法，枚举每个数的“选”与“不选”。需要用到一个64位整型的辅助标记X（相对于之前的哈希数组H），X表示所有已经选择数的“位或”和。那么第r个数“选”的话则需要满足它和X的“位与”等于0（这个简单的操作相当于之前提到的行冲突判定）。辅助标记X的判定和更新的时间复杂度都是O(1)的，所以总的时间复杂度就是穷举的复杂度，即O(2^R)。

  **【例题4】给定一个R×C（R <= 50, C <= 200）的01矩阵，问是否存在这样一个行集合，使得集合中每一列恰有一个“1”。**

  数据量进一步扩大，我们发现单纯的穷举已经完全没法满足需求，需要对算法进行进一步改进。

###   **4、回溯法**

  还是采用枚举的思想，不同的是这次的枚举相对较智能化。具体思路是当枚举某一行的时候，预先把和这行冲突的行、列都从矩阵中删除，这样一来避免下次枚举到无用的行，大大减少搜索的状态空间。

  以之前的6×7的矩阵为例，当枚举第一行“选”时，这行中有“1”的列就可以从原矩阵中删除了。如图二-4-1所示，其中红色的框代表选中的行，蓝色的框代表删除的列。

![img](https://img-blog.csdn.net/20180131193129576)

图二-4-1

  再深入一点，我们可以把那些删除的列上有“1”的行也进行删除，这个是显然的，因为一列上只能有一个“1”，所以已经选了一行，其它的行就都不需要了。如图二-4-2，左数第一个蓝色框（第二列）的第三个元素为“1”，所以可以直接删除第三行。同样的，第二个蓝色框（第六列）的第二个元素为“1”，所以直接删除第二行。绿色的框代表删除的行。

![img](https://img-blog.csdn.net/20180131193145597)

图二-4-2

  这样一来，一个6×7的矩阵经过一次枚举+删除，转变成了一个3×5的矩阵。继续用同样的方式求解这个小规模的矩阵。如图二-4-3所示，红色框的行选中后，需要删除蓝色列和绿色行。删除完后剩下的矩阵变成了空矩阵。本次搜索一共选择两行，选中的行中有“1”的列数为5，而总列数为7，所以这种方案宣告失败。

![img](https://img-blog.csdn.net/20180131193246483)

图二-4-3

  这时候我们需要回溯，首先将空矩阵恢复为3×5的矩阵，然后选择第二行。如图二-4-4，进行列和行的删除后，剩下的为一个1×3的矩阵，而且三个数字均为“1”。很明显，这一行必选。至此，我们找到了一个精确覆盖的可行解。

  这里需要注意的就是，在解输出时需要转换成最原始矩阵对应的行号，即第1行、第5行、第6行。

![img](https://img-blog.csdn.net/20180131193305953)

图二-4-4

## **三、Dancing Links X算法**

###   **1、X算法**

  上述的回溯法求解精确覆盖问题的算法，是由算法大师Donald Knuth提出的，被称为“X算法”。它是一个递归、非确定性、深度优先的回溯算法。算法的描述如下（译自维基百科<https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X>）：

   1) 如果矩阵A没有列（即空矩阵），则当前记录的解为一个可行解；算法终止，成功返回；

   2) 否则选择矩阵A中“1”的个数最少的列c；（确定性选择）

   3) a.如果存在A[r][c]=1的行r，将行r放入可行解列表，进入步骤4)；（非确定性选择）

​     b.如果不存在A[r][c]=1的行r，则剩下的矩阵不可能完成精确覆盖，说明之前的选择有错（或者根本就无解），需要回溯，并且恢复此次删除的行和列，然后跳到步骤3)a；

   4)对于所有的满足A[r][j]=1的列j
         对于所有满足A[i][j]=1的行i，将行i从矩阵A中删除；
      将列j从矩阵A中删除；

   5) 在不断减少的矩阵A上递归重复调用上述算法；

  上述的行r的“非确定性选择”的意思是这个算法本质把自身复制给多个独立的子算法；每个子算法继承当前矩阵A。所有子算法搭建了一棵搜索树，树根是初始矩阵，每个被删除行列的矩阵是这棵搜索树树上的内部结点，空矩阵是叶子结点，回溯就是前序遍历树的过程，即深度优先。任意一个深度为k的搜索树结点中的矩阵代表了一个选择k行后剩下的矩阵。

###   **2、搜索树**

  如图三-2-1展示的是一棵矩阵A经过X算法后产生的搜索树。

![img](https://img-blog.csdn.net/20180131193334218)


图三-2-1

  黑色箭头代表了搜索树的树边，连接的两个树结点分别代表搜索前的矩阵状态和搜索后的矩阵状态，正向（即橙色箭头）代表了状态转移，也就是选择某一行，并且根据规则进行删列、删行的过程；反向（即咖啡色箭头）代表了回溯，也就是恢复之前删除的行、列的过程。为了使问题更加清晰，我们将上面的图进行一个转化。

![img](https://img-blog.csdn.net/20180131193342418)

图三-2-2

  如图三-2-2，每个矩阵代表一个状态，S0代表了初始矩阵，S1代表选中第四行并且删除相应列和行之后剩下的矩阵，S2、S3同理可得，S4代表空矩阵。这个就是本次X算法的完整搜索树，随着行数的增加，搜索树的结点数可能呈指数级增长。

  在这个搜索流程中，涉及到了大量的矩阵缓存和矩阵回溯，于是，设计一种合适的数据结构来存储矩阵显得尤为重要。

###   **3、传统矩阵存储**

  传统存储矩阵的方式是二维数组，但是在搜索中删除行列之后需要将后面的行列数据相应往前移，对于一个R×C的矩阵来说，最坏的时间复杂度是O(R*C)。更加糟糕的是，每次搜索失败的时候，需要回退删除的数据，也就是矩阵回溯，又是一个O(R*C)的时间复杂度，理解起来显得很繁琐，实现起来也很容易写错。当然，你可以选择每次搜索时生成一个新的矩阵，然后将原矩阵中未删除的数据拷贝过来，虽然相对来说易于理解，但是时间复杂度还是没有解决，而且带来了一个额外空间开销，当递归深度过深还可能导致栈溢出。

###   **4、Dancing Links**

  于是算法大师Donald Knuth提出了Dancing Links X算法，Dancing Links翻译成舞蹈链，并不是算法本身，而是一种链式的数据结构，利用链表的性质，在缓存和回溯矩阵中运用得恰到好处，不需要额外开辟空间，在这种结构存储的矩阵，列删除是O(1)的、行删除是O(C)的（当然这个取决于如何实现）。正是由于这种删除、恢复的操作是指针之间的跳跃仿佛精妙的舞蹈一般，由此得名。

  “Dancing Links X”的含义正是利用“舞蹈链”来求解“X算法”的意思。

  设想一个双向循环链表中的某个结点x，x->left 和 x->right 分别指向它的左右结点。我们可以通过图三-4-1的操作将x从这个双向循环链表中移除。

![img](https://img-blog.csdn.net/20180131193352679)

图三-4-1

  你可能会说，这种写法存在一定隐患。因为移除后，x的内存还在，x->left 和 x->right 还是指向链表中的元素，应该将它们置空并且释放掉x的内存。

  但是上面说的是将x移除，而并非删除，也就是说x对于我们来说还有利用价值，所以我们只是暂时将它从链表中移除，等到时机成熟，我们还想将它添加回链表中。如图三-4-2，由于当时移除x的时候并没有修改x的left和right指针，也没有释放x的内存，所以重新添加x就变得如此简单。

![img](https://img-blog.csdn.net/20180131193400120)

图三-4-2

  这两步操作正是Dancing Links的精华，联想之前的矩阵的删除和恢复，x的移除类似矩阵的删行和删列，而将x重新添加回来类似矩阵的回溯。只不过矩阵是二维的，这个双向循环链表是一维的，那么没关系，我们可以将维数进行扩展。

  Dancing Links正是十字交叉双向循环链表。

###   **5、十字交叉双向循环链表**

  这种链表结构的每个结点有两类数据，分别为指针域和数据域。指针域为left、right、up、down，分别指向左、右、上、下四个其它结点；数据域则存储一些信息，比如这个结点对应于原始矩阵的行编号rowIdx，列编号colIdx等等。

  原始矩阵中值为“1”的位置对应了一个Dancing Links结点，“0”的位置不是我们需要关心的。

  那么接下来我们来看下，如何将一个矩阵转变为一个十字交叉双向循环链表。我们把Dancing Links结点分成以下四类：总表头head、列首结点col[]、行首结点row[]、元素结点node。

​    1) 总表头head：将列首结点col[]在水平方向串联起来，head->right指向矩阵的第一列的列首结点，head->left指向矩阵的最后一列的列首结点。特别的，当这个矩阵为空矩阵，也就是没有任何列时，head->right和head->left指向head本身，这也正是X算法的终止条件。

​    2) 列首结点col[]：令初始矩阵的列数为colCount，那么col[i]->right指向col[i+1]，特殊的，col[colCount-1]->right指向head；同理，col[0]->left指向head，其它的col[i]->left指向col[i-1]。col[i]->down和col[i]->up分别指向第i列的第一个“1”和最后一个“1”对应的结点，当col[i]的up和down都指向本身说明这列全是0；

​    3) 行首结点row[]：令初始矩阵的行数为rowCount，那么row[i]->up和row[i]->down都是无用指针，直接指向自己即可；row[i]->right和row[i]->left分别指向第i行的第一个“1”和最后一个“1”对应的结点。

​    4) 元素结点node：矩阵中“1”对应的结点，up、down指向其它node或列首结点；left、right指向其它node或行首结点。

  如图三-5-1表示了之前的那个矩阵的十字交叉双向循环链表的数据结构表示。所有箭头的左右边界循环相连（上下边界亦循环相连）。每个元素结点代表了原矩阵中的那个“1”，即图中的蓝色方块，其中的数字代表对应内存池中的编号。初始化时，所有的行首结点的左右指针和列首结点的上下指针都指向自己，然后对矩阵进行行、列分别递增的顺序进行读取，读到“1”就执行结点插入操作，这正对应了图中蓝色结点的递增序。别以为这是飞行棋...

![img](https://img-blog.csdn.net/20180131193423986)


图三-5-1

###   **6、额外结点的意义**

  我们发现，图三-5-1中，除了蓝色结点，其它三种结点都是额外的，那么为什么要引入额外结点呢？

  列首结点、行首结点都是存在既有数组中的，所以进行插入操作的时候可以达到O(1)，试想如果只有列首结点没有行首结点，那么插入一个处于(r, c)位置的结点时，c可以定位到列首结点col[c]，在进行对应行的插入时只能遍历竖向链表，插入的时间复杂度就变成O(R)了；同样，如果只有行首结点没有列首结点，那么插入复杂度就是O(C)的。

  列首结点还有一个作用是区分不存在的列和全“0”的列。如果列c在搜索过程中被删除，那么列c的列首结点不会出现在链表结构中；而一个全“0”的列c，列首结点会在链表结构中，并且它的上下指针都指向自己。

  总表头head主要还是为了空矩阵而存在的，试想如果一个矩阵为空，那么势必它的所有列首结点都没有了，那用什么来表示空矩阵呢？引入总表头后，只要总表头的左右指针都指向自己，就代表这是一个空矩阵。

## **四、Dancing Links X算法的具体实现**

###   **1、结点定义DLXNode**

​    四类结点都定义为DLXNode，并且除了left、right、up、down四个指针数据外，还需要一些额外信息记录：

​    1）对于总表头，不需要额外记录信息；

​    2）对于列首结点，需要记录列编号colIdx，该列的结点个数colSum；

​    3）对于行首结点，需要记录行编号rowIdx；

​    4）对于元素结点，需要记录行编号rowIdx，列首指针colhead；



```cpp
/*
DLXNode
	left, right        十字交叉双向循环链表的左右指针
	up, down           十字交叉双向循环链表的上下指针
	<用于列首结点>
	colSum             列的结点总数
	colIdx             列的编号
		
	<用于行首结点/元素结点>
	colHead            指向列首结点的指针
	rowIdx             DLXNode结点在原矩阵中的行标号
*/
class DLXNode {
public:
	DLXNode *left, *right, *up, *down;
	union {
		struct {
			DLXNode *colHead;   
			int rowIdx;
		}node;
		struct {
			int colIdx;
			int colSum;
		}col;
	}data;
};
```

###   **2、链表定义DLX**

​    十字交叉双向循环链表对于整个搜索来说，只有一个对象，所以这里采用单例实现。因为结点个数可能很多，所以可以将结点内存放在堆上避免栈溢出，row和col分别代表行首和列首结点，dlx_pool则为元素结点的对象池。可以在构造函数中利用new生成这些动态结点，在析构函数中delete。

```cpp
/*
DLX （单例）
	head               head 只有左右（left、right）两个指针有效，指向列首
	rowCount, colCount 本次样例矩阵的规模（行列数）
	row[]              行首结点列表
	col[]              列首结点列表
	
	dlx_pool           结点对象池（配合dlx_pool_idx取对象）
*/
class DLX {
	DLXNode *head;             // 总表头
	int rowCount, colCount;    // 本次样例矩阵的规模（行列数） 
	DLXNode *row, *col;        // 行首结点列表 / 列首结点列表
	
	DLXNode *dlx_pool;         // 结点对象池
	int dlx_pool_idx;              // 结点对象池下标
};
```




```cpp
	dlx_pool = new DLXNode[MAXR*MAXC];
	col = new DLXNode[MAXC+1];
	row = new DLXNode[MAXR];
```

###   **3、初始化**

​    1)设置本次问题的规模总行数rowCount，总列数colCount，结点对象池下标dlx_pool_idx置零；

​    2)初始化列首结点，将总表头head和col[i]在水平方向用left和right指针串联起来，col[i]的up和down指针指向自己，代表这列在矩阵中均为“0”；对于每个列首结点col[i]，将其列编号置为i，列结点总数colSum置零；

​    3)初始化行首结点，将行首结点row[i]的四个指针都指向自己，将其行编号rowIdx置为i，对应列首结点的指针置NULL；

###   **4、结点插入**

​    按行递增、列递增的方式枚举R×C的矩阵A，如果第r行第c列的值A[r][c] = 1，则插入一个(r, c)的结点：

​    1)取出结点对象池中的一个结点Node（注意需要返回指针或者引用）；

​    2)取列首结点col[c]，将它设置为Node的列首结点，并且将Node插入到col[c]和col[c]->up之间，将col[c]的结点总数colSum自增1；

​    3)取行首结点row[r]，将Node的行编号rowIdx设置为r，并且将Node插入到row[r]和row[r]->left之间；

###   **5、删列**

​    删除列c包含两步：

​    1)移除列首结点col[c]，这里的移除指只移除水平方向，竖直方向不作任何修改；

​    2)从列首结点col[c]往下枚举，将每个元素结点对应的行进行移除（即删行）；

###   **6、删行**

​    删除行r的操作只需要修改row[r]上所有元素结点的up和down指针，只移除竖直方向，水平方向不作任何修改；

###   **7、开始跳舞**

​    X算法的主体，具体步骤之前已经描述过，现直接给出深度优先搜索的实现如下：

```cpp
bool DLX::dance(int depth) {
	// 当前矩阵为空，说明找到一个可行解，算法终止 
	if(isEmpty()) {
		resultCount = depth;
		return true;
	}
	DLXNode *minPtr = get_min_col();
	// 删除minPtr指向的列 
	cover(minPtr);
	// minPtr为结点数最少的列，枚举这列上所有的行
	for(DLXNode *p = minPtr->down; p != minPtr; p = p->down) {
		// 令r = p->getRowIdx()，行r放入当前解 
		result[depth] = p->getRowIdx();
		// 行r上的结点对应的列进行删除 
		for(DLXNode *q = p->right; q != p; q = q->right) {
			cover(q->getColHead());
		}
		// 进入搜索树的下一层 
		if(dance(depth+1, maxDepth)) {
			return true;
		}
		// 行r上的结点对应的列进行恢复 
		for(DLXNode *q = p->left; q != p; q = q->left) {
			uncover(q->getColHead());
		}
	}
	// 恢复minPtr指向的列
	uncover(minPtr); 
	return false;
}
```



  其中cover和uncover对应列的删除和恢复，传入参数为列首结点的指针。

(注：本文结尾会给出Dancing Links X算法的完整代码)



## **五、精确覆盖的应用**

  对于精确覆盖问题有很多变形，但是不变的是都可以转化成01矩阵，并且矩阵的行代表问题的所有情况，矩阵的列代表问题的约束条件。

###    **1、开关切换问题  **

【例题5】N盏灯由M个开关控制（N<=100， M<=100），每个开关可以调出“-”、“0”、“+”三种状态，给出三维01矩阵T[M][N][3]。其中T[i][j][s]=1代表了第i个开关能够在s状态下将第j个灯点亮。特殊的，如果多个开关同时点亮某盏灯，会将它熄灭。求一种方案使得所有灯都被点亮。**   行代表问题的所有情况，列代表问题的约束条件。那么问题的所有情况，即所有开关的所有状态，情况数为3*M（即Dancing Links的01矩阵的每行代表每个开关的其中一种状态）。问题的约束条件有三个，其中第三个是隐含条件：
   1) 一盏灯只能被一个开关控制；
   2) 所有灯都亮起；
   3) 每种开关只能调一种状态；
   那么构建Dancing Links的01矩阵，矩阵前N列代表在这个开关的状态下，对应的灯是否亮起（“1”代表亮，“0”代表不亮）；后M列代表使用了对应的开关，也就是第i个开关代表的三行（因为有三种状态所以是三行）对应的列的值为“1”。
 所以总的矩阵规模为3*M行，N+M列的01矩阵，对前N列求一次精确覆盖即可。 

![img](https://img-blog.csdn.net/20180131194712238)

图五-1-1



###   **2、N皇后问题** 

  **【例题6】在一个N×N的棋盘上放置N个皇后(N <= 50)，使得任何两个皇后之间不相互攻击（即同一行、同一列、同一对角线不能有大于1个皇后）。求一种摆放方案。**



  经典N皇后问题有构造算法。这里介绍一下将问题转换成精确覆盖后用Dancing  Links X求解。

Dancing Links矩阵的

行代表问题的所有情况，列代表问题的约束条件。

每个皇后能够放置的位置总共有N^2种，也就是我们构建的Dancing  Links矩阵会有N^2行。

列分为四种约束条件：

   1）列[0, N)       代表了棋盘N行的占据情况

   2）列[N, 2N)      代表了棋盘N列的占据情况

   3）列[2N, 4N-1)    代表了棋盘2N-1条主对角线的占据情况

   4）列[4N-1, 6N-2)  代表了棋盘2N-1条副对角线的占据情况

  所以这是个行N^2，列6N-2的01矩阵。

 ![img](https://img-blog.csdn.net/20180131194806225)


 图五-2-1

  四个约束条件分情况讨论：对于(i,j)的位置，占据的行为i；占据的列为j；主对角线可以通过i和j的相对情况来判断，所有i-j相同的占据的主对角线为N+i-j；副对角线类似，所有i+j相同的位置占据的副对角线为i+j-1；

  枚举所有情况，对每个(i,j)（其中(i,j)属于[1，N]×[1, N]）建立对应的约束条件，就相当于建立了Dancing Links的01矩阵（见图五-2-1）。

  然而，N皇后问题不能直接求精确覆盖，因为我们发现，行和列必须完全覆盖到，但是主和副对角线没有要求一定要全部覆盖，所以我们问题的求解转变成对于这样一个01矩阵，求选出一些行，使得前2N列每列恰有一个“1”（这2N列分别对应行和列的约束条件）。那么也很简单，只需要修改两个地方：

   1）在选择“1”元素最少的列的时候只选择[0,2N)；

   2）如果[0，2N)列中都已经没有“1”可以选择了，那么算法终止；

  以上就是求解N皇后问题的全部过程。

![img](https://img-blog.csdn.net/20180131195034387)


图五-2-2

  **【思考题1】在一个N×N的棋盘上放置N个皇后(N <= 50)，使得任何两个皇后之间不相互攻击（即同一行、同一列、同一对角线不能有大于1个皇后），并且有K个皇后已经放置在某些互不攻击的位置，求放置剩余N-K个皇后满足N皇后问题。**

（提示：还是精确覆盖问题）



###   **3、骨牌覆盖问题** 



  **【例题7】****给出以下12块骨牌，每个骨牌只能用一次，但是可以旋转或者翻转，要求铺满一个M×N的棋盘，保证M×N  = 60。求方案数。如图五-3-2，为3×20的棋盘的其中一种方案。**

![img](https://img-blog.csdn.net/20180131195054045)

图五-3-1

![img](https://img-blog.csdn.net/20180131195103118)


图五-3-2

  先来考虑一个简化点的版本，假如每个骨牌无法旋转和翻转，该如何求解？

我们可以将每个骨牌尝试在M×N的棋盘的每个位置都去摆一遍。之前说了Dancing Links矩阵的行代表了问题的所有情况，那么每个骨牌在每个位置都放一遍正好对应了所有的这些情况（估计约60×12=720行）。而列的约束条件，分为两种：



1) 每个格子是否被占据；总共60个格子，即60列；

2) 当前骨牌放置方案用的是哪类骨牌，总共12列； 

对于条件1)每个格子都要被覆盖，对于条件2)每个骨牌都需要用到，于是就转化成了一个720×72的精确覆盖问题。

引入骨牌的翻转/旋转后，只是让矩阵的行数增加，列数不变，即能够摆放的情况数增多了。这里需要注意翻转和旋转后骨牌最多有8（2×4）种情况，但是如果一旦翻转或旋转得到的结果一样的话只能算一种，所以这里可以采用二进制哈希标记每种旋转/翻转状态。如图五-3-3，代表了Dancing  Links的矩阵。

![img](https://img-blog.csdn.net/20180131195115066)


图五-3-3

###   **4、数独问题**






 【例题8】对于一个N阶的数独，由N^2×N^2个格子组成，如图为一个3阶的数独。要求满足四个限制条件：

1) 每个格子只能填1个数；

2) 每行的数字集合为[1, N^2]，且不能重复；

3) 每列的数字集合为[1, N^2]，且不能重复；

4) 每个“宫”的数字集合为[1, N^2]，且不能重复（其中“宫”的意思就是N×N的格子。对于N=3的情况，就是“九宫格”）；

现在问题是给定一个已经填了一些数字的数独，求当N=3时的一种解,满足以上四个限制条件。

![img](https://img-blog.csdn.net/20180131195434447)

图五-4-1

转变为精确覆盖问题。行代表问题的所有情况，列代表问题的约束条件。每个格子能够填的数字为[1,9]，并且总共有9×9(即3^2×3^2)个格子，所以总的情况数为729种。也就是DancingLinks的行为729行。

列则分为四种：

1) [0, 81)列  分别对应了81个格子是否被放置了数字。

2) [82, 2*81)列  分别对应了9行，每行[1, 9]个数字的放置情况；

3) [2*81, 3*81)列 分别对应了9列，每列[1, 9]个数字的放置情况；

4) [3*81, 4*81)列 分别对应了9个“宫”，每“宫”[1, 9]个数字的放置情况；

所以总的列数为4*81=324列。如图五-4-2所示。

![img](https://img-blog.csdn.net/20180131195509253)


图五-4-2

  举个例子，对于在数独棋盘的i行j列的格子(i, j)上放置一个数字k，那么对应的Dancing Links的01矩阵行，一行上有四个“1”，分别对应四种约束条件：

​    1) 格子限制： 行号*9 + 列号

​    2) 行不重复限制： 81 + 行号*9 + (k-1)

​    3) 列不重复限制： 2*81 + 列号*9 + (k-1)

​    4) “宫”不重复限制：3*81 + 宫号*9 + (k-1)

  行号是i，列号是j，比较好理解；那么宫号我们定义如下图：

![img](https://img-blog.csdn.net/20180131195526330)

图五-4-3

  宫号的计算方式可以通过行号和列号得出。即 宫号 = (i/3)*3 + (j/3)；

  那么构建01矩阵的时候，我们从上到下，从左到右遍历数独，对于在(i, j)上有数字k的只需要插入一行，这行上有四列为“1”。对于没有填写数字的需要枚举[1, 9]，把在(i, j)位置上填[1, 9]的情况都进行插入，一共9行。

  矩阵构建完毕，求一次精确覆盖即可。

  **【思考题2】请设计一个四阶的数独的精确覆盖矩阵，并说说和三阶的区别是什么？**

 

## **六、重复覆盖**

###   **1、重复覆盖的定义**

  **【例题9】给定一个R×C（R, C <= 50）的01矩阵，问是否存在这样一个行集合，使得集合中每一列至少一个“1”。**

  

重复覆盖是精确覆盖的一般情况，限制条件远远没有精确覆盖强。回忆一下X的算法思路，我们发现重复覆盖可以参照精确覆盖的方法构建Dancing Links链表，然后枚举行的选取，进而删除该行上有“1”的列，但是仅此而已，无法再删除列对应的行。如图六-1-1所示，选择第一行（红色框），然后删除蓝色的列（试想一下，如果是精确覆盖，我们还可以删除绿色的行），然而重复覆盖无法删除绿色的行，这是因为选取的行集合允许在每列上有多个“1”，如果过多的删除有可能导致可行解的擦肩而过。

![img](https://img-blog.csdn.net/20180131195547568)


图六-1-1

 这样带来的问题就是矩阵规模的下降速度会大大减慢，从而使得搜索的状态空间树十分庞大，这时候往往需要剪枝，在介绍剪枝之前，让我们先来看一个更加复杂的情况。

  **【例题10】给定一个R×C（R, C <= 50）的01矩阵，找出最少的行集合，使得集合中每一列至少****一个“1”。**


为了满足搜索的行数最小这个条件，我们需要引入迭代加深。



###   **2、迭代加深（IDA\*）**


迭代加深，顾名思义，就是深度的迭代。即枚举一个最大深度，然后对问题进行搜索，搜索过程记录当前深度，如果当前深度大于最大深度则无条件返回。例如，假设枚举的最大深度为3，那么搜索选取01矩阵的行时最多只能选择3行，当深度大于3深搜函数必须返回。

那么可以枚举深度，然后再进行搜索。



###   **3、启发式函数**

引入最大深度的原因，除了能第一时间找到“最少”，更大程度上是便于启发性剪枝。考虑到当前枚举深度depth，最大枚举深度maxDepth，令K  = ()。则在这种情况下，还有K步决策，或者说是只能再最多选择K行，那么如果我们能够设计一个估价函数H()，函数返回的是至少还需要多少行才能完成重复覆盖（这是个估计值，不是确定的）。并且H() > K，则代表当前搜索条件下，头已经不可能搜到可行解了，可以直接返回，使得搜索树的一些分支不需要再进行无谓的搜索，此所谓“剪枝”。
 H()函数是一个估计值，并不能精确计算出来（如果能精确计算出来，那问题本身就可以直接用这个函数来计算了），并且这个估计值一定是要比实际值小的，即 实际值 > 估计值H() > K。
 H()函数原理：X算法的终止条件是列为空，那么我们现在要做的就是要模拟删除所有的列，这里说的删除并不是真正的删除，而是做一个标记。假设Dancing Links 的01矩阵的列数小于64，那么每一行可以压缩成一个INT64的整型（当然，如果列数大于64的话，可以压缩在一个INT64的数组里，总之目的就是利用位运算减少轮询操作），用R[i]表示第i行的那个64位整数。用一个全局标记X来记录剩下列的模拟删除情况（X的二进制第i位为“1”代表第i列已经被模拟删除）。
 H()函数计算过程：任意找一个未被模拟删除的列c，计数器cnt+1，选中列c上有“1”的行r，令X = X or R[r]，依次往复直到不存在这样的列c。最后的计数器cnt就是那个估计值。

###   **4、引用计数**

重复覆盖的时候，每次选择一行，删除行上有“1”的列时，有可能会枚举到已经删除的列，如果已经删除则需要进行标记，但是不能标记为已经“删除”和“未删除”两种状态。因为除了删除，还需要恢复，所以删除的状态其实是有“被删除0次”、“被删除1次”、“被删除2次”、...“被删除N次”这样的多种状态组成的。

正确做法是用一个标记数组D[i]，标记第i列删除的次数。每次执行删除时，标记+1，并且判断标记为1才执行删除；每次执行恢复时，标记-1，并且判断标记为0时才执行恢复。

## **七、重复覆盖的应用**

###   **1、回顾彩票问题**


**相信经过前面的解说，彩票那个问题都已经会了，直接转化成01矩阵后，利用IDA\*求解重复覆盖即可。**

 

###   **2、雷达系统** 

  **【例题11】给定N个城市和M个雷达的位置，雷达扫描范围为圆形，半径未知（N,M <= 50）。现在想选出其中K个雷达，覆盖所有的城市，为了节约成本，希望扫描半径最小。求这个最小的扫描半径。** 



  这个问题非常经典，曾经是百度之星决赛的题目。很容易看出来的是，扫描半径如果无限大，那么必然可以找到K个雷达覆盖所有城市；同理，如果扫描半径为0，那么这件事情就很难办到。雷达覆盖所有城市的概率相对于扫描半径的大小单调递增，换言之，扫描半径越大，雷达覆盖城市越容易。

​    于是，我们可以二分枚举这个半径R，然后根据这个半径来建立雷达和城市之间的关系矩阵A。A[i][j]=1当且仅当第i个雷达到第j个城市的距离小于等于R，建立好关系矩阵后，我们发现，现在的问题就是看能不能选择K行（每行代表每个雷达）满足每行上的列都至少有一个“1”（每个列代表每个城市），完美转化成重复覆盖问题。

###   **3、支配集问题**


重复覆盖问题其实是个支配问题，用“行”来支配“列”，任何能够转化为行列关系的问题，都可以转换成重复覆盖问题进行求解。



## **八、跳舞链相关题集整理**

###  精确覆盖

|题目|难度||
|----|----|----|
| [EasyFinding](http://poj.org/problem?id=3740)              |        ★★☆☆☆      |     赤裸精确覆盖|
| [TreasureMap](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemId=3372)                    |  ★★☆☆☆     |      经典精确覆盖|
| [Dominoes](http://acm.hdu.edu.cn/showproblem.php?pid=2518)    |                     ★★★☆☆  |        | 经典骨牌覆盖|
| [APuzzlingProblem](http://acm.hdu.edu.cn/showproblem.php?pid=1603)    |             ★★☆☆☆      |     经典骨牌覆盖|
| [NQUEEN](http://www.spoj.com/problems/NQUEEN/)                |           ★★★☆☆        |   N皇后问题|
| [Lamp](http://acm.hdu.edu.cn/showproblem.php?pid=2828)                |             ★★★☆☆         |  精确覆盖|
| [PowerStations](http://acm.hdu.edu.cn/showproblem.php?pid=3663)         |           ★★★☆☆          | 精确覆盖|

 

###  数独系列

|题目|难度||
|----|----|----|
| [SudokuKiller](http://acm.hdu.edu.cn/showproblem.php?pid=1426)                 |    ★☆☆☆☆           |基础题|
| [Su-Su-Sudoku](http://acm.hdu.edu.cn/showproblem.php?pid=2780)             |        ★☆☆☆☆     |      基础题|
| [Sudoku](http://acm.hdu.edu.cn/showproblem.php?pid=3111)         |                  ★☆☆☆☆           |基础题|
| [Sudoku](http://acm.hdu.edu.cn/showproblem.php?pid=5547)        |                   ★☆☆☆☆  |         基础题|
| [Sudoku](http://poj.org/problem?id=2676)       |                    ★★☆☆☆   |        基础题|
| [Sudoku](http://poj.org/problem?id=3074)           |                ★★☆☆☆       |    基础题|
| [Sudoku](http://poj.org/problem?id=3076)                     |      ★★☆☆☆        |   基础题|
| [Sudoku](http://acm.hdu.edu.cn/showproblem.php?pid=3476)               |            ★★★☆☆           |有意思的题|
| [Sudoku](http://acm.hdu.edu.cn/showproblem.php?pid=3909)              |             ★★★☆☆           |最全的数独题|
| [SquigglySudoku](http://acm.hdu.edu.cn/showproblem.php?pid=4069)         |          ★★★☆☆          | 数独+连通分量|

 

###  重复覆盖

|题目|难度||
|----|----|----|
| [神龙的难题](http://acm.fzu.edu.cn/problem.php?pid=1686)              |           ★★☆☆☆      |     基础重复覆盖|
| [whosyourdaddy](http://acm.hdu.edu.cn/showproblem.php?pid=3498)       |             ★★★☆☆  |         重复覆盖A*基础|
| [Radar](http://acm.hdu.edu.cn/showproblem.php?pid=2295)                     |       ★★★☆☆           |二分+重复覆盖A*|
| [Bomberman](http://acm.hdu.edu.cn/showproblem.php?pid=3529)            |            ★★★☆☆        |   重复覆盖A*|
| [RepairDepots](http://acm.hdu.edu.cn/showproblem.php?pid=3156)          |           ★★★★☆           |三角形外心+重复覆盖A*|
| [Firestation](http://acm.hdu.edu.cn/showproblem.php?pid=3656)              |        ★★★☆☆           |重复覆盖A*|
| [StreetFighter](http://acm.hdu.edu.cn/showproblem.php?pid=3957)               |     ★★★★☆         |  精确覆盖+重复覆盖|
| [SquareDestroyer](http://poj.org/problem?id=1084)               |   ★★☆☆☆      |     古董题|
| [Airport](http://acm.hdu.edu.cn/showproblem.php?pid=5046)                 |         ★★☆☆☆           |基础重复覆盖A*|
| [ASimpleMathProblem](http://acm.hdu.edu.cn/showproblem.php?pid=4979)               |★★☆☆☆       |    重复覆盖+打表|





 

Dancing Links X算法C++代码：[DLX完整代码](https://github.com/WhereIsHeroFrom/Code_Templates/blob/master/Dancing%20Links%20%E6%A8%A1%E6%9D%BF.cpp)

Dancing Links X算法 解题报告：[DLX解题报告](http://blog.csdn.net/whereisherofrom/article/details/79188290)


