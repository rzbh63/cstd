
# ESRGAN：基于GAN的增强超分辨率方法（附代码解析） - Paper weekly - CSDN博客


2019年01月18日 13:08:19[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：167


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨左育莘
学校丨西安电子科技大学
研究方向丨计算机视觉

之前看的文章里有提到 GAN 在图像修复时更容易得到符合视觉上效果更好的图像，所以也是看了一些结合 GAN 的图像修复工作。

**ESRGAN: Enhanced Super-Resolution Generative Adversarial Networks**发表于 ECCV 2018 的 Workshops，**作者在 SRGAN 的基础上进行了改进**，包括改进网络的结构、判决器的判决形式，以及更换了一个用于计算感知域损失的预训练网络。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnb0o4hv8GBxjxVSQqBcSqwxr8uezfw49jgfo4ZWJWG3ibktdcP0NibFcg/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLn5ghq9icTLQicNWq570oDlRZ7pdklI6LzHic8Sqo61utsbZM3CNB5nprqA/640?wx_fmt=png)

**超分辨率生成对抗网络（SRGAN）**是一项开创性的工作，能够在单一图像超分辨率中生成逼真的纹理。这项工作发表于 CVPR 2017。

但是，放大后的细节通常伴随着令人不快的伪影。为了更进一步地提升视觉质量，**作者仔细研究了 SRGAN 的三个关键部分：**1）网络结构；2）对抗性损失；3）感知域损失。并对每一项进行改进，得到 ESRGAN。

具体而言，**文章提出了一种 Residual-in-Residual Dense Block (RRDB) 的网络单元**，在这个单元中，去掉了 BN（Batch Norm）层。此外，**作者借鉴了 Relativistic GAN 的想法**，让判别器预测图像的真实性而不是图像“是否是 fake 图像”。

最后，**文章对感知域损失进行改进，使用激活前的特征**，这样可以为亮度一致性和纹理恢复提供更强的监督。在这些改进的帮助下，ESRGAN 得到了更好的视觉质量以及更逼真和自然的纹理。

改进后的效果图（4 倍放大）：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnsSyg8kGH76NsEEpcmycmxBO3w93kaYOOafBuiaNPQJEibCAkj5tcCwNA/640?wx_fmt=png)
**▲**在纹理和细节上，ESRGAN都优于SRGAN

# SRGAN的思考与贡献

现有的超分辨率网络在不同的网络结构设计以及训练策略下，超分辨的效果得到了很大的提升，特别是 PSNR 指标。但是，**基于 PSNR 指标的模型会倾向于生成过度平滑的结果**，这些结果缺少必要的高频信息。PSNR 指标与人类观察者的主观评价从根本上就不统一。

一些基于感知域信息驱动的方法已经提出来用于提升超分辨率结果的视觉质量。例如，感知域的损失函数提出来用于在特征空间（instead of 像素空间）中优化超分辨率模型；生成对抗网络通过鼓励网络生成一些更接近于自然图像的方法来提升超分辨率的质量；语义图像先验信息用于进一步改善恢复的纹理细节。

通过结合上面的方法，SRGAN 模型极大地提升了超分辨率结果的视觉质量。但是 SRGAN 模型得到的图像和 GT 图像仍有很大的差距。

# ESRGAN的改进

**文章对这三点做出改进：**

1. 网络的基本单元从基本的残差单元变为 Residual-in-Residual Dense Block (RRDB)；

2. GAN 网络改进为 Relativistic average GAN (RaGAN)；

3. 改进感知域损失函数，使用激活前的 VGG 特征，这个改进会提供更尖锐的边缘和更符合视觉的结果。

# 网络结构及思想

**生成器部分**

首先，作者参考 SRResNet 结构作为整体的网络结构，SRResNet 的基本结构如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnU1I0yjplOibiaLzGObE5roNeY9nYI0JwrAk7P6ibHhPdTRVburPcTibAfQ/640?wx_fmt=png)
**▲**SRResNet基本结构

**为了提升 SRGAN 重构的图像质量，作者主要对生成器 G 做出如下改变：**

1. 去掉所有的 BN 层；

2. 把原始的 block 变为 Residual-in-Residual Dense Block (RRDB)，这个 block 结合了多层的残差网络和密集连接。
如下图所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnurtGHqqtknyWchGsWppCno9HJ8xm3nlC4wiaEGhBFAnRVMg5aic9Xic3g/640?wx_fmt=png)

**思想**

**BN 层的影响：**对于不同的基于 PSNR 的任务（包括超分辨率和去模糊）来说，去掉 BN 层已经被证明会提高表现和减小计算复杂度。

