# Kaldi笔记

 

# Kaldi

Kaldi是一个语音识别的工具包。它由Daniel Povey于2009年创建。

> Daniel Povey，Johns Hopkins University副教授。
>  个人主页：
>  <http://danielpovey.com/>

官网：<https://github.com/kaldi-asr/kaldi>

文档：<http://kaldi-asr.org/doc/>

此外，Daniel的主页上也有一些教程：<http://danielpovey.com/kaldi-lectures.html>

数据：<http://www.openslr.org/>

这是Daniel Povey为了Kaldi中的示例所创建的资源网站。它还有个国内的镜像：

<http://cn-mirror.openslr.org/>

参考：<https://www.eleanorchodroff.com/tutorial/kaldi/kaldi-intro.html>

这是Eleanor Chodroff写的kaldi指南。

> Eleanor Chodroff，New York University本科（2012）+Johns Hopkins University硕博（2014，2017）。现为Northwestern University博士后。

<https://blog.csdn.net/wbgxx333>

一个语音识别+Kaldi的blog。这个博主还写了一个[《kaldi资料归纳和总结》](https://www.baidu.com/s?wd=%E3%80%8Akaldi%E8%B5%84%E6%96%99%E5%BD%92%E7%BA%B3%E5%92%8C%E6%80%BB%E7%BB%93%E3%80%8B&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的文章，可加入博主的QQ群，在群文件中获得。

<http://jrmeyer.github.io/asr/2016/12/15/DNN-AM-Kaldi.html>

How to Train a Deep Neural Net Acoustic Model with Kaldi

# 编译

1.检查依赖。

```shell
cd tools
extras/check_dependencies.sh
```

根据提示，缺什么就装什么。

2.编译tools文件夹

```shell
make -j 4
```

3.编译src文件夹

```shell
cd ../src
sudo apt install libatlas-base-dev
./configure --shared
make depend -j 4
make -j 4
```

# 运行示例

示例放在egs文件夹下，目前已经有很多示例了。这里以thchs30为例进行介绍。这里主要参考了下文：

<https://blog.csdn.net/shichaog/article/details/73655628>

kaldi在线中文识别系统搭建

thchs30下有个s5文件夹。Daniel教程中提到，在原来的老示例中还有s1、s2这样的文件夹，因此这个5姑且可以看成版本号吧。

**Kaldi只是一个工具包，而非框架。**因此，它并不能直接运行，而只能被别人调用。

在示例中，一般是通过run.sh对Kaldi的功能进行调用。

1.下载数据

<http://cn-mirror.openslr.org/18/>

[2.修改cmd.sh](http://2.xn--cmd-5n0el78g.sh)。

```shell
export train_cmd=run.pl
export decode_cmd="run.pl --mem 4G"
export mkgraph_cmd="run.pl --mem 8G"
export cuda_cmd="run.pl --gpu 1"
#export train_cmd=queue.pl
#export decode_cmd="queue.pl --mem 4G"
#export mkgraph_cmd="queue.pl --mem 8G"
#export cuda_cmd="queue.pl --gpu 1"
```

3.修改run.sh中的数据路径，并运行之。

# 不成功的探险

上文中被替换的queue.pl实际上是run.pl的分布式计算版本，用到了SLURM等技术。

```shell
sudo apt install slurm-wlm-torque slurm-wlm-doc
```

SLURM和Torque都是job scheduler。他们的官网分别是：

<https://slurm.schedmd.com/>

<http://www.adaptivecomputing.com/products/open-source/torque/>

3.在浏览器中运行/usr/share/doc/slurm-wlm-doc/html/configurator.html以生成slurm.conf文件，然后把该文件保存到/etc/slurm-llnl下。

但是遗憾的是，由于我填的不对，最终没有走通这条路。

LLNL是Lawrence Livermore National Laboratory的简称，它上面有几篇文章不错：

<https://computing.llnl.gov/tutorials/parallel_comp/>

Introduction to Parallel Computing

<https://computing.llnl.gov/tutorials/linux_clusters/>

Linux Clusters Overview

# 目录结构

kaldi的调用层次可分为三级：

1.**脚本**。包括shell脚本和perl脚本。主要在egs/wsj文件夹下。

2.**工具**。一些可执行文件。在src/XXXbin文件夹下。

3.**算法实现**。在src/XXX文件夹下。

工具和算法实现基本都是C/C++写的。外部工具在tools文件夹下，主要是openfst、sctk、sph2pipe。

# 术语

LM：language model

KWS：Keyword Search

CMVN：cepstral mean and variance

G2P：Grapheme-to-Phoneme

WSJ：Wall Street Journal

# OpenFst

OpenFst是一个构造、合并、优化和搜索加权有限状态机FST的库。

官网：

<http://www.openfst.org/>

# SCTK

Speech Recognition Scoring Toolkit是NIST（National Institute of Standards and Technology, 美国国家标准与技术协会）提出的一套工具集。

官网：

<https://github.com/usnistgov/SCTK>

以下网页包含了SCTK支持的几种数据格式：trn, txt, stm, ctm。这在Kaldi中用的比较多。

<http://www1.icsi.berkeley.edu/Speech/docs/sctk-1.2/infmts.htm>

# sph2pipe

Kaldi里的音频默认是16k采样率，16bits，单通道。如果是其他参数，需要根据配置来修改。此外，kaldi数据处理部分还有个音量跟语速的脚本，这部分在kaldi里通过sox来实现的。

Kaldi里有很大一部分数据是LDC的，比如timit，rm，wsj等。它们虽然是wave的格式，但其实不是真正的wav格式，其实是nist的SPHERE格式，kaldi里通过sph2pipe这个来把格式转成真正的wave格式。

如果有人要在windows下看这些音频，可以在linux下通过sph2pipe转换下，或者有个叫voicebox的matlab程序里有个readsph的程序来转下。

此外，kaldi里librispeech其实是FLAC格式，这是一个无损的格式，也需要通过flac命令来转成wave来处理。

官网：

<https://www.ldc.upenn.edu/language-resources/tools/sphere-conversion-tools>

# SRILM

SRILM - The SRI Language Modeling Toolkit是由SRI International提出的一套工具集，主要用于创建和使用统计语言模型。

官网：

<http://www.speech.sri.com/projects/srilm/>

> SRI International是一家非营利的研究机构，创建于1946年。SRI是Stanford Research Institute的缩写。

thchs30中的lm文件（例如lm_phone/phone.3gram.lm）就是由SRILM生成的。

参考：

<http://www.52nlp.cn/language-model-training-tools-srilm-details>

语言模型训练工具SRILM详解

# 命令行粗解

kaldi的命令行脚本之所以不好读，主要在于它有一套自己的语法。

```
head -1 $featdir/raw_mfcc_train.1.scp | copy-feats scp:- ark:- | copy-feats ark:- ark,t:- | head
```

上面是一个典型的kaldi脚本的片段。可以看出kaldi命令是一个典型的pipeline结构，用`|`作为命令间的分隔符，这和一般的Linux shell是一致的。比较让人困惑的是scp和ark。

.ark（archive）是数据文件，可以是text或binary（默认）格式。

.scp（script）是描述文件，记录对应ark的路径，它是text-only的格式的。

.scp相当于C语言的指针，而.ark相当于指针指向的内容。

`ark,t:-`中的t是IO描述符，IO描述符分为读和写两大类，t是读描述符，表示text。

而`-`是文件描述符，`-`表示标准输入输出设备。它也可以是其他命令的输出，例如：

```
ark:gunzip -c $srcdir/fsts.JOB.gz
```

# thchs30之run.sh粗解

1.数据准备。

最好是那种标注了发音开始和结束时间的语料。但不幸的是，绝大多数开源语料都不满足该条件，必须由算法搞定发音和标签的时间对齐问题。

如果要做声纹识别的话，则语料中还必须有speaker ID。

2.训练monophone模型。

monophone是指那种不包含前后音节的上下文无关的音节模型。它也是后面构建更复杂的上下文相关音节模型的基础。

3.强制对齐（Forced Alignment）。

使用steps/align_si.sh。这里一般使用Viterbi training算法替代传统的Baum-Welch算法。前者的计算效率更高，但精度不如后者。对齐操作可以有效提升标签和语音的吻合度，为后续训练提供便利。

论文：

《Comparative Study of the Baum-Welch and Viterbi Training Algorithms Applied to Read and Spontaneous Speech Recognition》

《Comparative Analysis of Viterbi Training and Maximum Likelihood Estimation for HMMs》

4.用上一步的结果，训练tri1模型（三因素训练）。

这一步很关键，整个thchs30的训练流程，都是用粗糙模型，训练更精细的模型。

5.训练tri2模型（LDA-MLLT特征变换）。

6.训练tri3模型（Speaker Adapted Training，SAT）。主要用到了fMLLR算法。这一步之后的强制对齐使用steps/align_fmllr.sh。

7.训练tri4模型。这一步不再有feature-space的变换，而是对之前特征的综合，比如使用GMM算法。

8.训练DNN模型。

9.被噪声干扰的语音可以使用基于深度自动编码器（DAE）的噪声消除方法。这一步是可选的。

参考：

<https://blog.csdn.net/Anymake_ren/article/details/52275412>

kaldi中文语音识别thchs30模型训练代码功能和配置参数解读

<https://blog.csdn.net/snowdroptulip/article/details/78943748>

运行thchs30

# 数据格式

## text

```
utt_id WORD1 WORD2 WORD3 WORD4 ...
110236_20091006_82330_F_0001 I'M WORRIED ABOUT THAT
110236_20091006_82330_F_0002 AT LEAST NOW WE HAVE THE BENEFIT
110236_20091006_82330_F_0003 DID YOU EVER GO ON STRIKE
```

## segments

```
utt_id file_id start_time end_time
110236_20091006_82330_F_001 110236_20091006_82330_F 0.0 3.44
110236_20091006_82330_F_002 110236_20091006_82330_F 4.60 8.54
110236_20091006_82330_F_003 110236_20091006_82330_F 9.45 12.05
110236_20091006_82330_F_004 110236_20091006_82330_F 13.29 16.13
```

## wav.scp

```
file_id path/file
110236_20091006_82330_F path/110236_20091006_82330_F.wav
111138_20091215_82636_F path/111138_20091215_82636_F.wav
111138_20091217_82636_F path/111138_20091217_82636_F.wav
```

## utt2spk

```
utt_id spkr_id
110236_20091006_82330_F_0001 110236
110236_20091006_82330_F_0002 110236
110236_20091006_82330_F_0003 110236
```

## spk2utt

```
spkr_id utt_id1 utt_id2 utt_id3
```

## lexicon.txt

lexicon.txt就是发声词典。

```
WORD        W ER D
LEXICON     L EH K S IH K AH N
```

# ARPA文件格式

SRILM生成的lm文件是ARPA文件格式。

```
\data\
ngram 1=64000
ngram 2=522530
ngram 3=173445

\1-grams:
-5.24036     'cause  -0.2084827
-4.675221    'em     -0.221857
-4.989297    'n      -0.05809768
-5.365303    'til    -0.1855581
```

这个文件的头部比较好懂，不解释。

1-grams部分中每一条包括了3项，其中第二项显然是条目文本本身。

第一项表示ngram的条件概率，即$P(word_N∣word_1，word_2，…，word_{N−1})$。

第三项表示回退（backoff）概率。

下面我们举例说明条件概率和回退概率的用法。

假设arpa的最高元是3元，则句子ABCDEF发生的概率为：

$P(ABCDEF)=P(A)∗P(B∣A)∗P(C∣AB)∗P(D∣BC)∗P(E∣CD)∗(F∣DE)$

其中，P(A)通过访问arpa中的1-grams项获得，$P(B∣A)$通过访问2-grams项获得，其他的概率通过访问3-grams项获得。

P(A)的计算直接找对应A的文法项获得概率即可。如果2-grams中存在“A B”项，$P(B∣A)$

的概率也很容易获得，但是如果“A B”词组在2-grams中不存在，我们就需要利用回退概率计算$P(B∣A)$，计算公式为$P(B∣A)=α(A)∗P(B)$，其中的$α(A)$就表示A的回退概率。