
# Backbone.js学习之Model - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月15日 13:36:30[Jlins](https://me.csdn.net/dyllove98)阅读数：1050标签：[javascript																](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)个人分类：[javaScript																](https://blog.csdn.net/dyllove98/article/category/1158758)



首先，我们看一下官方文档中对Model的解释（或者说定义）：
> Models are the heart of any JavaScript application, containing the interactive data as well as a large part of the logic surrounding it: conversions, validations, computed properties, and access control. You extend Backbone.Model with your domain-specific methods, and Model provides a basic set of functionality for managing changes.
翻译成中文就是：
> Model是js应用的核心，包括交互数据以及围绕着这些数据的大部分逻辑：数据转换、验证、属性计算和访问控制。你可以使用特定方法来扩展Backbone.Model，并且Model本身就提供了一系列基本的管理功能。
然后，我们还在文档中看到Model拥有好多方法，这里就不一一介绍了。
## 简单的Model例子
定义了一个最基础的Model，只是实现了initialize这个初始化方法。这个函数会在Model被实例化时调用。
```python
var
```
```python
Man = Backbone.Model.extend({
        constructor:
```
```python
function
```
```python
()
```
```python
{
            alert(
```
```python
'Hey, you create me!'
```
```python
);
        }
         });
```
```python
var
```
```python
man =
```
```python
new
```
```python
Man;
```
初始化函数，也可以叫构造函数。比如你这里完全可以把constructor写成initialize，效果是一样的。至于为什么一样？这还得等到熟悉这个框架过后看看Backbone源码才能知道。
## Molde对象属性赋值的两种方法
一种是设置默认值
```python
var
```
```python
Man = Backbone.Model.extend({
        initialize:
```
```python
function
```
```python
()
```
```python
{
            alert(
```
```python
'Hey, you create me!'
```
```python
);
        },
        defaults: {
            name:
```
```python
'张三'
```
```python
,
            age:
```
```python
'38'
```
```python
}
    });
```
```python
var
```
```python
man =
```
```python
new
```
```python
Man;
```
```python
console
```
```python
.info(man.get(
```
```python
"name"
```
```python
));
```
或者这样赋值
```python
man.set({name:
```
```python
'the5fire'
```
```python
,age:
```
```python
'10'
```
```python
});
```
```python
console
```
```python
.info(man.get(
```
```python
"name"
```
```python
));
```
```python
console
```
```python
.info(man);
```
从这个对象的取值方式可以知道，属性在一个Model是以字典（或者类似字典）的方式存在的，第一种设定默认值的方式，只不过是实现了Backbone的defaults这个方法，或者是给defaults进行了赋值。通过打印出man这个对象，我们可以发现，属性是存在一个叫做attributes的对象中的。
## Model对象中的方法
```python
var
```
```python
Man = Backbone.Model.extend({
        initialize:
```
```python
function
```
```python
()
```
```python
{
            alert(
```
```python
'Hey, you create me!'
```
```python
);
        },
        defaults: {
            name:
```
```python
'张三'
```
```python
,
            age:
```
```python
'38'
```
```python
},
        aboutMe:
```
```python
function
```
```python
()
```
```python
{
```
```python
return
```
```python
'我叫'
```
```python
+
```
```python
this
```
```python
.get(
```
```python
'name'
```
```python
) +
```
```python
',今年'
```
```python
+
```
```python
this
```
```python
.get(
```
```python
'age'
```
```python
) +
```
```python
'岁'
```
```python
;
        }
    });
```
```python
var
```
```python
man =
```
```python
new
```
```python
Man;
    alert(man.aboutMe());
```
也是比较简单，只是增加了一个新的属性，值是一个function。说到这，不知道你是否发现，在所有的定义或者赋值操作中，都是通过字典的方式来完成的，比如extend Backbone的Model，以及定义方法，定义默认值。方法的调用和其他的语言一样，直接`.`即可，参数的定义和传递也一样。
## 监听Model对象中属性的变化
假设你有在对象的某个属性发生变化时去处理一些业务的话，下面的示例会有帮助。依然是定义那个类，不同的是我们在构造函数中绑定了name属性的change事件。这样当name发生变化时，就会触发这个function。
```python
var
```
```python
Man = Backbone.Model.extend({
        initialize:
```
```python
function
```
```python
()
```
```python
{
            alert(
```
```python
'Hey, you create me!'
```
```python
);
```
```python
//初始化时绑定监听
```
```python
this
```
```python
.bind(
```
```python
"change:name"
```
```python
,
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
name =
```
```python
this
```
```python
.get(
```
```python
"name"
```
```python
);
                alert(
```
```python
"你改变了name属性为："
```
```python
+ name);
            });
        },
        defaults: {
            name:
```
```python
'张三'
```
```python
,
            age:
```
```python
'38'
```
```python
}
    });
```
```python
var
```
```python
man =
```
```python
new
```
```python
Man;
```
```python
//触发绑定的change事件，alert。
```
```python
man.set({name:
```
```python
'the5fire'
```
```python
});
```
```python
//触发绑定的change事件，alert。
```
```python
man.set({name:
```
```python
'the5fire.com'
```
```python
});
```
## 为Model对象添加验证规则，以及错误提示
```python
var
```
```python
Man = Backbone.Model.extend({
        initialize:
```
```python
function
```
```python
()
```
```python
{
            alert(
```
```python
'Hey, you create me!'
```
```python
);
```
```python
//初始化时绑定监听, change事件会先于validate发生
```
```python
this
```
```python
.bind(
```
```python
"change:name"
```
```python
,
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
name =
```
```python
this
```
```python
.get(
```
```python
"name"
```
```python
);
                alert(
```
```python
"你改变了name属性为："
```
```python
+ name);
            });
```
```python
this
```
```python
.bind(
```
```python
"invalid"
```
```python
,
```
```python
function
```
```python
(model,error)
```
```python
{
                alert(error);
            });
        },
        defaults: {
            name:
```
```python
'张三'
```
```python
,
            age:
```
```python
'38'
```
```python
},
        validate:
```
```python
function
```
```python
(attributes)
```
```python
{
```
```python
if
```
```python
(attributes.name ==
```
```python
''
```
```python
) {
```
```python
return
```
```python
"name不能为空！"
```
```python
;
            }
        }
    });
```
```python
var
```
```python
man =
```
```python
new
```
```python
Man;
```
```python
// 这种方式添加错误处理也行
```
```python
// man.on('invalid', function(model, error){
```
```python
//         alert(error);
```
```python
// });
```
```python
//默认set时不进行验证
```
```python
man.set({name:
```
```python
''
```
```python
});
```
```python
//save时触发验证。根据验证规则，弹出错误提示。
```
```python
man.save();
```
```python
//手动触发验证, set时会触发
```
```python
man.set({name:
```
```python
'moyi'
```
```python
}, {
```
```python
'validate'
```
```python
:
```
```python
true
```
```python
});
```
```python
//验证通过时信息
```
```python
if
```
```python
(man.isValid()) {
  alert(man.get(
```
```python
"name"
```
```python
)+
```
```python
"验证通过"
```
```python
);
}
```

**其他精彩文章**
## [jQuery教程(19)-jquery ajax操作之序列化表单](http://www.itmmd.com/201501/497.html)
## [jQuery教程(18)-ajax操作之执行POST请求](http://www.itmmd.com/201501/496.html)
## [jQuery教程(17)-ajax操作之向服务器传递数据 执行GET请求](http://www.itmmd.com/201501/493.html)
## [jQuery教程(16)-ajax操作之加载XML文档](http://www.itmmd.com/201501/492.html)
## [jQuery教程(15)-ajax操作之执行脚本](http://www.itmmd.com/201501/491.html)

## [jQuery教程(14)-ajax操作之操作 JavaScript 对象](http://www.itmmd.com/201501/490.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


