# 带权最短路 Dijkstra, SPFA, Bellman-Ford, ASP, Floyd-Warshall 算法分析

August 4, 2013 / [算法](https://www.renfei.org/blog/category/algorithm/)

图论中，用来求最短路的方法有很多，适用范围和时间复杂度也各不相同。

本文主要介绍的算法的代码主要来源如下：

1. Dijkstra: *Algorithms*（《算法概论》）Sanjoy Dasgupta, Christos Papadimitriou, Umesh Vazirani;《算法竞赛入门经典—训练指南》刘汝佳、陈峰。
2. SPFA (Shortest Path Faster Algorithm): *Data Structure and Algorithms Analysis in C*, 2nd ed.（《数据结构与算法分析》）Mark Allen Weiss.
3. Bellman-Ford: *Algorithms*（《算法概论》）Sanjoy Dasgupta, Christos Papadimitriou, Umesh Vazirani.
4. ASP (Acyclic Shortest Paths): *Introduction to Algorithms - A Creative Approach*（《算法引论—一种创造性方法》）Udi Manber.
5. Floyd-Warshall: *Data Structure and Algorithms Analysis in C*, 2nd ed.（《数据结构与算法分析》）Mark Allen Weiss.

它们的使用限制和运行时间如下：

1. Dijkstra: 不含负权。运行时间依赖于优先队列的实现，如 *O*((∣*V*∣+∣*E*∣)log∣*V*∣)



SPFA: 无限制。运行时间*O*(*k*⋅∣*E*∣) (*k*≪∣*V*∣)



Bellman-Ford：无限制。运行时间*O*(∣*V*∣⋅∣*E*∣)



ASP: 无圈。运行时间*O*(∣*V*∣+∣*E*∣)



Floyd-Warshall: 无限制。运行时间*O*(∣*V*∣3)

1. 

其中  1~4 均为单源最短路径 (Single Source Shortest Paths) 算法； 5 为全源最短路径 (All Pairs  Shortest Paths)  算法。顺便说一句，为什么没有点对点的最短路径？如果我们只需要一个起点和一个终点，不是比计算一个起点任意终点更节省时间么？答案还真不是，目前还没有发现比算从源点到所有点更快的算法。

## 图的表示

本文中，前四个算法的图都采用邻接表表示法，如下：

```cpp
struct Edge
{
    int from;
    int to;
    int weight;

    Edge(int f, int t, int w):from(f), to(t), weight(w) {}
};

int num_nodes;
int num_edges;
vector<Edge> edges;
vector<int> G[max_nodes]; // 每个节点出发的边编号
int p[max_nodes]; // 当前节点单源最短路中的上一条边
int d[max_nodes]; // 单源最短路径长
```

## Dijkstra 方法

Dijkstra   方法依据其优先队列的实现不同，可以写成几种时间复杂度不同的算法。它是图论－最短路中最经典、常见的算法。关于这个方法，网上有许多分析，但是我最喜欢的还是《算法概论》中的讲解。为了理解  Dijkstra 方法，首先回顾一下无权最短路的算法。无权最短路算法基于  BFS，每次从源点向外扩展一层，并且给扩展到的顶点标明距离，这个距离就是最短路的长。我们完全可以仿照这个思路，把带权图最短路问题规约到无权图最短路问题——只要把长度大于  1 的边填充进一些「虚顶点」即可。如下图所示。

![Dijkstra](https://img.renfei.org/2013/08/11.png)

这个办法虽然可行，但是显然效率很低。不过，Dijkstra 方法*E**C*,*E**B*,*E**D*

分别出发，经过一系列「虚节点」，依次到达

*D*,*B*,*C*

 。为了不在虚节点处浪费时间，出发之前，我们设定三个闹钟，时间分别为

4,3,2

提醒我们预计在这些时刻会有重要的事情发生（经过实际节点）。更一般地说，假设现在我们处理到了某个顶点

*u*

，和

*u*

相邻接的顶点为

*v*1,*v*2,…,*v**n*

，它们和

*u*

的距离为

*d*1,*d*2,…,*d**n*

。我们为

*v*1,*v*2,…,*v**n*

各设定一个闹钟。如果还没有设定闹钟，那么设定为

*d*

 ；如果设定的时间比

*d*

晚，那么重新设定为

*d*

（此时我们沿着这条路比之前的某一条路会提前赶到）。每次闹钟响起，都说明可能经过了实际节点，我们都会更新这些信息，直到不存在任何闹钟。综上所述，也就是随着  BFS  的进行，我们一旦发现更近的路径，就立即更新路径长，直到处理完最后（最远）的一个顶点。由此可见，由于上述「虚顶点」并非我们关心的实际顶点，因此  Dijkstra 方法的处理方式为：直接跳过了它们。

还需要解决的一个问题，就是闹钟的管理。闹钟一定是从早到晚按顺序响起的，然而我们设闹钟的顺序却不一定按照时间升序，因此需要一个优先队列来管理。Dijkstra 方法实现的效率严重依赖于优先队列的实现。一个使用标准库容器适配器 `priority_queue` 的算法版本如下：

```cpp
typedef pair<int, int> HeapNode;
void Dijkstra(int s)
{
    priority_queue< HeapNode, vector<HeapNode>, greater<HeapNode> > Q;
    for (int i=0; i<num_nodes; ++i)
        d[i] = __inf;

    d[s] = 0;
    Q.push(make_pair(0, s));
    while (!Q.empty()) {
        pair<int, int> N = Q.top();
        Q.pop();
        int u = N.second;
        if (N.first != d[u]) continue;
        for (int i=0; i<G[u].size(); ++i) {
            Edge &e = edges[G[u][i]];
            if (d[e.to] > d[u] + e.weight) {
                d[e.to] = d[u] + e.weight;
                p[e.to] = G[u][i];
                Q.push(make_pair(d[e.to], e.to));
            }
        }
    }
}
```

## Bellman-Ford：一个简单的想法

Dijkstra 方法的本质是进行一系列如下的**更新操作**：

*d*(*v*)=min{*d*(*v*), *d*(*u*)+*l*(*u*,*v*)}

然而，如果边权含有负值，那么 Dijkstra 方法将不再适用。原因解释如下。

假设最终的最短路径为：

*s*→*u*1→*u*2→*u*3→…→*u**k*→*t*

不难看出，如果按照 (*s*, *u*1), (*u*1, *u*2), …,(*u**k*, *t*)

 的顺序执行上述更新操作，最终

*t*

的最短路径一定是正确的。而且，只要保证上述更新操作全部按顺序执行即可，并不要求上述更新操作是连续进行的。Dijkstra 算法所运行的更新序列是经过选择的，而选择基于这一假设：

*s*→*t*

的最短路一定

不会经过

和

*s*

距离大于

*l*(*s*, *t*)

的点。对于正权图这一假设是显然的，对于负权图这一假设是错误的。

因此，为了求出负权图的最短路径，我们需要保证一个合理的更新序列。但是，我们并不知道最终的最短路径！因此一个简单的想法就是：更新所有的边，每条边都更新∣*V*∣−1

次。由于多余的更新操作总是无害的，因此算法（几乎）可以正确运行。等等，为什么是

∣*V*∣−1

次？这是由于，任何含有

∣*V*∣

个顶点的图两个点之间的最短路径最多含有

∣*V*∣−1

条边。这意味着最短路不会包含环。理由是，如果是负环，最短路不存在；如果是正环，去掉后变短；如果是零环，去掉后不变。

算法实现中唯一一个需要注意的问题就是**负值圈**  (negative-cost  cycle)。负值圈指的是，权值总和为负的圈。如果存在这种圈，我们可以在里面滞留任意长而不断减小最短路径长，因此这种情况下最短路径可能是不存在的，可能使程序陷入无限循环。好在，本文介绍的几种算法都可以判断负值圈是否存在。对于  Bellman-Ford 算法来说，判断负值圈存在的方法是：在∣*V*∣−1

次循环之后再执行一次循环，如果还有更新操作发生，则说明存在负值圈。

Bellman-Ford 算法的代码如下：

```cpp
bool Bellman_Ford(int s)
{
    for (int i=0; i<num_nodes; ++i)
        d[i] = __inf;

    d[s] = 0;
    for (int i=0; i<num_nodes; ++i) {
        bool changed = false;
        for (int e=0; e<num_edges; ++e) {
            if (d[edges[e].to] > d[edges[e].from] + edges[e].weight 
               && d[edges[e].from] != __inf) {
                d[edges[e].to] = d[edges[e].from] + edges[e].weight;
                p[edges[e].to] = e;
                changed = true;
            }
        }
        if (!changed) return true;
        if (i == num_nodes && changed) return false;
    }
    return false; // 程序应该永远不会执行到这里
}
```

注记：

1. 如果某次循环没有更新操作发生，以后也不会有了。我们可以就此结束程序，避免无效的计算。
2. 上述程序中第 11 行的判断：如果去掉这个判断，只要图中存在负值圈函数就会返回 `false`。否则，仅在给定源点可以达到负值圈时才返回 `false`。

## SPFA：一个改进的想法

看来，Bellman-Ford  算法多少有些浪费。这里介绍的 SPFA 可以算作是 Bellman-Ford 的队列改进版。在 Dijkstra 方法中，随着 BFS  的进行，最短路一点一点地「生长」。然而如果存在负权，我们的算法必须允许「绕回去」的情况发生。换句话说，我们需要在某些时候撤销已经形成的最短路。同时，我们还要改变  Bellman-Ford 算法盲目更新的特点，只更新有用的节点。SPFA 中，一开始，我们把源点 *s*

放入队列中，然后每次循环让一个顶点

*u*

出队，找出所有与

*u*

邻接的顶点

*v*

，更新最短路，并当

*v*

不在队列里时将它入队。循环直到队列为空（没有需要更新的顶点）。

可以显示出 SPFA 和 Bellman-Ford 算法相比的一个重大改进的最经典的一个例子，就是图为一条链的情形。

容易看出，如果存在负值圈，这个算法将无限循环下去。因此需要一个机制来确保算法得以中止。由于最短路最长只含有∣*V*∣−1

条边，因此如果任何一个顶点已经出队

∣*V*∣+1

次，算法停止运行。

SPFA 的代码如下：

```cpp
bool in_queue[max_nodes];
int cnt[max_nodes];
bool SPFA(int s)
{
    int u;
    queue<int> Q;
    memset(in_queue, 0, sizeof(in_queue));
    memset(cnt, 0, sizeof(cnt));
    for (int i=0; i<num_nodes; ++i)
        d[i] = __inf;

    d[s] = 0;
    in_queue[s] = true;
    Q.push(s);
    while (!Q.empty()) {
        in_queue[u=Q.front()] = false;
        Q.pop();
        for (int i=0; i<G[u].size(); ++i) {
            Edge &e = edges[G[u][i]];
            if (d[e.to] > d[u] + e.weight) {
                d[e.to] = d[u] + e.weight;
                p[e.to] = G[u][i];
                if (!in_queue[e.to]) {
                    Q.push(e.to);
                    in_queue[e.to] = true;
                    if (++cnt[e.to] > num_nodes)
                        return false;
                }
            }
        }
    }
    return true;
}
```

我们已经给出 SPFA 的运行时间为*O*(*k*⋅∣*E*∣) (*k*≪∣*V*∣)

。实际上，可以

期望

*k*<2

。但是可以构造出使 SPFA 算法变得很慢的针对性数据。

## Acyclic Shortest Path

如果图是无圈的（acyclic）（或称为有向无环图，DAG），那么情况就变的容易了。我们可以构造一个**拓扑升序序列**，由拓扑排序的性质，无圈图的任意路径中，顶点都是沿着「拓扑升序序列」排列的，因此我们只需要按照这个序列执行更新操作即可。显然，这样可以在线性时间内解决问题。

实现上，拓扑排序和更新可以一趟完成。这种算法的代码如下：

```cpp
int indegree[max_nodes];
void asp(int s)
{
    queue<int> Q;
    for (int i=0; i<num_nodes; ++i) {
        d[i] = __inf;
        indegree[i] = 0;
    }
    for (int i=0; i<num_edges; ++i)
        ++indegree[edges[i].to];
    for (int i=0; i<num_nodes; ++i)
        if (indegree[i] == 0) Q.push(i);

    d[s] = 0;
    while (!Q.empty()) {
        int w = Q.front();
        Q.pop();
        for (int i=0; i<G[w].size(); ++i) {
            if (d[edges[G[w][i]].to] > d[w] + edges[G[w][i]].weight && d[w] != __inf) { 
                d[edges[G[w][i]].to] = d[w] + edges[G[w][i]].weight;
                p[edges[G[w][i]].to] = G[w][i];
            }
            if (--indegree[edges[G[w][i]].to] == 0)
                Q.push(edges[G[w][i]].to);
        }
    }
}
```

## Floyd-Warshall

与前面四种不同，Floyd-Warshall 算法是所谓的「全源最短路径」，也就是任意两点间的最短路径。它并**不是**对单源最短路径∣*V*∣

次迭代的一种**渐进改进**，但是对非常稠密的图却可能更快，因为它的循环更加紧凑。而且，这个算法支持负的权值。

Floyd-Warshall 算法如下：

```cpp
int dist[max_nodes][max_nodes]; // 记录路径长
int path[max_nodes][max_nodes]; // 记录实际路径
bool Floyd_Warshall()
{
    for (int i=0; i<num_nodes; ++i)
        for (int j=0; j<num_nodes; ++j)
            path[i][j] = j;

    for (int k=0; k<num_nodes; ++k) {
        for (int i=0; i<num_nodes; ++i) {
            for (int j=0; j<num_nodes; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]
                 && dist[i][k] != __inf && dist[k][j] != __inf) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[i][k];
                    if (i == j && dist[i][j] < 0)
                        return false;
                }
            }
        }
    }
    return true;
}
```

其中 `dist` 数组应初始化为邻接矩阵。需要提醒的是， `dist[i][i]` 实际上表示「从顶点 i 绕一圈再回来的最短路径」，因此图存在负环当且仅当 dist[i][i]<0。初始化时， `dist[i][i]`可以初始化为`0`，也可以初始化为∞

 。

## 显示实际路径

显示实际路径实际上非常简单。利用前四个算法提供的 `int *p`，还原实际路径的一个方法如下：

```cpp
void printpath(int from, int to, bool firstcall = true)
{
    if (from == to) {
        printf("%d", from);
        return;
    }
    if (p[to] == -1) return;
    if (firstcall) printf("%d ->", from);
    int v = edges[p[to]].from;
    if (v == from) {
        if (firstcall) printf(" %d", to);
        return;
    }
    printpath(from, v, false);
    printf(" %d ->", v+1);
    if (firstcall) printf(" %d", to);
}
```

利用 Floyd-Warshall 算法提供的 `int **path`，还原实际路径的一个方法如下：

```cpp
void showpath(int from, int to)
{
    int c = from;
    printf("%d -> %d:(%2d)  %d", from, to, dist[from][to], from);
    while (c != to) {
        printf(" -> %d", path[c][to]);
        c = path[c][to];
    }
    printf("\n");
}
```

 来自话题 [algorithm](https://www.renfei.org/blog/topic/algorithm/) / [dijkstra](https://www.renfei.org/blog/topic/dijkstra/) / [spfa](https://www.renfei.org/blog/topic/spfa/) / [bellman-ford](https://www.renfei.org/blog/topic/bellman-ford/) / [floyd-warshall](https://www.renfei.org/blog/topic/floyd-warshall/)