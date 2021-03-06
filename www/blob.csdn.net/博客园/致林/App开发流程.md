
# App开发流程 - 致林 - 博客园






# [App开发流程](https://www.cnblogs.com/bincoding/p/5557076.html)

###### 搭建UI框架
搭建UI框架需要我们根据产品的导航模式来设计，市场上常用的导航模式有如下图几种：
![app导航](http://img.blog.csdn.net/20151128164757842)
我们的app如果不出意外一定是其中的一种导航模式，一般线框图出来我们就应该知道即将要开发的app长什么样子，开发者不必等视觉稿和素材出来才开始动工，我们先大致搭个架子，等视觉稿出来之后我们再做调整。
###### 选用开发库
一般我们app涉及到的库会有：
- UI框架（比如下拉刷新PullToRefresh、侧滑菜单Slidingmenu）
- 网络请求库（比如okhtttp、AndroidAsyncHttp、Volley）
- 数据操作库（比如GreenDao、Ormlite）
- 图片缓存框架（比如Universal-Imageloader）
- 数据解析库（比如Gson）
之所以要选用这些库，肯定是为了避免重复造轮子，在开发效率的角度来说，选用优秀的开源库能大大缩短开发周期和提高开发效率，但从个人提升角度来看的话，我们可能就成了一个只会用API的程序猿了，如果想提升的话，造轮子或者分析这些优秀的源代码是一个不错的途径。
###### 第三方服务集成
我们开发app的时候，肯定会遇到一些需求，比如推送的需求、自动升级、数据统计、社会化分享、用户反馈等等，然而对于一个刚起步的企业或者个人开发者的话，全都要自己去开发的话，那岂不是累死，像推送这种有一定的技术门槛，能做好都能成立一家公司了，所以选用一些第三方服务是一个可选之举。如果说你以后做大了，用第三方怕不好控制，那就自己做呗，有钱任性招兵买马就自己做，谁叫咱有钱呢。
---
前面这些东西开发一个app够了，开发出来能不能用还得有靠谱的测试，有没有crash，操作流不流畅，体验好不好才会有用户去用。这里不从产品的角度去评判一个app的好与坏，程序员要考虑的是从代码层面、性能层面去让我们的app变得更好。
##### 云测
我们开发完毕之后，需要给测试工程师进行基本的功能需求测试，他们传统的做法就是根据事先写好的测试用例来做回归测试，再把测试出来的bug反馈给工程师，工程师再去修bug，但这样实在是太不靠谱了，有时候我们太在意功能而忽略了一些更重要的东西，那就是体验，给用户最直接的感受就是你这个app够不够惊艳，够不够流畅，用户可能根本就不在乎你这个功能做的有多牛逼。所以我们更应该从非功能性方向去做测试，我们的目的是让用户用的爽，而不是加一些乱七八糟的功能。那怎么测非功能性的一些因素，这里就要提到『云测』这个东西，因为现在设备太多了，如果公司要买一堆设备来做测试，那得多少成本，况且设备更新得太快，你根本就跟不上，所以就有了云测这个东西，它是一个云测试平台服务，提供了一大批主流机型，我们就直接省去购买设备的成本，还能得到完善的测试报告。
再来说一下它的好处：
- 终端云，省去测试设备购买租赁成本
- 高效率 节省测试人员成本及时间
- 包含兼容性测试、性能测试、功能测试
- 操作简单、详细测试报告生成
这么多好处，你在缺少测试工程师的时候，不去尝试那实在说不过去。
##### 打包上线
前面的开发环节、测试环节都没问题之后，你离实现一个app的完整开发就不远了，正常的互联网公司，会把签名打包的apk给到运营，交给他们去写文案，上传到应用渠道，让渠道给我们去首发和推广。如果是个人开发者，这些工作就得我们自己做了。