BN 层在训练时，使用一个 batch 的数据的均值和方差对该 batch 特征进行归一化，在测试时，使用在整个测试集上的数据预测的均值和方差。当训练集和测试集的统计量有很大不同的时候，BN 层就会倾向于生成不好的伪影，并且限制模型的泛化能力。

作者发现，BN 层在网络比较深，而且在 GAN 框架下进行训练的时候，更会产生伪影。这些伪影偶尔出现在迭代和不同的设置中，违反了对训练稳定性能的需求。所以为了稳定的训练和一致的性能，作者去掉了 BN 层。此外，去掉 BN 层也能提高模型的泛化能力，减少计算复杂度和内存占用。

**Trick**

除了上述的改进，**作者也使用了一些技巧来训练深层网络：**

1. 对残差信息进行 scaling，即将残差信息乘以一个 0 到 1 之间的数，用于防止不稳定；

2. 更小的初始化，作者发现当初始化参数的方差变小时，残差结构更容易进行训练。

**判别器部分**

除了改进的生成器，作者也基于 Relativistic GAN 改进了判别器。判别器 D 使用的网络是 VGG 网络，SRGAN 中的判别器 D 用于估计输入到判别器中的图像是真实且自然图像的概率，而 Relativistic 判别器则尝试估计**真实图像相对来说比 fake 图像更逼真的概率**。

如下图所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnqO8BrhiaTRPm9Mf4ImnqaQ2OTlC3QjpVTaAXRsQ8WLCFVPuw6PtbLHw/640?wx_fmt=png)

具体而言，作者把标准的判别器换成 Relativistic average Discriminator（RaD），所以判别器的损失函数定义为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLn0ibwWnc5tnJ3PocGSe4q1xDfLumuRoaI0zuWbDqRB7n1IrWyS6uB4mw/640?wx_fmt=png)

对应的生成器的对抗损失函数为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLn35FyfHx5KLLbLk49axIibfIhiceic5rqWdkewu2icdrgHvOr4pabEwBhAg/640?wx_fmt=png)

求均值的操作是通过对 mini-batch 中的所有数据求平均得到的，xf 是原始低分辨图像经过生成器以后的图像。

可以观察到，对抗损失包含了 xr 和 xf，所以这个生成器受益于对抗训练中的生成数据和实际数据的梯度，这种调整会使得网络学习到更尖锐的边缘和更细节的纹理。

**感知域损失**

文章也提出了一个更有效的感知域损失，使用激活前的特征（VGG16 网络）。

感知域的损失当前是定义在一个预训练的深度网络的激活层，这一层中两个激活了的特征的距离会被最小化。

与此相反，文章使用的特征是激活前的特征，这样会克服两个缺点。第一，激活后的特征是非常稀疏的，特别是在很深的网络中。这种稀疏的激活提供的监督效果是很弱的，会造成性能低下；第二，使用激活后的特征会导致重建图像与 GT 的亮度不一致。

如图所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnUG5Ro9BicRb4a2CcHFDrOFoeWZibu9UrXS0hZhzWSlV6cdcgd5dHygicQ/640?wx_fmt=png)
**▲**使用激活前与激活后的特征的比较：a. 亮度 b. 细节

作者对使用的感知域损失进行了探索。与目前多数使用的用于图像分类的 VGG 网络构建的感知域损失相反，作者提出一种更适合于超分辨的感知域损失，这个损失基于一个用于材料识别的 VGG16 网络（MINCNet），这个网络更聚焦于纹理而不是物体。**尽管这样带来的增益很小，但作者仍然相信，探索关注纹理的感知域损失对超分辨至关重要。**

# 损失函数

经过上面对网络模块的定义和构建以后，再定义损失函数，就可以进行训练了。

对于生成器 G，它的损失函数为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnTgoVCqnRAvuK7nPg4cyRz3zk0icAu7ia1X7PK3SNNAx6Esgr6JHicTnAQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnfq179bg4QXAsYKL7uXInMWds88icqMwhyBiaebwRE0vtcZ6LeUnmR5Zw/640?wx_fmt=png)即为感知域损失，作者的源码取的是 L1 Loss，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnIuPmf2ZpzqchHIFvb0hib9VRRoKqvRDtKcpkIDpgG0iaOesicxu8xjkicg/640?wx_fmt=png)即为上面定义的生成器损失，而 L1 则为 pixel-wise 损失，即![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnibMKjTyMPdU2YAN2msuhcmAic0vv2CibPrYsyzD8j4QGJz6OAfETCr6DQ/640?wx_fmt=png), 实验中取![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnmpbTvDbJ9oN5QrmufHfzIKiaeHMtZTFHj3NSkXlO3V41byX9W4CpuRg/640?wx_fmt=png)。

对于判别器，其损失函数就是上面提到的：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLn6R3TZXqJbfp2Ezfyr181I09qT7ZJEiaQ0rxAp9ucPFziccS2oNYHmkibg/640?wx_fmt=png)

