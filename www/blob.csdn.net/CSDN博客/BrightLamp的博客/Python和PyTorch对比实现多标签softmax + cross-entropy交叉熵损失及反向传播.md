
# Python和PyTorch对比实现多标签softmax + cross-entropy交叉熵损失及反向传播 - BrightLamp的博客 - CSDN博客


2018年11月14日 21:18:14[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：1037所属专栏：



## 摘要
本文使用纯 Python 和 PyTorch 对比实现多标签softmax + cross-entropy交叉熵损失函数及其反向传播.
## 相关
*原理和详细解释, 请参考文章 :*
多标签softmax + cross-entropy交叉熵损失函数详解及反向传播中的梯度求导
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
```python
import
```
```python
torch
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
class
```
```python
SolfmaxEntropyLoss
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```
```python
self
```
```python
.
```
```python
nx
```
```python
=
```
```python
None
```
```python
self
```
```python
.
```
```python
ny
```
```python
=
```
```python
None
```
```python
self
```
```python
.
```
```python
softmax
```
```python
=
```
```python
None
```
```python
self
```
```python
.
```
```python
entropy
```
```python
=
```
```python
None
```
```python
self
```
```python
.
```
```python
loss
```
```python
=
```
```python
None
```
```python
self
```
```python
.
```
```python
dnx
```
```python
=
```
```python
None
```
```python
def
```
```python
__call__
```
```python
(
```
```python
self
```
```python
,
```
```python
nx
```
```python
,
```
```python
ny
```
```python
)
```
```python
:
```
```python
self
```
```python
.
```
```python
nx
```
```python
=
```
```python
nx
        self
```
```python
.
```
```python
ny
```
```python
=
```
```python
ny
        shifted_x
```
```python
=
```
```python
nx
```
```python
-
```
```python
np
```
```python
.
```
```python
max
```
```python
(
```
```python
nx
```
```python
)
```
```python
ex
```
```python
=
```
```python
np
```
```python
.
```
```python
exp
```
```python
(
```
```python
shifted_x
```
```python
)
```
```python
sum_ex
```
```python
=
```
```python
np
```
```python
.
```
```python
sum
```
```python
(
```
```python
ex
```
```python
)
```
```python
self
```
```python
.
```
```python
softmax
```
```python
=
```
```python
ex
```
```python
/
```
```python
sum_ex
        self
```
```python
.
```
```python
entropy
```
```python
=
```
```python
-
```
```python
np
```
```python
.
```
```python
log
```
```python
(
```
```python
self
```
```python
.
```
```python
softmax
```
```python
)
```
```python
*
```
```python
ny
        self
```
```python
.
```
```python
loss
```
```python
=
```
```python
np
```
```python
.
```
```python
sum
```
```python
(
```
```python
self
```
```python
.
```
```python
entropy
```
```python
)
```
```python
return
```
```python
self
```
```python
.
```
```python
loss
```
```python
def
```
```python
backward
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```
```python
self
```
```python
.
```
```python
dnx
```
```python
=
```
```python
self
```
```python
.
```
```python
softmax
```
```python
.
```
```python
copy
```
```python
(
```
```python
)
```
```python
*
```
```python
np
```
```python
.
```
```python
sum
```
```python
(
```
```python
self
```
```python
.
```
```python
ny
```
```python
)
```
```python
self
```
```python
.
```
```python
dnx
```
```python
-=
```
```python
self
```
```python
.
```
```python
ny
```
```python
return
```
```python
self
```
```python
.
```
```python
dnx

np
```
```python
.
```
```python
set_printoptions
```
```python
(
```
```python
precision
```
```python
=
```
```python
8
```
```python
,
```
```python
suppress
```
```python
=
```
```python
True
```
```python
,
```
```python
linewidth
```
```python
=
```
```python
120
```
```python
)
```
```python
x_numpy
```
```python
=
```
```python
np
```
```python
.
```
```python
array
```
```python
(
```
```python
[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
7
```
```python
,
```
```python
8
```
```python
,
```
```python
9
```
```python
]
```
```python
,
```
```python
dtype
```
```python
=
```
```python
np
```
```python
.
```
```python
float
```
```python
)
```
```python
y_numpy
```
```python
=
```
```python
np
```
```python
.
```
```python
array
```
```python
(
```
```python
[
```
```python
0.1
```
```python
,
```
```python
0.2
```
```python
,
```
```python
0.3
```
```python
,
```
```python
0.4
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.6
```
```python
,
```
```python
0.7
```
```python
,
```
```python
0.8
```
```python
,
```
```python
0.9
```
```python
]
```
```python
,
```
```python
dtype
```
```python
=
```
```python
np
```
```python
.
```
```python
float
```
```python
)
```
```python
x_tensor
```
```python
=
```
```python
torch
```
```python
.
```
```python
tensor
```
```python
(
```
```python
x_numpy
```
```python
,
```
```python
requires_grad
```
```python
=
```
```python
True
```
```python
)
```
```python
y_tensor
```
```python
=
```
```python
torch
```
```python
.
```
```python
tensor
```
```python
(
```
```python
y_numpy
```
```python
)
```
```python
solfmax_entropy_loss
```
```python
=
```
```python
SolfmaxEntropyLoss
```
```python
(
```
```python
)
```
```python
loss_numpy
```
```python
=
```
```python
solfmax_entropy_loss
```
```python
(
```
```python
x_numpy
```
```python
,
```
```python
y_numpy
```
```python
)
```
```python
dx_numpy
```
```python
=
```
```python
solfmax_entropy_loss
```
```python
.
```
```python
backward
```
```python
(
```
```python
)
```
```python
log_softmax_layer
```
```python
=
```
```python
torch
```
```python
.
```
```python
nn
```
```python
.
```
```python
LogSoftmax
```
```python
(
```
```python
dim
```
```python
=
```
```python
0
```
```python
)
```
```python
log_softmax_tensor
```
```python
=
```
```python
log_softmax_layer
```
```python
(
```
```python
x_tensor
```
```python
)
```
```python
entropy_tensor
```
```python
=
```
```python
-
```
```python
log_softmax_tensor
```
```python
*
```
```python
y_tensor
loss_tensor
```
```python
=
```
```python
entropy_tensor
```
```python
.
```
```python
sum
```
```python
(
```
```python
)
```
```python
loss_tensor
```
```python
.
```
```python
backward
```
```python
(
```
```python
)
```
```python
dx_tensor
```
```python
=
```
```python
x_tensor
```
```python
.
```
```python
grad
```
```python
print
```
```python
(
```
```python
loss_numpy
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
loss_tensor
```
```python
.
```
```python
data
```
```python
.
```
```python
numpy
```
```python
(
```
```python
)
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
)
```
```python
print
```
```python
(
```
```python
dx_numpy
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
dx_tensor
```
```python
.
```
```python
data
```
```python
.
```
```python
numpy
```
```python
(
```
```python
)
```
```python
)
```
```python
"""
代码输出:
14.0634827759
14.063482775853203
[-0.09904564 -0.19740579 -0.29294821 -0.38083126 -0.44789396 -0.45836109 -0.31498552  0.24657787  1.9448936 ]
[-0.09904564 -0.19740579 -0.29294821 -0.38083126 -0.44789396 -0.45836109 -0.31498552  0.24657787  1.9448936 ]
"""
```

