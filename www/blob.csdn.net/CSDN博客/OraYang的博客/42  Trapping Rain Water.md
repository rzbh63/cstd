
# 42. Trapping Rain Water - OraYang的博客 - CSDN博客

2018年03月05日 15:20:23[OraYang](https://me.csdn.net/u010665216)阅读数：264所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目

Given*n*non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.


For example,
Given`[0,1,0,2,1,0,1,3,2,1,2,1]`, return`6`.
![这里写图片描述](https://img-blog.csdn.net/2018030515112583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
The above elevation map is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped.
# 思路
我实现了两种方法，一种方法超时，一种方法AC。首先说下AC的思路：
我们可以用累积的方式来考虑，而不是用高度*宽度来计算面积。换句话说，我们只需要求每个柱状的面积之和（宽度为1）
从左到右进行搜索，分别保持左和右的最大高度，就像部分容器的单侧墙一样。固定较高的一边，从较低的部分开始补充水。例如，如果当前的左边的高度较低，我们就在左边的箱子里填满水，然后不断求和，一旦发现左边的高度大于右边，我们再从右边补充水直到我们把整个集装箱填满了。
我一开始是考虑从底部一层一层计算雨水的。但是超时了。
## AC
```python
class Solution {
```
```python
public
```
```python
int
```
```python
trap
```
```python
(
```
```python
int
```
```python
[] height) {
```
```python
int
```
```python
left=
```
```python
0
```
```python
,right = height.length-
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
maxleft=
```
```python
0
```
```python
,maxright=
```
```python
0
```
```python
,res=
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
(left<=right)
        {
```
```python
if
```
```python
(height[left]<=height[right])
            {
```
```python
if
```
```python
(height[left]>maxleft) maxleft = height[left];
```
```python
else
```
```python
res+=maxleft-height[left];
                left++;
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
(height[right]>maxright) maxright = height[right];
```
```python
else
```
```python
res+=maxright-height[right];
                right--;
            }
        }
```
```python
return
```
```python
res;
    }
}
```
## 超时
```python
class Solution {
```
```python
public
```
```python
int
```
```python
trap
```
```python
(
```
```python
int
```
```python
[] height) {
```
```python
//step 1 find max and min
```
```python
int
```
```python
max =Integer.MIN_VALUE;
```
```python
int
```
```python
min = Integer.MAX_VALUE;
```
```python
if
```
```python
(height.length==
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
;i<height.length;i++)
        {
            max = Math.max(max,height[i]);
            min = Math.min(min,height[i]);
        }
```
```python
if
```
```python
(min==max)
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
int
```
```python
res =
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
i = min+
```
```python
1
```
```python
;i<=max;i++)
        {
```
```python
int
```
```python
[] index = findindex(height,i-
```
```python
1
```
```python
);
```
```python
int
```
```python
front_index = index[
```
```python
0
```
```python
];
```
```python
int
```
```python
back_index = index[
```
```python
1
```
```python
];
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
j=front_index;j<=back_index;j++)
            {
```
```python
if
```
```python
(height[j]<i)
                {
                    res++;
                }    
            }
        }
```
```python
return
```
```python
res;
    }
```
```python
public
```
```python
int
```
```python
[]
```
```python
findindex
```
```python
(
```
```python
int
```
```python
[] height,
```
```python
int
```
```python
value){
```
```python
//step2 find front index and back index(front >min ; back>min)
```
```python
int
```
```python
[] index =
```
```python
new
```
```python
int
```
```python
[
```
```python
2
```
```python
];
```
```python
int
```
```python
front_index =
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
back_index =
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
;i<height.length;i++)
        {
```
```python
if
```
```python
(height[i]> value)
            {
                front_index = i;
```
```python
break
```
```python
;
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
i=height.length-
```
```python
1
```
```python
;i>=
```
```python
0
```
```python
;i--)
        {
```
```python
if
```
```python
(height[i]> value)
            {
                back_index = i;
```
```python
break
```
```python
;
            }
        }
        index[
```
```python
0
```
```python
] = front_index;
        index[
```
```python
1
```
```python
] = back_index;
```
```python
return
```
```python
index;
    }
}
```

