# 你所不知道的Python奇技淫巧



有时候你会看到很Cool的Python代码，你惊讶于它的简洁，它的优雅，你不由自主地赞叹：竟然还能这样写。其实，这些优雅的代码都要归功于Python的特性，只要你能掌握这些Pythonic的技巧，你一样可以写出像诗一样的Python代码。

![img](https://images2015.cnblogs.com/blog/919328/201608/919328-20160807183908840-259949602.jpg)

## 1.导入模块

你是不是经常对调用模块时输入一长串模块索引感到头疼？说实在的，数量少的时候或许还可以勉强忍受，一旦程序规模上去了，这也是一项不容小觑的工程。

```python
#Bad

import urllib.request

url = r'http://www.landsblog.com'

req = urllib.request.Request(url)

response = urllib.request.urlopen(req)

#Good

form urllib import request 

url = r'http://www.landsblog.com'

req = request.Request(url)

response = request.urlopen(req)
```

这样是不是节省了一点时间呢？

但是这样简写可能造成模块名重复的问题，如何解决呢？

```python
from module_a import fun as a_fun
from module_b import fun as b_fun
```

这样的方法还适用于模块名比较长的模块，笔者印象最深的就是BeautifulSoup模块

```python
from bs4 import BeautifulSoup as BS

html = '''
       <html>
            ......
       </html>
       '''
soup = BS(html)
```

省时省力。

## 2.关于 "_" ##

这是一个非常有用的功能，可惜很少人知道。

当你在交互界面敲代码，获得一个临时结果，却没有用变量名去保存它的时候，可以用"_"来获取最近一次临时结果。

```python
>>> 1 + 1
2
>>> _
2
```

在"_"中存储最后输出的值。这在交互式模式中是非常有用的，当你在过程中没有保存计算结果，或者你想看最后一步执行的输出结果。

## 3.合并字符串

这是一个老生常谈的问题，当我们需要将数个字符串合并的时候，习惯性的使用"+"作为连接字符串的手段。

然而，由于不可变对象在内存中生成后无法修改，合并后的字符串会重新开辟出一块内存空间来存储。这样像滚雪球一样，将内存快速消耗殆尽。

```python
# Bad
string = ['a','b','c','d','e','f','g','h']

def fun(string):
    all_string = ''
    for i in string:
        all_string += i
    return all_string


# Good
string = ['a','b','c','d','e','f','g,'h']

def fun(string):
    all_string = ''.join(string)
    return all_string
```

## 4.强大的zip()

它是Python的内建函数，zip函数接受任意多个（包括0个和1个）序列作为参数，返回一个包含tuple的list。zip()函数可以在很多场景简化你的代码。

**矩阵的行列互换**

```python
# Bad
a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

re_a = [[row[col] for row in a] for col in range(len(a))]

>>> [[1, 4, 7], [2, 5, 8], [3, 6, 9]]

# Good
a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]

re_a = list(zip(*a))

>>> [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
```

**交换dict的键值**

```python
# Bad
a = {'a': 1, 'b': 2, 'c': 3, 'd': 4}

def reverse_dict(a):
    new_dict = {}
    for k,v in m.items():
        new_dict[v] = k
    return new_dict

# Good
a = {'a': 1, 'b': 2, 'c': 3, 'd': 4}

def reverse_dict(a):
    k = a.keys()
    v = a.values()
    new_dict = dict(zip(v, k))
    return new_dict
```

**合并list相邻项**

```python
a = [1, 2, 3, 4, 5, 6]

list(zip( a[::2], a[1::2] ))
>>> [(1, 2), (3, 4), (5, 6)]
```

## 5.变量值交换

```python
# Bad
tmp = a
a = b
b = tmp

#Good
a, b = b, a
```

## 6.在循环中获取索引(数组下标)？

```python
a = [8, 23, 45, 12, 78]
for index, value in enumerate(a):
print(index , value)
```

## 7.如何在只用一行捕获多个异常？

```python
try:
    pass
except (ExceptionA,ExceptionB,.....) as e:
    pass
```

## 8.把列表分割成同样大小的块？

```python
a = [1, 2, 3, 4, 5, 6]

list(zip( *[iter(a)]*2 ))
>>> [(1, 2), (3, 4), (5, 6)]
```

## 9.如何查找列表中某个元素的下标？

```python
a = ['a', 'b', 'c', 'd', 'e', 'f']

a_i = a.index(a)
>>> 0 
```

## 10.如何快速反转字符串？

```python
#Bad
a = 'Python is a powerful languange.'

list_a = list(a)
list_a.reverse()
re_a = ''.join(list_a) 

#Good
a = 'Python is a powerful languange.'
re_a = a[::-1]
```

## 11.数值比较

```python
x = 2
if 1< x <3:
    print(x)
>>> 2
if 1< x >0:
    print(x)
>>> 2
```

## 12.优雅的打开文件

平时在使用类似文件的流对象时，使用完毕后要调用close方法关闭。with…as语句提供了一个非常方便的替代方法：open打开文件后将返回的文件流对象赋值给f，然后在with语句块中使用。with语句块完毕之后，会隐藏地自动关闭文件。

```python
with open('nothing.txt','r') as f:
    f.read()
```

## 13.和你的内存说再见

```python
crash = dict(zip(range(10 **0xA), range(10 **0xA))) 
```

欢迎交流，转载请注明出处~ (^ _ ^)/~~





分类: [Python学习笔记](https://www.cnblogs.com/Lands-ljk/category/810998.html)