# [Lua的string和string库总结](https://www.cnblogs.com/zrtqsk/p/4372889.html)



　　Lua有7种数据类型，分别是nil、boolean、number、string、table、function、userdata。这里我总结一下Lua的string类型和string库，复习一下，以便加深记忆。

个人认为string是Lua编程使用数据结构的时候，重要性仅次于table的类型。十分重要！

**一、string基础。**

　　Lua并没有字符类型，Lua的string类型表示字符序列。所以，长度为1的string就表示了单个字符。Lua的字符类型有这些特征：

1、string字符序列中的字符采用完全8位编码，即可以存放任何二进制数据。

2、string是不可变的值。相当于java中的String类。每次修改都会返回一个新的string，而原有的string保持不变。如：

```
a = "abc"
b = a
a = "cba"
print(a)        -->cba
print(b)        -->abc
```

 

3、string的字面值，用匹配的单引号、双引号括起来都行。同时跟其他语言一样可以用反斜杠“\”的转义序列。如：

```
c = ‘abc\ndef’            
print(c)        -->输出换行了    
```

 

4、可以通过<转义符“\”+数值>来指定字符串中的字符，数值为最多3位数组成的序列。如：

```
d = "\97\98c"
print(d)        --abc
```

 

5、可以用一对[[XXX]]来界定一个字符串XXX。字符串可以为任意字符。如：

```
e = [[it's mine!
it's not yours! --'Oh'   --"Wow!!!!"
]]
print (e)        --输出[[]]里面的所有内容
```

 

6、如上的表示有一个bug，就是当字符串包含[[或]]子串的时候，会表达错误。Lua提供了[===[XXX]===]这样的形式来包含字符串XXX。其中，两边中括号之间的“=”数量要匹配。如：

```
f = [==[abc[=]defgh[[=]]]==]
print(f)        --abc[=]defgh[[=]]
```

 

7、Lua提供运行时数字和字符的自动转换。即一个数字和一个字符串相加，Lua会尝试把这个字符串转换成数字再求值。不需要我们显式转换。如：

```
print("5"+6)        --11
print("5e-2"+3)        --3.05
```

 

8、长度操作符“#”，可用于求字符串的长度，即字符串包含的字符数。如：

```
print(#"abcd\n")    --5
str = "abc"
print(#str)            --3
```

 

 

 

**二、string库**

　　（科普一下，估计有不少Lua初学者跟我一样第一次看到Lua的API的时候都会很奇怪，像string.byte (s [, i [, j]])里面这些“[]”都是些什么来的？呵呵，中括号代表可选的参数。

就是说，可以这样调用：string.byte("abc")，也可以这样调用：string.byte("abc",1)，当然也可以这样调用：string.byte ("abc",1,-1);）

　　API的详细解释不用重复造轮子了吧，云神已经翻译了，[看这里](http://cloudwu.github.io/lua53doc/contents.html)，这边我简要解释一下。

　　1、**string.byte (s [, i [, j]])**

　　返回字符串的内部数字编码，i、j为字符串的索引，i、j限定了多少字符就返回多少值。如下：

```
k1,k2,k3 = string.byte("abcdef",1,3)
print (k1,k2,k3)    --97    98    99
```

 

　　2、**string.char (···)**

　　跟byte()相反，把数字编码转换为字符串。如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
s = string.char(97,98,99)
print(s)            --abc

n = string.char()
print(n)            --什么都没输出
print(type(n))        --string
print(string.char(string.byte("hello",1,-2)))    --hell
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

　　3、**string.dump (function [, strip])**

　　这个函数是用来序列化函数的。传入一个函数，返回一个字符串。通过load该字符串可以反序列化该函数。用法如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
function max( a,b )
    return a>b and a or b
end
--序列化
du = string.dump(max)
print(type(du))        --string
print(du)            --LuaQ
--反序列化
max2 = load(du)    
--调用函数
print(max2(1,2))    --2
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

　　4、**string.find (s, pattern [, init [, plain]])**

　　这个函数如其名，用来查找匹配的pattern，返回该pattern的索引。找到一个匹配就返回。如果找不到，返回空。如下：

```
txt = "it's very very good!"
i ,j = string.find(txt, "very")
print(i,j)    --6    9
i ,j = string.find(txt, "big")
print(i,j)    --nil    nil　　
```

 

　　5、**string.format (formatstring, ···)**

　　这个函数用来格式化字符串。API文档很复杂，用法很多，可查看文档。如下：

```
print(string.format("i want %d apples", 5))    --i want 5 apples
```

 

　　6、string.match (s, pattern [, init])

　　这个函数与find()函数类似，不同的是，find返回匹配的索引，这个函数返回第一个匹配的内容本身，如下：

```
print(string.format("i want %d apples", 5))    --i want 5 apples
```

 

　　7、**string.gmatch (s, pattern)**

　　这个函数基本就是用来配合for循环使用的，返回一个迭代器函数，每次调用这个迭代器函数都会返回一个匹配该字符串的值。

　　Lua5.3参考手册里面的示例很经典了，如下：

```
s = "hello world from Lua"
for w in string.gmatch(s, "%a+") do
    print(w)    --连续输出每个单词
end
```

 

　　8、**string.gsub (s, pattern, repl [, n])**

这个函数用来进行字符替换的。将每个匹配的字符串替换成指定的字符串repl。返回替换完毕的字符串和替换的次数。个人觉得这个函数非常有意思。

若repl为函数则会用匹配到的参数作为参数调用这个函数，若repl为table，则会用匹配到的参数作为键去查找这个表。如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
--字符串
print(string.gsub("i have an apple", "apple", "peach"))
--函数
function ff( arg )
    print("function arg : " .. arg)
end
print(string.gsub("my name is qsk", "%a+", ff))
--table
t = {}
metat = {}
metat.__index = function ( table,key )        
    return "!!" .. key
end
setmetatable(t, metat)
print(string.gsub("my name is qsk", "%a+", t))
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　测试表的时候，给该表设置了一个元表。输出如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
i have an peach    1
function arg : my
function arg : name
function arg : is
function arg : qsk
my name is qsk    4
!!my !!name !!is !!qsk    4
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

匹配字符串中的单词，匹配了4次所以输出的第二个参数是4. 

 

　　9、**string.len (s)、string.lower (s)、string.upper (s)**

　　这个两个函数都太简单了，一个是求字符串的长度。另一个是将字符串转换为小写。如下：

```
print(string.len("abcd"))        --4
print(string.lower("MACOS"))    --macos
print(string.upper("12abAB"))    --12ABAB
```

 

　　10、**string.rep (s, n [, sep])**

　　这个函数就是用来将某个字符串自我复制链接起来。如下：

```
print(string.rep("s", 5,"-"))        --s-s-s-s-s
print(string.rep("ab", 5))            --ababababab
```

 

　　11、**string.reverse (s)**

　　如名，用来反转字符串，反转字符串中字符的序列。如下：

```
print(string.reverse("abcdefg"))        --gfedcba
```

 

　　12、**string.sub (s, i [, j])**

　　这个函数就是用来截取字符串。很简单，如下：

```
print(string.sub("abcdefg", 3,5))        --cde
```

 

　　我们可以看到，Lua的字符串处理实在是很强大。相对于Java的String增加了很多匹配函数，使用起来会方便很多。

　　关于Lua的string还有一项更强大的功能，那就是Lua的字符串“模式”，用于字符串匹配，具体的用法，参照云神翻译的[《Lua5.3参考手册》](http://cloudwu.github.io/lua53doc/contents.html)就好了。

 