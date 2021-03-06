
# 带你深入AI（4）- 目标检测领域：R-CNN，faster R-CNN，yolo，SSD, yoloV2 - 谢杨易的博客 - CSDN博客

2018年04月15日 11:09:29[谢杨易](https://me.csdn.net/u013510838)阅读数：2802


系列文章，请多关注
[Tensorflow源码解析1 – 内核架构和源码结构](https://blog.csdn.net/u013510838/article/details/84103503)
[自然语言处理1 – 分词](https://blog.csdn.net/u013510838/article/details/81673016)
[带你深入AI（1） - 深度学习模型训练痛点及解决方法](https://blog.csdn.net/u013510838/article/details/79835563)
[带你深入AI（2）- 深度学习激活函数，准确率，优化方法等总结](https://blog.csdn.net/u013510838/article/details/79845455)
[带你深入AI（3）- 物体分类领域：AlexNet VGG Inception ResNet mobileNet](https://blog.csdn.net/u013510838/article/details/79866740)
[带你深入AI（4）- 目标检测领域：R-CNN，faster R-CNN，yolo，SSD, yoloV2](https://blog.csdn.net/u013510838/article/details/79947553)
[带你深入AI（5）- 自然语言处理领域：RNN LSTM GRU](https://blog.csdn.net/u013510838/article/details/80024144)
[带你深入AI（6）- 详解bazel](https://blog.csdn.net/u013510838/article/details/80102438)
[带你深入AI（7）- 深度学习重要Python库](https://blog.csdn.net/u013510838/article/details/80412127)
## 1 引言
深度学习目前已经应用到了各个领域，应用场景大体分为三类：物体识别，目标检测，自然语言处理。上文我们对物体识别领域的技术方案，也就是CNN进行了详细的分析，对LeNet-5 AlexNet VGG Inception ResNet MobileNet等各种优秀的模型框架有了深入理解。本文着重与分析目标检测领域的深度学习方法，对其中的经典模型框架进行深入分析。
目标检测可以理解为是物体识别和物体定位的综合，不仅仅要识别出物体属于哪个分类，更重要的是得到物体在图片中的具体位置。
为了完成这两个任务，目标检测模型分为两类。一类是two-stage，将物体识别和物体定位分为两个步骤，分别完成，这一类的典型代表是R-CNN, fast R-CNN, faster-RCNN家族。他们识别错误率低，漏识别率也较低，但速度较慢，不能满足实时检测场景。为了解决这一问题，另一类方式出现了，称为one-stage, 典型代表是Yolo, SSD, YoloV2等。他们识别速度很快，可以达到实时性要求，而且准确率也基本能达到faster R-CNN的水平。下面针对这几种模型进行详细的分析。

## 2 R-CNN
2014年R-CNN算法被提出，基本奠定了two-stage方式在目标检测领域的应用。它的算法结构如下图
![](https://img.alicdn.com/tfs/TB1gGxqlH5YBuNjSspoXXbeNFXa-1606-452.png)
算法步骤如下
获取输入的原始图片
使用选择性搜索算法（selective search）评估相邻图像之间的相似度，把相似度高的进行合并，并对合并后的区块打分，选出感兴趣区域的候选框，也就是子图。这一步大约需要选出2000个子图。
分别对子图使用卷积神经网络，进行卷积-relu-池化以及全连接等步骤，提取特征。这一步基本就是物体识别的范畴了
对提取的特征进行物体分类，保留分类准确率高的区块，以作为最终的物体定位区块。
R-CNN较传统的目标检测算法获得了50%的性能提升，在使用VGG-16模型作为物体识别模型情况下，在voc2007数据集上可以取得66%的准确率，已经算还不错的一个成绩了。其最大的问题是速度很慢，内存占用量很大，主要原因有两个
候选框由传统的selective search算法完成，速度比较慢
对2000个候选框，均需要做物体识别，也就是需要做2000次卷积网络计算。这个运算量是十分巨大的。

## 3 Fast R-CNN
针对R-CNN的部分问题，2015年微软提出了fast R-CNN算法，它主要优化了两个问题
提出ROI pooling池化层结构，解决了候选框子图必须将图像裁剪缩放到相同尺寸大小的问题。由于CNN网络的输入图像尺寸必须是固定的某一个大小（否则全连接时没法计算），故R-CNN中对大小形状不同的候选框，进行了裁剪和缩放，使得他们达到相同的尺寸。这个操作既浪费时间，又容易导致图像信息丢失和形变。fast R-CNN在全连接层之前插入了ROI pooling层，从而不需要对图像进行裁剪，很好的解决了这个问题。
如下图，剪切会导致信息丢失，缩放会导致图像形变。
![](https://img.alicdn.com/tfs/TB17KxClL1TBuNjy0FjXXajyXXa-1576-354.png)
ROI pooling的思路是，如果最终我们要生成MxN的图片，那么先将特征图水平和竖直分为M和N份，然后每一份取最大值，输出MxN的特征图。这样就实现了固定尺寸的图片输出了。ROI pooling层位于卷积后，全连接前。
提出多任务损失函数思想，将分类损失和边框定位回归损失结合在一起统一训练，最终输出对应分类和边框坐标。
Fast R-CNN的结构如如下
![](https://img.alicdn.com/tfs/TB1hL8blQOWBuNjSsppXXXPgpXa-1338-536.png)
4 Faster R-CNN
R-CNN和fast R-CNN均存在一个问题，那就是由选择性搜索来生成候选框，这个算法很慢。而且R-CNN中生成的2000个左右的候选框全部需要经过一次卷积神经网络，也就是需要经过2000次左右的CNN网络，这个是十分耗时的（fast R-CNN已经做了改进，只需要对整图经过一次CNN网络）。这也是导致这两个算法检测速度较慢的最主要原因。faster R-CNN 针对这个问题，提出了RPN网络来进行候选框的获取，从而摆脱了选择性搜索算法，也只需要一次卷积层操作，从而大大提高了识别速度。这个算法十分复杂，我们会详细分析。它的基本结构如下图
![](https://img.alicdn.com/tfs/TB1Rs4XlL9TBuNjy0FcXXbeiFXa-1498-796.png)
主要分为四个步骤
卷积层。原始图片先经过conv-relu-pooling的多层卷积神经网络，提取出特征图。供后续的RPN网络和全连接层使用。faster R-CNN不像R-CNN需要对每个子图进行卷积层特征提取，它只需要对全图进行一次提取就可以了，从而大大减小了计算时间。
RPN层，region proposal networks。RPN层用于生成候选框，并利用softmax判断候选框是前景还是背景，从中选取前景候选框（因为物体一般在前景中），并利用bounding box regression调整候选框的位置，从而得到特征子图，称为proposals。
ROI层，fast R-CNN中已经讲过了ROI层了，它将大小尺寸不同的proposal池化成相同的大小，然后送入后续的全连接层进行物体分类和位置调整回归
分类层。利用ROI层输出的特征图proposal，判断proposal的类别，同时再次对bounding box进行regression从而得到精确的形状和位置。
使用VGG-16卷积模型的网络结构如下图
![](https://img.alicdn.com/tfs/TB1bsuclN9YBuNjy0FfXXXIsVXa-1714-824.png)
4.1 卷积层
卷积层采用的VGG-16模型，先将PxQ的原始图片，缩放裁剪为MxN的图片，然后经过13个conv-relu层，其中会穿插4个max-pooling层。所有的卷积的kernel都是3x3的，padding为1，stride为1。pooling层kernel为2x2, padding为0，stride为2。
MxN的图片，经过卷积层后，变为了(M/16) x (N/16)的feature map了。
4.2 RPN层
faster R-CNN抛弃了R-CNN中的选择性搜索（selective search）方法，使用RPN层来生成候选框，能极大的提升候选框的生成速度。RPN层先经过3x3的卷积运算，然后分为两路。一路用来判断候选框是前景还是背景，它先reshape成一维向量，然后softmax来判断是前景还是背景，然后reshape恢复为二维feature map。另一路用来确定候选框的位置，通过bounding box regression实现，后面再详细讲。两路计算结束后，挑选出前景候选框（因为物体在前景中），并利用计算得到的候选框位置，得到我们感兴趣的特征子图proposal。
4.2.1 候选框的生成 anchors
卷积层提取原始图像信息，得到了256个feature map，经过RPN层的3x3卷积后，仍然为256个feature map。但是每个点融合了周围3x3的空间信息。对每个feature map上的一个点，生成k个anchor（k默认为9）。anchor分为前景和背景两类（我们先不去管它具体是飞机还是汽车，只用区分它是前景还是背景即可）。anchor有[x,y,w,h]四个坐标偏移量，x,y表示中心点坐标，w和h表示宽度和高度。这样，对于feature map上的每个点，就得到了k个大小形状各不相同的选区region。
4.2.2 softmax判断选区是前景还是背景
对于生成的anchors，我们首先要判断它是前景还是背景。由于感兴趣的物体位于前景中，故经过这一步之后，我们就可以舍弃背景anchors了。大部分的anchors都是属于背景，故这一步可以筛选掉很多无用的anchor，从而减少全连接层的计算量。
对于经过了3x3的卷积后得到的256个feature map，先经过1x1的卷积，变换为18个feature map。然后reshape为一维向量，经过softmax判断是前景还是背景。此处reshape的唯一作用就是让数据可以进行softmax计算。然后输出识别得到的前景anchors。
4.2.3 确定候选框位置
另一路用来确定候选框的位置，也就是anchors的[x,y,w,h]坐标值。如下图所示，红色代表我们当前的选区，绿色代表真实的选区。虽然我们当前的选取能够大概框选出飞机，但离绿色的真实位置和形状还是有很大差别，故需要对生成的anchors进行调整。这个过程我们称为bounding box regression。
![](https://img.alicdn.com/tfs/TB1tPo5lpmWBuNjSspdXXbugXXa-1276-476.png)
假设红色框的坐标为[x,y,w,h], 绿色框，也就是目标框的坐标为[Gx, Gy,Gw,Gh], 我们要建立一个变换，使得[x,y,w,h]能够变为[Gx, Gy,Gw,Gh]。最简单的思路是，先做平移，使得中心点接近，然后进行缩放，使得w和h接近。如下
![img](https://img.alicdn.com/tfs/TB1lf3Nl21TBuNjy0FjXXajyXXa-1574-318.png)
我们要学习的就是dx dy dw dh这四个变换。由于是线性变换，我们可以用线性回归来建模。设定loss和优化方法后，就可以利用深度学习进行训练，并得到模型了。对于空间位置loss，我们一般采用均方差算法，而不是交叉熵（交叉熵使用在分类预测中）。优化方法可以采用自适应梯度下降算法Adam。
4.2.4 输出特征子图proposal
得到了前景anchors，并确定了他们的位置和形状后，我们就可以输出前景的特征子图proposal了。步骤如下
得到前景anchors和他们的[x y w h]坐标
按照anchors为前景的不同概率，从大到小排序，选取前pre_nms_topN个anchors，比如前6000个
剔除非常小的anchors
通过NMS非极大值抑制，从anchors中找出置信度较高的。这个主要是为了解决选取交叠问题。首先计算每一个选区面积，然后根据他们在softmax中的score（也就是是否为前景的概率）进行排序，将score最大的选区放入队列中。接下来，计算其余选区与当前最大score选区的IOU（IOU为两box交集面积除以两box并集面积，它衡量了两个box之间重叠程度）。去除IOU大于设定阈值的选区。这样就解决了选区重叠问题
选取前post_nms_topN个结果作为最终选区proposal进行输出，比如300个。
经过这一步之后，物体定位应该就基本结束了，剩下的就是物体识别了
4.3 ROI Pooling层
和fast R-CNN中类似，这一层主要解决之前得到的proposal大小形状各不相同，导致没法做全连接。全连接计算只能对确定的shape进行运算，故必须使proposal大小形状变为相同。通过裁剪和缩放的手段，可以解决这个问题，但会带来信息丢失和图片形变问题。我们使用ROI pooling可以有效的解决这个问题。
ROI pooling中，如果目标输出为MxN，则在水平和竖直方向上，将输入proposal划分为MxN份，每一份取最大值，从而得到MxN的输出特征图。
4.4 分类层
ROI Pooling层后的特征图，通过全连接层与softmax，就可以计算属于哪个具体类别，比如人，狗，飞机，并可以得到cls_prob概率向量。同时再次利用bounding box regression精细调整proposal位置，得到bbox_pred，用于回归更加精确的目标检测框。
![img](https://img.alicdn.com/tfs/TB1eYMclVGWBuNjy0FbXXb4sXXa-1382-334.png)
这样就完成了faster R-CNN的整个过程了。算法还是相当复杂的，对于每个细节需要反复理解。faster R-CNN使用resNet101模型作为卷积层，在voc2012数据集上可以达到83.8%的准确率，超过yolo ssd和yoloV2。其最大的问题是速度偏慢，每秒只能处理5帧，达不到实时性要求。
5 Yolo：you only look once
针对于two-stage目标检测算法普遍存在的运算速度慢的缺点，yolo创造性的提出了one-stage。也就是将物体分类和物体定位在一个步骤中完成。yolo直接在输出层回归bounding box的位置和bounding box所属类别，从而实现one-stage。通过这种方式，yolo可实现45帧每秒的运算速度，完全能满足实时性要求（达到24帧每秒，人眼就认为是连续的）。它的网络结构如下图
![img](https://img.alicdn.com/tfs/TB10_fPl3mTBuNjy1XbXXaMrVXa-2434-950.png)
主要分为三个部分：卷积层，目标检测层，NMS筛选层
5.1 卷积层
采用Google inceptionV1网络，对应到上图中的第一个阶段，共20层。这一层主要是进行特征提取，从而提高模型泛化能力。但作者对inceptionV1进行了改造，他没有使用inception module结构，而是用一个1x1的卷积，并联一个3x3的卷积来替代。（可以认为只使用了inception module中的一个分支，应该是为了简化网络结构）
5.2 目标检测层
先经过4个卷积层和2个全连接层，最后生成7x7x30的输出。先经过4个卷积层的目的是为了提高模型泛化能力。yolo将一副448x448的原图分割成了7x7个网格，每个网格要预测两个bounding box的坐标(x,y,w,h)和box内包含物体的置信度confidence，以及物体属于20类别中每一类的概率（yolo的训练数据为voc2012，它是一个20分类的数据集）。所以一个网格对应的参数为（4x2+2+20) = 30。如下图
![img](https://img.alicdn.com/tfs/TB1gIkzl7yWBuNjy0FpXXassXXa-1558-718.png)
bounding box坐标: 如上图，7x7网格内的每个grid（红色框），对应两个大小形状不同的bounding box（黄色框）。每个box的位置坐标为（x,y,w,h),  x和y表示box中心点坐标，w和h表示box宽度和高度。通过与训练数据集上标定的物体真实坐标（Gx,Gy,Gw,Gh)进行对比训练，可以计算出初始bounding box平移和伸缩得到最终位置的模型。
bounding box置信度confidence：这个置信度只是为了表达box内有无物体的概率，并不表达box内物体是什么。
confidence=![Pr(Object) \ast IOU^{truth}_{pred}](https://www.zhihu.com/equation?tex=Pr%28Object%29+%5Cast+IOU%5E%7Btruth%7D_%7Bpred%7D)
其中前一项表示有无人工标记的物体落入了网格内，如果有则为1，否则为0。第二项代表bounding box和真实标记的box之间的重合度。它等于两个box面积交集，除以面积并集。值越大则box越接近真实位置。
分类信息：yolo的目标训练集为voc2012，它是一个20分类的目标检测数据集。常用目标检测数据集如下表
Name\# Images (trainval)\# ClassesLast updated|
|---|---|---|---|
ImageNet450k2002015|
COCO120K902014|
Pascal VOC12k202012|
Oxford-IIIT Pet7K372012|
KITTI Vision7K3|
每个网格还需要预测它属于20分类中每一个类别的概率。分类信息是针对每个网格的，而不是bounding box。故只需要20个，而不是40个。而confidence则是针对bounding box的，它只表示box内是否有物体，而不需要预测物体是20分类中的哪一个，故只需要2个参数。虽然分类信息和confidence都是概率，但表达含义完全不同。
5.3 NMS筛选层
筛选层是为了在多个结果中（多个bounding box）筛选出最合适的几个，这个方法和faster R-CNN 中基本相同。都是先过滤掉score低于阈值的box，对剩下的box进行NMS非极大值抑制，去除掉重叠度比较高的box（NMS具体算法可以回顾上面faster R-CNN小节）。这样就得到了最终的最合适的几个box和他们的类别。
5.4 yolo损失函数
yolo的损失函数包含三部分，位置误差，confidence误差，分类误差。具体公式如下
![img](https://img.alicdn.com/tfs/TB1B1pVmmBYBeNjy0FeXXbnmFXa-1590-1088.png)
误差均采用了均方差算法，其实我认为，位置误差应该采用均方差算法，而分类误差应该采用交叉熵。由于物体位置只有4个参数，而类别有20个参数，他们的累加和不同。如果赋予相同的权重，显然不合理。故yolo中位置误差权重为5，类别误差权重为1。由于我们不是特别关心不包含物体的bounding box，故赋予不包含物体的box的置信度confidence误差的权重为0.5，包含物体的权重则为1。
yolo算法开创了one-stage检测的先河，它将物体分类和物体检测网络合二为一，都在全连接层完成。故它大大降低了目标检测的耗时，提高了实时性。但它的缺点也十分明显
每个网格只对应两个bounding box，当物体的长宽比不常见（也就是训练数据集覆盖不到时），效果很差。
原始图片只划分为7x7的网格，当两个物体靠的很近时，效果很差
最终每个网格只对应一个类别，容易出现漏检（物体没有被识别到）。
对于图片中比较小的物体，效果很差。这其实是所有目标检测算法的通病，SSD对它有些优化，我们后面再看。
6 SSD: Single Shot MultiBox Detector
Faster R-CNN准确率mAP较高，漏检率recall较低，但速度较慢。而yolo则相反，速度快，但准确率和漏检率不尽人意。SSD综合了他们的优缺点，对输入300x300的图像，在voc2007数据集上test，能够达到58 帧每秒( Titan X 的 GPU )，72.1%的mAP。
SSD网络结构如下图
![img](https://img.alicdn.com/tfs/TB1sVXkmhGYBuNjy0FnXXX5lpXa-2212-1196.png)
和yolo一样，也分为三部分：卷积层，目标检测层和NMS筛选层
6.1 卷积层
SSD论文采用了VGG16的基础网络，其实这也是几乎所有目标检测神经网络的惯用方法。先用一个CNN网络来提取特征，然后再进行后续的目标定位和目标分类识别。
6.2 目标检测层
这一层由5个卷积层和一个平均池化层组成。去掉了最后的全连接层。SSD认为目标检测中的物体，只与周围信息相关，它的感受野不是全局的，故没必要也不应该做全连接。SSD的特点如下
6.2.1 多尺寸feature map上进行目标检测
每一个卷积层，都会输出不同大小感受野的feature map。在这些不同尺度的feature map上，进行目标位置和类别的训练和预测，从而达到多尺度检测的目的，可以克服yolo对于宽高比不常见的物体，识别准确率较低的问题。而yolo中，只在最后一个卷积层上做目标位置和类别的训练和预测。这是SSD相对于yolo能提高准确率的一个关键所在。
![img](https://img.alicdn.com/tfs/TB1vTvPl3mTBuNjy1XbXXaMrVXa-1950-762.png)
如上所示，在每个卷积层上都会进行目标检测和分类，最后由NMS进行筛选，输出最终的结果。多尺度feature map上做目标检测，就相当于多了很多宽高比例的bounding box，可以大大提高泛化能力。
6.2.2 多个anchors，每个anchor对应4个位置参数和21个类别参数
和faster R-CNN相似，SSD也提出了anchor的概念。卷积输出的feature map，每个点对应为原图的一个区域的中心点。以这个点为中心，构造出6个宽高比例不同，大小不同的anchor（SSD中称为default box）。每个anchor对应4个位置参数(x,y,w,h)和21个类别概率（voc训练集为20分类问题，在加上anchor是否为背景，共21分类）。如下图所示
![img](https://img.alicdn.com/tfs/TB1yZExl1SSBuNjy0FlXXbBpVXa-2446-820.png)
另外，在训练阶段，SSD将正负样本比例定位1：3。训练集给定了输入图像以及每个物体的真实区域（ground true box），将default box和真实box最接近的选为正样本。然后在剩下的default box中选择任意一个与真实box IOU大于0.5的，作为正样本。而其他的则作为负样本。由于绝大部分的box为负样本，会导致正负失衡，故根据每个box类别概率排序，使正负比例保持在1：3。SSD认为这个策略提高了4%的准确率
另外，SSD采用了数据增强。生成与目标物体真实box间IOU为0.1 0.3 0.5 0.7 0.9的patch，随机选取这些patch参与训练，并对他们进行随机水平翻转等操作。SSD认为这个策略提高了8.8%的准确率。
6.3 筛选层
和yolo的筛选层基本一致，同样先过滤掉类别概率低于阈值的default box，再采用NMS非极大值抑制，筛掉重叠度较高的。只不过SSD综合了各个不同feature map上的目标检测输出的default box。
SSD基本已经可以满足我们手机端上实时物体检测需求了，TensorFlow在Android上的目标检测官方模型ssd_mobilenet_v1_android_export.pb，就是通过SSD算法实现的。它的基础卷积网络采用的是mobileNet，适合在终端上部署和运行。
7 YoloV2, Yolo9000和其他模型
针对yolo准确率不高，容易漏检，对长宽比不常见物体效果差等问题，结合SSD的特点，提出了yoloV2。它主要还是采用了yolo的网络结构，在其基础上做了一些优化和改进，如下
网络采用DarkNet-19：19层，里面包含了大量3x3卷积，同时借鉴inceptionV1，加入1x1卷积核全局平均池化层。结构如下
![img](https://img.alicdn.com/tfs/TB1EPggl7OWBuNjSsppXXXPgpXa-1504-914.png)
去掉全连接层：和SSD一样，模型中只包含卷积和平均池化层（平均池化是为了变为一维向量，做softmax分类）。这样做一方面是由于物体检测中的目标，只是图片中的一个区块，它是局部感受野，没必要做全连接。而是为了输入不同尺寸的图片，如果采用全连接，则只能输入固定大小图片了。
batch normalization：卷积层后加入BN，对下一次卷积输入的数据做归一化。可以在增大学习率的前提下，同样可以稳定落入局部最优解。从而加速训练收敛，在相同耗时下，增大了有效迭代次数。
使用anchors：借鉴faster R-CNN和SSD，对于一个中心点，使用多个anchor，得到多个bounding box，每个bounding box包含4个位置坐标参数(x y w h)和21个类别概率信息。而在yolo中，每个grid（对应anchor），仅预测一次类别，而且只有两个bounding box来进行坐标预测。
pass through layer：yolo原本最终特征图为13x13x256。yoloV2还利用了之前的26x26的特征图进行目标检测。26x26x256的feature map分别按行和列隔点采样，得到4幅13x13x256的feature map，将他们组织成一幅13x13x2048的feature map。这样做的目的是提高小物体的识别率。因为越靠前的卷积，其感受野越小，越有利于小物体的识别。
高分辨率输入Training：yolo采用224x224图片进行预训练，而yoloV2则采用448x448
Multi-Scale Training：输入不同尺寸的图片，迭代10次，就改变输入图片尺寸。由于模型中去掉了全连接层，故可以输入不同尺寸的图片了。从320x320，到608x608

yolo和yoloV2只能识别20类物体，为了优化这个问题，提出了yolo9000，可以识别9000类物体。它在yoloV2基础上，进行了imageNet和coco的联合训练。这种方式充分利用imageNet可以识别1000类物体和coco可以进行目标位置检测的优点。当使用imageNet训练时，只更新物体分类相关的参数。而使用coco时，则更新全部所有参数。
![img](https://img.alicdn.com/tfs/TB190Eul1SSBuNjy0FlXXbBpVXa-1590-986.png)
当前目标检测模型算法也是层出不穷。在two-stage领域，2017年Facebook提出了mask R-CNN。CMU也提出了A-Fast-RCNN 算法，将对抗学习引入到目标检测领域。Face++也提出了Light-Head R-CNN，主要探讨了 R-CNN 如何在物体检测中平衡精确度和速度。
one-stage领域也是百花齐放，2017年首尔大学提出 R-SSD 算法，主要解决小尺寸物体检测效果差的问题。清华大学提出了 RON 算法，结合 two stage 名的方法和 one stage 方法的优势，更加关注多尺度对象定位和负空间样本挖掘问题。
8 总结
目标检测领域的深度学习算法，需要进行目标定位和物体识别，算法相对来说还是很复杂的。当前各种新算法也是层不出穷，但模型之间有很强的延续性，大部分模型算法都是借鉴了前人的思想，站在巨人的肩膀上。我们需要知道经典模型的特点，这些tricks是为了解决什么问题，以及为什么解决了这些问题。这样才能举一反三，万变不离其宗。综合下来，目标检测领域主要的难点如下
检测速度：实时性要求高，故网络结构不能太复杂，参数不能太多，卷积层次也不能太多。
位置准确率：（x y w h）参数必须准确，也就是检测框大小尺寸要匹配，且重合度IOU要高。SSD和faster RCNN通过多个bounding box来优化这个问题
漏检率：必须尽量检测出所有目标物体，特别是靠的近的物体和尺寸小的物体。SSD和faster RCNN通过多个bounding box来优化这个问题
物体宽高比例不常见：SSD通过不同尺寸feature map，yoloV2通过不同尺寸输入图片，来优化这个问题。
靠的近的物体准确率低
小尺寸物体准确率低：SSD取消全连接层，yoloV2增加pass through layer，采用高分辨率输入图片，来优化这个问题
系列文章，请多关注
[Tensorflow源码解析1 – 内核架构和源码结构](https://blog.csdn.net/u013510838/article/details/84103503)
[自然语言处理1 – 分词](https://blog.csdn.net/u013510838/article/details/81673016)
[带你深入AI（1） - 深度学习模型训练痛点及解决方法](https://blog.csdn.net/u013510838/article/details/79835563)
[带你深入AI（2）- 深度学习激活函数，准确率，优化方法等总结](https://blog.csdn.net/u013510838/article/details/79845455)
[带你深入AI（3）- 物体分类领域：AlexNet VGG Inception ResNet mobileNet](https://blog.csdn.net/u013510838/article/details/79866740)
[带你深入AI（4）- 目标检测领域：R-CNN，faster R-CNN，yolo，SSD, yoloV2](https://blog.csdn.net/u013510838/article/details/79947553)
[带你深入AI（5）- 自然语言处理领域：RNN LSTM GRU](https://blog.csdn.net/u013510838/article/details/80024144)
[带你深入AI（6）- 详解bazel](https://blog.csdn.net/u013510838/article/details/80102438)
[带你深入AI（7）- 深度学习重要Python库](https://blog.csdn.net/u013510838/article/details/80412127)

