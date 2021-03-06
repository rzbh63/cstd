
# 相似度：最长公共子序列--LCS - lpty的博客 - CSDN博客

2018年04月26日 16:23:32[lpty](https://me.csdn.net/sinat_33741547)阅读数：907所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、概念
### 1、子序列
一个特定序列的子序列就是将给定序列中零个或多个元素去掉后得到的结果（不改变元素间相对次序）。如序列[A,B,C,B,D,A,B]的子序列有：[A,B]，[B,C,A]，[A,D,A,B]等.。
### 2、公共子序列
给定序列X和Y，序列Z是X的子序列，也是Y的子序列，则Z是X和Y的公共子序列。例如X=[A,B,C,B,D,A,B]，Y=[B,D,C,A,B,A]，那么序列Z=[B,C,A]为X和Y的公共子序列，其长度为3。
### 3.最长公共子序列
上述Z=[B,C,A]不是X和Y的最长公共子序列，存在序列[B,C,B,A]和[B,D,A,B]为X和Y的公共子序列，长度为4，因为X和Y不存在长度大于等于5的公共子序列，所以长度为4的公共子序列为最长公共子序列。
## 二、暴力枚举法
假设序列A有n个元素，序列B有m个元素，那么A、B分别有2^n，2^m个子序列，如果任意两个子序列一一比较，那么这时候的的算法复杂度为至少是2^(m+n)，随序列长度指数增长，这对长序列是不适用的。
## 三、动态规划法
### 1、思想
假设存在序列A长度为M， 序列B长度为N，生成大小为(M+1)*(N+1)的矩阵dp， 初始元素全部为0
（1）Ax代表字符串A的前i个字符组成的序列，By代表字符串B的前j个字符组成的序列
（2）Ax[i]代表字符串A的第i个字符，i>0；By[j]代表字符串B的第j个字符，j>0；
（3）dp[i][j] 代表序列Ax与序列By的最长公共子序列的长度
解法如下：
1）如果Ax[i] = By[j]，由于最长公共子序列为非连续序列，那么Ax和By的最长公共子序列LCS的最后一个元素即为当前元素，所以： dp[i][j]=dp[i-1][j-1]+1
即历史最长公共子序列的长度加1
2）如果Ax[i] != By[j]，那么dp[i][j]的值存在两种情况：
情况一：可能是dp[i-1][j]的值，这代表字符串A的前i-1个元素与字符串B的前j个元素的最长公共子序列长度
情况二：可能是dp[i][j-1]的值，这代表字符串A的前i个元素与字符串B的前j-1个元素的最长公共子序列长度
两者取之间最大的值，即为当前dp[i][j]的值，即当前最长公共子序列的长度。
3）至此，重复上述计算方式直到矩阵dp最后一个值，即为字符串A与字符串B的最长公共子序列值。
### 2、实战
假设存在序列A = [A,B,C,B,D,A,B]，序列B = [B,D,C,A,B,A]，动态规划法求最长公共子序列可以用下图表示：
![这里写图片描述](https://img-blog.csdn.net/20180426145809442?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180426145809442?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（1）设当前矩阵初始值均为0，由于i，j均大于0，忽略矩阵的第0行,及第0列
（2）从第i行，第1列开始计算，此时By[1] = B，
当i=1时，Ax[1] = A，两者不相等，取dp[i-1][j]与dp[i][j-1]两者之间最大的值，可知此时为0，
当i=2时，Ax[2] = B，两者相等，取dp[i-1][j-1]+1，可知此时应为0+1=1
当i=3时，以此类推
（3）重复上述步骤，最终得到最长子序列长度为4

