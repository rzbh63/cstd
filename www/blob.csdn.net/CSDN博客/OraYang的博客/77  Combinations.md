
# 77. Combinations - OraYang的博客 - CSDN博客

2017年09月25日 11:51:56[OraYang](https://me.csdn.net/u010665216)阅读数：199个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given two integers n and k, return all possible combinations of k numbers out of 1 … n.
For example,
If n = 4 and k = 2, a solution is:
[
[2,4],
[3,4],
[2,3],
[1,2],
[1,3],
[1,4],
]
## 思路
本题是一个典型的组合问题，第一种思路是递归的思想，代码如下：
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
combine(
```
```python
int
```
```python
n,
```
```python
int
```
```python
k) {
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
res;
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
tmp;
        helper(n, k,
```
```python
1
```
```python
, tmp, res);
```
```python
return
```
```python
res;
    }
```
```python
void
```
```python
helper(
```
```python
int
```
```python
n,
```
```python
int
```
```python
k,
```
```python
int
```
```python
level,
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
& tmp,
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
& res) {
```
```python
if
```
```python
(tmp.size() == k) res.push_back(tmp);
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
i = level; i <= n; ++i) {
            tmp.push_back(i);
            helper(n, k, i +
```
```python
1
```
```python
, tmp, res);
            tmp.pop_back();
        }
    }
};
```
第二种是官方的解法，很巧妙，如下：
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
combine(
```
```python
int
```
```python
n,
```
```python
int
```
```python
k) {
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
res;
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
tmp(k,
```
```python
0
```
```python
);
```
```python
int
```
```python
i =
```
```python
0
```
```python
;
```
```python
while
```
```python
(i >=
```
```python
0
```
```python
) {
            ++tmp[i];
```
```python
if
```
```python
(tmp[i] > n) --i;
```
```python
else
```
```python
if
```
```python
(i == k -
```
```python
1
```
```python
) res.push_back(tmp);
```
```python
else
```
```python
{
                ++i;
                tmp[i] = tmp[i -
```
```python
1
```
```python
];
            }
        }
```
```python
return
```
```python
res;
    }
};
```

