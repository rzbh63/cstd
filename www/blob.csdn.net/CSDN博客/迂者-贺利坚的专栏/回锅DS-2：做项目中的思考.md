
# 回锅DS-2：做项目中的思考 - 迂者-贺利坚的专栏 - CSDN博客

2019年01月04日 14:22:29[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：83个人分类：[学习指导																](https://blog.csdn.net/sxhelijian/article/category/1106461)
所属专栏：[IT学子成长指导](https://blog.csdn.net/column/details/itstudy.html)



（本系列出自一位在工作后“回锅数据结构”且坚持写学习反思的网友。他用邮件发给我，征得他的同意，我在我的博客中当“原创”连载发出来。和我的关联是，他的学习路线中，的确有我提供的方法的影子。）
1.编程思路：看别人代码，整理思路，一定要思路清晰，这样才能模仿写出自己的代码，整体思路有了，意即框架有了，然后往里面填充，填充的时候就灵活多变了，这时候可以多尝试几种方法去实现，这样就把知识用活了。这个程序是要干嘛，要实现什么，代码的目的是实现功能，所以要翻译成人类语言，来说明他是干什么的。注释也写好，注释可以把整体思路再体现出来。既有整体框架思路的注释，还有一些值得注意的细节的注释。
2.做项目的时候遇到问题要及时记录和总结。工作中有很多都是宝贵的财富，所以也要及时觉察，并不是只学习的时候才觉察，工作干活的时候更要觉察实践中宝贵的经验。以前不重视工作中的总结，现在要开始重视，工作和学习一样，都很重要，都能总结出很多东西，并不是仅仅是学习帮助工作，工作也可以反过来帮助学习。有时候回头看自己写出来的代码，会惊呼“这是我写出来的呢，是怎么想到的呢”，这应该也是在工作的时候缺乏思考，缺乏总结，没有趁着这股劲把工作中的收获总结出来，和知识联系起来，只是单纯的在拿知识解决问题。
`for (var i=0; i<regionLayer.length; i++){
    var divLayer = regionLayer.eq(i).children();
    for (var j=0;j<divLayer.length;j++){
        if ( divLayer.eq(j).find(".number").find("em").length==1) {
            divLayer.eq(j).find(".number").html(numberFormat(divMillion(res[p++])) + "<em>millions</em>");
        }else {
            divLayer.eq(j).find(".number").html(percentage(res[p++]));
        }
        setMerchantUpDown(divLayer.eq(j).find("p").eq(0),parseFloat(res[p]));
        divLayer.eq(j).find("span").eq(0).html(formatPercentValue(res[p++]));
        setMerchantUpDown(divLayer.eq(j).find("p").eq(1),parseFloat(res[p]));
        divLayer.eq(j).find("span").eq(1).html(formatPercentValue(res[p++]));
    }
}`看了自己的这段代码，又回头把jQuery原生的选择器和函数选择器看了下，对比了下，是等价的，可以自由使用。之前看jQuery函数选择器的时候以为是原生选择器实现的一部分，回头看时发现是等价的，虽然函数选择器少，但是却实现了全部的功能，纠正了之前脑海中模模糊糊的认知。对find等的函数理解更加深刻。能够做到对元素指哪打哪。
3.揣摩别人说的话的意思，挖掘出价值，能挖掘出多少价值，其实是主要看自己的，而不是主要靠别人的。
4.做经分项目的思考：干活用到新东西，然后去学。学的时候不能走以前的模式，快速的学，然后干活，反复回来看；而应该像学java时候一个一个模块的逐个击破，这样每个模块都能掌握的很牢固，然后再去干活的，这时候就灵活组合了。
5.干活中用到新东西了，用完要回头看一下，在复述一遍，自己给自己讲一遍。也不要有别人帮自己看着东西，自己偷懒的习惯了。抓住感觉，多问几个为什么，不要让感觉白白溜掉。不断的自我进化。
6.破除系统学习的情结，并不是完全不需要系统学习，也要分情况。避免很多东西只有短暂的开始，就没有后续了。有些东西是可以从简单的实践开始，然后一步步开展，学用结合来进行的，而不是理论完全学完再去实践，实践的时候不想理论。而且是相互促进的。实践的时候不要担心这不会那不会的，只管继续进行，知道自己一直在学会新东西就行，到一定阶段然后后头总结一遍，把之前学的理顺，形成网络就好。
7.学完就用的效果是最好的。不要空学。就像前一段学了那么多理论，都没用上，所以是空学。不管是学什么，都要学完就用，如果没有实际项目，那么就自己给自己布置任务，写demo出来。而且demo要越来越难，认真对待demo，当做一项重要的任务去完成。

