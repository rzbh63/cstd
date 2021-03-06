
# 游戏制作之路（24）Camera（摄像机）的清除标志Skybox - 大坡3D软件开发 - CSDN博客

2018年10月15日 10:02:44[caimouse](https://me.csdn.net/caimouse)阅读数：232


在前面学习了摄像机的属性：Clear Flags，知道它有四个选项，但是什么时候使用哪一个选项呢？这是一个必须学习会的知识点，否则你是做不出来游戏的，就像数学里充分必要条件一样。由于摄像机的重要性，就因为游戏显示的一切都是从它显示出来的。在这里先来学习第一个选项：Skybox（天空盒）。
什么是天空盒呢？这个天空盒有什么作用呢？我们可以先从现实的生活来观察一下，比如你们来到深圳的莲花山顶，可以看到市民中心，可以看到比市民中心更远一点位于香港的高山，再远一点，就是蓝色的天空。当你环绕山顶一圈，总会看到天空，四周的天空总是围成一个以自己为中心的半球天空。由于整个地球是圆形的，并且由于万有引力的作用，导致整个大气层是半球形状，因此观察到整个天空也是半球的形状。这个就是现实观察到的真实世界，那么我们要在游戏里实现这个场景，当然是照搬现实世界了。好吧，在游戏里实现一个天空体，叫做Skydome，就是一个内半球。在这个内半球里贴上天空和云的纹理图片，就像现实世界里看到的天空一样了，有太阳，也有月亮，更可以有星星。当角色在这个天空体里行走时，同时移动这个天空体，让角色永远在这个圆心点上，这样无边无际的天空视野就实现了。
上面使用天空半球来实现，当然是最真实的情况，但在计算量上还是复杂了一些，有没有更简单的方法呢？其实还是有的。比如我们来观察一下室内情况，就会发现每个房子都是四四方方的，如果采用房子的结构来代替半球的情况，这样就更简单了。因为一个房子，就只有6个面，每个面使用两个三角形，才12个三角形就解决了。比一个半球显然少了不少的计算量。这样的天空体设计，就叫做天空盒，它的主要作用是模拟天空、室内场景。
如下6个图片，就可以显示一个天空盒：
![](https://img-blog.csdn.net/20181015094054372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从这个纹理图片上，就可以看到天空体的6个面，每个面设计是不一样的，四周是地球的边缘地貌，头顶上是天空云，地面是山地。
在unity里摄像机包含有显示对象的颜色和深度，这里的深度是指显示对象到摄像机的显示平面的距离。当摄像机要显示一帧画面时，它需要考虑几个因素，比如显示物体与显示平面的深度距离，还要考虑物体的颜色，再有上一次显示图片之间的关系。在这里的清除标志，其实就是与这三者都有关系，四个选项里选择那一个，就是选择了怎么样的深度、颜色和上一帧图片叠加的效果。**第一个选项skybox，意味着摄像机要显示一帧新画面时，把上一帧的画面全部清空，让它显示以天空体为背景，再在上面按从远到近来显示摄像机范围可视的物体，如果没有游戏物体部分，就会显示天空体的图片**。
最后来看一下，透视投影的摄像机，会加深理解它：
![](https://img-blog.csdn.net/20181015100118712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到这里，就学会了第一个选项的使用。
五子棋游戏开发
[http://edu.csdn.net/course/detail/5487](http://edu.csdn.net/course/detail/5487)
RPG游戏从入门到精通
[http://edu.csdn.net/course/detail/5246](http://edu.csdn.net/course/detail/5246)
WiX安装工具的使用
[http://edu.csdn.net/course/detail/5207](http://edu.csdn.net/course/detail/5207)


