
# C++范型程序补充阅读 - 迂者-贺利坚的专栏 - CSDN博客

2016年06月03日 10:05:27[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：2050个人分类：[C++																](https://blog.csdn.net/sxhelijian/article/category/908261)



(1)下面程序通过输入流迭代器来填充vector容器，通过输出流迭代器来写入“流cout”。程序执行时，如果输入为：1 2 3 4 5 q
则输出为：1,2,3,4,5,
1 | 2 | 3 | 4 | 5 |
请填空将程序补充完整。
```python
#include<iostream>
```
```python
#include<vector>
```
```python
#include<algorithm>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main()
{
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
col1;
   istream_iterator<
```
```python
int
```
```python
> intreader(
```
```python
cin
```
```python
),eof;;
```
```python
while
```
```python
(intreader!=eof)
   {
      col1.push_back(_______________);
```
```python
// （1）
```
```python
_______________;
```
```python
// （2）
```
```python
}
   _______________ intwriter(
```
```python
cout
```
```python
,
```
```python
","
```
```python
);
```
```python
// （3）
```
```python
copy(col1.begin(),col1.end(),intwriter);
```
```python
cout
```
```python
<<endl;
   copy(col1.begin(),col1.end(),_______________);
```
```python
// （4）
```
```python
cout
```
```python
<<endl;
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
参考解答：
（1）*intreader  　（2）++intreader
（3）ostream_iterator         （4）ostream_iterator(cout,” | “)
等价程序：
```python
#include<iostream>
```
```python
#include<vector>
```
```python
#include<algorithm>
```
```python
#include<iterator>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main()
{
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
col1(
```
```python
5
```
```python
);
   copy(istream_iterator<
```
```python
int
```
```python
>(
```
```python
cin
```
```python
),istream_iterator<
```
```python
int
```
```python
>(),col1.begin());
   ostream_iterator<
```
```python
int
```
```python
> intwriter(
```
```python
cout
```
```python
,
```
```python
","
```
```python
);   
   copy(col1.begin(),col1.end(),intwriter);
```
```python
cout
```
```python
<<endl;
   copy(col1.begin(),col1.end(),ostream_iterator<
```
```python
int
```
```python
>(
```
```python
cout
```
```python
,
```
```python
"|"
```
```python
));
```
```python
cout
```
```python
<<endl;
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
(2)set用法
```python
#include <algorithm>
```
```python
#include<set>
```
```python
#include<iterator>
```
```python
#include<iostream>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main()
{
```
```python
set
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
eg1;
```
```python
//插入
```
```python
eg1.insert(
```
```python
1
```
```python
);
    eg1.insert(
```
```python
100
```
```python
);
    eg1.insert(
```
```python
5
```
```python
);
    eg1.insert(
```
```python
1
```
```python
);
```
```python
//元素1因为已经存在所以set中不会再次插入1
```
```python
eg1.insert(
```
```python
10
```
```python
);
    eg1.insert(
```
```python
9
```
```python
);
```
```python
//遍历set，可以发现元素是有序的
```
```python
set
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
::iterator set_iter=eg1.begin();
```
```python
cout
```
```python
<<
```
```python
"Set named eg1:"
```
```python
<<endl;
```
```python
for
```
```python
(; set_iter!=eg1.end(); set_iter++)
```
```python
cout
```
```python
<<*set_iter<<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<<endl;
```
```python
//使用size()函数可以获得当前元素个数
```
```python
cout
```
```python
<<
```
```python
"Now there are "
```
```python
<<eg1.size()<<
```
```python
" elements in the set eg1"
```
```python
<<endl;
```
```python
if
```
```python
(eg1.find(
```
```python
200
```
```python
)==eg1.end())
```
```python
//find()函数可以查找元素是否存在
```
```python
cout
```
```python
<<
```
```python
"200 isn't in the set eg1"
```
```python
<<endl;
```
```python
set
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
eg2;
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
6
```
```python
; i<
```
```python
15
```
```python
; i++)
        eg2.insert(i);
```
```python
cout
```
```python
<<
```
```python
"Set named eg2:"
```
```python
<<endl;
```
```python
for
```
```python
(set_iter=eg2.begin(); set_iter!=eg2.end(); set_iter++)
```
```python
cout
```
```python
<<*set_iter<<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<<endl;
```
```python
//获得两个set的并
```
```python
set
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
eg3;
```
```python
cout
```
```python
<<
```
```python
"Union:"
```
```python
;
    set_union(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),
              insert_iterator<
```
```python
set
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
>(eg3,eg3.begin()));
```
```python
//注意第五个参数的形式
```
```python
copy(eg3.begin(),eg3.end(),ostream_iterator<
```
```python
int
```
```python
>(
```
```python
cout
```
```python
,
```
```python
" "
```
```python
));
```
```python
cout
```
```python
<<endl;
```
```python
//获得两个set的交，注意进行集合操作之前接收结果的set要调用clear()函数清空一下
```
```python
eg3.clear();
    set_intersection(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<
```
```python
set
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
>(eg3,eg3.begin()));
```
```python
cout
```
```python
<<
```
```python
"Intersection:"
```
```python
;
    copy(eg3.begin(),eg3.end(),ostream_iterator<
```
```python
int
```
```python
>(
```
```python
cout
```
```python
,
```
```python
" "
```
```python
));
```
```python
cout
```
```python
<<endl;
```
```python
//获得两个set的差
```
```python
eg3.clear();
    set_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<
```
```python
set
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
>(eg3,eg3.begin()));
```
```python
cout
```
```python
<<
```
```python
"Difference:"
```
```python
;
    copy(eg3.begin(),eg3.end(),ostream_iterator<
```
```python
int
```
```python
>(
```
```python
cout
```
```python
,
```
```python
" "
```
```python
));
```
```python
cout
```
```python
<<endl;
```
```python
//获得两个set的对称差，也就是假设两个集合分别为A和B那么对称差为AUB-A∩B
```
```python
eg3.clear();
    set_symmetric_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<
```
```python
set
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
>(eg3,eg3.begin()));
    copy(eg3.begin(),eg3.end(),ostream_iterator<
```
```python
int
```
```python
>(
```
```python
cout
```
```python
,
```
```python
" "
```
```python
));
```
```python
cout
```
```python
<<endl;
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
(3)multiset
```python
#include <algorithm>
```
```python
#include<set>
```
```python
#include<iterator>
```
```python
#include<iostream>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main()
{
```
```python
multiset
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
eg1;
```
```python
//插入
```
```python
eg1.insert(
```
```python
1
```
```python
);
    eg1.insert(
```
```python
100
```
```python
);
    eg1.insert(
```
```python
5
```
```python
);
    eg1.insert(
```
```python
1
```
```python
);
```
```python
//是否插入
```
```python
eg1.insert(
```
```python
10
```
```python
);
    eg1.insert(
```
```python
9
```
```python
);
```
```python
//遍历set，可以发现元素是有序的
```
```python
multiset
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
::iterator set_iter=eg1.begin();
```
```python
cout
```
```python
<<
```
```python
"Set named eg1:"
```
```python
<<endl;
```
```python
for
```
```python
(; set_iter!=eg1.end(); set_iter++)
```
```python
cout
```
```python
<<*set_iter<<
```
```python
" "
```
```python
;
```
```python
cout
```
```python
<<endl;
```
```python
//使用size()函数可以获得当前元素个数
```
```python
cout
```
```python
<<
```
```python
"Now there are "
```
```python
<<eg1.size()<<
```
```python
" elements in the set eg1"
```
```python
<<endl;
```
```python
if
```
```python
(eg1.find(
```
```python
200
```
```python
)==eg1.end())
```
```python
//find()函数可以查找元素是否存在
```
```python
cout
```
```python
<<
```
```python
"200 isn't in the set eg1"
```
```python
<<endl;
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
(4)理解map
```python
#include <algorithm>
```
```python
#include<map>
```
```python
#include<iterator>
```
```python
#include<iostream>
```
```python
#include<cstring>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
struct
```
```python
ltstr
{
```
```python
bool
```
```python
operator
```
```python
()(
```
```python
const
```
```python
char
```
```python
* s1,
```
```python
const
```
```python
char
```
```python
* s2)
```
```python
const
```
```python
{
```
```python
return
```
```python
strcmp
```
```python
(s1, s2) <
```
```python
0
```
```python
;
    }
};
```
```python
int
```
```python
main()
{
```
```python
map
```
```python
<
```
```python
const
```
```python
char
```
```python
*,
```
```python
int
```
```python
, ltstr>
```
```python
months;
    months[
```
```python
"january"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"february"
```
```python
] =
```
```python
28
```
```python
;
    months[
```
```python
"march"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"april"
```
```python
] =
```
```python
30
```
```python
;
    months[
```
```python
"may"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"june"
```
```python
] =
```
```python
30
```
```python
;
    months[
```
```python
"july"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"august"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"september"
```
```python
] =
```
```python
30
```
```python
;
    months[
```
```python
"october"
```
```python
] =
```
```python
31
```
```python
;
    months[
```
```python
"november"
```
```python
] =
```
```python
30
```
```python
;
    months[
```
```python
"december"
```
```python
] =
```
```python
31
```
```python
;
```
```python
cout
```
```python
<<
```
```python
"june -> "
```
```python
<< months[
```
```python
"june"
```
```python
] << endl;
```
```python
map
```
```python
<
```
```python
const
```
```python
char
```
```python
*,
```
```python
int
```
```python
, ltstr>
```
```python
::iterator cur  = months.find(
```
```python
"june"
```
```python
);
```
```python
map
```
```python
<
```
```python
const
```
```python
char
```
```python
*,
```
```python
int
```
```python
, ltstr>
```
```python
::iterator prev = cur;
```
```python
map
```
```python
<
```
```python
const
```
```python
char
```
```python
*,
```
```python
int
```
```python
, ltstr>
```
```python
::iterator next = cur;
    ++next;
    --prev;
```
```python
cout
```
```python
<<
```
```python
"Previous (in alphabetical order) is "
```
```python
<< (*prev).first << endl;
```
```python
cout
```
```python
<<
```
```python
"Next (in alphabetical order) is "
```
```python
<< (*next).first << endl;
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
(5)map再例
```python
#include <map>
```
```python
#include <iostream>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main( )
{
```
```python
map
```
```python
<
```
```python
int
```
```python
,
```
```python
int
```
```python
>
```
```python
m1, m2, m3;
```
```python
map
```
```python
<
```
```python
int
```
```python
,
```
```python
int
```
```python
>
```
```python
::iterator m1_Iter;
    m1.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
1
```
```python
,
```
```python
10
```
```python
) );
    m1.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
2
```
```python
,
```
```python
20
```
```python
) );
    m1.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
3
```
```python
,
```
```python
30
```
```python
) );
    m2.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
