
# SSD论文解读 - intflojx的博客 - CSDN博客


2019年01月04日 09:59:36[paulpanwang](https://me.csdn.net/intflojx)阅读数：868


# 1
# 论文简述
|题目
|《|SSD: Single Shot MultiBox Detector|》
|
|作者
|Wei Liu, Dragomir Anguelov, Dumitru Erhan ,
|Christian Szegedy,Scott Reed , Cheng-Yang Fu ,
|Alexander C. Berg
|
|联系邮箱
|[wliu@cs.unc.edu](mailto:wliu@cs.unc.edu)|[drago@zoox.com](mailto:drago@zoox.com)
|fdumitru,szegedyg@google.com
|[reedscot@umich.edu](mailto:reedscot@umich.edu)
|fcyfu,abergg@cs.unc.edu
|
|出处
|European Conference on Computer Vision (ECCV), 2016 (In press)
|
|发表时间
|April 2016
|
|被引量
|18
|
|代码链接
|https://github.com/weiliu89/caffe/tree/ssd
|

# 2
# 摘要
Wei Liu等人提出了一种使用单个深层神经网络检测图像中目标的方法，名为SSD，将边界框的输出空间离散化为一组默认框，该默认框在每个特征图位置有不同的宽高比和尺寸。在预测期间，网络针对每个默认框中的每个存在目标类别生成分数，并且对默认框进行一些调整以更好地匹配对象形状。另外，网络组合了来自不同分辨率的多个特征图的预测，以适应处理各种尺寸的对象。Wei Liu等人的SSD模型相对于需要region proposal的方法是简单的，因为它完全消除了proposal生成和后续的像素或特征重采样阶段，并将所有计算封装在一个网络之中。这使得SSD容易训练和直接集成到需要检测组件的系统。在PASCAL VOC，MS COCO和ILSVRC数据集的实验结果证实，SSD与使用另外的region proposal的方法具有相当的准确性，并且速度更快，同时为训练和参考提供统一的框架。与其他单级方法相比，即使输入图像尺寸更小，SSD仍然具有更好的精度。对VOC2007，在300×300输入，SSD在Nvidia Titan X上58FPS时达到72.1％的mAP，500×500输入SSD达到75.1％的mAP，优于类似的现有技术Faster R-CNN模型。
**关键词**：实时目标检测 ；卷积神经网络
# 3
# 引言
现有目标检测系统都是基于以下方法：假设边界框，对每个框重新取样像素或特征，再应用高质量的分类器进行分类。选择性搜索方法之后，Faster R-CNN在PASCAL VOC，MS COCO和ILSVRC检测取得领先结果，它的流程成为了检测领域的一个里程碑，它能提取更深的特征。尽管它准确度高，但这些方法对于嵌入式系统来说计算量过大，即使对于高端硬件，对于实时或接近实时的应用来说也太慢。这些方法的检测速度通常以每秒帧数为单位进行测量，高精度检测器(基于Faster R-CNN）最快仅以每秒7帧（FPS）运行。目前，已有很多的尝试：如通过研究检测流程的每个阶段来建立更快的检测器，但是迄今为止，显著增加的速度仅仅是靠大幅度降低检测精度为代价。
Wei Liu等人提出了第一个基于深层网络的目标检测器，它不会对边界框假设的像素或特征进行重新取样，但可以和这种做法一样准确识别目标。这使高精度检测速度有显著提高（在VOC2007测试中, 58 FPS下 72.1％ mAP，对Faster R-CNN 7 FPS 下mAP 73.2％，YOLO 45 FPS 下mAP 63.4％）。用消除边界框proposal和随后的像素或特征重采样阶段来提高速度。通过增加一系列改进提高了它的准确性。Wei Liu等人的改进包括使用不同宽高比检测的单独的预测器（滤波器），预测边界框中的对象类别和偏移，并且将这些滤波器应用于网络后期的多个特征图，以便进行多尺度检测。通过这些修改，Wei Liu等人可以使用相对低分辨率的输入实现高精度检测，进一步提高处理速度，所得系统提高了PASCAL VOC的高速检测的准确性，从YOLO的63.4％mAP提高到72.1％mAP。相比近期工作，这是在检测精度上和残差网络上的较大提高。此外，有效提高高质量检测的速度可以拓宽计算机视觉的应用范围。
# 4
# 贡献点
Wei Liu等人介绍了SSD，一个单次检测器——可用于多个类别检测，比先前技术的单次检测器（YOLO）速度更快，并且准确度更高。实际上它和使用region proposal、pooling的更慢技术一样准确（包括Faster RCNN）。
SSD方法的核心是使用小卷积滤波器来预测特征图上固定的一组默认边界框的类别分数和位置偏移。
为了实现高检测精度，从不同尺度的特征图进行不同尺度的预测，并且通过宽高比来明确地分离预测。
这些设计特性得到了简单的“端到端”训练和高精度，进一步提高速度和精度的权衡，即使输入的是低分辨率图像也是如此。
实验包括在PASCAL VOC，MS COCO和ILSVRC上评估不同输入图像大小下模型耗时和精度分析，并与一系列最新的先进方法进行比较。
# 5
# 模型及方法
SSD检测的框架如图1所示，如图1(a)，SSD在训练期间仅需要每个对象的输入图像和真实标签框。卷积处理时，在具有不同尺度（例如图1(b)和图1(c)中的8×8和4×4）的若干特征图中的每个位置处评估不同宽高比的小集合默认框。对于每个默认框，预测对所有对象类别((c1，c2，...，cp))的形状偏移和置信度。在训练时，首先对这些默认框匹配得到标签框。例如，两个默认框匹配到猫和狗，这些框为正，其余视为负。模型损失是位置损失）和置信损失之间的加权和。
![](https://img-blog.csdnimg.cn/20190104094515108.png)
|(a)带标签图像
|(b)4×4特征图
|(c)8×8特征图
|
图1     SSD架构图
## 5.1
## 方法概述
SSD方法基于前馈卷积网络，其产生固定大小的边界框集合和框中目标类别的对应分数，接着按照非最大化抑制步骤产生最终的检测结果。早期网络都是基于高质量的图像分类的标准架构，Wei Liu等人使用了VGG-16网络作为基础，然后向网络添加辅助结构，产生了具有以下主要特征的检测：
**多尺度特征图检测**：卷积特征层尺寸逐渐减小，得到多个尺度检测的预测值，并添加到截断的基础网络末尾。
**检测的卷积预测器**：如图2所示，每个添加的特征层可以使用一组卷积滤波器产生固定的预测集。对于具有p个通道的大小为m×n的特征层，使用3×3×p卷积核卷积操作，产生类别的分数或相对于默认框的坐标偏移。在每个应用卷积核运算的m×n大小位置处，产生一个输出值。边界框偏移输出值是相对于默认框测量，默认框位置则相对于特征图。
**默认框与宽高比**：将一组默认边界框与顶层网络每个特征图单元关联。具体来说，对于在给定位置的k个框中每个框计算c类分数和相对于原始默认框的4个偏移量，这使得在特征图中的每个位置需要总共（c+4）k个滤波器，对于m×n特征图产生（c+4）kmn个输出。Wei Liu等人的方法在多个特征图中使用不同的默认框形状，可以有效地离散可能的输出框形状空间。
![](https://img-blog.csdnimg.cn/2019010409463867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图2   两个单次检测模型之间的比较
## 5.2
## 训练方法
SSD的真实标签信息需要被指定到固定的检测器输出集合中的某一特定输出，一旦确定了该指定，则端对端地应用损失函数和反向传播。训练还涉及选择用于检测的默认框和尺度集合，以及hard negative mining和数据增广策略。
**匹配策略**：从默认框中选择真实标签框，这些默认框随位置、纵横比和比例而变化。启始时，匹配每个真实标签框与默认框最好重叠，以确保每个真实标签框有一个匹配的默认框。添加这些匹配使得有多个重叠默认框时网络预测获得高置信度。
**训练**：以*x**ij**p*=1 表示第i个默认框与类别p的第j个真实标签框相匹配，相反的*x**ij**p*=0 。由∑*i**x**ij**p*≥ 1，意味着可以有多于一个与第j个真实标签框相匹配的默认框。总体目标损失函数是位置损失（loc）和置信损失（conf）的加权和：
![](https://img-blog.csdnimg.cn/20190104094604999.png)
|（1）
|
其中N是匹配的默认框的数量，位置损失是预测框和真实标签值框参数之间的平滑L1损失。
**选择默认框的比例和纵横比**：Wei Liu等人使用低层和高层的特征图进行检测预测。图1示出了在框架中使用的两个示例特征图（8×8和4×4）。
在SSD框架内，默认框不需要对应于每层的实际receptive field sizes。假设使用m个特征图做预测，每个特征图的默认框的比例计算如下：

![](https://img-blog.csdnimg.cn/20190104094704477.png)
|（2）
|
其中smin是0.2，smax是0.95，意味着最低层具有0.2的刻度，最高层具有0.95的刻度，并且其间的所有层是规则间隔的。对默认框施以不同的宽高比，表示为ar∈{1,2,3,1/2,1/3}，因此可以计算每个默认框的宽度和高度。对于宽高比为1，添加一个缩放为![](https://img-blog.csdnimg.cn/20190104094744749.png)的默认框。设定每个默认框中心为![](https://img-blog.csdnimg.cn/20190104094801514.png)，其中|*f**k*| 是第k个正方形特征图的大小,![](https://img-blog.csdnimg.cn/20190104094817308.png)。
**Hard negative mining**：在匹配之后，使用每个默认框的最高置信度对它们进行排序，使得正负样本之间的比率最多为3：1，以代替使用所有的负样本。
**数据增广**：为了使模型对于各种输入对象具有更好的鲁棒性，每个训练图像通过以下三种方法之一随机采样：1）使用整个原始输入图像；2）采样一个片段，使对象最小的jaccard重叠为0.1,0.3,0.5,0.7或0.9；3）随机采样一个片段。
每个采样片段的大小为原始图像大小的[0.1,1]，横宽比在1/2和2之间。如果真实标签框中心在采样片段内，则保留重叠部分。在上述采样步骤之后，将每个采样片大小调整为固定大小，并以0.5的概率水平翻转。
## 5.3
## 原文实验结果
Wei Liu等人的实验基于VGG16网络，在ILSVRC CLS-LOC数据集预训练。Wei Liu等人将fc6和fc7转换为卷积层，从fc6和fc7两层采样得到参数，将pool5从2×2-s2更改为3×3-s1，并使用atrous算法填“洞”。并删除了所有的dropout层和fc8层，使用SGD对这个模型进行fine-tune，初始学习率*10**-3*，0.9 momentum, 0.0005 weight decay, batch大小32。
### 5.3.1 PASCAL VOC2007
在这个数据集上，Wei Liu等人比较了Fast R-CNN和Faster R-CNN。在VOC2007train val和VOC2012train val（16551images）上训练，在VOC2007（4952图像）测试。
Wei Liu等人使用conv4_3，conv7（fc7），conv8_2，conv9_2，conv10_2和pool11来预测位置和置信度（对SSD500模型，额外增加了conv11_2用于预测），用“xavier”方法初始化所有新添加的卷积层的参数。由于conv4_3的大小较大（38×38），因此只在其上放置3个默认框 :一个0.1比例的框和另外纵横比为1/2和2的框。对于所有其他层，我们设置6个默认框。由于conv4_3与其他层相比具有不同的特征尺度，因此引入L2正则化技术，将特征图中每个位置处的特征范数缩放为20，并在反向传播期间学习比例。使用10-3 学习速率进行SSD300模型已经比Fast R-CNN更准确。当以更大的500×500输入图像训练SSD，结果更准确，甚至惊人的超过了Faster R-CNN 1.9% mAP。
表1  PASCAL VOC2007测试集检测结果
![](https://img-blog.csdnimg.cn/20190104094848784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
Fast和Faster R-CNN输入图像最小尺寸为600，两个SSD模型除了输入图像尺寸（300*300和500*500），其他设置与其相同。很明显，较大的输入尺寸得到更好的结果。
图4显示SSD对边界框尺寸非常敏感，它对较小的对象比较大的对象具有更差的性能，在大对象上表现很好。因为对每个特征图位置使用各种长宽比的默认框，因此对于不同的对象宽高比鲁棒性较好。
### 5.3.2
### 模型分析
为了更好地理解SSD，Wei Liu等人进行了人为控制的实验，以检查每个组件如何影响最终性能。
表2 不同选择和组件对SSD表现的影响
![](https://img-blog.csdnimg.cn/2019010409484981.png)
关键的数据增广Fast和Faster R-CNN使用原始图像和水平翻转（0.5概率）图像训练。Wei Liu等人使用更广泛的采样策略，类似于YOLO，但它使用了光度失真。表2显示，用这个抽样策略提高6.7％的mAP。
![](https://img-blog.csdnimg.cn/20190104094900767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图3 VOC2007测试集上SSD 500对动物、车辆和家具性能的可视化
第一行显示由于定位不良（Loc），与类似类别（Sim）、其他类别（Oth）或背景（BG）混淆的正确检测（Cor）、假阳性检测的累积分数。红色实线反映了随着检测次数的增加，“强”标准（0.5jaccard重叠）的召回率变化。红色虚线使用“弱”标准（0.1 jaccard重叠）底行显示排名靠前的假阳性类型的分布。
![](https://img-blog.csdnimg.cn/20190104094913670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图4  灵敏度和不同物体特性对VOC2007测试集的影响
更多特征图的提升 ：受许多语义分割工作启发，Wei Liu等人也使用底层特征图来预测边界框输出，比较使用conv4_3预测的模型和没有它的模型。从表2，可以看出，通过添加conv4_3进行预测，它有明显更好的结果（72.1％ vs 68.1％），conv4_3可以捕获对象更好的细粒度，特别是细小的细节。
更多的默认框形状效果更好：默认情况下，每个位置使用6个默认框。如果删除具有1/3和3宽高比的框，性能下降0.9％。通过进一步移除1/2和2纵横比的框，性能再下降2％。使用多种默认框形状似乎使网络预测任务更容易。
Atrous算法更好更快 ：使用VGG16的atrous版本遵循DeepLabLargeFOV。如果使用完整的VGG16，保持pool5与2×2-s2，并且不从fc6和fc7的采集参数，添加conv5_3，结果稍差（0.7％），而速度减慢大约50％。
### 5.3.3 PASCAL VOC2012
采用和VOC2007上一样的设置，这次，用VOC2012的训练验证集和VOC2007的训练验证集、测试集（21503张图像）训练，在VOC2012测试集（10991张图像）测试。由于有了更多的训练数据，模型训练时以10-3 学习率进行60K次迭代，再减小到10-4 继续迭代20K次。
表 3  PASCAL VOC2012测试检测结果
![](https://img-blog.csdnimg.cn/2019010409494248.png)
表3显示了SSD300和SSD500模型的结果。可以看到与VOC2007测试中观察到的相同的性能趋势。SSD300已经优于Fast R-CNN，并且非常接近Faster R-CNN（只有0.1％的差异）。可能是由于使用来自多个特征图的卷积默认框和训练期间的匹配策略，与YOLO相比，SSD显著更好。
### 5.3.4  MS COCO
为了进一步验证SSD架构，Wei Liu等人MS COCO数据集上训练了SSD300和SSD500模型。由于COCO中的对象往往较小，因此对所有图层使用较小的默认框，最小的默认框具有0.1缩放比例，并且conv4_3上默认框的缩放比例是0.07（例如，对应于300×300图像的21个像素）。
表4  COCO test-dev2015检测结果
![](https://img-blog.csdnimg.cn/20190104095000689.png)
Wei Liu等人使用trainval35k来训练模型。首先用8×10-4 的学习率迭代4K次训练模型，接着以10-3 学习率进行140K次迭代，再以10-5 学习率迭代60K次，10-5 学习率迭代40K次。表4显示了test-dev2015上的结果。SSD300在mAP@0.5和mAP@[0.5：0.95]中优于Fast R-CNN，在mAP @ [0.5：0.95]与Faster R-CNN接近。通过将图像大小增加到500×500， SSD500在两个标准中都优于Faster R-CNN。此外， SSD500模型也比ION更好，它是一个多尺寸版本的Fast R-CNN，使用循环网络显式模拟上下文。在图5中展示了使用SSD500模型在MS COCO test-dev的一些检测示例。
### 5.3.5 ILSVRC
### 初步结果
在ILSVRC DET数据集使用与MS COCO相同的网络架构，并使用ILSVRC2014 DET train和val1来训练SSD300模型。Wei Liu等人首先以8×10-4 的学习率迭代4K次训练模型，再用10-3 学习率进行320k次迭代训练该模型，然后用10-4 进行100k次迭代和10-5 继续训练60k次迭代，可以在val2集上实现41.1mAP。
![](https://img-blog.csdnimg.cn/2019010409501734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图5  采用SSD512型号的COCO test-dev的检测实例
# 6
# 相关工作
目前有两种已建立的用于图像中对象检测的方法，一种基于滑动窗口，另一种基于region proposal分类。在卷积神经网络出现之前，用于检测的两种方法Deformable Part Model（DPM）和选择性搜索性能接近。然而，在R-CNN带来的显着改进之后，其结合了选择性搜索region proposal和基于卷积网络的后分类，region proposal对象检测方法变得普遍。
原始的R-CNN方法已经以各种方式进行了改进。第一组方法提高了后分类的质量和速度，因为它需要对成千上万的图像作物进行分类，这是昂贵和耗时的。SPPnet对原始的R-CNN方法大大提速。它引入了空间金字塔池化层，其对区域大小和尺度更加鲁棒，并且允许分类层重用在若干图像分辨率生成的特征图特征。Fast R-CNN扩展了SPPnet，使得它可以通过最小化置信度和边界框回归的损失来对所有层进行端对端微调，这在MultiBox中首次引入用于学习对象。
第二组方法使用深层神经网络提高proposal生成的质量。在最近的工作中，例如MultiBox，基于低层图像特征的选择性搜索region proposal被直接从单独的深层神经网络生成的proposal所替代。这进一步提高了检测精度，但导致了一些复杂的设置，需要训练两个神经网络及其之间的依赖。Faster R-CNN通过从region proposal网络（RPN）中学习的方案替换了选择性搜索proposal，并且引入了通过微调共享卷积层和两个网络的预测层之间交替来集成RPN与Fast R-CNN的方法。用这种方式region proposal池化中层特征图，最终分类步骤更快速。我们的SSD与Faster R-CNN中的region proposal网络（RPN）非常相似，因为我们还使用固定的（默认）框来进行预测，类似于RPN中的achor框。但是，不是使用这些来池化特征和评估另一个分类器，我们同时在每个框中为每个对象类别产生一个分数。因此，我们的方法避免了将RPN与Fast R-CNN合并的复杂性，并且更容易训练，更易于集成到其他任务中。
另一组方法与我们的方法直接相关，完全跳过proposal步骤，直接预测多个类别的边界框和置信度。 OverFeat是滑动窗口方法的深度版本，在知道基础对象类别的置信度之后直接从最顶层特征图的每个位置预测边界框。YOLO 使用整个最高层特征图来预测多个类别和边界框（这些类别共享）的置信度。我们的SSD方法属于此类别，因为我们没有提案步骤，但使用默认框。然而，我们的方法比现有方法更灵活，因为我们可以在不同尺度的多个特征图中的每个特征位置上使用不同宽高比的默认框。如果顶层特征图的每个位置只使用一个默认框，我们的SSD将具有与OverFeat类似的架构;如果我们使用整个顶层特征图并且添加一个全连接层用于预测而不是我们的卷积预测器，并且没有明确考虑多个宽高比，我们可以近似地再现YOLO。
# 7
# 结果重现
首先在作者给的三个模型文件中，我们运行得到三个精度依次提高的结果（SSDSSD代码和模型常在更新，如果有同学遇到“shape mismatch”这种错误，从 github下载最新的模型即可）。由于工作环境搭建在笔记本电脑上，并没有明显感觉出速度的差别，不过均能在1-2s得到单张图片结果，效果还是比较可观的，相比于fast-CNN的几十秒一张性能要好。
|(a)多目标检测结果1
|(b)多目标检测结果2
|(c)多目标检测结果3
|
第一个结果基本上框出了所有显著性目标，但是在人那边重现了重叠。第二个结果对于目标解决了重复检测的问题，但是对于小物体的检测还不是很好。第三个结果则可以兼顾小目标和显著性目标，准确率较高。由于作者的原图已经标注过，我们对现有图像进行检测与识别，得到结果如下图所示。均取得较好的结果。
![](https://img-blog.csdnimg.cn/20190104095707464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190104095733445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
我们设想能识别人脸，在知道人脸身份的时候输出身份，在不知道人脸身份的时候输出性别和年龄，为此我们也在做了一些努力，利用《Age and Gender Classification using Convolutional Neural Networks》的模型文件得到下面的结果，效果还是比较令人满意的。
![](https://img-blog.csdnimg.cn/20190104095801273.png)
|(a)原始图像
![](https://img-blog.csdnimg.cn/20190104095806757.png)
|(b)卷积层可视化
![](https://img-blog.csdnimg.cn/20190104095810327.png)
|(c)隐藏层可视化
|
|最后输出结果：|Female  Age:[0,2]
|
![](https://img-blog.csdnimg.cn/20190104095822999.png)
|(a)|原始图像
![](https://img-blog.csdnimg.cn/20190104095831161.png)
|(b)|卷积层可视化
![](https://img-blog.csdnimg.cn/20190104095838640.png)
|(c)|隐藏层可视化
|
|最后输出结果：|Male  Age:[25,32]
|
# 8
# 个人总结
SSD在多尺度下面计算特征，然后评估在该尺度下的区域存在的概率以及相关偏置，是一种遍历所有区域的方式。SSD获得的是图像中的矩形区域以及该区域所对应的类型以及类型得分。
训练整体流程：
Step1:图像经过卷积网络，得到图像的base特征A
Step2:在对这个特征进行多层级的提取feature map B
Step3:在每个feature map中各个位置location，每个location对应多个default box
Step4:计算每个default box的loc offset 以及 class score
Step5: 根据default box以及loc offset 计算区域位置P，再根据class score，计算每个default box的损失函数，累加得到最终的损失函数。
论文中指出由于negative 的矩形区域明显多于positive的矩形区域，所以根据计算的confidence 排序获得前N个矩形区域求损失函数，negative 与positive的比例大致是3:1。论文的优势使用图像在各个scale下各个位置的特征进行回归，既保证了速度，也保证了准确度。可能可以进一步提高的方面：其在各个位置中用到的特征仅仅只是该尺度下的特征，没有结合上层或者下层特征。
Wei Liu等人提出的SSD用于多个类别的快速单次对象检测器。该模型的一个关键特点是使用多尺度卷积边界框输出附加到网络顶部的多个特征图。这种表示能够有效地模拟可能的框形状空间。经过实验验证，给定适当的训练策略，更大量的仔细选择的默认边界框得到了性能的提高。给定相同的VGG-16基础架构，SSD在精度和速度方面胜过最先进的对象检测器。该模型的SSD500型号在PASCAL VOC和MS COCO的精度方面明显优于最先进的Faster R-CNN，速度快了3倍。且实时SSD300模型运行在58 FPS，这比当前的实时YOLO更快，同时有显著高质量的检测。


