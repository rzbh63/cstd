
# Shell 命令 curl 和 wget 使用代理采集网页的总结大全 - 阳光岛主 - CSDN博客

2018年03月01日 20:58:27[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2976


Linux Shell 提供两个非常实用的命令来爬取网页，它们分别是 curl 和 wget
[米扑代理](https://proxy.mimvp.com)，作为大数据分析研究的基础服务，对其做了深入的研究和总结。
curl 和 wget 使用代理
curl 支持 http、https、socks4、socks5
wget 支持 http、https

**Shell curl wget 示例**
```python
#!/bin/bash
#
# curl 支持 http、https、socks4、socks5
# wget 支持 http、https
#
# 米扑代理示例：
# https://proxy.mimvp.com/demo2.php
#
# 米扑代理购买：
# https://proxy.mimvp.com
#
# mimvp.com
# 2015-11-09
#【米扑代理】：本示例，在CentOS、Ubuntu、MacOS等服务器上，均测试通过
#
# http代理格式 		http_proxy=http://IP:Port
# https代理格式 		https_proxy=http://IP:Port

## proxy no auth
# curl和wget，爬取http网页
{'http': 'http://120.77.176.179:8888'}
curl -m 30 --retry 3 -x http://120.77.176.179:8888 http://proxy.mimvp.com/test_proxy2.php        			# http_proxy
wget -T 30 --tries 3 -e "http_proxy=http://120.77.176.179:8888" http://proxy.mimvp.com/test_proxy2.php  	# http_proxy
# curl和wget，爬取https网页（注意：添加参数，不经过SSL安全验证）
{'https': 'http://46.105.214.133:3128'}
curl -m 30 --retry 3 -x http://46.105.214.133:3128 -k https://proxy.mimvp.com/test_proxy2.php        							# https_proxy
wget -T 30 --tries 3 -e "https_proxy=http://46.105.214.133:3128" --no-check-certificate https://proxy.mimvp.com/test_proxy2.php	# https_proxy
    
# curl 支持socks
# 其中，socks4和socks5两种协议的代理，都可以同时爬取http和https网页
{'socks4': '101.255.17.145:1080'}
curl -m 30 --retry 3 --socks4 101.255.17.145:1080 http://proxy.mimvp.com/test_proxy2.php
curl -m 30 --retry 3 --socks4 101.255.17.145:1080 https://proxy.mimvp.com/test_proxy2.php
    
{'socks5': '82.164.233.227:45454'}
curl -m 30 --retry 3 --socks5 82.164.233.227:45454 http://proxy.mimvp.com/test_proxy2.php
curl -m 30 --retry 3 --socks5 82.164.233.227:45454 https://proxy.mimvp.com/test_proxy2.php
# wget 不支持socks

## proxy auth（代理需要用户名和密码验证）
# curl和wget，爬取http网页
curl -m 30 --retry 3 -x http://username:password@210.159.166.225:5718 http://proxy.mimvp.com/test_proxy2.php				# http
curl -m 30 --retry 3 -x http://username:password@210.159.166.225:5718 https://proxy.mimvp.com/test_proxy2.php				# https
curl -m 30 --retry 3 -U username:password -x http://210.159.166.225:5718 http://proxy.mimvp.com/test_proxy2.php				# http
curl -m 30 --retry 3 -U username:password -x http://210.159.166.225:5718 https://proxy.mimvp.com/test_proxy2.php			# https
curl -m 30 --retry 3 --proxy-user username:password -x http://210.159.166.225:5718 http://proxy.mimvp.com/test_proxy2.php	# http
curl -m 30 --retry 3 --proxy-user username:password -x http://210.159.166.225:5718 https://proxy.mimvp.com/test_proxy2.php	# https
wget -T 30 --tries 3 -e "http_proxy=http://username:password@2.19.16.5:5718" http://proxy.mimvp.com/test_proxy2.php
wget -T 30 --tries 3 -e "https_proxy=http://username:password@2.19.16.5:5718" https://proxy.mimvp.com/test_proxy2.php
wget -T 30 --tries 3 --proxy-user=username --proxy-password=password -e "http_proxy=http://2.19.16.5:5718" http://proxy.mimvp.com/test_proxy2.php
wget -T 30 --tries 3 --proxy-user=username --proxy-password=password -e "https_proxy=http://2.19.16.5:5718" https://proxy.mimvp.com/test_proxy2.php

# curl 支持socks
curl -m 30 --retry 3 -U username:password --socks5 21.59.126.22:57216 http://proxy.mimvp.com/test_proxy2.php				# http
curl -m 30 --retry 3 -U username:password --socks5 21.59.126.22:57216 https://proxy.mimvp.com/test_proxy2.php				# https
curl -m 30 --retry 3 --proxy-user username:password --socks5 21.59.126.22:57216 http://proxy.mimvp.com/test_proxy2.php		# http
curl -m 30 --retry 3 --proxy-user username:password --socks5 21.59.126.22:57216 https://proxy.mimvp.com/test_proxy2.php		# https
# wget 不支持socks
```


**wget 配置文件设置代理**

```python
vim ~/.wgetrc
http_proxy=http://120.77.176.179:8888:8080
https_proxy=http://12.7.17.17:8888:8080
use_proxy = on
wait = 30
# 配置文件设置后，立即生效，直接执行wget爬取命令即可
wget -T 30 --tries 3 http://proxy.mimvp.com/test_proxy2.php
wget -T 30 --tries 3 https://proxy.mimvp.com/test_proxy2.php
```

**Shell 设置临时局部代理**

```python
# proxy no auth
export http_proxy=http://120.77.176.179:8888:8080
export https_proxy=http://12.7.17.17:8888:8080
# proxy auth（代理需要用户名和密码验证）
export http_proxy=http://username:password@120.77.176.179:8888:8080
export https_proxy=http://username:password@12.7.17.17:8888:8080
# 直接爬取网页
curl -m 30 --retry 3 http://proxy.mimvp.com/test_proxy2.php			# http_proxy
curl -m 30 --retry 3 https://proxy.mimvp.com/test_proxy2.php		# https_proxy
wget -T 30 --tries 3 http://proxy.mimvp.com/test_proxy2.php			# http_proxy
wget -T 30 --tries 3 https://proxy.mimvp.com/test_proxy2.php		# https_proxy
# 取消设置
unset http_proxy
unset https_proxy
```

**Shell 设置系统全局代理**

```python
# 修改 /etc/profile，保存并重启服务器
sudo vim /etc/profile		# 所有人有效
或
sudo vim ~/.bashrc			# 所有人有效
或
vim ~/.bash_profile			# 个人有效
	
	
## 在文件末尾，添加如下内容
# proxy no auth
export http_proxy=http://120.77.176.179:8888:8080
export https_proxy=http://12.7.17.17:8888:8080
# proxy auth（代理需要用户名和密码验证）
export http_proxy=http://username:password@120.77.176.179:8888:8080
export https_proxy=http://username:password@12.7.17.17:8888:8080

## 执行source命令，使配置文件生效（临时生效）
source /etc/profile
或
source ~/.bashrc
或
source ~/.bash_profile

## 若需要机器永久生效，则需要重启服务器
sudo reboot
```

米扑代理示例
米扑代理，专注为企业提供国内大数据研究服务，技术团队来自百度、小米、阿里、创新工场等，为国内企业提供大数据采集、数据建模分析、结果导出展示等服务。
米扑代理示例，包含Python、Java、PHP、C\#、Go、Perl、Ruby、Shell、NodeJS、PhantomJS、Groovy、Delphi、易语言等十多种编程语言或脚本，通过大量的可运行实例，详细讲解了使用代理IP的正确方法，方便网页爬取、数据采集、自动化测试等领域。
![shell-curl-he-wget-shi-yong-dai-li-ip-01](http://blog.mimvp.com/wp-content/uploads/2017/08/shell-curl-he-wget-shi-yong-dai-li-ip-01.png)

米扑代理示例官网：
[https://proxy.mimvp.com/demo2.php](https://proxy.mimvp.com/demo2.php)



