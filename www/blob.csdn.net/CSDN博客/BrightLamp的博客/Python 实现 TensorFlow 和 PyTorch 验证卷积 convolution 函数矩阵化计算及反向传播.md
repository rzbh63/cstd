
# Python 实现 TensorFlow 和 PyTorch 验证卷积 convolution 函数矩阵化计算及反向传播 - BrightLamp的博客 - CSDN博客


2019年01月05日 20:17:27[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：231



## 摘要
本文使用纯 Python 实现 TensorFlow 和 PyTorch 验证卷积 convolution 函数矩阵化计算及反向传播.
## 相关
*原理和详细解释, 请参考文章 :*
卷积 convolution 函数的矩阵化计算方法及其梯度的反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. Batch2ConvMatrix类和Conv2d类
文件目录 : vanilla_nn/convolution_matrix.py
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
Batch2ConvMatrix
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
,
```
```python
stride
```
```python
,
```
```python
kernel_h
```
```python
,
```
```python
kernel_w
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
stride
```
```python
=
```
```python
stride
        self
```
```python
.
```
```python
kernel_h
```
```python
=
```
```python
kernel_h
        self
```
```python
.
```
```python
kernel_w
```
```python
=
```
```python
kernel_w
        self
```
```python
.
```
```python
x
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
conv_size
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
x
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
x
```
```python
=
```
```python
x
        x_nums
```
```python
,
```
```python
x_channels
```
```python
,
```
```python
x_height
```
```python
,
```
```python
x_width
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
shape
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
x
```
```python
)
```
```python
conv_height
```
```python
=
```
```python
int
```
```python
(
```
```python
(
```
```python
x_height
```
```python
-
```
```python
self
```
```python
.
```
```python
kernel_h
```
```python
)
```
```python
/
```
```python
self
```
```python
.
```
```python
stride
```
```python
)
```
```python
+
```
```python
1
```
```python
conv_width
```
```python
=
```
```python
int
```
```python
(
```
```python
(
```
```python
x_width
```
```python
-
```
```python
self
```
```python
.
```
```python
kernel_w
```
```python
)
```
```python
/
```
```python
self
```
```python
.
```
```python
stride
```
```python
)
```
```python
+
```
```python
1
```
```python
scan
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
zeros
```
```python
(
```
```python
(
```
```python
x_nums
```
```python
,
```
```python
conv_height
```
```python
,
```
```python
conv_width
```
```python
,
```
```python
x_channels
```
```python
,
```
```python
self
```
```python
.
```
```python
kernel_h
```
```python
,
```
```python
self
```
```python
.
```
```python
kernel_w
```
```python
)
```
```python
)
```
```python
for
```
```python
n
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
x_nums
```
```python
)
```
```python
:
```
```python
for
```
```python
h
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
conv_height
```
```python
)
```
```python
:
```
```python
for
```
```python
w
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
conv_width
```
```python
)
```
```python
:
```
```python
for
```
```python
c
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
x_channels
```
```python
)
```
```python
:
```
```python
start_h
```
```python
=
```
```python
h
```
```python
*
```
```python
self
```
```python
.
```
```python
stride
                        start_w
```
```python
=
```
```python
w
```
```python
*
```
```python
self
```
```python
.
```
```python
stride
                        end_h
```
```python
=
```
```python
start_h
```
```python
+
```
```python
self
```
```python
.
```
```python
kernel_h
                        end_w
```
```python
=
```
```python
start_w
```
```python
+
```
```python
self
```
```python
.
```
```python
kernel_w
                        scan
```
```python
[
```
```python
n
```
```python
,
```
```python
h
```
```python
,
```
```python
w
```
```python
,
```
```python
c
```
```python
]
```
```python
=
```
```python
\
                            x