# 网络细节

**生成器网络G**

要定义 RDDB 模块，首先要定义 Dense Block，而 Dense Block 里面又有卷积层，LReLU 层以及密集连接，所以首先将卷积层和 LReLU 层进行模块化，这部分的代码如下（PyTorch）：

```python
def
```
```python
conv_block
```
```python
(in_nc, out_nc, kernel_size, stride=
```
```python
1
```
```python
, dilation=
```
```python
1
```
```python
, groups=
```
```python
1
```
```python
, bias=True, \
```

```python
pad_type=
```
```python
'zero'
```
```python
, norm_type=None, act_type=
```
```python
'relu'
```
```python
, mode=
```
```python
'CNA'
```
```python
)
```
```python
:
```

```python
'''
```

```python
Conv layer with padding, normalization, activation
```

```python
mode: CNA --> Conv -> Norm -> Act
```

```python
NAC --> Norm -> Act --> Conv (Identity Mappings in Deep Residual Networks, ECCV16)
```

```python
'''
```

```python
assert
```
```python
mode
```
```python
in
```
```python
[
```
```python
'CNA'
```
```python
,
```
```python
'NAC'
```
```python
,
```
```python
'CNAC'
```
```python
],
```
```python
'Wong conv mode [{:s}]'
```
```python
.format(mode)
```

```python
padding = get_valid_padding(kernel_size, dilation)
```

```python
p = pad(pad_type, padding)
```
```python
if
```
```python
pad_type
```
```python
and
```
```python
pad_type !=
```
```python
'zero'
```
```python
else
```
```python
None
```

```python
padding = padding
```
```python
if
```
```python
pad_type ==
```
```python
'zero'
```
```python
else
```
```python
0
```

```python
c = nn.Conv2d(in_nc, out_nc, kernel_size=kernel_size, stride=stride, padding=padding, \
```

```python
dilation=dilation, bias=bias, groups=groups)
```

```python
a = act(act_type)
```
```python
if
```
```python
act_type
```
```python
else
```
```python
None
```

```python
if
```
```python
'CNA'
```
```python
in
```
```python
mode:
```

```python
n = norm(norm_type, out_nc)
```
```python
if
```
```python
norm_type
```
```python
else
```
```python
None
```

```python
return
```
```python
sequential(p, c, n, a)
```

```python
elif
```
```python
mode ==
```
```python
'NAC'
```
```python
:
```

```python
if
```
```python
norm_type
```
```python
is
```
```python
None
```
```python
and
```
```python
act_type
```
```python
is
```
```python
not
```
```python
None
```
```python
:
```

```python
a = act(act_type, inplace=
```
```python
False
```
```python
)
```

```python
# Important!
```

```python
# input----ReLU(inplace)----Conv--+----output
```

```python
#        |________________________|
```

```python
# inplace ReLU will modify the input, therefore wrong output
```

```python
n = norm(norm_type, in_nc)
```
```python
if
```
```python
norm_type
```
```python
else
```
```python
None
```

```python
return
```
```python
sequential(n, a, p, c)
```

注意这里的 pad_type='zero' 并不是指 padding=0，源码中定义了两个函数，针对不同模式下的 padding：

```python
def
```
```python
pad
```
```python
(pad_type, padding)
```
```python
:
```

```python
# helper selecting padding layer
```

```python
# if padding is 'zero', do by conv layers
```

```python
pad_type = pad_type.lower()
```

```python
if
```
```python
padding ==
```
```python
0
```
```python
:
```

```python
return
```
```python
None
```

```python
if
```
```python
pad_type ==
```
```python
'reflect'
```
```python
:
```

```python
layer = nn.ReflectionPad2d(padding)
```

```python
elif
```
```python
pad_type ==
```
```python
'replicate'
```
```python
:
```

```python
layer = nn.ReplicationPad2d(padding)
```

```python
else
```
```python
:
```

```python
raise
```
```python
NotImplementedError(
```
```python
'padding layer [{:s}] is not implemented'
```
```python
.format(pad_type))
```

```python
return
```
```python
layer
```


```python
def
```
```python
get_valid_padding
```
```python
(kernel_size, dilation)
```
```python
:
```

```python
kernel_size = kernel_size + (kernel_size -
```
```python
1
```
```python
) * (dilation -
```
```python
1
```
```python
)
```

```python
padding = (kernel_size -
```
```python
1
```
```python
) //
```
```python
2
```

```python
return
```
```python
padding
```

所以当 pad_type='zero' 时，执行的是 get_valid_padding 函数，根据输入参数可知此时 padding=1。

模块化以后，对 Dense Block 进行定义：

```python
class
```
```python
ResidualDenseBlock_5C
```
```python
(nn.Module)
```
```python
:
```

