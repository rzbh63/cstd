
# CMeshO数据结构1 - paohui0134的博客 - CSDN博客


2016年11月16日 22:15:11[paohui0134](https://me.csdn.net/paohui0134)阅读数：324


# CMeshO数据结构1
CMeshO是meshlab中存储mesh数据的类，继承自vcg中的类
```python
class
```
```python
CMeshO
```
```python
:
```
```python
public
```
```python
vcg::tri::TriMesh
```
```python
<
```
```python
vcg::vertex
```
```python
::
```
```python
vector_ocf
```
```python
<
```
```python
CVertexO
```
```python
>,
```
```python
vcg::face::vector_ocf
```
```python
<
```
```python
CFaceO
```
```python
> >
```
```python
{
public
```
```python
:
```
```python
int sfn;
```
```python
//
```
```python
The
```
```python
number of selected faces.
    int svn;
```
```python
//
```
```python
The
```
```python
number of selected vertices.
```
```python
vcg:
```
```python
:Matrix44f
```
```python
Tr
```
```python
;
```
```python
//
```
```python
Usually
```
```python
it is the identity.
```
```python
It
```
```python
is applied
```
```python
in
```
```python
rendering
```
```python
and
```
```python
filters can
```
```python
or
```
```python
cannot use it. (most of the filter will ignore this)
    const
```
```python
vcg:
```
```python
:Box3f
```
```python
&trBB()
    {
        static
```
```python
vcg:
```
```python
:Box3f
```
```python
bb;
        bb.
```
```python
SetNull
```
```python
();
        bb.
```
```python
Add
```
```python
(
```
```python
Tr
```
```python
,bbox);
```
```python
return
```
```python
bb;
    }
};
```
主要分析基类vcg::tri::TriMesh
```python
设
```
```python
T1
```
```python
=
```
```python
vcg:
```
```python
:vertex
```
```python
:
```
```python
:vector_ocf<CVertexO>
```
```python
T2
```
```python
=
```
```python
vcg:
```
```python
:face
```
```python
:
```
```python
:vector_ocf<CFaceO>
```
```python
X
```
```python
=
```
```python
BaseMeshTypeHolder
```
```python
<typename
```
```python
T1::
```
```python
value_type:
```
```python
:TypesPool>
```
```python
class
```
```python
TriMesh
```
```python
:
```
```python
public
```
```python
MArity4
```
```python
<
```
```python
X
```
```python
,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container3
```
```python
,
```
```python
Der
```
```python
>
```
大致继承关系如下
```python
TriMesh：MArity4
```
```python
<
```
```python
X,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container3
```
```python
,
```
```python
Der
```
```python
>
```
```python
: 13
Der
```
```python
<
```
```python
MArity3
```
```python
<
```
```python
X
```
```python
,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container2
```
```python
,
```
```python
Der
```
```python
>
```
```python
,Container3>:      12
MeshTypeHolder
```
```python
<
```
```python
T,Container3,
```
```python
Container3::value_type::IAm
```
```python
>
```
```python
:              11
MArity3
```
```python
<
```
```python
X,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
,
```
```python
Container2
```
```python
,
```
```python
Der
```
```python
>
```
```python
:                      10
Der
```
```python
<
```
```python
MArity2
```
```python
<
```
```python
X
```
```python
,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
>
```
```python
,Container2>:                       9
MeshTypeHolder
```
```python
<
```
```python
T,Container2,
```
```python
Container2::value_type::IAm
```
```python
>
```
```python
:              8
MArity2
```
```python
<
```
```python
X,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
,
```
```python
T2
```
```python
,
```
```python
Der
```
```python
>
```
```python
:                                       7
Der
```
```python
<
```
```python
MArity1
```
```python
<
```
```python
X
```
```python
,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
>
```
```python
,T2>:                                        6
MeshTypeHolder
```
```python
<
```
```python
T,T2,
```
```python
T2::value_type::IAm
```
```python
>
```
```python
:              5
MArity1
```
```python
<
```
```python
X,
```
```python
T1
```
```python
,
```
```python
Der
```
```python
>
```
```python
:                                                4
Der
```
```python
<
```
```python
X,
```
```python
T1
```
```python
>
```
```python
:                                                         3
MeshTypeHolder
```
```python
<
```
```python
X,T1,
```
```python
T1::value_type::IAm
```
```python
>
```
```python
:              2
BaseMeshTypeHolder
```
```python
<
```
```python
typename
```
```python
T1::value_type::TypesPool
```
```python
>
```
```python
1
```
从1开始分析，这是最底层的基类
```python
template <
```
```python
class
```
```python
TYPESPOOL
```
```python
>
```
```python
struct
```
```python
BaseMeshTypeHolder
```
```python
定义了与
```
```python
TYPESPOOL
```
```python
相关的一些类型，此时
```
```python
TYPESPOOL
```
```python
=
```
```python
vcg:
```
```python
:vertex
```
```python
:
```
```python
:vector_ocf<CVertexO>
```
```python
:
```
```python
:value_type
```
```python
::TypesPool
```
```python
也就是
```
```python
CVertexO::TypesPool
```
```python
,对于
```
```python
CVertexO
```
```python
之后分析
```
接着是第二行MeshTypeHolder
```python
MeshTypeHolder<X,T1, T1::value_type::IAm>,对应特化模板类：
```
```python
template
```
```python
<
```
```python
class
```
```python
T,
```
```python
typename
```
```python
CONT>
```
```python
struct
```
```python
MeshTypeHolder<T, CONT, AllTypes::AVertexType>:
```
```python
public
```
```python
T
定义了一些vector_ocf<CVertexO>相关的类型
```
第五行对应特化模板类：
```python
template
```
```python
<
```
```python
typename
```
```python
T,
```
```python
class
```
```python
CONT>
```
```python
struct
```
```python
MeshTypeHolder< T, CONT,  AllTypes::AFaceType>:
```
```python
public
```
```python
T 
定义了一些vector_ocf<CFaceO>相关的类型
```
第八行和第十一行，因为Container3与Container4都对应默认的参数
```python
struct
```
```python
DummyContainer{
```
```python
struct
```
```python
value_type{
```
```python
typedef
```
```python
int
```
```python
IAm;}; };
所以MeshTypeHolder对应一般模板类
```
```python
template
```
```python
<
```
```python
class
```
```python
T,
```
```python
typename
```
```python
CONT,
```
```python
class
```
```python
TRAIT >
```
```python
struct
```
```python
MeshTypeHolder:
```
```python
public
```
```python
T {};
也就是空类
```
总的来说，参数`vcg::vertex::vector_ocf<CVertexO>`与`vcg::face::vector_ocf<CFaceO>`覆盖了基类BaseMeshTypeHolder中对于顶点容器与面容器的一些类型定义，而CVertexO：：TypesPool则决定了顶点、面、边、HEdge的具体类型

