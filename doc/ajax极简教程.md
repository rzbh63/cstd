# ajax极简教程

**一、什么是ajax**

ajax即异步JavaScript和XML，它是一种用于创建快速动态网页的技术。作用是通过在后台与服务器进行少量数据交换，使网页实现异步更新。这意味着可以在不重新加载整个网页的情况下，对网页的某部分进行更新。

 

**二、实现ajax基本步骤**

- 创建XMLHttpRequest对象,也就是创建一个异步调用对象.
- 创建一个新的HTTP请求,并指定该HTTP请求的方法、URL及验证信息.
- 设置响应HTTP请求状态变化的函数.
- 发送HTTP请求.
- 获取异步调用返回的数据.
- 使用JavaScript和DOM实现局部刷新.

 

**三、ajax实现方式**

这里我们不做原生ajax的介绍，因为JQuery对ajax进行了封装。可以用少量的代码实现原生ajax的功能，同时又帮助我们解决了浏览器的兼容问题，没有道理不选它。

 

$.ajax是对ajax最基础的封装，来看一个简单例子

```javascript
$.ajax({  
    type : "post",  //提交方式  
    url : "${pageContext.request.contextPath}/del.action",//接口url
       data : {  
           "name" : "${name}"  //交互数据
       },
       success : function(result) {//返回数据根据结果进行相应的处理  
           if ( result.success ) {  
                 alert("删除成功");
           } else {  
                 alert("删除失败");
           }  
      }  
});
```

 

关键参数介绍：

- method   //数据的提交方式
- url   //数据的提交url
- async   //是否支持异步刷新，默认是true
- data    //需要提交的数据
- dataType   //服务器返回数据的类型，例如xml,String,Json等
- success    //请求成功后的回调函数
- error   //请求失败后的回调函数

 

**四、常用请求类型介绍**

- multipart/form-data类型主要是上传文件时用到；
- application/x-www-form-urlencoded类型主要是提交k-v时用到，当然这种方法也可以将json设置在v中提交json数据；
- application/json类型主要是传递json数据用到，层次比较深的数据；

 

**五、常见响应状态码介绍**

- 100：客户必须继续发出请求
- 101：客户要求服务器根据请求转换HTTP协议版本
- 200：请求成功
- 201：提示知道新文件的URL
- 202：接受和处理、但处理未完成
- 203：返回信息不确定或不完整
- 204：请求收到，但返回信息为空
- 205：服务器完成了请求，用户代理必须复位当前已经浏览过的文件
- 206：服务器已经完成了部分用户的GET请求
- 300：请求的资源可在多处得到
- 301：删除请求数据
- 302：在其他地址发现了请求数据
- 303：建议客户访问其他URL或访问方式
- 304：客户端已经执行了GET，但文件未变化
- 305：请求的资源必须从服务器指定的地址得到
- 306：前一版本HTTP中使用的代码，现行版本中不再使用
- 307：申明请求的资源临时性删除
- 400：错误请求，如语法错误
- 401：请求授权失败
- 402：保留有效ChargeTo头响应
- 403：请求不允许
- 404：没有发现文件、查询或URl
- 405：用户在Request-Line字段定义的方法不允许
- 406：根据用户发送的Accept拖，请求资源不可访问
- 407：类似401，用户必须首先在代理服务器上得到授权
- 408：客户端没有在用户指定的时间内完成请求
- 409：对当前资源状态，请求不能完成
- 410：服务器上不再有此资源且无进一步的参考地址
- 411：服务器拒绝用户定义的Content-Length属性请求
- 412：一个或多个请求头字段在当前请求中错误
- 413：请求的资源大于服务器允许的大小
- 414：请求的资源URL长于服务器允许的长度
- 415：请求资源不支持请求项目格式
- 416：请求中包含Range请求头字段，在当前请求资源范围内没有range指示值，请求也不包含If-Range请求头字段
- 417：服务器不满足请求Expect头字段指定的期望值，如果是代理服务器，可能是下一级服务器不能满足请求
- 500：服务器产生内部错误
- 501：服务器不支持请求的函数
- 502：服务器暂时不可用，有时是为了防止发生系统过载
- 503：服务器过载或暂停维修
- 504：关口过载，服务器使用另一个关口或服务来响应用户，等待时间设定值较长
- 505：服务器不支持或拒绝支请求头中指定的HTTP版本

对一些状态码要明白它的含义，可以帮助我们快速定位程序哪里可能出问题了，或者对不同的结果做详细的失败提示。

 

**六、ajax的优点**

- 减轻服务器负担，按需要获得数据。
- 无刷新更新页面，减少用户的实际和心理的等待时间。
- 更好的用户体验。
- 减轻宽带的负担。

 

**七、ajax的缺点**

- ajax对浏览器的兼容性。
- ajax局部刷新，所以浏览器的回退功能失效。
- 对搜索引擎支持不友好。