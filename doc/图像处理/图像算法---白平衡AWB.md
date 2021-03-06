# 图像算法---白平衡AWB


 本文转载wzwxiaozheng的白平衡算法，主要包括两部分：色温曲线和色温计算。原文http://blog.csdn.net/wzwxiaozheng/article/details/38434391

1，白平衡算法---色温曲线

本文大体讲解了白平衡的算法流程,适用于想了解和学习白平衡原理的筒子们.

一般情况下要实现AWB算法需要专业的图像和算法基础,本文力图通过多图的方式,深入浅出,降低初学者理解上的门槛,让大家都理解到白平衡算法流程.

看到这里还在继续往下瞄的同学,一定知道了色温的概念,并且知道sensor原始图像中的白色如果不经AWB处理,在高色温(如阴天)下偏蓝,低色温下偏黄,如宾馆里的床头灯(WHY!OTZ) (如下图).

 ![img](https://img-blog.csdn.net/20140808092904203)



 

下面这个T恤的图片非常经典,怎么个经典后续再说,不过大体可以看出有偏黄和偏蓝的情况.虽然如此,却已经是AWB矫正以后的效果.



 ![img](https://img-blog.csdn.net/20140808093125875)

所以,为了眼前的女神白富美在镜头里不变成阿凡达和黄脸婆,这时就需要白平衡来工作了.

 

流程原理很简单:

1,在各个色温下(2500~7500)拍几张白纸照片,假设拍6张(2500,3500…7500),可以称作色温照.

2,把色温照进行矫正,具体是对R/G/B通道进行轿正,让偏色的白纸照变成白色,并记录各个通道的矫正参数.

   实际上只矫正R和B通道就可以,这样就得到了6组矫正参数(Rgain,Bgain).

3,上面是做前期工作,爱思考的小明发现,只要知道当前场景是什么色温,再轿正一下就可以了.事实上也就是如此.

 

所以,AWB算法的核心就是判断图像的色温,是在白天,晚上,室内,室外,是烈日还是夕阳,还是在阳光下的沙滩上.或者是在卧室里”暖味”的床头灯下.

 

之前拍了6张色温照以及6组矫正参数.可是6够么,当然不够, 插值一下可以得到无数个值,我们把点连成线, 得到了一个神奇的曲线------色温曲线.大概是下面这个样子.



 ![img](https://img-blog.csdn.net/20140808093139634)

上面提到了三个值(RG,BG,色温),这应该是个三维的.没关系,我们再来一条RG跟色温的曲线,这样只要知道色温,就知道RG,知道RG,就知道BG,知道RG,BG就能轿正了,yes!



 ![img](https://img-blog.csdn.net/20140808093207074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

至此,我们的前期工作已经全部做完了, 并得到了AWB的色温曲线,下一步只要计算得到当前色温,顺藤摸瓜就能得到当前的矫正参数(Rgain,Bgain),那白平衡的工作就作完了.(放心,当然没这么简单)

2，色温计算

本文主要讲解了白平衡算法中估算当前场景色温的流程.

色温计算的原理并不复杂,但是要做好,还是要细心做好每一步工作,这需要大量的测试,并对算法不断完善.

 

首先简单说一下流程:

1, 取一帧图像数据,并分成MxN块,假设是25x25,并统计每一块的基本信息(,白色像素的数量及R/G/B通道的分量的均值).

![img](https://img-blog.csdn.net/20141029145233467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2, 根据第1步中的统计值, 找出图像中所有的白色块,并根据色温曲线判断色温.

3, 至此,我们得出来了图像中所有的可能色温,如果是单一光源的话,可以取色温最多的,当作当前色温.

   比如25x25=625 个块中,一共找出了100个有效白色块, 里面又有80个白色块代表了色温4500左右, 那当前色温基本就是4500.

   根据4500色温得出的Rgain,Bgain来调整当前图像,就不会差(很多!).

![img](https://img-blog.csdn.net/20141029145413687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



下面我们再详细讲解一下,每一步中需要做的工作:

第1步, 计算每一块的基本信息.

  

   关于白色像素统计,大家知道sensor原始图像是偏色的,怎么统计块中的白色点呢,那只有设置一个颜色范围,只要在范围中,就可以认为是白色像素,范围见下图:

![img](https://img-blog.csdn.net/20141029145413526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



统计白色像素个数的用处是,1,如果块中的白色像素太少,可以抛弃掉. 2,如果白色像素太多,多到每一个像素点都是,那也要抛弃掉,因为很可能在该区域过曝了

   接着把统计到的白色像素点R/G/B取均值, 并得到该block 的R/G,  B/G值

   至此,我们得到了每一块的白点数目及R/G,B/G的值. (请自动对应第1部分中色温曲线).

 

第二步 计算当前色温

   这个比较复杂, 大自然绚丽多彩,景色万千. 上一步中统计的”白色点”难免会有失误的地方,比较常见的如黄色皮肤容易被误判为低色温下的白点,淡蓝色的窗帘,容易被误判为高色温下的白点,一张图中既有白色,也有黄色,也有蓝色的时候,是不是感觉情况有点复杂,其它的大家可继续脑补.

   这时我们需要一定的策略来正确的判断出到哪个才是真的白.

   通常我们会把取到的白色块,计算一下到曲线的距离,再设置相应的权重.话不多说,上个图大家就都明白了.

![img](https://img-blog.csdn.net/20141029145553156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



假设有上面这样一幅图,该图是在没有开AWB的前提下截取的,可以看到左边白色地方略有偏绿,当前色温是室内白炽灯,大概4000~5000k左右.(请忽略颜色不正的问题,我们在讨论白平衡)

下面我们就根据之前的统计信息和测量好的色温曲线进行白平衡矫正.

首先要找出白区,如下图:

![img](https://img-blog.csdn.net/20141029145545192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



上面这个图中的数字标示出了检测到的白色区域,数字相同的表示一个白区,根据统计信息(白点数,rg/bg值)来区分的.可以看到有误判的地方,比如色卡左上第二块的肤色块.还有最右边从上面数第二块也是容易被判断成低色温白块的情况.

针对这种误判的情况,对不同块根据统计信息进行权重设置,以求让误判的区域对最终结果影响小一些.

![img](https://img-blog.csdn.net/20141029145642600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



上面这个图标注了权重,基本是根据统计信息中白点数来确定的.可以看到图中一片白色被标识了高权重.其它情况被标识了低权重. 权重高低一是看块中白色点数量,二是看rg/bg到色温曲线的距离.

   通过上面两个图,大家就可以明显的找到白色区,并根据曲线来矫正,即使不通过曲线矫正,把白色区的r/g,b/g值向1趋近,让r=g=b,也会得到非常好的白平衡效果.如下图所示:

![img](https://img-blog.csdn.net/20141029145720071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



至此,白平衡的基本流程就讲完了,有图有真相,大家一定看着也方便.

总结一下:第一次做白平衡,感觉理论很简单,不用什么基础也能看懂.实际算法调试时,可谓差之毫厘,失之千里.总是感觉不由自主就走上歪路.中间参考了大量资料,比如网上有许多基于色温/灰度世界/白点检测的白平衡算法,实际个人感觉应该把它们都结合起来,让算法强壮,健康才是我们想要的.

还记得第一章中开始的那两张白色T恤的图么,算了,我再贴一下:

![img](https://img-blog.csdn.net/20141029145851191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3p3eGlhb3poZW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这张图可以理解为在多光源下的白平衡调整.阴影色温比阳光下色温要高一些,如果阳光下是5000k,阴影可能是7000k.有光就有影,它们经常出现在一个镜头里,对着其中一个色温调,另一边就会偏色.为了整体效果好,要把翘翘板平衡起来,可以加一些策略在里面。