```python
'''
```

```python
Residual Dense Block
```

```python
style: 5 convs
```

```python
The core module of paper: (Residual Dense Network for Image Super-Resolution, CVPR 18)
```

```python
'''
```

```python
def
```
```python
__init__
```
```python
(self, nc, kernel_size=
```
```python
3
```
```python
, gc=
```
```python
32
```
```python
, stride=
```
```python
1
```
```python
, bias=True, pad_type=
```
```python
'zero'
```
```python
, \
```

```python
norm_type=None, act_type=
```
```python
'leakyrelu'
```
```python
, mode=
```
```python
'CNA'
```
```python
)
```
```python
:
```

```python
super(ResidualDenseBlock_5C, self).__init__()
```

```python
# gc: growth channel, i.e. intermediate channels
```

```python
self.conv1 = conv_block(nc, gc, kernel_size, stride, bias=bias, pad_type=pad_type, \
```

```python
norm_type=norm_type, act_type=act_type, mode=mode)
```

```python
self.conv2 = conv_block(nc+gc, gc, kernel_size, stride, bias=bias, pad_type=pad_type, \
```

```python
norm_type=norm_type, act_type=act_type, mode=mode)
```

```python
self.conv3 = conv_block(nc+
```
```python
2
```
```python
*gc, gc, kernel_size, stride, bias=bias, pad_type=pad_type, \
```

```python
norm_type=norm_type, act_type=act_type, mode=mode)
```

```python
self.conv4 = conv_block(nc+
```
```python
3
```
```python
*gc, gc, kernel_size, stride, bias=bias, pad_type=pad_type, \
```

```python
norm_type=norm_type, act_type=act_type, mode=mode)
```

```python
if
```
```python
mode ==
```
```python
'CNA'
```
```python
:
```

```python
last_act =
```
```python
None
```

```python
else
```
```python
:
```

```python
last_act = act_type
```

```python
self.conv5 = conv_block(nc+
```
```python
4
```
```python
*gc, nc,
```
```python
3
```
```python
, stride, bias=bias, pad_type=pad_type, \
```

```python
norm_type=norm_type, act_type=last_act, mode=mode)
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
x1 = self.conv1(x)
```

```python
x2 = self.conv2(torch.cat((x, x1),
```
```python
1
```
```python
))
```

```python
x3 = self.conv3(torch.cat((x, x1, x2),
```
```python
1
```
```python
))
```

```python
x4 = self.conv4(torch.cat((x, x1, x2, x3),
```
```python
1
```
```python
))
```

```python
x5 = self.conv5(torch.cat((x, x1, x2, x3, x4),
```
```python
1
```
```python
))
```

```python
return
```
```python
x5.mul(
```
```python
0.2
```
```python
) + x
```

前面提到的对残差信息进行 scaling，在这里可以看出来，系数为 0.2。可以看到在 kernel size（3×3）和 stride=1，padding=1 的设置下，特征图的大小始终不变，但是通道数由于 concat 的原因，每次都会增加 gc 个通道，但是会在最后一层由变回原来的通道数 nc，这里的参数 norm_type=None，表示不要 Batch Norm。

定义了 Dense Block 以后，就可以组成 RDDB 了：

```python
class
```
```python
RRDB
```
```python
(nn.Module)
```
```python
:
```

```python
'''
```

```python
Residual in Residual Dense Block
```

```python
(ESRGAN: Enhanced Super-Resolution Generative Adversarial Networks)
```

```python
'''
```

```python
def
```
```python
__init__
```
```python
(self, nc, kernel_size=
```
```python
3
```
```python
, gc=
```
```python
32
```
```python
, stride=
```
```python
1
```
```python
, bias=True, pad_type=
```
```python
'zero'
```
```python
, \
```

```python
norm_type=None, act_type=
```
```python
'leakyrelu'
```
```python
, mode=
```
```python
'CNA'
```
```python
)
```
```python
:
```

```python
super(RRDB, self).__init__()
```

```python
self.RDB1 = ResidualDenseBlock_5C(nc, kernel_size, gc, stride, bias, pad_type, \
```

```python
norm_type, act_type, mode)
```

```python
self.RDB2 = ResidualDenseBlock_5C(nc, kernel_size, gc, stride, bias, pad_type, \
```

```python
norm_type, act_type, mode)
```

```python
self.RDB3 = ResidualDenseBlock_5C(nc, kernel_size, gc, stride, bias, pad_type, \
```

```python
norm_type, act_type, mode)
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
out = self.RDB1(x)
```

```python
out = self.RDB2(out)
```

```python
out = self.RDB3(out)
```

```python
return
```
```python
out.mul(
```
```python
0.2
```
```python
) + x
```

因为特征图大小始终不变，所以需要定义上采样模块进行放大，得到最后的结果：

