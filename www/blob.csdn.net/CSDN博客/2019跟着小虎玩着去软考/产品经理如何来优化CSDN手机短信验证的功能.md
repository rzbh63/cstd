
# 产品经理如何来优化CSDN手机短信验证的功能 - 2019跟着小虎玩着去软考 - CSDN博客

2017年04月13日 07:41:34[littletigerat](https://me.csdn.net/littletigerat)阅读数：927标签：[产品设计																](https://so.csdn.net/so/search/s.do?q=产品设计&t=blog)[用户思维																](https://so.csdn.net/so/search/s.do?q=用户思维&t=blog)[互动																](https://so.csdn.net/so/search/s.do?q=互动&t=blog)[交流																](https://so.csdn.net/so/search/s.do?q=交流&t=blog)[用户体验																](https://so.csdn.net/so/search/s.do?q=用户体验&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=交流&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=互动&t=blog)个人分类：[点滴生活																](https://blog.csdn.net/littletigerat/article/category/666611)[程序人生																](https://blog.csdn.net/littletigerat/article/category/779495)[学习与思考																](https://blog.csdn.net/littletigerat/article/category/646894)[
							](https://blog.csdn.net/littletigerat/article/category/779495)
[
																								](https://blog.csdn.net/littletigerat/article/category/666611)

[
		](https://so.csdn.net/so/search/s.do?q=互动&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=用户思维&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=产品设计&t=blog)
**产品经理如何来优化CSDN手机短信验证的功能**
昨天4月12日星期三，博友都在说：“我的手机验证不过呀”。
CSDN运营PLMM也一直抱怨：“今天一天博客的事都没完成，一天都是登录手机验证的问题，我要哭死了！”
小虎怜香惜玉![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)，辗转反侧，夜不能寐，苦思冥想，绞尽脑汁，有没有什么好的办法帮帮她，拯救MM于水深火热之中，小虎不才，斗胆尝试一把，也算**网虫救美呗**，小虎可不是啥英雄啊!  小虎顶多算个超级网虫，呵呵。

**先来看看，问题是什么？**
![](https://img-blog.csdn.net/20170413065604494)
就是要求用户输入一个验证码和编辑一条短信。
**现在问题的两大焦点是：**
1.用户不知道，后台处理怎么样了，完全一片空白。
2.后台程序不知道用户发送了没有，发送到指定的号码没有。
要是小虎是CSDN产品经理，这么设计可能会比较好：
（希望与大家共同讨论交流，如何来设计一个好的产品，有很强的用户体验，技术上也容易落地实现，投入成本最少，达到用户体验效果最佳。**算是抛砖引玉吧！**）

1.第一步：输入手机号码；
2.第二步：
如果手机号码正确，显示验证码；
验证码，不要啥阴影，去掉“换一张”，在右边，就是醒目的英文字符
**好处：一个手机号，先只生成一个验证码和一个短信编辑校验信息**
3.**只有**第二步验证通过了，才有编辑手机短信这档子事。
![](https://img-blog.csdn.net/20170413072135054)
将发送按钮添加：一个是否已经正确编辑的界面，如上图所示。
4.
**建议：**再加**一个倒计时的按钮**，提示用户，耐心等候，后台正在处理，
1分钟到了，还不行，要求重新再来。
重新再来，就是：系统重新为该手机号码生成1个校验码和1个手机编辑码，因为原来的信息失效了。
体现了：与用户的交互，互动。

【终极建议】
在上面的步骤中，
**去掉要求用户输入验证码。**
因为系统在后台可以拿到这个验证码。
客户已经通过自己手机发送了校验信息了，保证了用户手机唯一性了。
希望与大家共同交流，如何来设计一个好的产品，有很强的用户体验，技术上也容易落地实现。
呵呵，小虎真是为CSDN操碎了心啊！

[
](https://so.csdn.net/so/search/s.do?q=产品设计&t=blog)
