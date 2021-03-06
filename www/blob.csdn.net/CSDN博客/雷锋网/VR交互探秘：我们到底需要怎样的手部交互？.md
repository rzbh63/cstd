
# VR交互探秘：我们到底需要怎样的手部交互？ - 雷锋网 - CSDN博客


2016年11月28日 17:51:22[leiphone](https://me.csdn.net/leiphone)阅读数：1382


导语：从现在开始的相当长一段时间里，手部交互依然是最成熟的控制类人机交互方式，并且体验也远远没到完美，值得投入完善。

![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab3af22959.jpg?imageMogr2/format/jpg/quality/90)
我们与现实世界进行交互的主要方式是手，我们与VR世界的交互同样如此。
从最初级的手势交互，例如英梅吉和 HoloLens 仅有固定几个手势，到追踪用户手指关节运动在虚拟空间中模拟整只手出来的Leap Motion、uSens，再到手拿手柄在虚拟空间里进行交互的 Vive 手柄和 Touch，还有加入更多交互维度的产品，比如Dexmo和微软都有研发的力反馈。
关于用户的手与 VR 世界交互这块几乎所有VR公司都要涉及，当下的手部交互存在哪些问题？如何让我们与VR世界的交互更加真实、自然？
本期公开课我们邀请到了VR领域技术专家张梦晗为大家解读这些问题。
## 嘉宾介绍
张梦晗，男，VR领域技术专家，高级工程师。2012年至今从事VR/AR领域技术研发，目前聚焦在VR/AR交互领域进行技术探索和研究。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aa925f3e93.jpg?imageMogr2/format/jpg/quality/90)
以下内容整理自公开课。
## 当下主流的VR交互方式
在回答这个问题之前，我想先解释下交互在AR&VR里面的意义。
现在的VR产品的产品形态逐渐趋同，世界上能提供合格屏幕、光学、芯片等等关键器件和材料的公司也就那么几家，大厂进来以后各种参考设计也越来越多，VR头盔未来就是一个制造显示器的活，能产生差异化的点越来越少了。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab2626fa7c.jpg?imageMogr2/format/jpg/quality/90)
Microsoft Kinect
而目前大家都在VR交互方面下大功夫，短期来说是产品本身的差异化需求，这不仅仅关系到用户体验，还有内容生态本身的原因。以Microsoft Kinect为例，基于体感交互设计开发的游戏成为了事实上的平台独占游戏，并且成为很多人购买Xbox的核心理由。
长期来看，VR和AR设备在交互领域是相通的，而且**目前的阶段正是一个拓荒期：操作界面正在从二维平面转移到三维空间，但是还没有形成一个类似PC时代下鼠标键盘操作界面这样的大家公认的交互范式，这样的机会应该说是所有做人机交互的专业人士一辈子难得遇到的机会。**
在这样的背景下，我倾向于将VR&AR交互的范畴划的大一些：
**一类是大家普遍关注的由人主动发起的交互动作**，比如手部交互、体感交互等等，这类交互往往发出的都是确定性的指令性信息，要求准确达成某些特定的目的，也会耗费最多的精力和体力。
**还有一类则是由机器主动发起的交互动作**，比如眼球跟踪、脑电肌电、头部位置跟踪、表情识别等等，这类交互是机器来感知使用者的状态来进行交互，很多时候使用者自身不需要牵扯太多精力。
**再有一类不能忽视的是机器对环境的感知交互**，比如三维重建、情景感知、物体分割识别等等，很多时候尤其在AR中，只有先做好对外在环境的感知，人与机器之间的交互才能顺畅。
这是我理解的交互范畴，帽子有点大盖住的东西太多，但是今天我们还是集中于一个关键点，也就是手部交互来进行探讨。
**VR手部交互的重要性**
手对你有多重要，手部交互就有多重要。
除了嘴之外，手应该是人日常最经常使用的的信息输出器官了，传统的大部分交互接口也都是围绕人手来设计，确定性高，兼容性好。
PC时代那么多交互范式，最后落到鼠标键盘都是和手部相关。
当然我们不能因此就说只有手部交互才是VR&AR的未来，做好交互永远是一个综合治理的过程。
而且随着AI的发展，由机器发起的交互逐渐也可以实现确定性的指令判决了，那么手部交互的重要性可能会变弱。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab1aa1c70c.jpg?imageMogr2/format/jpg/quality/90)
游戏“光晕”图，里面的士官长可以直接和寄宿在脑中芯片的AI交互，这是比较理想化的情况
但是从现在开始的相当长一段时间里，手部交互依然是最成熟的控制类人机交互方式，并且体验也远远没到完美，值得投入完善。
**VR手部输入方式的种类**
按我的区分方式就是裸手和非裸手两类，根据产品形态和技术原理的不同又可以细分。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab0c4d6d2d.png?imageMogr2/format/jpg/quality/90)
Google Project Soil 裸手交互
**裸手就是不需要任何介质的手部交互**，比如Leap Motion基于计算机视觉的，Google Soli基于毫米波雷达的，还有一些基于超声、红外的解决方案，总的来说基于视觉图像处理的发展的最成熟，效果目前来说也最好。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab0143d3a8.jpg?imageMogr2/format/jpg/quality/90)
Oculus Touch
**非裸手就是需要某种介质的手部交互**，常见的就是Daydream Controller、Oculus Touch这种3DoF或者6DoF手柄，还有比较多的就是各种各样的数据手套，基于视觉、惯性、弯曲传感器等等，总的来说手柄还是接受程度最高的，但是现在的趋势是手柄和手套会逐渐融合成一体，形成一个新形态的人机交互媒介。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583ab01794fd9.jpg?imageMogr2/format/jpg/quality/90)
诺亦腾全身动捕套装
在VR里，实际上裸手还是非裸手都有比较广泛的应用，而且相对来说非裸手还有一定的优势：**在VR下人主要和虚拟物体和环境交互，通过某个媒介能提高交互的真实感。**
在AR里，因为人不仅仅要与虚拟物体交互同时也能看到现实场景和物体，所以裸手情况在AR中更多。
## 不同手部输入方式的优劣势
评价某项技术优劣势有多种维度的考量，从产品角度成本和功耗就能干死绝大部分的交互方案，当然目前我们还是希望能从体验的角度来进行分析。
**裸手的优劣**
按我之间的分类方法，我们先谈谈裸手的优劣：
方便，你不用每天带一个手套在手上，或者带上头盔四处乱摸找手柄；
如果不是要购买独立模组（用手机上的摄像头）或者付高昂的授权费用，裸手也会更便宜；
至于很多人所说的裸手交互更自然更符合人的习惯，在VR下我不赞同，但是在AR或者MR下大部分情况是对的。
劣势就是：准确性还不达标，在VR环境下使用缺乏很多要素，这个我后面再细讲。
**非裸手的优劣**
准确性好，你按一下就是一下，两下就两下，大部分时间不用担心输入错误；
靠媒介可以提供反馈，这个在VR下很关键；
劣势：不方便，走哪你都要想着带；有学习成本，当然也看UX设计的好坏；在AR和MR环境下会阻碍人与现实的交互，这个最致命。
总体来说，裸手和非裸手的优缺点是能够互补的，所以在VR环境下这两种手部交互会同时存在，而长期来看，AR/MR的时代裸手会逐渐成为主流，或者我们能找到一个融合方案，我个人觉得这个是一个值得大家一起努力的方向。
**为什么祼手交互没有成为主流？**
抛开成本和功耗不谈（其实这两个是最要命的），就体验来说根据上面我说的优劣势来判断你也能知道问题所在的症结了。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aaf21b7f4f.jpg?imageMogr2/format/jpg/quality/90)
应用于PC上的Leap Motion
Leap Motion这类手部跟踪方案出现于PC时代的末期，目标是替代鼠标和键盘，结果大家也都看到了，因为PC下的鼠标键盘实际上已经是公认的范式，大家在使用过程中并没有感觉有太多不便，就算不方便也习惯了，这就导致绝大部分人没有接受一种新的PC交互方式的需求，做产品的都知道没有需求意味着啥。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aaf6b159f4.jpg?imageMogr2/format/jpg/quality/90)
应用于VR交互的Leap Motion
而VR兴起了之后，裸手交互总算迎来了一个机遇，因为在VR下鼠标键盘都不好用了，这下裸手交互总算是有需求了吧？事实上还是高兴的有些早，因为这个时候和裸手竞争的还有非裸手的手柄和触摸板（参考EPSON BT200）。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aae47de7e0.jpg?imageMogr2/format/jpg/quality/90)
EPSON BT200
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aae7ddbb22.jpg?imageMogr2/format/jpg/quality/90)
华为VR头盔的触摸版
而和这些交互设备相比，裸手交互的优势也并不明显：**裸手交互在正确输入前提下的确认度低这个老大难问题实际上一直没有解决，就算有95%的确认率，玩游戏的时候也是20次开枪有一次哑火，而手柄在正确输入下的确认率毫无疑问是接近100%的。**
仅仅这一项就导致裸手还无法取代手柄这类交互设备，这就带来了**另一个更为关键的问题：裸手交互的必要充分场景在哪里？**这也是我一直在和很多做手势交互的朋友们探讨的问题。
一项还不够成熟的、不便宜的技术，如果想被广泛的接受就必须有一个前提，我们要为它找到一个充分必要的场景，在这个场景下，它具有不可替代性，除了裸手没有别的交互能做的更好，而如果这个场景是一个刚性场景，那么就算再不成熟也可以忍耐。
但是在VR下裸手交互还没有找到那个场景，国内有很多厂家用裸手交互来操作进度条、暂停播放等等，这样的操作我用手柄或者头盔上的触摸板可以更容易解决；还有描述车载家庭等场景下的裸手交互也是有问题，在这些场景下语音可能更有优势（尤其是开车，手不能离开方向盘的）。
所以从手机时代直到VR，我觉得裸手交互都是很痛苦的，因为没有实实在在的刚性需求，用户和开发者往往会被Leap Motion的酷炫所吸引，但这种冲动式的热情会很快被裸手的很多问题所磨灭，所以我个人认为最紧急的事情是找到裸手交互存在的充分必要场景，这样大家就都能活下去。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aae00b4064.jpg?imageMogr2/format/jpg/quality/90)
而从VR到AR/MR，裸手交互的必要性才会越来越大，（当然也要强调交互设计，Hololens的那种手势就过于生硬，明显是硬塞进去的），我个人觉得这种充分必要的场景会在这个时间段出现，现在还是有些早。
**为什么很多VR里的手显得很假？**
首先假不等于差，但是当一个人觉得某件事物假，那么一定不是说某一点出了问题。
首先要说在VR手部交互方面借助手柄在某些场景下反而更真实，很多第一人称设计类游戏角色本身是一直手中有枪的，这种场景下拿一个手柄会比裸手更真实，目前也有一些依靠剪切力来模拟挥动以及开枪后坐力的方案，通过手柄媒介能做很多事情，更容易提升体验。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aadb3c16ff.jpg?imageMogr2/format/jpg/quality/90)
而如果我们希望在VR交互环境中是裸手操作，那么就会遇到更多显得假的情况。首先强调的就是VR中需要的不是手势识别（Gesture Recognition），而是手部跟踪（Hand Tracking）。手势识别和手部跟踪估计是两码事，我们在日常生活中用到手势的唯一场景就是打手语，所以你在VR环境中也用手势来进行操作是假的不能再假的事情，肯定会让人别扭，这也是最容易出现的误区。
而Hand Tracking虽然更符合人日常的交互习惯，我能看到整个手部模型并且能实时跟踪（不管是有Bone还是仅仅是Tips和Palm），依然有很多显得假的情况：估计的不准容易抖动，双手交叠容易丢失，如果基于视觉做的话超出视距范围也会丢失等等。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aacb88f872.jpg?imageMogr2/format/jpg/quality/90)
这还仅仅是手假，你在VR环境中所有的物体和场景也是假的，这会导致很著名的“幽灵手”现象。你的手想抓住一个虚拟的物体，摸到一个虚拟的墙壁或桌子，你会发现抓不到，摸不着，而视觉方面又不好做反馈，只好将手设计成可以穿过任何物体，你在这样的场景下就是一个幽灵一般的存在，当然要多假有多假。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aad25e0962.png?imageMogr2/format/jpg/quality/90)
Jobs Simulator里手的设计
所以大家可以看到实际上导致“假"的原因是比较综合的，短期内想完全解决比较难，做好视觉的反馈包括UI的设计实际上是类似Leap Motion这样的解决方案目前所关注的，也有了不小的成效。用户虽然觉得假，但也不会觉得有更多的不适应，而且有的时候如果你真的没有办法做的完全真实，假一点反而有好处，任何仿真都是有恐怖谷的问题。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aab5238ac5.jpg?imageMogr2/format/jpg/quality/90)
一张Leap Motion的图，可以看到里面不仅有手部，还有前臂部分的跟踪
另外还要强调下，除了Hand部分的真假，很多开发者其实还忽视了Arm部分的重要性，如果只是悬空显示一双手的话，也是会显的很假，Leap Motion考虑到了这一点将Arm也包括进来，实际上不仅对算法鲁棒性，对改善交互者的体验也是有很大帮助。
## 对于“虚拟的手”，方便比真实更重要
刚才说在VR的手部交互方面借助手柄反而会更真实，因为手中有一个实时在在的反馈。而你（主持人）所说的虚拟用户手的行为应该就是指Oculus Touch，因为别的手柄暂时都没有这个能力来模拟一个真实的手。我们很多人都体验过Oculus Touch，原理也都知道的差不多，它依靠几个点的接近光传感器，确实能够模拟出手部的一些细微动作变化，设计的很巧妙。
但是你所说的自然我觉得在VR中是不存在的，这个其实也是我一直想强调的事情，**自然不见得是VR场景下必须追求的事情，因为物体是虚拟的，你的手也是仿真估计出来的，这个毫无自然可言，我们更多的是追求流畅、快速、用最小的能耗省时省力的完成交互，而这些有的时候和自然会有矛盾。**
比如现在很多交互设计，刻意做成和现实场景完全一致，比如抓取一些细微的零件、走老远拉出一个抽屉，看上去蛮酷的，但是时间长了没人会受得了，就像是在演哑剧，明明哪里没有东西，我们还要按有东西来操作，这种不协调感会加剧人的疲劳和不适。
![VR交互探秘：我们到底需要怎样的手部交互？ ｜ 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201611/583aab1c16a2a.jpg?imageMogr2/format/jpg/quality/90)
所以很多时候我们既然没有办法在VR中把交互做的那么真实，那还不如就多思考如何让交互更方便，比如把交互控件做的大一些、引入raycast等方式操作远距离的物体，不真实，但是简单有效省时省力，反而是目前消费者更为需要的。
若是就是不惜一切代价的追求真实，除非是一些垂直行业的仿真场景，这种对成本不敏感，就完全是另外一种思路，而且我不觉得VR需要走到那一步。
## 如何让裸手交互更受欢迎？
**基于视觉的裸手交互的问题归纳起来有如下几点：正确输入下的确认率差，鲁棒性也差，缺乏反馈，交互范围相对窄（脱离视距范围会丢失），易疲劳（更像真实交互所带来的副作用，恐怖谷效应）。这些问题每一个都是挺要命的，需要综合的治理和解决。**
前面的几条的改善就是比算法硬功夫，你模型估计的越准确，实时性好，就会有所收益，但是即使做的再完美，后面的反馈问题和交互疲劳性问题也没有办法解决，所以两部分要同时进行。
有一些设计将人的手作为操作界面，用另一只手点击虚拟出的菜单来完成交互，虽然会遇到交叠跟踪丢失等问题，但可以提供反馈，依然是一个很好的思路。
交互范围窄的问题在Inside-Out的场景下更为严重，Leap Motion使用双鱼眼镜头（Dual Fisheye Camera）的主要原因就是希望能尽可能的扩大交互范围，而结构光和ToF的FoV与分辨率的矛盾是硬伤，这需要器件本身的规格提升。
至于如何设计能让用户和开发者更受欢迎，**我还是强调几点：忘掉所谓的自然，强调流畅快速省时省力。最最最最重要的是：找到一个只有裸手交互才能做的最好最舒服的充分必要场景，这个场景应该成为所有裸手交互研发人员共同最求的圣杯，找到了它大家才都能活的好。**
**力反馈手套解决的问题和存在的局限**
关于反馈的重要性我前面也提到，解决幽灵手的问题，加入反馈机制是必不可少的。但是抛开成本和造价不谈，力反馈手套还是有很多的问题。
首先所有的数据手套都没有解决好的一点就是佩戴的舒适性和方便性，这个是制约其进入消费级别市场的最大因素。很多数据手套通过多IMU融合来做手部位置跟踪捕捉，但是要想做的精确需要每个传感器精确放置在骨节上，并且还免不了要做初始化校准，这都不是一个消费级别产品能够接受的。
另外加入反馈的方式也有很多问题，反馈分为力反馈／震动反馈／触觉反馈，综合形成一个比较好的HAPICS体验，但是目前在力反馈领域还很难用一个小的机械结构提供足够大的阻尼力，而且机械结构本身就有飞车的风险，容易造成人的肌肉损伤。
其他的反馈机制则也有很多硬伤，比如非侵入式的触觉反馈，本身受环境影响很大，手一出汗沾水反馈效果就受到影响，而电极植入这种浸入式方案也明显不会被消费者接受。
反馈方面的话题可以展开说很多，对于数据手套来说，目前大家做的都还是比较早期，也只有对精度要求较高的工业场景，医疗场景和设计场景中会有明确的需求，距离消费级市场比较远。我们也看到很多公司在尝试简化系统，用比较低廉的价格尝试进入消费领域，不过我还是希望他们能解决好佩戴和使用便利性的问题，另外就是要想好如何使用它。
**手部交互的未来**
分开来看，手柄手套这种非裸手的交互会在相当长一段时间里一直存在下去，并且大家都在围绕这个东西来做改良，从Oculus Touch到下一代的Vive手柄，我们都能清楚的看到这个趋势，更多的功能被添加，更加贴合手型，可以做6DoF跟踪，震动和力反馈等等，这都是基于介质来做手部交互的优势，毫无疑问要想办法发扬光大。
而裸手交互可能在短期内依然会比较艰难，当然好在大家都看的比较清楚裸手交互在AR／MR时代的必要性，所以还是会不断的向前演进，除了提升算法本身的稳定性，一些能够改善体验的UX设计也能改善体验，撑着大家能够活到AR兴起的时期。另外还是要强调一点，寻找到那个圣杯：裸手交互的必要场景才是最主要的，别的我觉得都好说。
## Q&A 环节
**1、如何评价HoloLens的手势交互，这种程序的手势交互是否可以用作日常使用的产品？**
**张梦晗：**首先应该有不少朋友都体验过Hololens了，大家相信对它的体验都有感觉。
它是一种“手势识别”，而不是一种“手部跟踪”，视觉上并没有一个反馈给到你，你甚至都很难判断我当前的状态到底是不是处在有效的交互范围内。
这样的情况下，交互的效率自然就大打折扣，并且还有一个问题，既然我的系统整体是使用头瞄来选择交互目标，我用一个简单的触摸板（Touchpad）触碰会不会比那个不靠谱的双指掐击更方便确认。
至少目前来说，我个人认为Hololens的主要精力并没有放在手势上，只是捎带塞了一个交互方式进来。我甚至觉得这个交互方式有可能是硬件定型后才发现没有交互方式，所以才利用视觉系统做了个手势，其实我觉得侧面加个触摸板也许更好用。
抛开Hololens本身的设计不谈，我觉得裸手交互在AR设备上是绝对必要的。但也要有对应的交互设计配合才行。比如AR下很多时候我的目标交互物就是定位在某个现实物体之上的，甚至虚拟物体的运动会与现实物体发生交互，这个时候用裸手交互显然是不二的选择。
而回到Hololens，从它的交互界面来看我确实没有看到必须要使用手势的必要性，也就是说他们并没有找到我提到的圣杯。
**2、三维手势交互上面，目前技术上的问题更多还是设计上的问题更多？技术上解决三维交互比较好的方向是什么？离成熟还有多远？**
**张梦晗：**技术上和设计上的问题都很多，而最大的问题是那个必要充分的场景没有找到，这个我强调无数次了。
技术上我觉得单纯的提升准确率和精度已经无法完全解决深度手部跟踪的应用问题了，当然裸手交互方案提供商们还是会持续提升客观指标，但是我们会渐渐发现很多客观指标的提升已经很难在体验上表现出来。
尤其是Inside-out场景下，再高的准确率也无法解决疲劳感和交互范围狭小的问题，而解决后两者就不仅仅是技术问题，而是设计问题，所以我之前说过，要综合治理。
而在没有找到手势交互的必要场景之前，想留住用户让用户想用愿意用深度手势，我觉得关键在于利用视觉声学等综合反馈，给用户尽量提供明确的确认信息，尽可能让用户像使用手柄一样知道自己什么时候选中了。
只有这种确认性接近达到手柄的水平，裸手手势的方便性才能充分体现，如果不贵的话，也许我还愿意用一下。至于别的我觉得等这个小目标达到了再慢慢看吧。
或者我们直接搞定圣杯，找到一个只有裸手才能搞定的业务场景，那么问题再多我也愿意将就着用，慢慢等你改良。让用户没有选择，你才能立于不败之地。
**3、手势识别或是手势跟踪，主要基于机器视觉，或者是一些传感器融合，目前用的比较多，比较鲁棒的技术有哪些呢？**
**张梦晗：**鲁棒性的问题实际上和设计也有关系，相同的精度下操作小器件和大按钮的发生错误的概率肯定不同。基于多传感器融合，可以很好的解决超视距下的交互问题，手揣兜里也能跟踪，这是基于视觉的手部跟踪所没有办法解决的问题。
但是这种情况也会有陀螺仪长期累积漂移，需要初始化等问题，从这个角度来说，可以一直检测校准的视觉方案又有优势了。而最近有很多的解决方案强调自己在鲁棒性上有好处，比如说很多基于60GHz的毫米波雷达定位，主打的就是相对视觉方案来说更能抗干扰，尤其在室外环境下。
即使是基于视觉原理，不同的器件方案也有影响，RGB Dual Camera会比结构光在室外抗红外干扰能力强，但在暗光环境下又反过来。
没有缺点的方案还没有出现，目前采用Dual Camera比较多也是因为FoV可以做的比较大，交互范围的大小要比鲁棒性重要一些。
雷锋网原创文章，未经授权禁止转载。详情见转载须知。



