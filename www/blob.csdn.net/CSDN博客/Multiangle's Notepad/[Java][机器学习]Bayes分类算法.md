
# [Java][机器学习]Bayes分类算法 - Multiangle's Notepad - CSDN博客


2015年04月12日 21:11:51[multiangle](https://me.csdn.net/u014595019)阅读数：911


用Java实现Bayes分类算法。与决策树分类法相比，Bayes分类法可以轻松处理有多个分类的情况。它处理2个类和多个类本质上没什么区别。但是它相对于决策树也有一定的缺陷，即该算法是建立在各个属性统计独立的基础上的，如果属性间有关联，就会削弱算法的性能。
与之前的决策树分类算法一样，要事先定义好double[][]型的训练集和检测集，格式为
double[i][0]=标号
double[i][1]=分类。这里还是用之前决策树分类用的例子，分类高斯随机变量和均匀随机变量，所以这里分类值为0和1.如果有更多的分类，则可以继续往上加。
double[i][2~]各个属性的值。
一些重要属性事先定义好以后存放在ProjectInfo类里面。该类有如下属性：
int AttrNum ;  //属性值的数目，如有2种属性，则AttrNum=2
int[] AttrProperty ; //变量的属性，0表示连续变量，1表示布尔变量
String[] AttrDescrip ; //对Arrtibute的描述,非必须
int classNum ;  //所有数据中分类数。该例子中为2
String[] classDescrip ; //对class的描述，非必须
int dataNodeNum ;  //训练集的数目
boolean dependence ;    //true表示分量间独立，否则变量间不独立，即有关联.（目前变量间不独立的功能还没做）
int[] IntervalNum ;    //属性的区间数,如果相应的type=1，则该处无所谓
`定义好需要的量之后，就可以进行训练和分类了。
PropabilityCal pc=new PropabilityCal(trainset,pInfo) ;
Classify cf=new Classify(examset,pc,pInfo) ;
然后可以得到错误率
double err_ratio=Classify.check_error(cf,pInfo) ;
如果想要将分类后的结果输出，Classify类中有List<double[]>[] divided,可以自行撰写输出函数。`代码：
**1.ProjectInfo**
```python
package BayesClassifier;
import DecisionTree.FileIO;
```
```python
public
```
```python
class
```
```python
ProjectInfo {
```
```python
public
```
```python
int
```
```python
AttrNum ;
```
```python
//属性值的数目，如有2种属性，则AttrNum=2
```
```python
public
```
```python
int
```
```python
[] AttrProperty ;
```
```python
//变量的属性，0表示连续变量，1表示布尔变量
```
```python
public
```
```python
String[] AttrDescrip ;
```
```python
//对Arrtibute的描述,非必须
```
```python
public
```
```python
int
```
```python
classNum ;
```
```python
public
```
```python
String[] classDescrip ;
```
```python
//对class的描述，非必须
```
```python
public
```
```python
int
```
```python
dataNodeNum ;
```
```python
public
```
```python
boolean dependence ;
```
```python
//true表示分量间独立，否则变量间不独立，即有关联
```
```python
public
```
```python
int
```
```python
[] IntervalNum ;
```
```python
//属性的区间数,如果相应的type=1，则该处无所谓
```
```python
public
```
```python
ProjectInfo
```
```python
(
```
```python
int
```
```python
AttrNum,
```
```python
int
```
```python
[] AttrProperty,
```
```python
int
```
```python
classNum,
```
```python
int
```
```python
dataNodeNum,boolean dependence,
```
```python
int
```
```python
[] IntervalNum){
```
```python
this
```
```python
.AttrNum=AttrNum ;
```
```python
this
```
```python
.AttrProperty=AttrProperty ;
```
```python
this
```
```python
.AttrDescrip=
```
```python
null
```
```python
;
```
```python
this
```
```python
.classNum=classNum ;
```
```python
this
```
```python
.classDescrip=
```
```python
null
```
```python
;
```
```python
this
```
```python
.dataNodeNum=dataNodeNum ;
```
```python
this
```
```python
.dependence=dependence ;
```
```python
this
```
```python
.IntervalNum=IntervalNum ;
    }
```
```python
public
```
```python
ProjectInfo
```
```python
(
```
```python
int
```
```python
AttrNum,
```
```python
int
```
```python
[] AttrProperty,String[] AttrDescrip,
```
```python
int
```
```python
classNum,String[] classDescrip,
```
```python
int
```
```python
dataNodeNum,
            boolean dependence,
```
```python
int
```
```python
[] IntervalNum){
```
```python
this
```
```python
.AttrNum=AttrNum ;
```
```python
this
```
```python
.AttrProperty=AttrProperty ;
```
```python
this
```
```python
.AttrDescrip=AttrDescrip ;
```
```python
this
```
```python
.classNum=classNum ;
```
```python
this
```
```python
.classDescrip=classDescrip ;
```
```python
this
```
```python
.dataNodeNum=dataNodeNum ;
```
```python
this
```
```python
.dependence=dependence ;
```
```python
this
```
```python
.IntervalNum=IntervalNum ;
    }
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args){
```
```python
//create pInfo
```
```python
int
```
```python
[] AttrProperty={
```
```python
0
```
```python
,
```
```python
0
```
```python
} ;
        String[] AttrDescrip={
```
```python
"x"
```
```python
,
```
```python
"y"
```
```python
} ;
        String[] classDescrip={
```
```python
"average"
```
```python
,
```
```python
"gaussian"
```
```python
} ;
```
```python
int
```
```python
[] IntervalNum={
```
```python
20
```
```python
,
```
```python
20
```
```python
} ;
        ProjectInfo pInfo=
```
```python
new
```
```python
ProjectInfo(
```
```python
2
```
```python
,AttrProperty,AttrDescrip,
```
```python
2
```
```python
,classDescrip,
```
```python
2000
```
```python
,
```
```python
true
```
```python
,
                                            IntervalNum) ;
```
```python
//create trainset and examset
```
```python
FileIO ep=
```
```python
new
```
```python
FileIO() ;
```
```python
int
```
```python
len=
```
```python
2000
```
```python
;
```
```python
int
```
```python
width=
```
```python
3
```
```python
;
```
```python
//the column number of excel sheet
```
```python
int
```
```python
[] nodea={
```
```python
0
```
```python
,
```
```python
0
```
```python
} ;
```
```python
int
```
```python
[] nodeb={width-
```
```python
1
```
```python
,len-
```
```python
1
```
```python
} ;
```
```python
double
```
```python
[][] get1=ep.getArray(
```
```python
"origin"
```
```python
,
```
```python
0
```
```python
,nodea, nodeb) ;
```
```python
double
```
```python
[][] trainset=
```
```python
new
```
```python
double
```
```python
[len][pInfo.AttrNum+
```
```python
2
```
```python
] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<len;i++){
            trainset[i][
```
```python
0
```
```python
]=i+
```
```python
1
```
```python
;
            trainset[i][
```
```python
1
```
```python
]=get1[i][
```
```python
2
```
```python
] ;
            trainset[i][
```
```python
2
```
```python
]=get1[i][
```
```python
0
```
```python
] ;
            trainset[i][
```
```python
3
```
```python
]=get1[i][
```
```python
1
```
```python
] ;
        }
        len=
```
```python
1000
```
```python
;
        width=
```
```python
3
```
```python
;
        nodeb[
```
```python
0
```
```python
]=width-
```
```python
1
```
```python
;
        nodeb[
```
```python
1
```
```python
]=len-
```
```python
1
```
```python
;
```
```python
double
```
```python
[][] get2=ep.getArray(
```
```python
"origin"
```
```python
,
```
```python
1
```
```python
,nodea, nodeb) ;
```
```python
double
```
```python
[][] examset=
```
```python
new
```
```python
double
```
```python
[len][pInfo.AttrNum+
```
```python
2
```
```python
] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<len;i++){
            examset[i][
```
```python
0
```
```python
]=i+
```
```python
1
```
```python
;
            examset[i][
```
```python
1
```
```python
]=get2[i][
```
```python
2
```
```python
] ;
            examset[i][
```
```python
2
```
```python
]=get2[i][
```
```python
0
```
```python
] ;
            examset[i][
```
```python
3
```
```python
]=get2[i][
```
```python
1
```
```python
] ;
        }
```
```python
//check whether the formation is right
```
```python
if
```
```python
(!pInfo.checkFormat(pInfo, trainset)) {
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkError"
```
```python
);
            System.exit(
```
```python
0
```
```python
);
        }
        PropabilityCal pc=
```
```python
new
```
```python
PropabilityCal(trainset,pInfo) ;
        Classify cf=
```
```python
new
```
```python
Classify(examset,pc,pInfo) ;
```
```python
//System.out.println("执行完毕");
```
```python
double
```
```python
err_ratio=Classify.check_error(cf, pInfo) ;
        System.
```
```python
out
```
```python
.println(
```
```python
"错误率为: "
```
```python
+err_ratio);
    }
```
```python
public
```
```python
boolean
```
```python
checkFormat
```
```python
(ProjectInfo pInfo,
```
```python
double
```
```python
[][] input){
```
```python
if
```
```python
(input==
```
```python
null
```
```python
){
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_double[][]为空"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
if
```
```python
(pInfo.AttrNum!=input[
```
```python
0
```
```python
].length-
```
```python
2
```
```python
){
```
```python
//double[][] 的格式：SetNum ,class,各个属性值
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_AttrNum数值错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
if
```
```python
((pInfo.AttrDescrip!=
```
```python
null
```
```python
)&&(pInfo.AttrDescrip.length!=pInfo.AttrNum)){
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_AttrDescrip个数错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
if
```
```python
(pInfo.dataNodeNum!=input.length){
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_dataNodeNum数值错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<input.length;i++){
```
```python
if
```
```python
((input[i][
```
```python
1
```
```python
]>=pInfo.classNum)||(input[i][
```
```python
1
```
```python
]<
```
```python
0
```
```python
)){
                System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_classNum值或者double[][class]错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
            }
        }
```
```python
if
```
```python
(pInfo.classNum!=pInfo.classDescrip.length){
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_classDescrip长度错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
if
```
```python
(pInfo.IntervalNum.length!=pInfo.AttrNum){
            System.
```
```python
out
```
```python
.println(
```
```python
"ERROR_checkFormat_IntervalNum长度错误"
```
```python
) ;
```
```python
return
```
```python
false
```
```python
;
        }
```
```python
return
```
```python
true
```
```python
;
    }
}
```
**2.PropabilityCal**
```python
package BayesClassifier;
```
```python
//import java.util.* ;
```
```python
public
```
```python
class
```
```python
PropabilityCal {
```
```python
double
```
```python
[][][] interval ;
```
```python
// 1.attributes;2.Type;3.Interval;
```
```python
double
```
```python
[][] interval_valve ;
```
```python
//1.attributes 2.valve
```
```python
public
```
```python
PropabilityCal
```
```python
(
```
```python
double
```
```python
[][] trainset,ProjectInfo pInfo){
```
```python
// 首先构建interval_valve
```
```python
double
```
```python
[][] interval_value=
```
```python
new
```
```python
double
```
```python
[pInfo.AttrNum][] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<pInfo.AttrNum;i++){
```
```python
if
```
```python
(pInfo.AttrProperty[i]==
```
```python
0
```
```python
){
```
```python
//为连续变量
```
```python
int
```
```python
tempnum=pInfo.IntervalNum[i] ;
```
```python
double
```
```python
[] temp=
```
```python
new
```
```python
double
```
```python
[tempnum] ;
```
```python
double
```
```python
min,max ;
                min=max=trainset[
```
```python
0
```
```python
][i+
```
```python
2
```
```python
] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<trainset.length;j++){
```
```python
if
```
```python
(trainset[j][i+
```
```python
2
```
```python
]>max) max=trainset[j][i+
```
```python
2
```
```python
] ;
```
```python
if
```
```python
(trainset[j][i+
```
```python
2
```
```python
]<min) min=trainset[j][i+
```
```python
2
```
```python
] ;
                }
```
```python
double
```
```python
gap=(max-min)/(tempnum-
```
```python
1
```
```python
) ;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<tempnum;j++){
                    temp[j]=min+(j-
```
```python
1
```
```python
)*gap ;
                }
                interval_value[i]=temp ;
            }
```
```python
else
```
```python
if
```
```python
(pInfo.AttrProperty[i]==
```
```python
1
```
```python
){
```
```python
//为布尔变量
```
```python
double
```
```python
[] temp={
```
```python
0.5
```
```python
,
```
```python
2
```
```python
} ;
                interval_value[i]=temp ;
            }
```
```python
else
```
```python
{System.
```
```python
out
```
```python
.println(
```
```python
"ERROR:DataPreDeal_pInfo.AttrProperty["
```
```python
+i+
```
```python
"]的值错误"
```
```python
);}
        }
```
```python
this
```
```python
.interval_valve=interval_value ;
```
```python
// 多元值获取最大最小值，根据分段分好interval
```
```python
interval=
```
```python
new
```
```python
double
```
```python
[pInfo.AttrNum][][] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<pInfo.AttrNum;i++){
```
```python
if
```
```python
(pInfo.AttrProperty[i]==
```
```python
1
```
```python
){
```
```python
double
```
```python
[][] temp=
```
```python
new
```
```python
double
```
```python
[pInfo.classNum][
```
```python
2
```
```python
] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
a=
```
```python
0
```
```python
;a<pInfo.classNum;a++){
                    temp[a][
```
```python
0
```
```python
]=
```
```python
0
```
```python
;
                    temp[a][
```
```python
1
```
```python
]=
```
```python
0
```
```python
;
                }
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.dataNodeNum;j++){
```
```python
if
```
```python
(trainset[j][i+
```
```python
2
```
```python
]<interval_valve[i][
```
```python
0
```
```python
]){
                        temp[(
```
```python
int
```
```python
)trainset[j][
```
```python
1
```
```python
]][
```
```python
0
```
```python
]++ ;
                    }
```
```python
else
```
```python
{
                        temp[(
```
```python
int
```
```python
)trainset[j][
```
```python
1
```
```python
]][
```
```python
1
```
```python
]++ ;
                    }
                }
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++){
```
```python
double
```
```python
sum=temp[j][
```
```python
0
```
```python
]+temp[j][
```
```python
1
```
```python
] ;
                    temp[j][
```
```python
0
```
```python
]=temp[j][
```
```python
0
```
```python
]/sum ;
                    temp[j][
```
```python
1
```
```python
]=temp[j][
```
```python
1
```
```python
]/sum ;
                }
                interval[i]=temp ;
            }
```
```python
else
```
```python
{
```
```python
//首先进行快速排序
```
```python
double
```
```python
[][] sorted=QuickSort(trainset,i+
```
```python
2
```
```python
,
```
```python
0
```
```python
,trainset.length-
```
```python
1
```
```python
) ;
```
```python
//按照区间数建立临时数组
```
```python
double
```
```python
[][] temp=
```
```python
new
```
```python
double
```
```python
[pInfo.classNum][pInfo.IntervalNum[i]];
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++){
```
```python
for
```
```python
(
```
```python
int
```
```python
k=
```
```python
0
```
```python
;k<pInfo.IntervalNum[i];k++) temp[j][k]=
```
```python
0
```
```python
;
                }
```
```python
//然后从低值开始，以此放入区间内，建立interval数组
```
```python
int
```
```python
current_interval=
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<sorted.length;j++){
```
```python
if
```
```python
(current_interval==(pInfo.IntervalNum[i]-
```
```python
1
```
```python
)){
                        temp[(
```
```python
int
```
```python
)sorted[j][
```
```python
1
```
```python
]][pInfo.IntervalNum[i]-
```
```python
1
```
```python
]++ ;
                    }
```
```python
else
```
```python
{
```
```python
/*
                        if (sorted[j][i+2]<this.interval_valve[i][current_interval]){
                            temp[(int)sorted[j][1]][current_interval]++ ;
                        }else{
                            while(sorted[j][i+2]>this.interval_valve[i][current_interval]) current_interval++ ;
                            current_interval-- ;
                        }*/
```
```python
while
```
```python
(sorted[j][i+
```
```python
2
```
```python
]>
```
```python
this
```
```python
.interval_valve[i][current_interval]) current_interval++ ;
                        temp[(
```
```python
int
```
```python
)sorted[j][
```
```python
1
```
```python
]][current_interval]++ ;
                    }
                }
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++){
```
```python
double
```
```python
temp_sum=
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
k=
```
```python
0
```
```python
;k<pInfo.IntervalNum[i];k++) temp_sum+=temp[j][k] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
k=
```
```python
0
```
```python
;k<pInfo.IntervalNum[i];k++) temp[j][k]=temp[j][k]/temp_sum;
                }
                interval[i]=temp ;
            }   
        }
```
```python
this
```
```python
.interval=interval ;
    }
```
```python
private
```
```python
static
```
```python
double
```
```python
[][]
```
```python
QuickSort
```
```python
(
```
```python
double
```
```python
[][] input,
```
```python
int
```
```python
targetcol,
```
```python
int
```
```python
low,
```
```python
int
```
```python
high){
```
```python
if
```
```python
(targetcol>=input[
```
```python
0
```
```python
].length)
```
```python
return
```
```python
null
```
```python
;
```
```python
if
```
```python
(low>=high)
```
```python
return
```
```python
null
```
```python
;
```
```python
int
```
```python
first=low ;
```
```python
int
```
```python
last=high ;
```
```python
double
```
```python
[] key=input[low] ;
```
```python
while
```
```python
(first<last){
```
```python
while
```
```python
((first<last)&&(input[last][targetcol]>=key[targetcol])) --last ;
            input[first]=input[last] ;
```
```python
while
```
```python
((first<last)&&(input[first][targetcol]<=key[targetcol])) ++first ;
            input[last]=input[first] ;
        }
        input[first]=key ;
```
```python
double
```
```python
[][] res1,res2 ;
```
```python
if
```
```python
(first-
```
```python
1
```
```python
>low) {res1=QuickSort(input,targetcol,low,first-
```
```python
1
```
```python
) ;}
```
```python
else
```
```python
if
```
```python
(first-
```
```python
1
```
```python
==low) {
```
```python
double
```
```python
[][] temp={input[low]} ;res1=temp ;}
```
```python
else
```
```python
{res1=
```
```python
null
```
```python
;}
```
```python
if
```
```python
(high>first+
```
```python
1
```
```python
){res2=QuickSort(input,targetcol,first+
```
```python
1
```
```python
,high) ;}
```
```python
else
```
```python
if
```
```python
(high==first+
```
```python
1
```
```python
){
```
```python
double
```
```python
[][] temp={input[high]} ;res2=temp ;}
```
```python
else
```
```python
{res2=
```
```python
null
```
```python
;}
```
```python
double
```
```python
[][] finalres ;
        finalres=Combine(res1,res2,key) ;
```
```python
return
```
```python
finalres ;
    }
```
```python
private
```
```python
static
```
```python
double
```
```python
[][]
```
```python
Combine
```
```python
(
```
```python
double
```
```python
[][] res1,
```
```python
double
```
```python
[][] res2,
```
```python
double
```
```python
[] key){
```
```python
int
```
```python
len1,len2 ;
```
```python
if
```
```python
(res1==
```
```python
null
```
```python
) len1=
```
```python
0
```
```python
;
```
```python
else
```
```python
len1=res1.length ;
```
```python
if
```
```python
(res2==
```
```python
null
```
```python
) len2=
```
```python
0
```
```python
;
```
```python
else
```
```python
len2=res2.length ;
```
```python
double
```
```python
[][] res=
```
```python
new
```
```python
double
```
```python
[len1+len2+
```
```python
1
```
```python
][
```
```python
3
```
```python
] ;
```
```python
int
```
```python
index=
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<len1;i++) res[index++]=res1[i] ;
        res[index++]=key ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<len2;i++) res[index++]=res2[i] ;
```
```python
return
```
```python
res ;
    }
}
```
**3.Classify**
```python
package BayesClassifier;
import java.util.* ;
```
```python
public
```
```python
class
```
```python
Classify {
    List<
```
```python
double
```
```python
[]>[] divided ;
```
```python
public
```
```python
Classify
```
```python
(
```
```python
double
```
```python
[][] examset,PropabilityCal pc,ProjectInfo pInfo){
        divided=
```
```python
new
```
```python
ArrayList[pInfo.classNum] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<pInfo.classNum;i++) divided[i]=
```
```python
new
```
```python
ArrayList() ;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<examset.length;i++){
```
```python
// Elements
```
```python
double
```
```python
[] class_pro=
```
```python
new
```
```python
double
```
```python
[pInfo.classNum] ;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++) class_pro[j]=
```
```python
1
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++){
```
```python
//Class pick the highest propability
```
```python
for
```
```python
(
```
```python
int
```
```python
k=
```
```python
0
```
```python
;k<pInfo.AttrNum;k++){
```
```python
//int temp_index=get_individual_pro(pc.interval_valve[k],examset[i][k+2],0,pc.interval_valve[k].length-1);
```
```python
int
```
```python
low=
```
```python
0
```
```python
;
```
```python
int
```
```python
high=pc.interval_valve[k].length-
```
```python
1
```
```python
;
```
```python
int
```
```python
temp_index=-
```
```python
1
```
```python
;
                    boolean signal=
```
```python
false
```
```python
;
```
```python
while
```
```python
(
```
```python
true
```
```python
){
                        temp_index=(low+high)/
```
```python
2
```
```python
;
```
```python
int
```
```python
rs=check_match(pc.interval_valve[k],examset[i][k+
```
```python
2
```
```python
],temp_index) ;
```
```python
if
```
```python
(rs==
```
```python
0
```
```python
) {signal=
```
```python
true
```
```python
;
```
```python
break
```
```python
;}
```
```python
if
```
```python
(rs==-
```
```python
1
```
```python
) {
```
```python
if
```
```python
(high==
```
```python
0
```
```python
)
```
```python
break
```
```python
;
```
```python
else
```
```python
high=temp_index-
```
```python
1
```
```python
;
                        }
```
```python
if
```
```python
(rs==
```
```python
1
```
```python
){
```
```python
if
```
```python
(low==pc.interval_valve[k].length-
```
```python
1
```
```python
)
```
```python
break
```
```python
;
```
```python
else
```
```python
low=temp_index+
```
```python
1
```
```python
;
                        }
                    }
```
```python
if
```
```python
(signal==
```
```python
true
```
```python
) class_pro[j]*=pc.interval[k][j][temp_index] ;
```
```python
else
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"ERROR:Classify_Method:Classify_classpro"
```
```python
);
                }
            }
```
```python
double
```
```python
max_pro=
```
```python
0
```
```python
;
```
```python
int
```
```python
max_class=
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<pInfo.classNum;j++){
```
```python
if
```
```python
(max_pro<class_pro[j]){
                    max_pro=class_pro[j] ;
                    max_class=j ;
                }
            }
            divided[max_class].add(examset[i]) ;
        }
```
```python
this
```
```python
.divided=divided ;
    }
```
```python
public
```
```python
static
```
```python
double
```
```python
check_error
```
```python
(Classify c,ProjectInfo pInfo){
```
```python
double
```
```python
total_num=
```
```python
0
```
```python
;
```
```python
double
```
```python
err_num=
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<c.divided.length;i++){
```
```python
for
```
```python
(
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<c.divided[i].size();j++){
```
```python
double
```
```python
[]
```
```python
get
```
```python
=c.divided[i].
```
```python
get
```
```python
(j) ;
                total_num++ ;
```
```python
if
```
```python
(
```
```python
get
```
```python
[
```
```python
1
```
```python
]!=i) err_num++ ;
            }
        }
```
```python
return
```
```python
err_num/total_num ;
    }
```
```python
private
```
```python
static
```
```python
int
```
```python
check_match
```
```python
(
```
```python
double
```
```python
[] interval,
```
```python
double
```
```python
value
```
```python
,
```
```python
int
```
```python
want_index){
```
```python
if
```
```python
(want_index==
```
```python
0
```
```python
) {
```
```python
if
```
```python
(
```
```python
value
```
```python
<interval[
```
```python
0
```
```python
])
```
```python
return
```
```python
0
```
```python
;
```
```python
else
```
```python
return
```
```python
1
```
```python
;
        }
```
```python
else
```
```python
{
```
```python
if
```
```python
(want_index==interval.length-
```
```python
1
```
```python
){
```
```python
if
```
```python
(
```
```python
value
```
```python
>=interval[want_index-
```
```python
1
```
```python
])
```
```python
return
```
```python
0
```
```python
;
```
```python
else
```
```python
return
```
```python
-
```
```python
1
```
```python
;
            }
```
```python
else
```
```python
{
```
```python
if
```
```python
(
```
```python
value
```
```python
<interval[want_index]) {
```
```python
if
```
```python
(
```
```python
value
```
```python
>=interval[want_index-
```
```python
1
```
```python
]){
```
```python
return
```
```python
0
```
```python
;
                    }
```
```python
else
```
```python
return
```
```python
-
```
```python
1
```
```python
;
                }
```
```python
else
```
```python
return
```
```python
1
```
```python
;
            }
        }
    }
}
```

