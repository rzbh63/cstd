
# 59. Spiral Matrix II - OraYang的博客 - CSDN博客

2017年09月20日 11:43:59[OraYang](https://me.csdn.net/u010665216)阅读数：221个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.
For example,
Given n = 3,
You should return the following matrix:
[
[ 1, 2, 3 ],
[ 8, 9, 4 ],
[ 7, 6, 5 ]
]
## 思路
这道题目又是螺旋遍历，思路和前面一篇博客：[Spiral Matrix](http://blog.csdn.net/u010665216/article/details/78035480)一模一样
就不多说了，直接上代码：
## 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
vector
```
```python
<
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
>
```
```python
generateMatrix(
```
```python
int
```
```python
n) {
```
```python
//生成n*n的数组
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
tmp(
```
```python
abs
```
```python
(n),
```
```python
1
```
```python
);
```
```python
vector
```
```python
<
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
>
```
```python
matrix;
```
```python
if
```
```python
(n==
```
```python
0
```
```python
)
```
```python
return
```
```python
matrix;
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
;j<
```
```python
abs
```
```python
(n);j++)
            matrix.push_back(tmp);
```
```python
int
```
```python
rowbegin =
```
```python
0
```
```python
,rowend=matrix.size()-
```
```python
1
```
```python
,colbegin=
```
```python
0
```
```python
,colend = matrix[
```
```python
0
```
```python
].size()-
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
num =
```
```python
1
```
```python
;
```
```python
while
```
```python
(rowbegin<=rowend&&colbegin<=colend)
```
```python
//大循环
```
```python
{
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
i=colbegin;i<=colend;i++)
```
```python
//从左往右
```
```python
{
                matrix[rowbegin][i] =num ;
                num++;
            }
            rowbegin++;
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
i=rowbegin;i<=rowend;i++)
```
```python
//从上至下
```
```python
{
                matrix[i][colend] =num;
                num++;
            }
            colend--;
```
```python
if
```
```python
(rowbegin<=rowend)
            {
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
i=colend;i>=colbegin;i--)
```
```python
//从右往左
```
```python
{
                    matrix[rowend][i] = num;
                    num++;
                }
            }
            rowend--;
```
```python
if
```
```python
(colbegin<=colend)
            {
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
i=rowend;i>=rowbegin;i--)
```
```python
//从下往上
```
```python
{
                    matrix[i][colbegin]=num;
                    num++;
                }
            }
            colbegin++;
        }
```
```python
return
```
```python
matrix; 
    }
};
```

