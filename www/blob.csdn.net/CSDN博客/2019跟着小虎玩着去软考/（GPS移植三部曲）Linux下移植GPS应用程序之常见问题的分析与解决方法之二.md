
# （GPS移植三部曲）Linux下移植GPS应用程序之常见问题的分析与解决方法之二 - 2019跟着小虎玩着去软考 - CSDN博客

2012年05月22日 00:20:22[littletigerat](https://me.csdn.net/littletigerat)阅读数：5164



**Linux下移植GPS应用程序之常见问题的分析与解决方法之一**
# OS：
# Linux
# 一
# .
# 直接提供函数实现给第三方
原因：主芯片设计以及gps芯片的更改，导致gps驱动程序时常处于更新状态
**演化为：**给第三方提供.so动态链接库以及.h头文件
# 二
# .gps
# 参数写死到程序中
gps参数有：Serial串口设备的路径、GPIO设备的端口号、波特率、GPIO端口号、复位Reset的GPIO端口号、电源管理工具的路径、快速启动GPS所保存的文件路径（冷启动和热启动之分）；
开发板有：Lily、Bali以及Apollo1.0，Apollo1.1；
**演化为：**提供gps参数的配置文件，如：atgps.cnf以及navione.cnf两个配置文件

# 三
# .
# 直接将驱动开发的软件接口提供给第三方
原因：
1.驱动开发的软件接口，会经常发生变化，接口不稳定；
2.驱动开发的软件接口，还不是第三方真正需要的接口，还不能完全满足用户需求；
3.要基于驱动开发的软件接口，来进一步扩展和完善，使其满足第三方的需要；
**演化为：**基于驱动开发人员提供的接口，来进行封装，以满足实现客户的需要。
# 四
# .
# 第三方没有调用
# at_close_gps
# 函数，待机时间明显缩短
造成的后果：电流偏大，电流降不下来，待机时机偏短；
**解决办法：**确保第三方，调用at_close_gps函数，关闭gps设备。
# 五
# .
# 程序与数据没有分离，造成一个目录经常放不下所有程序和数据
数据：地图数据，声音数据
Linux分区的目录经常才400-500M，而GPS整个存储空间需要900M
**演化为：程序与数据分离**

# 六
# .
# 当前目录与运行程序的路径相混淆
当前目录经常会发生更改，当前目录不一定是运行程序的所在路径。
所以，一定要注意区分清楚。
# 七
# .
# 使用
# C++
# 类的形式，封装数据，隐藏信息
1．数据，信息是与具体的操作相关联的；
2．给第三方提供操作行为；
3．信息，属性隐藏；

# 八
# .
# 一个开发板不同版本Apollo1.0与Apollo1.1，需要2套gps代码
开发板：Apollo开发板就有Apollo1.0与Apollo1.1至少两个硬件版本之分，毕竟硬件还是有点差别的；
**解决办法：**一套代码可以适合多个不同的开发板，一个开发板可以拥有多个不同的版本。
# 九
# .
# 针对给第三方的接口，编写验证其接口正确性，其工具是：
# gpstool


