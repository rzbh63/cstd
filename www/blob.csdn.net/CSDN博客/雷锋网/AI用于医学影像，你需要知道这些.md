
# AI用于医学影像，你需要知道这些 - 雷锋网 - CSDN博客


2016年12月02日 17:57:58[leiphone](https://me.csdn.net/leiphone)阅读数：3057


导语：本期硬创公开课，雷锋网请到其中一家公司DeepCare的创始人兼CTO丁鹏博士，为我们讲讲人工智能应用于医学影像的那些事儿。

![AI用于医学影像，你需要知道这些 | 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/58400a5d009fc.jpg?imageMogr2/format/jpg/quality/90)
图像识别技术已经可以认出人脸，也能用于自动驾驶中，那它用于医学影像，识别病变中又如何呢？在《国内人工智能+医学影像公司大盘点》一文中，雷锋网(公众号：雷锋网)介绍了国内将深度学习等人工智能技术用于医学影像的众多公司，它可以削减读片时间，降低误诊的概率，推荐治疗方案，协助医生诊断。
本期硬创公开课，雷锋网请到其中一家公司DeepCare的创始人兼CTO丁鹏博士，为我们讲讲人工智能应用于医学影像的那些事儿。
### 嘉宾简介：
丁鹏，DeepCare创始人兼CTO，博士毕业于美国达特茅斯学院(Dartmouth College)，从事算法研发工作8年；在计算机视觉和深度学习技术方面有长时间的科研经历，且有工业级算法开发经验；对人工智能技术产业化有深刻的见解，在如何将人工智能技术与医学影像结合，开发商业产品方面，也是国内首屈一指的专家。
![AI用于医学影像，你需要知道这些 | 硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/584009c558260.jpg?imageMogr2/format/jpg/quality/90)
*以下内容来自丁鹏在雷锋网硬创公开课的分享：*
**雷锋网：可以简单介绍一下DeepCare所关注的领域吗？**
我们主要的方向是医学影像。医学影像是一个很大的方向，从细胞病理学到组织病理学到影像科，都属于这一范围。我们经过调研，确定做的是病理方向。原因是，病理医生是“医生中的医生”，地位非常重要，是确定有没有重大疾病（如癌症）的最后一道防线，而国内的病理医生又是奇缺的，存在很大的供需矛盾。
我们的定位是，为病理医生提供服务，提升他们的工作效率，帮助他们看片。一个方面是“查”，一个方面是“判”。查主要是指针对病理科的搜索、数据管理系统等，如以图搜图。二是判，指帮助病理医生看片子。目前主要是做乳腺和肺癌。
**雷锋网：总体上看，把计算机视觉等人工智能技术用在医疗上，在国内目前的发展，处于什么样的阶段？**
对于这个问题，要先看看深度学习等新技术，用在传统行业上（比如医生看病）的意义有哪些。无论在发达国家还是发展中国家，优质医疗资源的供需始终是一对矛盾，因为巨大的人口压力，这个问题在中国尤为突出。这也是为什么在协和医院门口是看到那么多外地人到北京看病，又挂不上专家号只能买黄牛票。
另一方面来说，医学影像是医生看病，及决策的最大的信息入口。而医生理解这些影像的本质，即是医生大脑经过长期的过程被大量数据训练出来的结果。一个客观事实是，高中低年资的医生在看同一张片子时候，得出的结论会有天壤之别，这也是为什么我们要去协和找“老大夫”看病的事实依据。
这是因为看病的效果和从医的年限高度相关，而从医的时间长短不同意味着所接受的信息量不同。那么如果一台机器学习了比人类医生多得多的案例或者图片，会不会超越人类呢？答案一定是肯定得。这也是应用深度学习技术训练计算机“看懂”医学影像的意义所在。
对于发展状况，我看来，无论在中国还是美国，在算法和数据上看，这一领域的发展都是在早期。它已经走出了实验室，但离商业化还有很长的路要走。
算法开发上，通用物体检测识别的算法又一个巨大的人群在做，但是在医学影像领域由于数据的可获得性差很多，所以研究的人和前者相比并不是特别多。但是越来越多的学者和研究机构看好这个方向，开始加大投入进来。
高质量的标注数据方面，在这个方面中国与美国相比很有可能会“弯道超车”，因为我们都知道在美国拿医疗数据是个非常困难的事情，无论是科研还是商用；但是在中国因为体制的原因和政府对于技术创新的重视，会让这个问题变得更容易一些。
**雷锋网：根据你们的经验将人工智能用于医疗，会面临哪些主要困难和阻碍？**
人工智能或者说深度学习相对来说是一个新技术，一个新的技术与老的行业相结合的时候往往会诞生出新的方向。目前的人工智能技术无论与哪一个行业结合，都必须聚焦到一个“场景”上来，比如说“医生读片”就是一个场景。在这个场景上应用新技术，特别是对于二三线城市中受训练较少的医生，不仅会提高医生判定的准确性，更会提高医生的工作效率，换句话说，释放了医生的生产力。
要完成这个事情，需要有两个必要条件。第一，适应于具体问题的算法；第二，高质量的标注数据。算法开发是一个难点，但相对比较容易搞定。高质量的小批量标注数据我们也已经获得，但是如何进行大批量数据的标注，以及标注质量控制确实是一个比较困难的问题。这涉及到最核心的资源，即高质量医生的时间与效率。除了时间，更重要的是如何做质量管控，比如同样一张图，标准及答案是什么，谁定义答案，由几个医生来标注，标注的是几个阶段，是盲标还是非盲标。这些都是要考虑的。
这个问题业界目前还没有给出一个清晰标准的答案，正在积极探索。
**雷锋网：在技术选择上，医学影像与其它图像识别是否会存在差异？**
这是必然的。自然世界里的图像与人体内的图像有着天然的巨大差异，因此针对于不同问题我们使用的算法也是不一样的，绝对不可能一刀切。我之前做过一种医学影像，它的病变的位置（做的是分割问题）有从十几个像素到几千个像素，为了检测小的病变点，如果downsampling次数太多，可能无法deconvelotion回来。
**雷锋网：主要会用到哪些技术和算法呢？**
在CV上有三个最基本问题：分类，检测和分割。我们处理的问题中也用的这三种方法，在病理上主要是做细胞层面上的，比如分类，细胞的分割，在细胞病理学上的检测工作等。
**雷锋网：深度学习等技术是否适合所有类别的医学影像，是否有些更为合适？**
将深度学习技术应用在计算机图像上的时候，有一个衡量问题难易程度的“标准”，即这个问题（图像）让人来理解的话是否可以做到。从我的经验来看，一个人普通人（比如我自己）经过简单的训练（比如一个小时左右）之后就可以在化验血液的图片中找到红细胞、白细胞等等；但是我需要经过很多次高强度的训练之后，才有可能在肺部组织病理切片中看出，哪里是癌细胞以及它们对应的亚型是什么。
从这个角度来看，我们做的理解组织病理切片的问题，应该是属于医学影像中非常难的问题之一。对医生难，因此对机器也不容易，但正是它的困难才带来了无与伦比的价值，所以病理科医生在被称为“医生中的医生”。病理科相当于，如果判断一个人是否有癌症，影像科所得的影像都不能据此下最后的结论，而病理医生可以下结论。
有几个数字，国内病理科医生在册的有1万多人，但经过我们的访谈，认为国内需要大约6、7万的人才能满足需求。但病理医生的培养周期特别长，北京某著名医生的病理科主任曾跟我说过，他们医院45岁以下的病理科医生会诊时没有发言权。因为细胞本身的差异非常大，医生也就要经过大量的训练才行。
由于体制的原因，病理科医生不直接面对患者，收入水平较低。几家特甲医院也需要花大价钱寻找病理医生。这种供需矛盾也带来了巨大的机会，这不仅是商业上的，也是有社会意义的。
**雷锋网：数据对人工智能来说很重要，那医学影像的数据来源及获取渠道有哪些？**
数据的来源的主体还是医院，不过目前越来越多的第三方病理中心也是我们获取数据的渠道之一。
**雷锋网：数据建设是一个难点吗？标注上是否会有更多困难？**
数据标注甚至比数据获取更重要。之前我们做过一个非常有意义的研究，找来了40张乳腺癌的病理切片，分别让中国的高年资和中低年资的医生标注，并和美国医生标注的“真值”做比较，结果是我们看到了恐怖的差异：在这40张片子上，高级医生和普通医生的水平差距在30个百分点。这个实验充分说明了找到领域内最厉害的医生进行标注是多么的至关重要。
**雷锋网：图像识别是否可以同时识别出多个部位的疾病？这里的难点又在哪？**
医学影像是医生看病一个最大的信息入口。一张病理切片大概是1-3个G不等，比一个高清电影还大，其中蕴含的信息也是海量的。因此可以说，从一种重大疾病（比如癌症）的筛查，到随诊，再到最后的判定，医学影像都在其中起到最重要的作用。
对于识别多个部位的疾病，现在是可以查出多种疾病来的，同时，针对某一种病做到知名医生的水平仍旧不容易。看病是一个过程，目前来看，算法的开发还是针对某一种器官，某一种病。如上所述，整个行业是在早期阶段，目前的做法是个个击破，比如肺部的病变就有很多种，现在的做法是分开进行。合并也是有方法的，比如有检测多种病的算法，挨个都试试，看效果如何。
但目前来说，能做好一种病的分析就很不容易。比如肺小结节在CT中的检测，8mm以上的结节已经有团队可以做到99%的检测水平，但5mm左右的检测仍非常难。所以，在评测这些算法时，也要有一个公正的方法。
**雷锋网：医学影像与病例病史等资料的整合，从而做出综合的智能分析，现在处于怎么样的阶段了？**
在我看来，病例方面关于文本的处理是走在影像的前面。目前我了解，这两个方面仍在独立进行，都不容易做好。回到数据源上来，在病例本身上，不同医院，甚至一家医院不同科室的医生，写病例的方式也不一样，它不是一个结构化的数据来源。
医生写病例时，是一种总结的方式，而不是一种连续的方式。比如，医生与病人交流时，它像一个分类器一样，问一个问题，然后得到病人的回答，然后再往下问，如果能抽象出来，就像一个树状分类器。但病例只写了一个分类器的结果，其中判断的过程并没有在病例中体现出来。
如果能让病例信息更全面，更结构化，这种信息才能真正利用起来。这也是我们选影像切入的原因，因为影像是更结构化的信息，以病理科为例，95%的判断仍是影像中细胞本身的判断。总体来说，影像与病例这种多模态的信息的融合是一个方向，但两者都是在早期阶段。
**雷锋网：图像识别类公司一般有提供API，或自己做硬件，以及做整套方案等区分，在医学影像上，会更适合哪一类吗？你们的选择是怎么样的？**
这是一个产品形态问题。我觉得无论是云端产品，还是前端硬件的方式都是可以的，主要看客户（比如医院）的需求，以及自身的网络限制，需要具体问题具体分析。
第三方影像中心有很多，他们的做法是将数据连到云端。但病理可能是另外一种情况，一张图可能是1-3GB，这样会受到医院带宽的限制，这种情况下可能是一种前端产品。以血液检测科为例，有国外的机器自身带有智能分析算法，做到了前端，最后需要的是医生确认。
商业路径选择要根据不同影像的特点来选择产品形态。
雷锋网原创文章，未经授权禁止转载。详情见转载须知。