```python
def
```
```python
upconv_blcok
```
```python
(in_nc, out_nc, upscale_factor=
```
```python
2
```
```python
, kernel_size=
```
```python
3
```
```python
, stride=
```
```python
1
```
```python
, bias=True, \
```

```python
pad_type=
```
```python
'zero'
```
```python
, norm_type=None, act_type=
```
```python
'relu'
```
```python
, mode=
```
```python
'nearest'
```
```python
)
```
```python
:
```

```python
# Up conv
```

```python
# described in https://distill.pub/2016/deconv-checkerboard/
```

```python
upsample = nn.Upsample(scale_factor=upscale_factor, mode=mode)
```

```python
conv = conv_block(in_nc, out_nc, kernel_size, stride, bias=bias, \
```

```python
pad_type=pad_type, norm_type=norm_type, act_type=act_type)
```

```python
return
```
```python
sequential(upsample, conv)
```

参考 SRResNet，还需要一个 Shortcut 连接模块：

```python
class
```
```python
ShortcutBlock
```
```python
(nn.Module)
```
```python
:
```

```python
#Elementwise sum the output of a submodule to its input
```

```python
def
```
```python
__init__
```
```python
(self, submodule)
```
```python
:
```

```python
super(ShortcutBlock, self).__init__()
```

```python
self.sub = submodule
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
output = x + self.sub(x)
```

```python
return
```
```python
output
```

```python
def
```
```python
__repr__
```
```python
(self)
```
```python
:
```

```python
tmpstr =
```
```python
'Identity + \n|'
```

```python
modstr = self.sub.__repr__().replace(
```
```python
'\n'
```
```python
,
```
```python
'\n|'
```
```python
)
```

```python
tmpstr = tmpstr + modstr
```

```python
return
```
```python
tmpstr
```

定义好上面的模块以后，就可以定义生成器网络 G（RDDBNet）：

```python
class
```
```python
RRDBNet
```
```python
(nn.Module)
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(self, in_nc, out_nc, nf, nb, gc=
```
```python
32
```
```python
, upscale=
```
```python
4
```
```python
, norm_type=None, \
```

```python
act_type=
```
```python
'leakyrelu'
```
```python
, mode=
```
```python
'CNA'
```
```python
, upsample_mode=
```
```python
'upconv'
```
```python
)
```
```python
:
```

```python
super(RRDBNet, self).__init__()
```

```python
n_upscale = int(math.log(upscale,
```
```python
2
```
```python
))
```

```python
if
```
```python
upscale ==
```
```python
3
```
```python
:
```

```python
n_upscale =
```
```python
1
```

```python
fea_conv = B.conv_block(in_nc, nf, kernel_size=
```
```python
3
```
```python
, norm_type=
```
```python
None
```
```python
, act_type=
```
```python
None
```
```python
)
```

```python
rb_blocks = [B.RRDB(nf, kernel_size=
```
```python
3
```
```python
, gc=
```
```python
32
```
```python
, stride=
```
```python
1
```
```python
, bias=
```
```python
True
```
```python
, pad_type=
```
```python
'zero'
```
```python
, \
```

```python
norm_type=norm_type, act_type=act_type, mode=
```
```python
'CNA'
```
```python
)
```
```python
for
```
```python
_
```
```python
in
```
```python
range(nb)]
```

```python
LR_conv = B.conv_block(nf, nf, kernel_size=
```
```python
3
```
```python
, norm_type=norm_type, act_type=
```
```python
None
```
```python
, mode=mode)
```

```python
if
```
```python
upsample_mode ==
```
```python
'upconv'
```
```python
:
```

```python
upsample_block = B.upconv_blcok
```

```python
elif
```
```python
upsample_mode ==
```
```python
'pixelshuffle'
```
```python
:
```

```python
upsample_block = B.pixelshuffle_block
```

```python
else
```
```python
:
```

```python
raise
```
```python
NotImplementedError(
```
```python
'upsample mode [{:s}] is not found'
```
```python
.format(upsample_mode))
```

```python
if
```
```python
upscale ==
```
```python
3
```
```python
:
```

```python
upsampler = upsample_block(nf, nf,
```
```python
3
```
```python
, act_type=act_type)
```

```python
else
```
```python
:
```

```python
upsampler = [upsample_block(nf, nf, act_type=act_type)
```
```python
for
```
```python
_
```
```python
in
```
```python
range(n_upscale)]
```

```python
HR_conv0 = B.conv_block(nf, nf, kernel_size=
```
```python
3
```
```python
, norm_type=
```
```python
None
```
```python
, act_type=act_type)
```

```python
HR_conv1 = B.conv_block(nf, out_nc, kernel_size=
```
```python
3
```
```python
, norm_type=
```
```python
None
```
```python
, act_type=
```
```python
None
```
```python
)
```

