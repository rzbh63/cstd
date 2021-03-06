
# 问题：找出二个集合S1与S2之间的最接近点对的距离（包括一个点集中最接近点对的讨论）复杂度为O（nlogn） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年12月28日 18:43:11[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：5210



# 问题：找出二个集合S1与S2之间的最接近点对的距离
这个问题很容易理解，似乎也不难解决。我们只要将S1每一点与S2每一个的距离算出，找出达到最小距离的两个点即可。然而，这样做效率太低，需要*O*(n2)的计算时间。在问题的计算复杂性中我们可以看到，一个集合S找出最接近点对的计算时间下界为*Ω*(*n*log*n*)。这个下界引导我们去找这个问题的一个*θ*(*n*log*n*)算法。
## 一．先讨论一个集合最接近点对的情况(参考《计算机算法设计与分析》)
这个问题显然满足分治法的第一个和第二个适用条件，我们考虑将所给的平面上n个点的集合S分成2个子集S1和S2，每个子集中约有n/2个点，·然后在每个子集中递归地求其最接近的点对。在这里，一个关键的问题是如何实现分治法中的合并步骤，即由S1和S2的最接近点对，如何求得原集合S中的最接近点对，因为S1和S2的最接近点对未必就是S的最接近点对。如果组成S的最接近点对的2个点都在S1中或都在S2中，则问题很容易解决。但是，如果这2个点分别在S1和S2中，则对于S1中任一点p，S2中最多只有n/2个点与它构成最接近点对的候选者，仍需做n2/4次计算和比较才能确定S的最接近点对。因此，依此思路，合并步骤耗时为*O*(n2)。整个算法所需计算时间T(n)应满足:　 
 T(n)=2T(n/2)+*O*(n2)
