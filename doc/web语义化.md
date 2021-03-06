## [web语义化](https://www.cnblogs.com/microcosm/p/7359034.html)

在万千的网页中，我们如何搜索到自己想获取的信息？答曰：搜索引擎。搜索引擎是如何获取页面信息的呢？答曰：机器借助HTML文件。机器是如何从HTML文件获取我们想搜索的内容呢？答曰：关键字？？？ 想要解决这个问题，来看下面的内容：

Web语义化的目的是什么，其实简单说来就是让机器可以读懂内容。

随着 Web 规模的不断扩大，信息量之大已经不在人肉处理的范围之内了。这个时候人们开始用机器来处理 Web  上发布的各种内容，搜索引擎就诞生了。再后来，人们又设计了各种智能程序来对索引好的内容作各种处理和挖掘。所以让机器能够更好地读懂 Web  上发布的各种内容就变得越来越重要。

其实 HTML 在刚开始设计出来的时候就是带有一定的「语义」的，包括段落、表格、图片、标题等等，但这些更多地只是方便浏览器等 UA 对它们作合适的处理。但逐渐地，**机器也要借助** **HTML** **提供的语义以及自然语言处理的手段来「读懂」它们从网上获取的 HTML** **文档，但它们无法读懂例如「红色的文字」或者是深度嵌套的表格布局中内容的含义，因为太多已有的内容都是专门为了可视化的浏览器设计的**。面对这种情况，出现了两种观点：我们可以让机器的理解能力越来越接近人类，人能看懂、听懂的东西，机器也能理解；我们应该在发布内容的时候，就用机器可读的、被广泛认可的语义信息来描述内容，来降低机器处理  Web 内容的难度（HTML本身就已经是朝这个方向迈出的一小步了）。

## 如何实现web语义化？

### HTML语义化

在之前的文章已经提到《[HTML5之语义化标签](http://www.cnblogs.com/microcosm/p/7125813.html)》HTML5语义化。比如这些标签：`<nav>`、`<header>`、`<footer>`、`<artical>`、`<section>`等等，这些标签不仅在丢失样式后还可以让页面清晰地呈现，而且有利于搜索引擎优化，对于视力障碍的用户，机器可以为他们“读”出页面。

### CSS语义化

CSS语义指的是class的语义。

class属性本意是用来描述元素内容的，而不是描述元素展现的。其典型‘反模式’代表就是METACSS。

看看这两段代码，哪一个更容易理解？
```html
<!-- 以表现为中心 -->
<div class="fl mr10">
    <span>userName:Kuitos</span>
<div>

<!-- 以信息为中心 -->
<div class="user-info">
    <span>userName:Kuitos</span>
<div>
```
class作为HTML描述属性集的一部分，本身是用来细化内容语义的，所谓的CSS语义化本质上就是HTML语义化

所以，web语义化就是借助一些人类可以在视觉上快速分辨的，同时机器也可以迅速捕获的的标签，来编写HTML文件。