```
```python
[
```
```python
n
```
```python
,
```
```python
c
```
```python
,
```
```python
start_h
```
```python
:
```
```python
end_h
```
```python
,
```
```python
start_w
```
```python
:
```
```python
end_w
```
```python
]
```
```python
conv_matrix
```
```python
=
```
```python
scan
```
```python
.
```
```python
reshape
```
```python
(
```
```python
x_nums
```
```python
*
```
```python
conv_height
```
```python
*
```
```python
conv_width
```
```python
,
```
```python
-
```
```python
1
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
conv_size
```
```python
=
```
```python
[
```
```python
x_nums
```
```python
,
```
```python
x_channels
```
```python
,
```
```python
conv_height
```
```python
,
```
```python
conv_width
```
```python
]
```
```python
return
```
```python
conv_matrix
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
,
```
```python
dx2m
```
```python
)
```
```python
:
```
```python
dx
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
zeros_like
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
x
```
```python
)
```
```python
kh
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
kernel_h
        kw
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
kernel_w
        xn
```
```python
,
```
```python
xc
```
```python
,
```
```python
ch
```
```python
,
```
```python
cw
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
conv_size
        dx2m
```
```python
=
```
```python
dx2m
```
```python
.
```
```python
reshape
```
```python
(
```
```python
(
```
```python
xn
```
```python
,
```
```python
ch
```
```python
,
```
```python
cw
```
```python
,
```
```python
xc
```
```python
,
```
```python
kh
```
```python
,
```
```python
kw
```
```python
)
```
```python
)
```
```python
for
```
```python
n
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
xn
```
```python
)
```
```python
:
```
```python
for
```
```python
c
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
xc
```
```python
)
```
```python
:
```
```python
for
```
```python
h
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
ch
```
```python
)
```
```python
:
```
```python
for
```
```python
w
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
cw
```
```python
)
```
```python
:
```
```python
start_h
```
```python
=
```
```python
h
```
```python
*
```
```python
self
```
```python
.
```
```python
stride
                        start_w
```
```python
=
```
```python
w
```
```python
*
```
```python
self
```
```python
.
```
```python
stride
                        end_h
```
```python
=
```
```python
start_h
```
```python
+
```
```python
self
```
```python
.
```
```python
kernel_h
                        end_w
```
```python
=
```
```python
start_w
```
```python
+
```
```python
self
```
```python
.
```
```python
kernel_w
                        dx
```
```python
[
```
```python
n
```
```python
,
```
```python
c
```
```python
]
```
```python
[
```
```python
start_h
```
```python
:
```
```python
end_h
```
```python
,
```
```python
start_w
```
```python
:
```
```python
end_w
```
```python
]
```
```python
\
```
```python
+=
```
```python
dx2m
```
```python
[
```
```python
n
```
```python
,
```
```python
h
```
```python
,
```
```python
w
```
```python
,
```
```python
c
```
```python
]
```
```python
return
```
```python
dx
```
```python
class
```
```python
Conv2d
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
,
```
```python
stride
```
```python
,
```
```python
weight
```
```python
=
```
```python
None
```
```python
,
```
```python
bias
```
```python
=
```
```python
None
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
stride
```
```python
=
```
```python
stride
        self
```
```python
.
```
```python
weight
```
```python
=
```
```python
weight
        self
```
```python
.
```
```python
bias
```
```python
=
```
```python
bias
        self
```
```python
.
```
```python
b2m
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
x2m
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
w2m
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
dw
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
db
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
x
```
```python
)
```
```python
:
```
```python
wn
```
```python
,
```
```python
wc
```
```python
,
```
```python
wh
```
```python
,
```
```python
ww
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
shape
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
weight
```
```python
)
```
```python
if
```
```python
self
```
```python
.
```
```python
b2m
```
```python
is
```
```python
None
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
b2m
```
```python
=
```
```python
Batch2ConvMatrix
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
stride
```
```python
,
```
```python
wh
```
```python
,
```
```python
ww
```
```python
)
```
```python
x2m
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
b2m
```
```python
(
```
```python
x
```
```python
)
```
```python
w2m
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
weight
```
```python
.
```
```python
reshape
```
```python
(
```
```python
wn
```
```python
,
```
```python
-
```
```python
1
```
```python
)
```
```python
xn
```
```python
,
```
```python
xc
```
```python
,
```
```python
oh
```
```python
,
```
```python
ow
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
b2m
```
```python
.
```
```python
conv_size
        out_matrix
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
matmul
```
```python
(
```
```python
x2m
```
```python
,
```
```python
w2m
```
```python
.
```
```python
T
```
```python
)
```
```python
+
```
```python
self
```
```python
.
```
```python
bias
        out
