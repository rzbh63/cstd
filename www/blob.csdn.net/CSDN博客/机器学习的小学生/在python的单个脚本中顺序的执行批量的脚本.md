
# 在python的单个脚本中顺序的执行批量的脚本 - 机器学习的小学生 - CSDN博客


2018年10月10日 13:53:15[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：369



```python
#coding=gbk
```
```python
import
```
```python
sys
```
```python
import
```
```python
train_task1
sys
```
```python
.
```
```python
modules
```
```python
[
```
```python
'train_task1'
```
```python
]
```
```python
.
```
```python
__dict__
```
```python
.
```
```python
clear
```
```python
(
```
```python
)
```
```python
import
```
```python
train_task2
sys
```
```python
.
```
```python
modules
```
```python
[
```
```python
'train_task2'
```
```python
]
```
```python
.
```
```python
__dict__
```
```python
.
```
```python
clear
```
```python
(
```
```python
)
```
```python
import
```
```python
train_task3
sys
```
```python
.
```
```python
modules
```
```python
[
```
```python
'train_task3'
```
```python
]
```
```python
.
```
```python
__dict__
```
```python
.
```
```python
clear
```
```python
(
```
```python
)
```
如果不利用sys.modules[‘模块名字’].**dict**.clear()释放内存的化，往往会提示内存不足，不能进行下一个脚本/模块的运行。