10
```
```python
,
```
```python
100
```
```python
) );
    m2.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
20
```
```python
,
```
```python
200
```
```python
) );
    m3.insert ( pair <
```
```python
int
```
```python
,
```
```python
int
```
```python
>  (
```
```python
30
```
```python
,
```
```python
300
```
```python
) );
```
```python
cout
```
```python
<<
```
```python
"The original map m1 is:"
```
```python
;
```
```python
for
```
```python
( m1_Iter = m1.begin( ); m1_Iter != m1.end( ); m1_Iter++ )
```
```python
cout
```
```python
<<
```
```python
" "
```
```python
<< m1_Iter->second;
```
```python
cout
```
```python
<<
```
```python
"."
```
```python
<< endl;
```
```python
// This is the member function version of swap
```
```python
//m2 is said to be the argument map; m1 the target map
```
```python
m1.swap( m2 );
```
```python
cout
```
```python
<<
```
```python
"After swapping with m2, map m1 is:"
```
```python
;
```
```python
for
```
```python
( m1_Iter = m1.begin( ); m1_Iter != m1.end( ); m1_Iter++ )
```
```python
cout
```
```python
<<
```
```python
" "
```
```python
<< m1_Iter -> second;
```
```python
cout
```
```python
<<
```
```python
"."
```
```python
<< endl;
```
```python
cout
```
```python
<<
```
```python
"After swapping with m2, map m2 is:"
```
```python
;
```
```python
for
```
```python
( m1_Iter = m2.begin( ); m1_Iter != m2.end( ); m1_Iter++ )
```
```python
cout
```
```python
<<
```
```python
" "
```
```python
<< m1_Iter -> second;
```
```python
cout
```
```python
<<
```
```python
"."
```
```python
<< endl;
```
```python
// This is the specialized template version of swap
```
```python
swap( m1, m3 );
```
```python
cout
```
```python
<<
```
```python
"After swapping with m3, map m1 is:"
```
```python
;
```
```python
for
```
```python
( m1_Iter = m1.begin( ); m1_Iter != m1.end( ); m1_Iter++ )
```
```python
cout
```
```python
<<
```
```python
" "
```
```python
<< m1_Iter -> second;
```
```python
cout
```
```python
<<
```
```python
"."
```
```python
<< endl;
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
(6)map三例
```python
#include <algorithm>
```
```python
#include<map>
```
```python
#include<iterator>
```
```python
#include<iostream>
```
```python
#include<cstring>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main()
{
```
```python
map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
mymap;
    mymap[
```
```python
'a'
```
```python
]=
```
```python
10
```
```python
;
    mymap[
```
```python
'b'
```
```python
]=
```
```python
60
```
```python
;
    mymap[
```
```python
'c'
```
```python
]=
```
```python
30
```
```python
;
    mymap[
```
```python
'd'
```
```python
]=
```
```python
90
```
```python
;
    mymap[
```
```python
'e'
```
```python
]=
```
```python
50
```
```python
;
```
```python
map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
second(mymap);
```
```python
map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
third(mymap.begin(),mymap.end());
```
```python
map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
::key_compare key_comp;
```
```python
map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
::iterator it;
    it=mymap.begin();
```
```python
for
```
```python
(;it!=mymap.end();it++)
    {
```
```python
cout
```
```python
<<it->first<<
```
```python
":"
```
```python
<<it->second<<endl;
    }
```
```python
cout
```
```python
<<
```
```python
"================================="
```
```python
<<endl;
    second.clear();
    second[
```
```python
'a'
```
```python
]=
```
```python
1002
```
```python
;
    second[
```
```python
'b'
```
```python
]=
```
```python
10023
```
```python
;
```
```python
while
```
```python
(!second.empty())
    {
```
```python
cout
```
```python
<< second.begin()->first <<
```
```python
" => "
```
```python
;
```
```python
cout
```
```python
<< second.begin()->second << endl;
        second.erase(second.begin());
    }
```
```python
cout
```
```python
<<
```
```python
"================================="
```
```python
<<endl;
    mymap.insert(pair<
```
```python
char
```
```python
,
```
```python
int
```
```python
>(
```
```python
'f'
```
```python
,
```
```python
100
```
```python
) );
    mymap.insert(pair<
```
```python
char
```
```python
,
```
```python
int
```
```python
>(
```
```python
'g'
```
```python
,
```
```python
200
```
```python
) );
```
```python
cout
```
```python
<<
```
```python
"f => "
```
```python
<<mymap.find(
```
```python
'f'
```
```python
)->second<<endl;
```
```python
cout
```
```python
<<
```
```python
"g => "
```
```python
<<mymap.find(
```
```python
'g'
```
```python
)->second<<endl;
```
```python
cout
```
```python
<<
```
```python
"================================="
```
```python
<<endl;
    key_comp=mymap.key_comp();
```
```python
cout
```
```python
<<
```
```python
"mymap contains:\n"
```
```python
;
```
```python
char
```
```python
highest=mymap.rbegin()->first;
```
```python
// key value of last element
```
```python
it=mymap.begin();
```
```python
do
```
```python
{
```
```python
cout
```
```python
<< (*it).first <<
```
```python
" => "
```
```python
<< (*it).second << endl;
    }
```
```python
while
```
```python
( key_comp((*it++).first, highest) );
```
```python
cout
```
```python
<< endl;
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