```python
self.model = B.sequential(fea_conv, B.ShortcutBlock(B.sequential(*rb_blocks, LR_conv)),\
```

```python
*upsampler, HR_conv0, HR_conv1)
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
x = self.model(x)
```

```python
return
```
```python
x
```

注意到这里有个参数 nb，这个参数控制网络中 RDDB 的数量，作者取的是 23。

**判别器网络D**

前面提到，判别器 D 的网络结构为 VGG 网络，定义如下（输入图像 size 为 128×128）：

```python
# VGG style Discriminator with input size 128*128
```

```python
class
```
```python
Discriminator_VGG_128
```
```python
(nn.Module)
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(self, in_nc, base_nf, norm_type=
```
```python
'batch'
```
```python
, act_type=
```
```python
'leakyrelu'
```
```python
, mode=
```
```python
'CNA'
```
```python
)
```
```python
:
```

```python
super(Discriminator_VGG_128, self).__init__()
```

```python
# features
```

```python
# hxw, c
```

```python
# 128, 64
```

```python
conv0 = B.conv_block(in_nc, base_nf, kernel_size=
```
```python
3
```
```python
, norm_type=
```
```python
None
```
```python
, act_type=act_type, \
```

```python
mode=mode)
```

```python
conv1 = B.conv_block(base_nf, base_nf, kernel_size=
```
```python
4
```
```python
, stride=
```
```python
2
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
# 64, 64
```

```python
conv2 = B.conv_block(base_nf, base_nf*
```
```python
2
```
```python
, kernel_size=
```
```python
3
```
```python
, stride=
```
```python
1
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
conv3 = B.conv_block(base_nf*
```
```python
2
```
```python
, base_nf*
```
```python
2
```
```python
, kernel_size=
```
```python
4
```
```python
, stride=
```
```python
2
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
# 32, 128
```

```python
conv4 = B.conv_block(base_nf*
```
```python
2
```
```python
, base_nf*
```
```python
4
```
```python
, kernel_size=
```
```python
3
```
```python
, stride=
```
```python
1
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
conv5 = B.conv_block(base_nf*
```
```python
4
```
```python
, base_nf*
```
```python
4
```
```python
, kernel_size=
```
```python
4
```
```python
, stride=
```
```python
2
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
# 16, 256
```

```python
conv6 = B.conv_block(base_nf*
```
```python
4
```
```python
, base_nf*
```
```python
8
```
```python
, kernel_size=
```
```python
3
```
```python
, stride=
```
```python
1
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
conv7 = B.conv_block(base_nf*
```
```python
8
```
```python
, base_nf*
```
```python
8
```
```python
, kernel_size=
```
```python
4
```
```python
, stride=
```
```python
2
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
# 8, 512
```

```python
conv8 = B.conv_block(base_nf*
```
```python
8
```
```python
, base_nf*
```
```python
8
```
```python
, kernel_size=
```
```python
3
```
```python
, stride=
```
```python
1
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
conv9 = B.conv_block(base_nf*
```
```python
8
```
```python
, base_nf*
```
```python
8
```
```python
, kernel_size=
```
```python
4
```
```python
, stride=
```
```python
2
```
```python
, norm_type=norm_type, \
```

```python
act_type=act_type, mode=mode)
```

```python
# 4, 512
```

```python
self.features = B.sequential(conv0, conv1, conv2, conv3, conv4, conv5, conv6, conv7, conv8,\
```

```python
conv9)
```

```python
# classifier
```

```python
self.classifier = nn.Sequential(
```

```python
nn.Linear(
```
```python
512
```
```python
*
```
```python
4
```
```python
*
```
```python
4
```
```python
,
```
```python
100
```
```python
), nn.LeakyReLU(
```
```python
0.2
```
```python
,
```
```python
True
```
```python
), nn.Linear(
```
```python
100
```
```python
,
```
```python
1
```
```python
))
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
x = self.features(x)
```

```python
x = x.view(x.size(
```
```python
0
```
```python
),
```
```python
-1
```
```python
)
```

```python
x = self.classifier(x)
```

```python
return
```
```python
x
```

可以看到，这里使用了 batch norm，层间的激活函数为 leakyReLU，base_nf参数为基础通道数，为 64。经过特征提取以后，原本为 128×128×1(/3) 的输入图像输出为 4×4×512。再经过其定义的 classifier 得到输出值。

**提取感知域损失的网络（Perceptual Network）**

文章使用了一个用于材料识别的**VGG16 网络**（MINCNet）来提取感知域特征，定义如下：

```python
class
```
```python
MINCNet
```
```python
(nn.Module)
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(self)
```
```python
:
```

```python
super(MINCNet, self).__init__()
```

