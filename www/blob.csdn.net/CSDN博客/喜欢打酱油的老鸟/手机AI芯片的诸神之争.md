
# 手机AI芯片的诸神之争 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月03日 08:19:41[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：91标签：[手机																](https://so.csdn.net/so/search/s.do?q=手机&t=blog)[AI芯片																](https://so.csdn.net/so/search/s.do?q=AI芯片&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=手机&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/i6630375745803780615/](https://www.toutiao.com/i6630375745803780615/)
2018-12-02 20:44:12
寒武纪科技创始人及CEO陈天石此前在一次公开活动上提到这样一个小八卦：谷歌大脑项目用了1.6万个CPU核跑了7天才完成猫脸识别。
讲这个八卦的意图在于说明，CPU/GPU用于智能信息处理效率十分低下，神经网络处理器是迄今为止最好解决方案。
麻省理工斯隆管理学院教授埃里克·布莱恩约弗森在《第二次机器革命》一书提到，创新所做的真正工作并不是提出什么大的和新鲜的事物，而是把已经存在的事物重组起来。
**手机处理器能不能把CPU/GPU和神经网络处理器结合（NPU）在一起，让手机同时具备三者的优势？这是过去几年半导体产业在探索的一个课题。**
11月中旬三星发布了旗舰处理器Exynos 9820处理器晶片组，预计将会在年底量产，在明年年初Galaxy S10系列旗舰机上使用。Exynos 9820最大的卖点在于，终于有独立的神经网络处理单元（NPU），这让装置内AI运算速度比前代快了7倍。
这仅仅只是今天移动处理器市场竞争的一个小切口。从华为、苹果、联发科的初露锋芒，再到高通、三星的迎头赶上。
半导体巨头们用回旋激荡的你追我赶，改变了“Tick-Tock”钟摆定律的线形增长趋势，直接让手机处理器市场迈入AI能力竞争的新阶段。
**华为的激进**
独立AI处理单元的竞争起源于2017年9月。9月2日，在德国IFA2017举办期间，华为正式发布全球首款人工智能移动计算平台麒麟970。
之所以称之为“人工智能移动计算平台”是因为，麒麟970第一次搭载了寒武纪的NPU——NPU和CPU、GPU的功能是相对应的，指的是就是在传统手机芯片核心CPU、GPU、ISP、DSP等基础上，加上一颗用于神经元计算的独立处理单元，英文名 Neural Network Processing Unit，简称NPU，中文含义为“神经元网络”。
![手机AI芯片的诸神之争](http://p99.pstatp.com/large/pgc-image/0d9cb33dc9c74367a16e0f1a54067dfa)
10天后，苹果发布iPhone X，A12处理器上同样搭载了芯片设计方面跟进华为集成类似NPU模块。这时业内才发现，独立神经网络处理器已经成了潮流。
华为为了抢NPU世界第一这个旗号，动作非常激进，特意把发布会提前到了苹果iPhone X的前面。之后甚至也能看到一大批《苹果A12处理器跟进华为集成NPU，手机AI设计看中国》的类似文章出现。
事实上，苹果和华为的规划几乎是在同期展开的。并不存在谁学谁的问题，华为的激进，目的在于做大国产芯片的市场口碑。抢到“全球首款独立AI处理单元”的发布，这对麒麟处理器的营销会有极大增益。
不可否认的是，华为这种策略推动了半导体产业对独立神经网络处理单元的重视，也加剧了这个产业的竞争。华为也愿意在神经网络处理单元军备竞赛中和其他企业“打对手牌”。在今年3月，联发科推出搭载双核APU处理器单元的P60之后，华为9月又在德国IFA展上发布了麒麟980，同样搭载了双核NPU。
**这种军备竞赛的策略，虽然有时会超出当下需要，但是却通过这种“过度竞争”的方式，让独立神经网络处理单元得到了普及。**
至此，有没有独立用于AI计算的处理单元，已经成了考验手机处理器的核心要素。
和两三年前手机处理器仅仅是性能堆叠不太一样，独立神经网络处理单元的出现让性能堆叠的线形竞争又上升了一个维度。
**高通的保守**
虽然华为很激进，相比行业其他厂商，高通显得保守、迟疑很多。
在2017年年底亮相后，众人大跌眼镜，骁龙845基本还是沿袭上一代骁龙835的架构。在AI方面，高通骁龙845并没有专用于处理AI任务的NPU，而是拿出原本做ISP影像处理的Hexagon 685 DSP來处理AI应用，性能不够的時候再转换使用GPU來应付。
![手机AI芯片的诸神之争](http://p99.pstatp.com/large/pgc-image/bdbeed3e6b1c473eb66810fffdfe11bd)
在AI和拍照同时工作这类重载场景，本来这些模块就已经处于负荷比较大的状态，再增加AI诉求，必然会对整个运行效率造成影响，不仅会拖慢速度，还比较耗电。
不管怎么说，高通还是通过这种曲线救国的策略支援上了AI功能。那么，高通为什么不愿意在高通骁龙845上加入NPU模块呢？
我们可以从很多细节处找到答案。雷军在小米MIX2S发布前夕，曾在微博上透露骁龙845芯片加上17％的进口增值税后，成本为500多元，是骁龙660芯片的三倍多——这个价格已经是非常高昂了。
业内分析，如果其中增加一块单独的NPU模组，势必要将芯片架构打乱，而且还会产生额外的研发、专利以及物料成本。
其实成本上升还是小事，高通作为安卓阵营的龙头老大，骁龙845旗舰处理器要供应给上十家手机企业，这些手机企业众口难调，有些企业讲究性价比，并不一定愿意上NPU。有些企业愿意上，却还要派出人和高通一起驻场适配，企业不一定能抽掉人手。
**高通可能自己也清楚NPU是大方向，但面临不同企业的不同诉求，选择不上NPU还是最经济、最实惠、最便捷的方案。**
高通保守的原因，大概率还是因为在市场地位稳固，变革的动力相对不足。外加之前要应付博通的并购、反垄断调查导致利润承压，失去了对市场的敏感度。
不过，在今年年中，高通看到行业形势变化，还是迅速反应过来，对外透露了内建NPU模块处理器的计划，包含骁龙710的下一代720及在明年初的旗舰处理器8150上也都会加入NPU模块。
高通的加入虽然有被动因素，却可以看作是对独立神经网络处理单元的最佳认可，有助于把整体市场做大。
**联发科的求变**
在华为、苹果、高通、三星那里，独立神经网络处理单元被称作为NPU，联发科则是进一步改进独立AI硬件处理单元，并展开了跨平台的延展，并将其称之为APU。
![手机AI芯片的诸神之争](http://p1.pstatp.com/large/pgc-image/483cb648465345a2b675bd7c28236592)
联发科在今年3月发布了P60处理器。这款处理器因为低功耗、高性能、AI逆光拍照等特性，发布之后不久就被OPPO、vivo这类大厂选中，成为中高端机种上搭载的核心处理器。
相比于华为、苹果在AI处理单元上的尝试，P60直接加入了双核APU——也就是两个独立AI处理单元。
而上个月联发科刚推出的P70也一样搭载了双核APU，而市场传闻，联发科接下来下一颗处理器会有旗舰级的AI算力，是联发科重点投入的产品，瞄准的是高通旗舰市场。
联发科已经有1年多在中高端市场面临高通的竞争压力，但是这款产品可能会帮助联发科夺回过去的中高端市场份额。
**之所以叫APU而不叫NPU，其实已经显示出了联发科对独立AI芯片的思考角度。联发科的开创之处在于，它并非是零敲碎打推出AI处理器，它有一套自己的体系。**
不同于高通、三星以单核为主，而是可扩充，实现单核到多核的人工智慧处理单元。要知道，未来手机上人工智能的操作只会越来越多，单核心肯定不够用，双核甚至四核是迟早的问题，早点上双核，实际上是在早做准备。
多核心的APU同样也可以适应有更丰富的场景应用，不仅仅只是照片处理。未来手机的AI使用场景也必然是越来越丰富，面部识别、面部美化、场景检测、手势检测、系统性能优化、语音识别等场景下，都会要用到AI功能，多核心的APU可以抽掉出算力去应付不同场景不同应用，提高手机的运算速度。
APU还支持市场上现有的几乎所有AI架构，和手机配合在一起，会有更好的体验。包括Google的TensorFlow、Caffe、Amazon的MXNet、Sony的NNabla等。操作系统方面，联发科同时支持Android与Linux系统，这对于物联网智能化的推广，起到关键的影响。
因为现在越来越多的智能硬件，如汽车等都是采用了Linux系统，适应不同的AI架构，便于把APU推广到不同的运算平台上。
**Tick-Tock要打破**
这也牵扯到了最后一个问题——AI处理器未来究竟会有哪些运用场景。
**随着5G技术和AI技术的串联，未来智能手机、智慧家庭到自动驾驶，甚至AR、VR会成需要一揽子的解决方案，独立神经网络处理单元的跨平台需求会愈加强烈。**
中国AI芯片“小三巨头”的说法——地平线、寒武纪和深鉴科技三者都是这个市场的开拓者。
![手机AI芯片的诸神之争](http://p3.pstatp.com/large/pgc-image/33ef90a153184509b520c16bf2e5971b)
寒武纪垂直于手机业务、深鉴科技主要聚焦于安防，地平线则是围绕智能零售、智慧城市、自动驾驶三个领域提交解决方案。
三小巨头的出现，恰恰说明了处理器市场寻找跨平台横向拓展的市场空间。
APU恰恰主打跨平台， 今年年初，联发科在CES发布了NeuroPilot人工智能平台，主攻智能手机、智能家庭、自驾车的终端边缘运算。联发科不仅仅整合AI处理器与NeuroPilot SDK软件开发套件技术，将AI带入广泛的消费性产品中。也让手机处理器的AI单元和其他消费电子产品相连。提前布局APU，其实也是在为未来计算平台出现、融合做考虑。
考虑这个问题的不只是联发科，还有三星。
韩媒ETNews在今年年初报道，三星电子LSI事业部正在开发名为“Exynos Auto”的汽车芯片，计划在今年年底前量产，并将供应给奥迪、哈曼等公司，后者已成为三星子公司。
三星Exynos Auto将成为首款内置NPU模块的三星芯片。NPU在汽车上可用于分析车内图像传感器接收到的图像信号。因此，它能够帮助构建更高效的ADAS（高级驾驶员辅助系统），ADAS可用来识别车道和障碍物。
多年以前，英特尔半导体开创了“Tick-Tock”钟摆定律。“工艺年-构架年”统治了十余年，每一年处理器也只有感知力极小的线形增长。
**伴随着“日拱一卒”演进，AI芯片终于到了要突破“Tick-Tock”这个铁律的阶段。**

