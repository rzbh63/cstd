
# 2013年SQLite3学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客

2013年03月18日 00:44:04[littletigerat](https://me.csdn.net/littletigerat)阅读数：2108


**2013****年****SQLite3****学习笔记之一**
# 一、
# 前言
## 1.源代码下载地址
**http://www.sqlite.org/**

## 2.最新版本
**截止****2013****年****3****月****18****日星期一**
**3.7.15.2**
**下载如下压缩文件可以获得****windows****的动态链接库****sqlite3.dll**
**sqlite-dll-win32-x86-3071502.zip**
![](https://img-my.csdn.net/uploads/201303/18/1363538898_4750.JPG)

# 二、
# 如何通过
# sqlite3.dll
# 与
# sqlite3.def
# 生成
# sqlite3.lib
# 文件
## 第一步
将LIB.EXE、LINK.EXE以及MSPDB60.exe3个文件
拷贝到和sqlite3.def，sqlite3.dll一个目录下来，具体如下所示：
![](https://img-my.csdn.net/uploads/201303/18/1363538833_1520.JPG)
LIB.EXE、LINK.EXE以及MSPDB60.exe这三个文件来自C:\Program Files\Microsoft Visual Studio的安装目录里
## 第二步
运行cmd.exe命令行工具
进入sqlite3.dll目录下
![](https://img-my.csdn.net/uploads/201303/18/1363538815_6885.JPG)
## 第三步
执行命令
执行LIB /DEF:sqlite3.def /machine:IX86命令，就可以生成sqlit3.lib
![](https://img-my.csdn.net/uploads/201303/18/1363538751_6145.JPG)