它的解为T(n)=*O*(n2)，即与合并步骤的耗时同阶，显示不出比用穷举的方法好。从解递归方程的套用公式法，我们看到问题出在合并步骤耗时太多。这启发我们把注意力放在合并步骤上。
为了使问题易于理解和分析，我们先来考虑一维的情形。此时S中的n个点退化为x轴上的n个实数x1,x2,..,xn。最接近点对即为这n个实数中相差最小的2个实数。我们显然可以先将x1,x2,..,xn排好序，然后，用一次线性扫描就可以找出最接近点对。这种方法主要计算时间花在排序上，因此如在排序算法中所证明的，耗时为*O*(*n*log*n*)。然而这种方法无法直接推广到二维的情形。因此，对这种一维的简单情形，我们还是尝试用分治法来求解，并希望能推广到二维的情形。
假设我们用x轴上某个点m将S划分为2个子集S1和S2，使得S1={x∈S|x≤m}；S2={x∈S|x>m}。这样一来，对于所有p∈S1和q∈S2有p<q。
递归地在S1和S2上找出其最接近点对{p1,p2}和{q1,q2}，并设*δ*=min{|p1-p2|,|q1-q2|}，S中的最接近点对或者是{p1,p2}，或者是{q1,q2}，或者是某个{p3,q3}，其中p3∈S1且q3∈S2。如图1所示。
[
](http://algorithm.chaoskey.com/02/08/image022.gif?attredirects=0)
图1 一维情形的分治法
我们注意到，如果S的最接近点对是{p3,q3}，即|p3-q3|<*δ*，则p3和q3两者与m的距离不超过*δ*，即|p3-m|<*δ*，|q3-m|<*δ*，也就是说，p3∈(m-*δ*,m]，q3∈(m,m+*δ*]。由于在S1中，每个长度为*δ*的半闭区间至多包含一个点（否则必有两点距离小于*δ*），并且m是S1和S2的分割点，因此(m-*δ*,m]中至多包含S中的一个点。同理，(m,m+*δ*]中也至多包含S中的一个点。由图1可以看出，如果(m-*δ*,m]中有S中的点，则此点就是S1中最大点。同理，如果(m,m+*δ*]中有S中的点，则此点就是S2中最小点。因此，我们用线性时间就能找到区间(m-*δ*,m]和(m,m+*δ*]中所有点，即p3和q3。从而我们用线性时间就可以将S1的解和S2的解合并成为S的解。也就是说，按这种分治策略，合并步可在*O*(n)时间内完成。这样是否就可以得到一个有效的算法了呢？还有一个问题需要认真考虑，即分割点m的选取，及S1和S2的划分。选取分割点m的一个基本要求是由此导出集合S的一个线性分割，即S=S1∪S2，S1∩S2=*Φ*，且S1{x|x≤m}；S2{x|x>m}。容易看出，如果选取m=[max(S)+min(S)]/2，可以满足线性分割的要求。选取分割点后，再用*O*(n)时间即可将S划分成S1={x∈S|x≤m}和S2={x∈S|x>m}。然而，这样选取分割点m，有可能造成划分出的子集S1和S2的不平衡。例如在最坏情况下，|S1|=1，|S2|=n-1，由此产生的分治法在最坏情况下所需的计算时间T(n)应满足递归方程:
T(n)=T(n-1)+*O*(n)
它的解是T(n)=*O*(n2)。这种效率降低的现象可以通过分治法中“平衡子问题”的方法加以解决。也就是说，我们可以通过适当选择分割点m，使S1和S2中有大致相等个数的点。自然地，我们会想到用S的n个点的坐标的中位数来作分割点。在选择算法中介绍的选取中位数的线性时间算法使我们可以在*O*(n)时间内确定一个平衡的分割点m。
至此，我们可以设计出一个求一维点集S中最接近点对的距离的算法CPAIR1如下。
function CPAIR1(S);
begin
if |S|=2 then δ=|x[2]-x[1]| // x[1..n]存放的是S中n个点的坐标
else if (|S|=1)
then δ:=∞
else begin
m:=S中各点的坐标值的中位数;
构造S1和S2,使S1={x∈S|x≤m}，S2={x∈S|x>m};
δ1:=CPAIRI(S1);
δ2:=CPAIRI(S2);
p:=max(S1);
q:=min(S2);
δ:=min(δ1,δ2,q-p);
end;
return(δ);
end;
由以上的分析可知，该算法的分割步骤和合并步骤总共耗时*O*(n)。因此，算法耗费的计算时间T(n)满足递归方程：
解此递归方程可得*T*(*n*)=*O*(*n*log*n*)。
**这个算法看上去比用排序加扫描的算法复杂，然而这个算法可以向二维推广。**
下面我们来考虑二维的情形。此时S中的点为平面上的点，它们都有2个坐标值x和y。为了将平面上点集S线性分割为大小大致相等的2个子集S1和S2，我们选取一垂直线l:x=m来作为分割直线。其中m为S中各点x坐标的中位数。由此将S分割为S1={p∈S|px≤m}和S2={p∈S|px>m}。从而使S1和S2分别位于直线l的左侧和右侧，且S=S1∪S2。由于m是S中各点x坐标值的中位数，因此S1和S2中的点数大致相等。
递归地在S1和S2上解最接近点对问题，我们分别得到S1和S2中的最小距离*δ*1和*δ*2。现设*δ*=min(*δ*1,*δ*1)。若S的最接近点对(p,q)之间的距离d(p,q)<*δ*则p和q必分属于S1和S2。不妨设p∈S1，q∈S2。那么p和q距直线l的距离均小于*δ*。因此，我们若用P1和P2分别表示直线l的左边和右边的宽为*δ*的2个垂直长条，则p∈S1，q∈S2，如图2所示。
图2 距直线l的距离小于*δ*的所有点
在一维的情形，距分割点距离为*δ*的2个区间(m-*δ*,m](m,m+*δ*]中最多各有S中一个点。因而这2点成为唯一的末检查过的最接近点对候选者。二维的情形则要复杂些，此时，P1中所有点与P2中所有点构成的点对均为最接近点对的候选者。在最坏情况下有n2/4对这样的候选者。但是P1和P2中的点具有以下的稀疏性质，它使我们不必检查所有这n2/4对候选者。考虑P1中任意一点p,它若与P2中的点q构成最接近点对的候选者，则必有d(p,q)<*δ*。满足这个条件的P2中的点有多少个呢？容易看出这样的点一定落在一个*δ*×2*δ*的矩形R中，如图3所示。
图3 包含点q的*δ*×2*δ*的矩形R
由*δ*的意义可知P2中任何2个S中的点的距离都不小于*δ*。由此可以推出矩形R中最多只有6个S中的点。事实上，我们可以将矩形R的长为2*δ*的边3等分，将它的长为*δ*的边2等分，由此导出6个（*δ*/2）×（2*δ/3）*的矩形。如图4(a)所示。
图4 矩形R中点的稀疏性
若矩形R中有多于6个S中的点，则由鸽舍原理易知至少有一个*δ*×2*δ*的小矩形中有2个以上S中的点。设u,v是这样2个点，它们位于同一小矩形中，则
因此d(u,v)≤5*δ*/6<*δ*。这与*δ*的意义相矛盾。也就是说矩形R中最多只有6个S中的点。图4(b)是矩形R中含有S中的6个点的极端情形。由于这种稀疏性质，对于P1中任一点p，P2中最多只有6个点与它构成最接近点对的候选者。因此，在分治法的合并步骤中，我们最多只需要检查6×n/2=3n对候选者，而不是n2/4对候选者。这是否就意味着我们可以在*O*(n)时间内完成分治法的合并步骤呢？现在还不能作出这个结论，因为我们只知道对于P1中每个S1中的点p最多只需要检查P2中的6个点，但是我们并不确切地知道要检查哪6个点。为了解决这个问题，我们可以将p和P2中所有S2的点投影到垂直线l上。由于能与p点一起构成最接近点对候选者的S2中点一定在矩形R中，所以它们在直线l上的投影点距p在l上投影点的距离小于*δ*。由上面的分析可知，这种投影点最多只有6个。因此，若将P1和P2中所有S的点按其y坐标排好序，则对P1中所有点p，对排好序的点列作一次扫描，就可以找出所有最接近点对的候选者，对P1中每一点最多只要检查P2中排好序的相继6个点。
至此，我们可以给出用分治法求二维最接近点对的算法CPAIR2如下:
function CPAIR2(S);
begin
if |S|=2 then δ:=S中这2点的距离
else if |S|=0
then δ:=∞
else begin
1.  m:=S中各点x坐标值的中位数;
构造S1和S2，使S1={p∈S|px≤m}和S2={p∈S|px>m}
2.  δ1:=CPAIR2(S1);δ2:=CPAIR2(S2);
3.  δm:=min(δ1,δ2);
4.  设P1是S1中距垂直分割线l的距离在δm之内的所有点组成的集合，P2是S2中距分割线l的距离在δm之内所有点组成的集合。将P1和 P2中的点依其y坐标值从小到大排序，并设P1*和P2*是相应的已排好序的点列;
5.  通过扫描P1*以及对于P1*中每个点检查P2*中与其距离在δm之内的所有点(最多6个)可以完成合并。当P1*中的扫描指针逐次向上移动时，P2*中的扫描指针可在宽为2δm的一个区间内移动。设δl是按这种扫描方式找到的点对间的最小距离;
6.  δ=min(δm,δl);
end;
return(δ);
end;
下面分析一下算法CPAIR2的计算复杂性。设对于n个点的平面点集S，算法耗时T(n)。算法的第1步和第5步用了*O*(n)时间，第3步和第6步用了常数时间，第2步用了2T(n/2)时间。若在每次执行第4步时进行排序，则在最坏情况下第4步要用*O*(*n*log*n*)时间。这不符合我们的要求。因此，在这里我们要作一个技术上的处理。我们采用设计算法时常用的预排序技术，即在使用分治法之前，预先将S中n个点依其y坐标值排好序，设排好序的点列为P*。在执行分治法的第4步时，只要对P*作一次线性扫描，即可抽取出我们所需要的排好序的点列P1*和P2*。然后，在第5步中再对P1*作一次线性扫描，即可求得*δ*l。因此，第4步和第5步的两遍扫描合在一起只要用*O*(n)时间。这样一来，经过预排序处理后的算法CPAIR2所需的计算时间T(n)满足递归方程：
显而易见T(n)=*O*(*n*log*n*)，预排序所需的计算时间为*O*(*n*1og*n*)。因此，整个算法所需的计算时间为*O*(*n*log*n*)。在渐近的意义下，此算法已是最优的了。
## 二．再讨论二个集合间最接近点对的情况
对于求二个集合间的最接近点对的算法，其实和普通一个集合的最近点对一样，分治，复杂度O(nlogn),只不过在合并的时候只有两个点属于不同的集合才会更新。这么做已经证明是对的，因为递归到2个点的时候最近的点对一定是合并的时候产生的，所以其实在整个算法过程中不断更新的答案一定是在合并的时候产生的，因此只需在合并的时候注意两个点属于不同集合就好了。因此在S1与S2集合中的每个点增加一个标记域，表示此点属于哪一个集合。时间复杂度与一个集合的是一样，为O(nlogn)。
if |S1|=0 || |S2|=0
{
δ:=∞
return δ;
}
else
begin
先给S1中的每一个点增加一个标识1，表示属于集合S1。给S2中的每一个点增加一个标识2，表示属于集合S2。
合并S1，S2为S，并对S按照横坐标值快速排序（也可以利用随机化快速排序，从而减小排序平均复杂度）。
function CPAIR2(S);
begin
if |S|=2
if 这二个点属于不同集合
δ:=S中这2点的距离
else
δ:=∞
else if |S|=0
δ:=∞
else
begin
1.  m:=S中各点x坐标值的中位数;
构造S1*和S2*，使S1*={p∈S|px≤m}和S2*={p∈S|px>m}
2.  δ1:=CPAIR2(S1*);δ2:=CPAIR2(S2*);
3.  δm:=min(δ1,δ2);
4.  设P1是S1*中距垂直分割线l的距离在δm之内的所有        点组成的集合， P2是S2*中距分割线l的距离在δm之内所有点组成的集合。将P1和P2中的点依其y坐标值从小到大排序，并设P1*和P2*是相应的已排好序的点列;
5.  通过扫描P1*以及对于P1*中每个点检查P2*中与其距离在δm之内的所有点(最多6个)可以完成合并。当P1*中的扫描指针逐次向上移动时，P2*中的扫描指针可在宽为2δm的一个区间内移动，同时需要满足检测的这对点的标识要不相等（即属于不同集合）。设δl是按这种扫描方式找到的点对间的最小距离;
6.  δ=min(δm,δl);
end;
return(δ);
end;
end;

## 三．C++实现代码
\#include<iostream>
\#include<cmath>
\#define MAXSIZE 10000 //所有数组中元素的个数最多为MAXSIZE个
\#define Infinity 65535 //定义无穷大
using namespace std;
//\#define point double*//点的基本类型，最好自己重新定义成结构体
//点的基本类型
typedef struct
{
double x;//x坐标
double y;//y坐标
int flag;//属于哪个集合，0表示属于S1，1表示属于S2
} point;
//s1,s2表示二个集合，s是s1与s2合并而来，t是s1与s2中距分割线l的距离在δm之内所有点组成的集合
point s1[MAXSIZE],s2[MAXSIZE],s[MAXSIZE],t[MAXSIZE];
//从一个点的集合s中，得到距离最近的两个点，返回其距离
double cp(int low,int high);
//求二点之间的距离，且要满足属于不同集合，如果属于相同集合则是它们之间的距离是无穷大(65535)
double distance(const point u,const point v)
{
if(u.flag != v.flag)//属于不同集合
return sqrt((u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y));
else//属于相同集合，则它们之间的距离为无穷大
return (double)Infinity;
}
//对点进行排序，快速排序，增序,flag=0依据横坐标排序，flag=1依据纵坐标排序
void fast_sort_X_Y(point *s,int begin,int end,bool flag)
{
if(!flag)//按横坐标排序
{
if(begin<end)
{
int i,j;
i = begin; j = end;
point temp = s[begin];
while(i<j)
{
while(i<j&&s[j].x>=temp.x)
j--;
s[i] = s[j];
while(i<j&&s[i].x<=temp.x)
i++;
s[j] = s[i];
}
s[i] = temp;
fast_sort_X_Y(s,begin,i-1,flag);
fast_sort_X_Y(s,i+1,end,flag);
}
}
else //按纵坐标排序
{
if(begin<end)
{
int i,j;
i = begin; j = end;
point temp = s[begin];
while(i<j)
{
while(i<j&&s[j].y>=temp.y)
j--;
s[i] = s[j];
while(i<j&&s[i].y<=temp.y)
i++;
s[j] = s[i];
}
s[i] = temp;
fast_sort_X_Y(s,begin,i-1,flag);
fast_sort_X_Y(s,i+1,end,flag);
}
}
}
//当s集合小于三个点
double distance_Of_Three_Or_Less(point *s,int low,int high)
{
double temp1,temp2,temp3;
if((high-low)<1)
{
cout<<"少于两个点，出错！"<<endl;
return Infinity;
}
else if((high-low)==1) //二个点
{
return distance(s[low],s[high]);
}
else
{
temp1 = distance(s[low],s[low+1]);
temp2 = distance(s[low],s[high]);
temp3 = distance(s[low+1],s[high]);
double temp = temp1<temp2?temp1:temp2;
return temp<temp3?temp:temp3;
}
}
//从一个点的集合s中，得到距离最近的两个点，返回其距离
double cp(point *s,int low, int high)
{
int mid,k,i,j;
double x0,dl,dr,d,d1;
if((high-low)<3)     //提供的点少于等于3个
{
return distance_Of_Three_Or_Less(s,low, high);
}
else
{
mid = (low+high)/2;
x0 = s[mid].x;
dl = cp(s,low,mid);
dr = cp(s,(mid+1),high);
d = (dl>dr)?dr:dl;
k = -1;
//从s[low...high]中抽取t
for(i=low;i<=high;i++)
{
if(fabs(s[i].x - x0) <= d)
{
k++;
t[k] = s[i];
}
}
fast_sort_X_Y(t,0,k,1);
d1 = 2*d;
k++;//k为t数组元素的个数
for(i=0;i<k-1;i++)
{
for(j=(i+1);j<(((i+7)<k)?(i+7):k);j++)
if(distance(t[i],t[j]) < d1)
d1 = distance(t[i],t[j]);
}
if(d1<d)
d=d1;
}
return d;
}

void main()
{
int n1,n2;
cout<<"请分别输入S1集合和S2集合中点的个数:"<<endl;
cin>>n1>>n2;
cout<<"请输入S1集合中的点坐标(横坐标，纵坐标):"<<endl;
int i;
//输入数据，并同时合并s1与s2到s中
for(i=0;i<n1;++i)
{
cin>>s1[i].x>>s1[i].y;
s1[i].flag = 1;
s[i] = s1[i];
}
cout<<"请输入S2集合中的点坐标(横坐标，纵坐标):"<<endl;
for(i=0;i<n2;++i)
{
cin>>s2[i].x>>s2[i].y;
s2[i].flag = 2;
s[n1+i] = s2[i];
}
fast_sort_X_Y(s,0,n1+n2-1,0);
cout<<"最短距离为："<<endl;
cout<<cp(s,0,n1+n2-1)<<endl;;
}

