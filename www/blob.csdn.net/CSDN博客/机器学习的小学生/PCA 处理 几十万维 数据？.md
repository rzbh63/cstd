
# PCA 处理 几十万维 数据？ - 机器学习的小学生 - CSDN博客


2017年11月22日 13:01:45[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：260个人分类：[工具																](https://blog.csdn.net/xuluhui123/article/category/6418741)



**问题**：
I have a large set of data, about 8GB. I want to use machine learning to analyze it. So I think I should do SVD then PCA to reduce the data dimension for efficiency. But MATLAB and Octave cannot load such a large dataset.
What tools I can use to do SVD with such a large amount of data?
**回答**：
First of all, dimensionality reduction is used when you have many covariated dimensions and want to reduce problem size by rotating data points into new orthogonal basis and taking only axes with largest variance. With 8 variables (columns) your space is already low-dimensional, reducing number of variables further is unlikely to solve technical issues with memory size, but may affect dataset quality a lot. In your concrete case it’s more promising to take a look at online learning methods. Roughly speaking, instead of working with the whole dataset, these methods take a little part of them (often referred to as “mini-batches”) at a time and build a model incrementally. (I personally like to interpret word “online” as a reference to some infinitely long source of data from Internet like a Twitter feed, where you just can’t load the whole dataset at once).
But what if you really wanted to apply dimensionality reduction technique like PCA to a dataset that doesn’t fit into a memory? Normally a dataset is represented as a data matrix X of size n x m, where n is number of observations (rows) and m is a number of variables (columns). Typically problems with memory come from only one of these two numbers.
**Too many observations (n >> m)**
When you have too many observations, but the number of variables is from small to moderate, you can build the covariance matrix incrementally. Indeed, typical PCA consists of constructing a covariance matrix of size m x m and applying singular value decomposition to it. With m=1000 variables of type float64, a covariance matrix has size 1000*1000*8 ~ 8Mb, which easily fits into memory and may be used with SVD. So you need only to build the covariance matrix without loading entire dataset into memory - pretty tractable task.
Alternatively, you can select a small representative sample from your dataset and approximate the covariance matrix. This matrix will have all the same properties as normal, just a little bit less accurate.
**Too many variables (n << m)**
On another hand, sometimes, when you have too many variables, the covariance matrix itself will not fit into memory. E.g. if you work with 640x480 images, every observation has 640*480=307200 variables, which results in a 703Gb covariance matrix! That’s definitely not what you would like to keep in memory of your computer, or even in memory of your cluster. So we need to reduce dimensions without building a covariance matrix at all.
**My favourite method for doing it is****[Random Projection](http://web.stanford.edu/~hastie/Papers/Ping/KDD06_rp.pdf)****.**In short, if you have dataset X of size n x m, you can multiply it by some sparse random matrix R of size m x k (with k << m) and obtain new matrix X’ of a much smaller size n x k with approximately the same properties as the original one. Why does it work? Well, you should know that PCA aims to find set of orthogonal axes (principal components) and project your data onto first k of them. It turns out that sparse random vectors are nearly orthogonal and thus may also be used as a new basis.
And, of course, you don’t have to multiply the whole dataset X by R - you can translate every observation x into the new basis separately or in mini-batches.
There’s also somewhat similar algorithm called Random SVD. I don’t have any real experience with it, but you can find example code with explanations here.
参考文献：
1.[https://datascience.stackexchange.com/questions/1159/how-to-do-svd-and-pca-with-big-data](https://datascience.stackexchange.com/questions/1159/how-to-do-svd-and-pca-with-big-data)
2.[https://stats.stackexchange.com/questions/2806/best-pca-algorithm-for-huge-number-of-features-10k/11934\#11934](https://stats.stackexchange.com/questions/2806/best-pca-algorithm-for-huge-number-of-features-10k/11934#11934)

