
# Python3.x的print()输出问题 - jerrygaoling的博客 - CSDN博客


2018年03月22日 11:32:14[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：457


## Python3.x的print输出问题
`print(*objects, sep=' ', end='\n', file=sys.stdout)`
objects – 复数，表示可以一次输出多个对象。输出多个对象时，需要用 , 分隔。
sep – 用来间隔多个对象，默认值是一个空格。
end – 用来设定以什么结尾。默认值是换行符 \n，我们可以换成其他字符串。
file – 要写入的文件对象。
### objects,sep的参数用法：
```python
for
```
```python
l
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
for
```
```python
l1
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
print
```
```python
(
```
```python
"*"
```
```python
,
```
```python
"%"
```
```python
)
```
```python
print
```
```python
(
```
```python
" "
```
```python
)
```
```python
for
```
```python
l
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
for
```
```python
l1
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
print
```
```python
(
```
```python
"*"
```
```python
,
```
```python
"%"
```
```python
,sep=
```
```python
"--"
```
```python
)
```
```python
print
```
```python
(
```
```python
" "
```
```python
)
```
输出结果：
没有end参数是，默认输出为\n的换行操作
![这里写图片描述](//img-blog.csdn.net/20180322112955748?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](//img-blog.csdn.net/20180322112955748?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](//img-blog.csdn.net/20180322113005311?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### end参数的用法：
```python
for
```
```python
l
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
for
```
```python
l1
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
print
```
```python
(
```
```python
"*"
```
```python
,
```
```python
end
```
```python
=
```
```python
"\t"
```
```python
)
```
```python
print
```
```python
(
```
```python
" "
```
```python
)
```
输出结果：
![这里写图片描述](//img-blog.csdn.net/20180322113018551?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### file参数的用法：
```python
with
```
```python
open
```
```python
(
```
```python
't1
```
```python
.txt',
```
```python
'w'
```
```python
) as f:
```
```python
for
```
```python
l
```
```python
in
```
```python
range
```
```python
(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
```
```python
for
```
```python
l1
```
```python
in
```
```python
range
```
```python
(
```
```python
0
```
```python
,
```
```python
3
```
```python
):
            print(
```
```python
"*"
```
```python
,
```
```python
end
```
```python
=
```
```python
"\t"
```
```python
,
```
```python
file
```
```python
=f)
        print(
```
```python
" "
```
```python
,
```
```python
file
```
```python
=f)
```
`open ('t1.txt','w')`为打开文档，并打开写入模式
file参数，将所有的显示内容，输出到t1.txt文档中。
结果：
![这里写图片描述](//img-blog.csdn.net/20180322113036139?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

