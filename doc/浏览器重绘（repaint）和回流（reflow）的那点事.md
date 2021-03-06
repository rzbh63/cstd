## [浏览器重绘（repaint）和回流（reflow）的那点事](https://www.cnblogs.com/microcosm/p/6820502.html)

 

第一次听到重绘和回流是在鹅厂实习面试，那个时候对浏览器没有任何的概念，所以面试官说到这个问题的时候整个人都蒙圈了。下面是近期学习总结：

**重绘（repaint）和回流（reflow）**

  文档初次加载时，HTML解析器会解析元素构建DOM树，CSS解析器将样式解析为样式结构体，之后通过DOM树和样式结构体构建渲染树，渲染树具有样式属性。然后遍历渲染树的每个渲染器将器布局到页面，最后浏览器将器绘制到页面。

 **重绘**：当元素的外观或外观可见性（visibility）发生变化时会触发重绘

 **回流**：render树中的部分或全部因为元素的规模尺寸、布局、隐藏等改变，需要重新计算render树。

​    回流时，浏览器会使渲染树中受影响的部分失效，然后重新构造这部分的render树。完成回流之后浏览器会重新布局、绘制受影响的部分到屏幕中，该过程就是重绘。所以回流必定会引起重绘，但重绘不一定引起回流。

​    每个页面至少需要一次回流，就是页面第一次加载时。回流变化涉及到部分页面（或整个页面）的布局。一个元素的回流导致其所有子元素以及DOM中紧随其后的祖先元素和其子元素都发生回流。

**是什么导致回流呢？**

​    （1） 调整窗口大小----发生resize事件时

​    （2） 元素位置改变

​    （3） 元素尺寸改变——边距、填充、边框、高度和宽度

​    （4） 内容改变——比如文本字体或图片大小改变而引起的宽度和高度改变

​    （5） 页面渲染初始化

​    （6） 添加或删除可见的DOM元素

**聪明的浏览器**

​      很多浏览器都会优化repaint和reflow操作，浏览器会维护1个队列，把所有会引起回流、重绘的操作放入这个队列，等队列中的操作到了一定的数量或者到了一定的时间间隔，浏览器就会把flush队列，进行一个批处理。这样就会让多次的回流、重绘变成一次回流重绘。   虽然有了浏览器的优化，但有时候我们写的一些代码可能会强制浏览器提前flush队列，比如设置width，offsetTop，scrollTop,clientTop精确值时，这样浏览器的优化可能就起不到作用了。

**避免回流和重绘方法**

​     减少回流、重绘其实就是需要减少对render tree的操作，并减少对一些style信息的请求，尽量利用好浏览器的优化策略

   （1）  避免操作DOM，创建一个documentFragment或div，在它上面应用所有DOM操作，最后再把它添加到window.document。也可以在一个display:none的元素上进行操作，最终把它显示出来。因为display:none上的DOM操作不会引发回流和重绘。

   （2） 让要操作的元素进行"离线处理"，处理完后一起更新，这里所谓的"离线处理"即让元素不存在于render tree中。如读取offsetLeft等属性。

   （3） 尽可能在DOM树的末端改变class ，尽可能在DOM树的里面改变class，可以限制回流的范围，使其影响尽可能少的节点。

   （4） 避免设置多层内联样式，因为每一个都会造成回流，样式合并在一个外部类，这样当该元素的class属性被操作时，只会产生一个reflow。

   （5） 将需要多次回流的元素position属性设为absolute或fixed，这样该元素就会脱离文档流，它的变化不会影响其他元素变化。比如动画效果应用到position属性为absolute或fixed的元素上。

​    （6） 牺牲平滑度换取速度，动画元素每次移动3像素可能在非常快的机器上看起来平滑度低了，但它不会导致CPU在较慢的机器和移动设备中抖动

​    （7） 避免使用table布局，在布局完全建立之前，table需要很多关口，table是可以影响之前已经进入的DOM元素的显示的元素。即使一些小的变化和会导致table中所有其他节点回流。

   （8） 避免使用css的JavaScript表达式，该规则较过时，但是个好主意。因为每次都需要重新计算文档，或部分文档、回流。





标签: [浏览器](https://www.cnblogs.com/microcosm/tag/%E6%B5%8F%E8%A7%88%E5%99%A8/)