```
```python
=
```
```python
out_matrix
```
```python
.
```
```python
reshape
```
```python
(
```
```python
(
```
```python
xn
```
```python
,
```
```python
oh
```
```python
,
```
```python
ow
```
```python
,
```
```python
wn
```
```python
)
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
x2m
```
```python
=
```
```python
x2m
        self
```
```python
.
```
```python
w2m
```
```python
=
```
```python
w2m
        out
```
```python
=
```
```python
out
```
```python
.
```
```python
transpose
```
```python
(
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
,
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
)
```
```python
)
```
```python
return
```
```python
out
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
,
```
```python
d_loss
```
```python
)
```
```python
:
```
```python
on
```
```python
,
```
```python
oc
```
```python
,
```
```python
oh
```
```python
,
```
```python
ow
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
shape
```
```python
(
```
```python
d_loss
```
```python
)
```
```python
d_loss
```
```python
=
```
```python
d_loss
```
```python
.
```
```python
transpose
```
```python
(
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
1
```
```python
)
```
```python
)
```
```python
d_loss
```
```python
=
```
```python
d_loss
```
```python
.
```
```python
reshape
```
```python
(
```
```python
(
```
```python
on
```
```python
*
```
```python
oh
```
```python
*
```
```python
ow
```
```python
,
```
```python
-
```
```python
1
```
```python
)
```
```python
)
```
```python
dw
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
matmul
```
```python
(
```
```python
d_loss
```
```python
.
```
```python
T
```
```python
,
```
```python
self
```
```python
.
```
```python
x2m
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
dw
```
```python
=
```
```python
dw
```
```python
.
```
```python
reshape
```
```python
(
```
```python
np
```
```python
.
```
```python
shape
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
weight
```
```python
)
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
db
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
d_loss
```
```python
,
```
```python
axis
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
dx2m
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
matmul
```
```python
(
```
```python
d_loss
```
```python
,
```
```python
self
```
```python
.
```
```python
w2m
```
```python
)
```
```python
dx
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
b2m
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
dx2m
```
```python
)
```
```python
return
```
```python
dx
```
## 2. TensorFlow验证
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
import
```
```python
tensorflow
```
```python
as
```
```python
tf
```
```python
from
```
```python
vanilla_nn
```
```python
.
```
```python
convolution_matrix
```
```python
import
```
```python
Conv2d
tf
```
```python
.
```
```python
enable_eager_execution
```
```python
(
```
```python
)
```
```python
tf
```
```python
.
```
```python
set_random_seed
```
```python
(
```
```python
123
```
```python
)
```
```python
np
```
```python
.
```
```python
random
```
```python
.
```
```python
seed
```
```python
(
```
```python
123
```
```python
)
```
```python
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
6
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
random
```
```python
.
```
```python
random
```
```python
(
```
```python
(
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
5
```
```python
,
```
```python
7
```
```python
)
```
```python
)
```
```python
x_tf
```
```python
=
```
```python
tf
```
```python
.
```
```python
constant
```
```python
(
```
```python
x_numpy
```
```python
)
```
```python
conv_tf
```
```python
=
```
```python
tf
```
```python
.
```
```python
layers
```
```python
.
```
```python
Conv2D
```
```python
(
```
```python
filters
```
```python
=
```
```python
4
```
```python
,
```
```python
kernel_size
```
```python
=
```
```python
3
```
```python
,
```
```python
strides
```
```python
=
```
```python
(
```
```python
2
```
```python
,
```
```python
2
```
```python
)
```
```python
,
```
```python
data_format
```
```python
=
```
```python
"channels_first"
```
```python
)
```
```python
with
```
```python
tf
```
```python
.
```
```python
GradientTape
```
```python
(
```
```python
persistent
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
as
```
```python
t
```
```python
:
```
```python
t
```
```python
.
```
```python
watch
```
```python
(
```
```python
x_tf
```
```python
)
```
```python
y_tf
```
```python
=
```
```python
conv_tf
```
```python
(
```
```python
x_tf
```
```python
)
```
```python
conv_numpy
```
```python
=
```
```python
Conv2d
```
```python
(
```
```python
stride
```
```python
=
```
```python
2
```
```python
,
```
```python
weight
```
```python
=
```
```python
conv_tf
```
```python
.
```
```python
get_weights
```
```python
(
```
```python
)
```
```python
[
```
```python
0
```
```python
]
```
```python
.
```
```python
transpose
```
```python
(
```
```python
3
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
0
```
```python
,
```
```python
1
```
```python
)
```
```python
,
```
```python
bias
```
```python
=
```
```python
conv_tf
```
```python
.
```
```python
get_weights
```
```python
(
```
```python
)
```
```python
[
```
```python
1
```
```python
]
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
conv_numpy
```
```python
(
```
```python
x_numpy
```
```python
)
```
```python
dy_numpy
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
random
```
```python
.
```
```python
random
```
```python
(
```
```python
y_numpy
```
```python
.
```
```python
shape
```
```python
)
```
```python
dy_tf
```
```python
=
```
```python
tf
```
```python
.
```
```python
constant
```
```python
(
```
```python
dy_numpy
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
conv_numpy
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
dy_numpy
```
```python
)
```
```python
dy_dx
```
```python
=
```
```python
t
```
```python
.
```
```python
gradient
```
```python
(
```
```python
y_tf
```
```python
,
```
```python
x_tf
```
```python
,
```
```python
dy_tf
```
```python
)
```
```python
dw_tf
```
```python
,
```
```python
db_tf
```
```python
=
```
```python
t
```
```python
.
```
```python
gradient
```
```python
(
```
```python
y_tf
```
```python
,
```
```python
conv_tf
```
```python
.
```
```python
weights
```
```python
,
```
```python
dy_tf
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
"y_numpy\n"
```
```python
,
```
```python
y_numpy
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"y_tf\n"
```
```python
,
```
```python
y_tf
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
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dx_numpy\n"
```
```python
,
```
```python
dx_numpy
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dx_tf\n"
```
```python
,
```
```python
dy_dx
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
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dw_numpy\n"
```
```python
,
```
```python
conv_numpy
```
```python
.
```
```python
dw
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dw_tf\n"
```
```python
,
```
```python
dw_tf
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
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"db_numpy\n"
```
```python
,
```
```python
conv_numpy
```
```python
.
```
```python
db
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
"db_tf\n"
```
```python
,
```
```python
db_tf
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
y_numpy
 [[-0.410179 -0.379269 -1.083951]
 [-0.340203 -0.57836  -0.620306]]
y_tf
 [[-0.410179 -0.379269 -1.083951]
 [-0.340203 -0.57836  -0.620306]]
dx_numpy
 [[-0.220716  0.140123 -0.246691  0.017347 -0.073237 -0.027538  0.058724]
 [-0.067507 -0.137531  0.273061 -0.048839  0.257607 -0.097253  0.445625]
 [-0.403482 -0.176619 -0.319394 -0.279691 -0.49433  -0.325839 -0.105012]
 [-0.114447 -0.17873   0.223171 -0.076912  0.27387  -0.123164  0.544131]
 [-0.089011 -0.26426   0.003028 -0.24742  -0.286531 -0.404775  0.071425]]
dx_tf
 [[-0.220716  0.140123 -0.246691  0.017347 -0.073237 -0.027538  0.058724]
 [-0.067507 -0.137531  0.273061 -0.048839  0.257607 -0.097253  0.445625]
 [-0.403482 -0.176619 -0.319394 -0.279691 -0.49433  -0.325839 -0.105012]
 [-0.114447 -0.17873   0.223171 -0.076912  0.27387  -0.123164  0.544131]
 [-0.089011 -0.26426   0.003028 -0.24742  -0.286531 -0.404775  0.071425]]
dw_numpy
 [[ 3.759475  4.10321   3.357347]
 [ 3.621035  3.578632  3.329442]
 [ 3.183582  4.388502  2.181801]]
dw_tf
 [[ 3.759475  2.767449  3.618431  3.229403]
 [ 3.718784  3.304698  3.817697  3.17113 ]
 [ 3.056882  1.940038  2.436826  2.322039]]
db_numpy
 [ 6.804211  5.354626  6.683215  5.545097]
db_tf
 [ 6.804211  5.354626  6.683215  5.545097]
 """
```
## 3. PyTorch 验证
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
from
```
```python
vanilla_nn
```
```python
.
```
```python
convolution_matrix
```
```python
import
```
```python
Conv2d
np
```
```python
.
```
```python
random
```
```python
.
```
```python
seed
```
```python
(
```
```python
123
```
```python
)
```
```python
torch
```
```python
.
```
```python
manual_seed
```
```python
(
```
```python
123
```
```python
)
```
```python
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
6
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
random
```
```python
.
```
```python
random
```
```python
(
```
```python
(
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
5
```
```python
,
```
```python
7
```
```python
)
```
```python
)
```
```python
x_torch
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
conv_torch
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
Conv2d
```
```python
(
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
(
```
```python
3
```
```python
,
```
```python
3
```
```python
)
```
```python
,
```
```python
stride
```
```python
=
```
```python
2
```
```python
)
```
```python
.
```
```python
double
```
```python
(
```
```python
)
```
```python
conv_numpy
```
```python
=
```
```python
Conv2d
```
```python
(
```
```python
stride
```
```python
=
```
```python
2
```
```python
,
```
```python
weight
```
```python
=
```
```python
conv_torch
```
```python
.
```
```python
weight
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
,
```
```python
bias
```
```python
=
```
```python
conv_torch
```
```python
.
```
```python
bias
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
y_numpy
```
```python
=
```
```python
conv_numpy
```
```python
(
```
```python
x_numpy
```
```python
)
```
```python
y_torch
```
```python
=
```
```python
conv_torch
```
```python
(
```
```python
x_torch
```
```python
)
```
```python
dy_numpy
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
random
```
```python
.
```
```python
random
```
```python
(
```
```python
y_numpy
```
```python
.
```
```python
shape
```
```python
)
```
```python
dy_torch
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
dy_numpy
```
```python
)
```
```python
y_torch
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
dy_torch
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
conv_numpy
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
dy_numpy
```
```python
)
```
```python
dx_torch
```
```python
=
```
```python
x_torch
```
```python
.
```
```python
grad
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
print
```
```python
(
```
```python
"y_numpy\n"
```
```python
,
```
```python
y_numpy
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"y_torch\n"
```
```python
,
```
```python
y_torch
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
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dx_numpy\n"
```
```python
,
```
```python
dx_numpy
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dx_torch\n"
```
```python
,
```
```python
dx_torch
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dw_numpy\n"
```
```python
,
```
```python
conv_numpy
```
```python
.
```
```python
dw
```
```python
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"dw_torch\n"
```
```python
,
```
```python
conv_torch
```
```python
.
```
```python
weight
```
```python
.
```
```python
grad
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
[
```
```python
0
```
```python
]
```
```python
[
```
```python
0
```
```python
]
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
"db_numpy\n"
```
```python
,
```
```python
conv_numpy
```
```python
.
```
```python
db
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
"db_torch\n"
```
```python
,
```
```python
conv_torch
```
```python
.
```
```python
bias
```
```python
.
```
```python
grad
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
y_numpy
 [[-0.400592 -0.385043 -0.5706  ]
 [-0.373743 -0.203867 -0.217639]]
y_torch
 [[-0.400592 -0.385043 -0.5706  ]
 [-0.373743 -0.203867 -0.217639]]
dx_numpy
 [[ 0.097144 -0.050192 -0.115058 -0.091209  0.023963  0.04729  -0.128907]
 [ 0.024621 -0.087676  0.238786  0.010403  0.297381 -0.112517  0.262634]
 [ 0.024772 -0.310948  0.129654 -0.152652  0.059663 -0.34918  -0.192246]
 [ 0.084056 -0.081453  0.207333 -0.126043  0.409333 -0.012     0.260899]
 [-0.026007 -0.175092 -0.040534 -0.303666  0.025274 -0.298108  0.191089]]
dx_torch
 [[ 0.097144 -0.050192 -0.115058 -0.091209  0.023963  0.04729  -0.128907]
 [ 0.024621 -0.087676  0.238786  0.010403  0.297381 -0.112517  0.262634]
 [ 0.024772 -0.310948  0.129654 -0.152652  0.059663 -0.34918  -0.192246]
 [ 0.084056 -0.081453  0.207333 -0.126043  0.409333 -0.012     0.260899]
 [-0.026007 -0.175092 -0.040534 -0.303666  0.025274 -0.298108  0.191089]]
dw_numpy
 [[ 3.569718  4.275913  4.107687]
 [ 3.066375  4.63871   3.118461]
 [ 3.838584  4.8143    3.607119]]
dw_torch
 [[ 3.569718  4.275913  4.107687]
 [ 3.066375  4.63871   3.118461]
 [ 3.838584  4.8143    3.607119]]
db_numpy
 [ 7.27808   6.566618  5.797415  4.544563]
db_torch
 [ 7.27808   6.566618  5.797415  4.544563]
y_numpy
 [[ 0.038209 -0.104653 -0.37583 ]
 [-0.101538 -0.192953 -0.152639]]
y_torch
 [[ 0.038209 -0.104653 -0.37583 ]
 [-0.101538 -0.192953 -0.152639]]
dx_numpy
 [[-0.040876  0.002517 -0.160813 -0.109504 -0.154843 -0.022449 -0.304437]
 [ 0.093016 -0.133319  0.289393 -0.048426  0.246752  0.000857 -0.040514]
 [-0.076863 -0.00029  -0.240455  0.11569  -0.242307  0.337566 -0.519987]
 [ 0.092797 -0.117967  0.250818 -0.041134  0.187269 -0.040959 -0.022162]
 [-0.077402 -0.066061 -0.083124  0.102747  0.026502  0.250037 -0.120596]]
dx_torch
 [[-0.040876  0.002517 -0.160813 -0.109504 -0.154843 -0.022449 -0.304437]
 [ 0.093016 -0.133319  0.289393 -0.048426  0.246752  0.000857 -0.040514]
 [-0.076863 -0.00029  -0.240455  0.11569  -0.242307  0.337566 -0.519987]
 [ 0.092797 -0.117967  0.250818 -0.041134  0.187269 -0.040959 -0.022162]
 [-0.077402 -0.066061 -0.083124  0.102747  0.026502  0.250037 -0.120596]]
dw_numpy
 [[ 3.759475  4.10321   3.357347]
 [ 3.621035  3.578632  3.329442]
 [ 3.183582  4.388502  2.181801]]
dw_torch
 [[ 3.759475  4.10321   3.357347]
 [ 3.621035  3.578632  3.329442]
 [ 3.183582  4.388502  2.181801]]
db_numpy
 [ 6.804211  5.354626  6.683215  5.545097]
db_torch
 [ 6.804211  5.354626  6.683215  5.545097]
 """
```

