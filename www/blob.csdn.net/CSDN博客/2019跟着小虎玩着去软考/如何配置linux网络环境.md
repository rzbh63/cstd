
# 如何配置linux网络环境 - 2019跟着小虎玩着去软考 - CSDN博客

2011年08月03日 17:32:33[littletigerat](https://me.csdn.net/littletigerat)阅读数：2275


**如何配置linux网络环境**
# 第一步：查看网络是否可以联通
**ping www.g.cn**
# 第二步：配置服务器信息
**vim /etc/resolv.conf**
**添加如下信息：**
**nameserver 211.166.66.88**
# 第三步：查看网络IP信息
**ifconfig**
# 第四步：配置网卡IP
**ifconfig eth0 192.168.13.21/15**
# 第五步：添加缺省网关
**route add default gw 192.168.0.1**
# 第六步：验证
**查看网络是否可以联通**
**ping  www.g.cn**