```python
self.ReLU = nn.ReLU(
```
```python
True
```
```python
)
```

```python
self.conv11 = nn.Conv2d(
```
```python
3
```
```python
,
```
```python
64
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv12 = nn.Conv2d(
```
```python
64
```
```python
,
```
```python
64
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.maxpool1 = nn.MaxPool2d(
```
```python
2
```
```python
, stride=
```
```python
2
```
```python
, padding=
```
```python
0
```
```python
, ceil_mode=
```
```python
True
```
```python
)
```

```python
self.conv21 = nn.Conv2d(
```
```python
64
```
```python
,
```
```python
128
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv22 = nn.Conv2d(
```
```python
128
```
```python
,
```
```python
128
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.maxpool2 = nn.MaxPool2d(
```
```python
2
```
```python
, stride=
```
```python
2
```
```python
, padding=
```
```python
0
```
```python
, ceil_mode=
```
```python
True
```
```python
)
```

```python
self.conv31 = nn.Conv2d(
```
```python
128
```
```python
,
```
```python
256
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv32 = nn.Conv2d(
```
```python
256
```
```python
,
```
```python
256
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv33 = nn.Conv2d(
```
```python
256
```
```python
,
```
```python
256
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.maxpool3 = nn.MaxPool2d(
```
```python
2
```
```python
, stride=
```
```python
2
```
```python
, padding=
```
```python
0
```
```python
, ceil_mode=
```
```python
True
```
```python
)
```

```python
self.conv41 = nn.Conv2d(
```
```python
256
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv42 = nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv43 = nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.maxpool4 = nn.MaxPool2d(
```
```python
2
```
```python
, stride=
```
```python
2
```
```python
, padding=
```
```python
0
```
```python
, ceil_mode=
```
```python
True
```
```python
)
```

```python
self.conv51 = nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv52 = nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
self.conv53 = nn.Conv2d(
```
```python
512
```
```python
,
```
```python
512
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
)
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
out = self.ReLU(self.conv11(x))
```

```python
out = self.ReLU(self.conv12(out))
```

```python
out = self.maxpool1(out)
```

```python
out = self.ReLU(self.conv21(out))
```

```python
out = self.ReLU(self.conv22(out))
```

```python
out = self.maxpool2(out)
```

```python
out = self.ReLU(self.conv31(out))
```

```python
out = self.ReLU(self.conv32(out))
```

```python
out = self.ReLU(self.conv33(out))
```

```python
out = self.maxpool3(out)
```

```python
out = self.ReLU(self.conv41(out))
```

```python
out = self.ReLU(self.conv42(out))
```

```python
out = self.ReLU(self.conv43(out))
```

```python
out = self.maxpool4(out)
```

```python
out = self.ReLU(self.conv51(out))
```

```python
out = self.ReLU(self.conv52(out))
```

```python
out = self.conv53(out)
```

```python
return
```
```python
out
```

再引入预训练参数，就可以进行特征提取：

```python
class
```
```python
MINCFeatureExtractor
```
```python
(nn.Module)
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(self, feature_layer=
```
```python
34
```
```python
, use_bn=False, use_input_norm=True, \
```

```python
device=torch.device
```
```python
(
```
```python
'cpu'
```
```python
)
```
```python
)
```
```python
:
```

```python
super(MINCFeatureExtractor, self).__init__()
```

```python
self.features = MINCNet()
```

```python
self.features.load_state_dict(
```

```python
torch.load(
```
```python
'../experiments/pretrained_models/VGG16minc_53.pth'
```
```python
), strict=
```
```python
True
```
```python
)
```

```python
self.features.eval()
```

```python
# No need to BP to variable
```

```python
for
```
```python
k, v
```
```python
in
```
```python
self.features.named_parameters():
```

```python
v.requires_grad =
```
```python
False
```

```python
def
```
```python
forward
```
```python
(self, x)
```
```python
:
```

```python
output = self.features(x)
```

```python
return
```
```python
output
```


# 网络插值思想

为了平衡感知质量和 PSNR 等评价值，**作者提出了一个灵活且有效的方法——网络插值。**具体而言，作者首先基于 PSNR 方法训练的得到的网络 G_PSNR，然后再用基于 GAN 的网络 G_GAN 进行 finetune。

然后，对这两个网络相应的网络参数进行插值得到一个插值后的网络 G_INTERP：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnUh1CAqIGPuNfb0GGdOJ92TSc99xR3d511SnPatPjvJ6nTcb3rYGQIg/640?wx_fmt=png)

这样就可以通过 α 值来调整效果。

# 训练细节

放大倍数：4；mini-batch：16。

通过 Matlab 的 bicubic 函数对 HR 图像进行降采样得到 LR 图像。

