
# python 和 matlab - 机器学习的小学生 - CSDN博客


2017年05月19日 15:32:30[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：609


|Python|Matlab|功能|
|---|---|---|
|str2 = str1.replace(old,new[,count])|dstStr= strrep(origStr,oldSubstr,newSubStr)
|filepath = 'E:/data/1.jpg';
|filepath2 = strrep(filepath,'data','lfw/data');
|disp(filepath2);|替换子串|
随机打乱:
**Matlabs:**
```python
nsample =
```
```python
10
```
```python
;
```
```python
if
```
```python
~exist(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'file'
```
```python
)
    rand_ind = randperm(nsample);
    save(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'rand_ind'
```
```python
);
```
```python
else
```
```python
load(
```
```python
'rand_ind.mat'
```
```python
);
```
```python
% load rand_ind
```
```python
disp
```
```python
(
```
```python
'load successfully!'
```
```python
)
```
```python
end
```
```python
disp
```
```python
(rand_ind);
```
**Python:**
```python
import
```
```python
random
```
```python
import
```
```python
os
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
rand_ind = range(
```
```python
10
```
```python
)
```
```python
if
```
```python
not
```
```python
os.path.exists(
```
```python
'rand_ind.npy'
```
```python
):
    random.shuffle(rand_ind)
```
```python
# 操作该对象本身
```
```python
np.save(
```
```python
'rand_ind.npy'
```
```python
,rand_ind)
```
```python
else
```
```python
:
    rand_ind = np.load(
```
```python
'rand_ind.npy'
```
```python
)
```
```python
print
```
```python
'load successfully!'
```
```python
print
```
```python
rand_ind
```
在进行实验的时候，我们经常希望对数据集进行随机的打乱，但是又希望这种随机打乱的结果具有可重现性。下面给出matlab和python的实现方法：
**Matlab:**
```python
nsample =
```
```python
10
```
```python
;
feat_len =
```
```python
1000
```
```python
;
train_data =
```
```python
rand
```
```python
(nsample,feat_len);
```
```python
%随机生成一个样本矩阵,含有10个样本，特征维数为1000
```
```python
if
```
```python
~exist(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'file'
```
```python
)
    rand_ind = randperm(nsample);
    save(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'rand_ind'
```
```python
);
```
```python
else
```
```python
load(
```
```python
'rand_ind.mat'
```
```python
);
```
```python
% load rand_ind
```
```python
disp
```
```python
(
```
```python
'load successfully!'
```
```python
)
```
```python
end
```
```python
% 将样本按照生成的随机序列打乱
```
```python
train_data = train_data(rand_ind,:);
```
```python
disp
```
```python
(rand_ind);
```
**Python:**
```python
import
```
```python
random
```
```python
import
```
```python
os
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
nsample =
```
```python
10
```
```python
feat_len =
```
```python
1000
```
```python
train_data = np.random.random((nsample,feat_len))
```
```python
print
```
```python
train_data.shape
rand_ind = range(nsample)
```
```python
if
```
```python
not
```
```python
os.path.exists(
```
```python
'rand_ind.npy'
```
```python
):
    random.shuffle(rand_ind)
```
```python
# 操作该对象本身
```
```python
np.save(
```
```python
'rand_ind.npy'
```
```python
,rand_ind)
```
```python
else
```
```python
:
    rand_ind = np.load(
```
```python
'rand_ind.npy'
```
```python
)
```
```python
print
```
```python
'load successfully!'
```
```python
print
```
```python
rand_ind
train_data2 = np.zeros(train_data.shape,dtype=train_data.dtype)
```
```python
print
```
```python
train_data2.shape
```
```python
print
```
```python
rand_ind[
```
```python
1
```
```python
]
```
```python
for
```
```python
i
```
```python
in
```
```python
xrange(len(rand_ind)):
    train_data2[i,:] = train_data[rand_ind[i],:]
```
对于list的话，可以使用列表推导式，如；
```python
mylist = [
```
```python
'a'
```
```python
,
```
```python
'b'
```
```python
,
```
```python
'c'
```
```python
,
```
```python
'd'
```
```python
,
```
```python
'e'
```
```python
]
myorder = [
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
,
```
```python
4
```
```python
]
mylist =[mylist[i]
```
```python
for
```
```python
i
```
```python
in
```
```python
myorder]
```
```python
print
```
```python
mylist
```

