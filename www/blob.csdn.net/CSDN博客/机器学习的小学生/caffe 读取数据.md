
# caffe 读取数据 - 机器学习的小学生 - CSDN博客


2017年12月26日 09:47:50[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：993



## (1) HDF5数据
**优点：**
通常为了利用多标签数据（大部分网上介绍的），将数据准备为HDF5格式。
使用HDF5数据，可以很方便的传入任何数据到caffe，例如仅需在train.prototxt中用如下的形式：
```python
layer {
   name:
```
```python
"data"
```
```python
type:
```
```python
"HDF5Data"
```
```python
top:
```
```python
"Features"
```
```python
# normalzied images
```
```python
top:
```
```python
"Headposes"
```
```python
# label1
```
```python
top:
```
```python
"Genders"
```
```python
# label2
```
```python
top:
```
```python
"Ages"
```
```python
# label3
```
```python
top:
```
```python
"Landmarks"
```
```python
# label4
```
```python
hdf5_data_param {
     source:
```
```python
"../hdf5_file/train_list.txt"
```
```python
# do not give the h5 files directly, but the list.
```
```python
batch_size:
```
```python
64
```
```python
}
   include { phase:TRAIN }
 }
```
**缺点：**
然而存储为HDF5数据通常采用single单精度，或者double双精度类型，使得数据量 动不动就几个G或者几十个G，然而，同一图像数据，准备不同尺度，都需要再度生成HDF5数据格式。
## (2) 利用python层
然而另外一个简洁的方式是，定义一个读取数据的python层，用来处理数据，有时间的话，在回来补充完整。
为了方便，贴出文献[1]中的读数据代码：
```python
#coding=gbk
```
```python
import caffe
import numpy as np
import scipy.io as io
from os.path import join, isfile
```
```python
class
```
```python
LDLDataLayer
```
```python
(
```
```python
caffe
```
```python
.
```
```python
Layer
```
```python
):
```
```python
def
```
```python
setup(
```
```python
self
```
```python
, bottom, top)
```
```python
:
```
```python
self
```
```python
.top_names = [
```
```python
'data'
```
```python
,
```
```python
'label'
```
```python
]
        params = eval(
```
```python
self
```
```python
.param_str)
```
```python
self
```
```python
.db_name = params[
```
```python
'db_name'
```
```python
]
```
```python
self
```
```python
.batch_size = params[
```
```python
'batch_size'
```
```python
]
```
```python
self
```
```python
.split_idx = params[
```
```python
'split_idx'
```
```python
]
```
```python
self
```
```python
.phase = params[
```
```python
'phase'
```
```python
]
```
```python
if
```
```python
params.has_key(
```
```python
'sub_mean'
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
.sub_mean = params[
```
```python
'sub_mean'
```
```python
]
```
```python
else:
```
```python
self
```
```python
.sub_mean =
```
```python
False
```
```python
assert(
```
```python
self
```
```python
.split_idx <=
```
```python
9
```
```python
)
```
```python
if
```
```python
isfile(join(
```
```python
'data/ldl/DataSets/'
```
```python
,
```
```python
self
```
```python
.db_name+
```
```python
'-shuffled.mat'
```
```python
))
```
```python
:
```
```python
# 如果存储随机大量索引的则加载
```
```python
mat = io.loadmat(join(
```
```python
'data/ldl/DataSets/'
```
```python
,
```
```python
self
```
```python
.db_name+
```
```python
'-shuffled.mat'
```
```python
))
```
```python
else:
```
```python
mat = io.loadmat(join(
```
```python
'data/ldl/DataSets/'
```
```python
,
```
```python
self
```
```python
.db_name+
```
```python
'.mat'
```
```python
))
            data = mat[
```
```python
'features'
```
```python
]
            label = mat[
```
```python
'labels'
```
```python
]
            shuffle_idx = np.random.choice(label.shape[
```
```python
0
```
```python
], label.shape[
```
```python
0
```
```python
])
```
```python
# 随机打乱label
```
```python
data = data[shuffle_idx,
```
```python
:
```
```python
]
            label = label[shuffle_idx,
```
```python
:
```
```python
]
            mat = dict({
```
```python
'features'
```
```python
:data
```
```python
,
```
```python
'labels'
```
```python
:label
```
```python
})
            io.savemat(join(
```
```python
'data/ldl/DataSets/'
```
```python
,
```
```python
self
```
```python
.db_name+
```
```python
'-shuffled.mat'
```
```python
), mat)
```
```python
self
```
```python
.features = mat[
```
```python
'features'
```
```python
]
```
```python
self
```
```python
.labels = mat[
```
```python
'labels'
```
```python
]
```
```python
self
```
```python
.
```
```python
N
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
D1
```
```python
=
```
```python
self
```
```python
.features.shape
```
```python
_
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
D2
```
```python
=
```
```python
self
```
```python
.labels.shape
```
```python
self
```
```python
.
```
```python
N
```
```python
= int(np.floor(
```
```python
self
```
```python
.labels.shape[
```
```python
0
```
```python
]/
```
```python
10
```
```python
)*
```
```python
10
```
```python
)
```
```python
#
```
```python
# discard extra samples # 为了完成 10 fold ，抛弃了额外的样本。
```
```python
self
```
```python
.features =
```
```python
self
```
```python
.features[
```
```python
0
```
```python
:self
```
```python
.
```
```python
N
```
```python
,
```
```python
:
```
```python
]
```
```python
self
```
```python
.labels =
```
```python
self
```
```python
.labels[
```
```python
0
```
```python
:self
```
```python
.
```
```python
N
```
```python
,
```
```python
:
```
```python
]
```
```python
Ntest
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
N
```
```python
/
```
```python
10
```
```python
self
```
```python
.
```
```python
Ntrain
```
```python
= int(
```
```python
self
```
```python
.
```
```python
N
```
```python
-
```
```python
Ntest
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
.phase==
```
```python
'test'
```
```python
:
```
```python
assert(
```
```python
self
```
```python
.batch_size ==
```
```python
Ntest
```
```python
)
        train_test_filter = np.array([
```
```python
False
```
```python
] *
```
```python
self
```
```python
.
```
```python
N
```
```python
)
        train_test_filter[
```
```python
self
```
```python
.split_idx*
```
```python
Ntest
```
```python
:
```
```python
(
```
```python
self
```
```python
.split_idx+
```
```python
1
```
```python
)*
```
```python
Ntest
```
```python
] =
```
```python
True
```
```python
self
```
```python
.test_data =
```
```python
self
```
```python
.features[train_test_filter,
```
```python
:
```
```python
]
```
```python
self
```
```python
.test_label =
```
```python
self
```
```python
.labels[train_test_filter,
```
```python
:
```
```python
]
```
```python
self
```
```python
.train_data =
```
```python
self
```
```python
.features[np.logical_not(train_test_filter),
```
```python
:
```
```python
]
```
```python
self
```
```python
.train_label =
```
```python
self
```
```python
.labels[np.logical_not(train_test_filter),
```
```python
:
```
```python
]
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
sub_mean:
```
```python
print
```
```python
"Subtract mean ... "
```
```python
data_mean = np.mean(
```
```python
self
```
```python
.train_data,
```
```python
0
```
```python
)
```
```python
self
```
```python
.train_data =
```
```python
self
```
```python
.train_data - np.tile(data_mean, [
```
```python
self
```
```python
.train_data.shape[
```
```python
0
```
```python
],
```
```python
1
```
```python
])
```
```python
self
```
```python
.test_data =
```
```python
self
```
```python
.test_data - np.tile(data_mean, [
```
```python
self
```
```python
.test_data.shape[
```
```python
0
```
```python
],
```
```python
1
```
```python
])
        top[
```
```python
0
```
```python
].reshape(
```
```python
self
```
```python
.batch_size,
```
```python
self
```
```python
.
```
```python
D1
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
1
```
```python
)
        top[
```
```python
1
```
```python
].reshape(
```
```python
self
```
```python
.batch_size,
```
```python
self
```
```python
.
```
```python
D2
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
1
```
```python
)
```
```python
def
```
```python
forward(
```
```python
self
```
```python
, bottom, top)
```
```python
:
```
```python
if
```
```python
self
```
```python
.phase ==
```
```python
'train'
```
```python
:
```
```python
rnd_select = np.random.choice(
```
```python
self
```
```python
.
```
```python
Ntrain
```
```python
,
```
```python
self
```
```python
.batch_size)
            top[
```
```python
0
```
```python
].data[
```
```python
:
```
```python
,
```
```python
:
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
0
```
```python
] =
```
```python
self
```
```python
.train_data[rnd_select,
```
```python
:
```
```python
]
            top[
```
```python
1
```
```python
].data[
```
```python
:
```
```python
,
```
```python
:
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
0
```
```python
] =
```
```python
self
```
```python
.train_label[rnd_select,
```
```python
:
```
```python
]
        elif
```
```python
self
```
```python
.phase ==
```
```python
'test'
```
```python
:
```
```python
top[
```
```python
0
```
```python
].data[
```
```python
:
```
```python
,
```
```python
:
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
0
```
```python
] =
```
```python
self
```
```python
.test_data
            top[
```
```python
1
```
```python
].data[
```
```python
:
```
```python
,
```
```python
:
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
0
```
```python
] =
```
```python
self
```
```python
.test_label
```
```python
def
```
```python
reshape(
```
```python
self
```
```python
, bottom, top)
```
```python
:
```
```python
pass
```
```python
def
```
```python
backward(
```
```python
self
```
```python
, top, propagate_down, bottom)
```
```python
:
```
```python
pass
```
其优点是显而易见的，例如如果你想实现triplet loss, 很容易通过python层实现。
参考文献：
1.[https://github.com/zeakey/LDLForests](https://github.com/zeakey/LDLForests)[LDLForests-master],

