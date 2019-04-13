
# Python中 sys.argv[]的用法简明解释 - sinat_33731745的博客 - CSDN博客

2018年01月28日 22:03:24[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：79标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[python																](https://blog.csdn.net/sinat_33731745/article/category/7252259)


sys.argv[]说白了就是一个从程序外部获取参数的桥梁，这个“外部”很关键，所以那些试图从代码来说明它作用的解释一直没看明白。因为我们从外部取得的参数可以是多个，所以获得的是一个列表（list)，也就是说sys.argv其实可以看作是一个列表，所以才能用[]提取其中的元素。其第一个元素是程序本身，随后才依次是外部给予的参数。
下面我们通过一个极简单的test.py程序的运行结果来说明它的用法。
```python
1
```
```python
#test.py
```
```python
2
```
```python
3
```
```python
import
```
```python
sys
```
```python
4
```
```python
a=sys.argv[
```
```python
0
```
```python
]
```
```python
5
```
```python
print(a)
```
将test.py保存在c盘的根目录下。
在程序中找到 ‘运行’->点击->输入”cmd”->回车键   进入控制台命令窗口（如下图），先输入cd c:\   (作用是将命令路径改到c盘根目录)，然后输入test.py运行我们刚刚写的程序：
![这里写图片描述](https://img-blog.csdn.net/20180128220116275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到的结果是C:\test.py，这就是0指代码（即此.py程序）本身的意思。
然后我们将代码中0改为1 :
a=sys.argv[1]
保存后，再从控制台窗口运行，这次我们加上一个参数，输入：test.py what
![这里写图片描述](https://img-blog.csdn.net/20180128220203177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到的结果就是我们输入的参数what，看到这里你是不是开始明白了呢。
那我们再把代码修改一下：
a=sys.argv[2：]
保存后，再从控制台窗台运行程序，这次多加几个参数，以空格隔开:
test.py a b c d e f
![这里写图片描述](https://img-blog.csdn.net/20180128220241201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到的结果为[‘b’, ’c’, ’d’, ’e’, ’f’]
应该大彻大悟了吧。Sys.argv[ ]其实就是一个列表，里边的项为用户输入的参数，关键就是要明白这参数是从程序外部输入的，而非代码本身的什么地方，要想看到它的效果就应该将程序保存了，从外部来运行程序并给出参数。

