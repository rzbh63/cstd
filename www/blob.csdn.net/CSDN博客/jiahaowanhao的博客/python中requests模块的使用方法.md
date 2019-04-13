
# python中requests模块的使用方法 - jiahaowanhao的博客 - CSDN博客


2018年07月07日 22:15:39[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：83


[python中requests模块的使用方法](http://cda.pinggu.org/view/26026.html)
本文实例讲述了python中requests模块的使用方法。分享给大家供大家参考。具体分析如下：
在HTTP相关处理中使用python是不必要的麻烦，这包括urllib2模块以巨大的复杂性代价获取综合性的功能。相比于urllib2,Kenneth Reitz的Requests模块更能简约的支持完整的简单用例。
简单的例子：
想象下我们试图使用get方法从http://example.test/获取资源并且查看返回代码，content-type头信息，还有response的主体内容。这件事无论使用urllib2 或者Requests都是很容易实现的。
urllib2
>>> import urllib2
>>> url = 'http://example.test/'
>>> response = urllib2.urlopen(url)
>>> response.getcode()
200
>>> response.headers.getheader('content-type')
'text/html; charset=utf-8'
>>> response.read()
'Hello, world!'
Requests
>>> import requests
>>> url = 'http://example.test/'
>>> response = requests.get(url)
>>> response.status_code
200
>>> response.headers['content-type']
'text/html; charset=utf-8'
>>> response.content
u'Hello, world!'
这两种方法很相似，相对于urllib2调用方法读取response中的属性信息，Requests则是使用属性名来获取对应的属性值。
两者还有两个细微但是很重要的差别：
1 Requests 自动的把返回信息有Unicode解码
2 Requests 自动保存了返回内容，所以你可以读取多次，而不像urllib2.urlopen()那样返回的只是一个类似文件类型只能读取一次的对象。
第二点是在python交互式环境下操作代码很令人讨厌的事情
一个复杂一点的例子：
现在让我们尝试下复杂点得例子：使用GET方法获取http://foo.test/secret的资源，这次需要基本的http验证。使用上面的代码作为模板，好像我们只要把urllib2.urlopen() 到requests.get()之间的代码换成可以发送username，password的请求就行了
这是urllib2的方法：
>>> import urllib2
>>> url = 'http://example.test/secret'
>>> password_manager = urllib2.HTTPPasswordMgrWithDefaultRealm()
>>> password_manager.add_password(None, url, 'dan', 'h0tdish')
>>> auth_handler = urllib2.HTTPBasicAuthHandler(password_manager)
>>> opener = urllib2.build_opener(auth_handler)
>>> urllib2.install_opener(opener)
>>> response = urllib2.urlopen(url)
>>> response.getcode()
200
>>> response.read()
'Welcome to the secret page!'
一个简单的方法中实例化了2个类，然后组建了第三个类，最后还要装载到全局的urllib2模块中，最后才调用了urlopen，那么那两个复杂的类是什么的
迷惑了吗，  这里所有urllib2的文档 http://docs.python.org/release/2.7/library/urllib2.html
那Requests是怎么样解决同样的问题的呢？
Requests
>>> import requests
>>> url = 'http://example.test/secret'
>>> response = requests.get(url,auth=('dan','h0tdish'))
>>> response.status_code
200
>>> response.content
u'Welcome to the secret page!'
只是在调用方法的时候增加了一个auth关键字函数
我敢打赌你不用查文档也能记住。
错误处理 Error Handling
Requests对错误的处理也是很非常方面。如果你使用了不正确的用户名和密码，urllib2会引发一个urllib2.URLError错误，然而Requests会像你期望的那样返回一个正常的response对象。只需查看response.ok的布尔值便可以知道是否登陆成功。
>>> response = requests.get(url,auth=('dan','wrongPass'))
>>> response.ok
False
其他的一些特性：
* Requests对于HEAD, POST, PUT, PATCH, 和 DELETE方法的api同样简单
* 它可以处理多部分上传，同样支持自动转码
* 文档更好
* 还有更多
Requests 是很好的，下次需要使用HTTP时候可以试试。
希望本文所述对大家的Python程序设计有所帮助。

