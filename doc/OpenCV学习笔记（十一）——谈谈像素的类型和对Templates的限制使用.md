# OpenCV学习笔记（十一）——谈谈像素的类型和对Templates的限制使用

 

Templates是c++的一个很强大的特征，可以是数据结构更加安全高效。但也会增加编译时间和代码的长度，当函数被频繁调用的时候便步那么高效，所以在目前的OpenCV版本不推荐过多的使用templates。矩阵元素可以是如下类型中的一种：

• 8-bit unsigned integer (uchar)
 • 8-bit signed integer (schar)
 • 16-bit unsigned integer (ushort)
 • 16-bit signed integer (short)
 • 32-bit signed integer (int)
 • 32-bit ﬂoating-point number (ﬂoat)
 • 64-bit ﬂoating-point number (double)

对于这些数据类型又定义了如下的枚举变量：



```cpp
CV_32FC1 == CV_32F;
CV_32FC2 == CV_32FC(2) == CV_MAKETYPE(CV_32F, 2);
CV_MAKETYPE(depth, n) == ((x&7)<<3) + (n-1);
```