HR patch 大小：128×128。实验发现使用大的 patch 时，训练一个深层网络效果会更好，因为一个增大的感受域会帮助模型捕捉更具有语义的信息。

**训练过程如下：**

1. 训练一个基于 PSNR 指标的模型（L1 Loss），初始化学习率：2×1e-4，每 200000 个 mini-batch 学习率除以 2；

2. 以 1 中训练的模型作为生成器的初始化。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnzpI5ED8qf9fZUDaU8ZZia0pl5WfbMPdWEgplMD22eWxQxCWIibKr0Nug/640?wx_fmt=png)

初始学习率：1e-4，并在 50k，100k，200k，300k 迭代后减半。

一个基于像素损失函数进行优化的预训练模型会帮助基于 GAN 的模型生成更符合视觉的结果，原因如下：

1. 可以避免生成器不希望的局部最优；

2. 再预训练以后，判别器所得到的输入图像的质量是相对较好的，而不是完全初始化的图像，这样会使判别器更关注到纹理的判别。

优化器：Adam (β1=0.9, β2=0.999)；交替更新生成器和判别器，直到收敛。

生成器的设置：1.16 层（基本的残差结构）；2.23层（RDDB）。

数据集：DIV2K，Flickr2K，OST（有丰富纹理信息的数据集会是模型产生更自然的结果）。

# 对比实验（4倍放大）

针对文中提到的各种改进，包括移除 BN，使用激活前特征作为感知域特征，修改 GAN 的判别条件，以及提出的 RDDB，作者做了详细的对比试验，结果如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnyKbYQdrlMOSpIPSvY3BEFTBibG7uoqvSPKd3ic9XGU8evvbytO0ghEPg/640?wx_fmt=png)

经过实验以后，**作者得出结论：**

**1. 去掉 BN：**并没有降低网络的性能，而且节省了计算资源和内存占用。而且发现当网络变深变复杂时，带 BN 层的模型更倾向于产生影响视觉效果的伪影；

**2. 使用激活前的特征：**得到的图像的亮度更准确，而且可以产生更尖锐的边缘和更丰富的细节；

**3. RaGAN：**产生更尖锐的边缘和更丰富的细节；

**4. RDDB：**更加提升恢复得到的纹理（因为深度模型具有强大的表示能力来捕获语义信息），而且可以去除噪声。

**网络插值实验**

为了平衡视觉效果和 PSNR 等性能指标，作者对网络插值参数 α 的取值进行了实验，结果如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnu3bUH7Jjb1s5b9zNmMEdB097mlbbAx9S4M9P8aU8vREErPt431rrTQ/640?wx_fmt=png)

此外，作者还对比了网络插值和图像插值的效果。图像插值即指分别由两个网络输出的图像进行插值。通过对比实验可以看到，图像插值对消除伪影的效果不如网络插值。

**与SOTA方法对比（4倍放大）**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmBzUAPP5jRLia2ucy765SLnpNt9Sz0y8ibmQtlBdfBiaeicdrUejic0myypZu92ibpiafFeQllqBpn4p5Sg/640?wx_fmt=png)

可以看到，ESRGAN 得到的图像 PSNR 值不高，但是从视觉效果上看会更好，Percpetual Index 值更小（越小越好），而且 ESRGAN 在 PIRM-SR 竞赛上也获得了第一名（在 Percpetual Index 指标上）。

# 总结

文章提出的 ESRGAN 在 SRGAN 的基础上做出了改进，包括去除 BN 层，基本结构换成 RDDB，改进 GAN 中判别器的判别目标，以及使用激活前的特征构成感知域损失函数，实验证明这些改进对提升输出图像的视觉效果都有作用。

此外，作者也使用了一些技巧来提升网络的性能，包括对残差信息的 scaling，以及更小的初始化。最后，作者使用了一种网络插值的方法来平衡输出图像的视觉效果和 PSNR 等指标值。



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****拉 票 时 间****\#**
**请为PaperWeekly打call!**

PaperWeekly 曾于 2016 年入选**“年度十大学术公众号”**（[2016年度学术公众号TOP10重磅发布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484048&idx=2&sn=92a3b20ea15453d14a3447131fb4039a&chksm=96e9dd10a19e5406abbbffd7f88896c86ef6c5ab62f838725939b19f33a794794326d4be7b64&scene=21#wechat_redirect)），并连续入围“2017 年度学术公众号”评选。

如果您喜欢 PaperWeekly，请在本文底部**点击“阅读原文”-勾选“PaperWeekly”**，为我们投上您宝贵的一票。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHknk6xkSN7XsibWpOo1m99Xoxfgd8AurorUBQDoibvuXypDvHAEW2EsguDFOia7KKKrVI6z8wtIlOw/640?wx_fmt=png)


🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
点击“阅读原文”，为“PaperWeekly”投票！


