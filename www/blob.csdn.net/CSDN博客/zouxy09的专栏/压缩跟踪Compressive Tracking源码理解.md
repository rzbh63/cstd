
# 压缩跟踪Compressive Tracking源码理解 - zouxy09的专栏 - CSDN博客


2012年11月21日 22:38:19[zouxy09](https://me.csdn.net/zouxy09)阅读数：29219


**压缩跟踪Compressive Tracking源码理解**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
在前面一个介绍《Real-Time Compressive Tracking》这个paper的感知跟踪算法的博文中，我说过后面会学习下它的C++源码，但是当时因为有些事，所以就没有看了。今天，上到博客，看到一朋友在这个博文中评论说，有个地方不太明白。然后，觉得该履行自己的承诺，去学习学习源码了。所以刚才就花了几个小时去看了C++的源码，做了详细的注释。希望对大家有点帮助。在这也感谢这位朋友。当然，因为自己也刚刚接触这个领域，所以也有很多地方我也看不懂或者理解错了，也渴望大家的指导。
下面是这个算法的工程网站：里面包含了上面这篇论文、Matlab和C++版本的代码，还有测试数据、demo等。
http://www4.comp.polyu.edu.hk/~cslzhang/CT/CT.htm
之前自己学习这个《Real-Time Compressive Tracking》介绍的感知跟踪算法：
http://blog.csdn.net/zouxy09/article/details/8118360
非常感谢Kaihua等的paper《Real-Time Compressive Tracking》，非常感谢它的C++代码的编写和贡献者Yang
 Xian。
这个C++代码编写的非常简洁、清晰和漂亮。另外，经原作者提示，代码注释中不明白的地方（我打问号的地方）可以看本博文的原作者的评论。非常感谢Yang
 Xian的指导。
好了，废话不多说了。下面是自己注释的源码。因为代码编写的流程非常清晰，所以我就不总结流程了。这个工程包含三个文件：CompressiveTracker.cpp、CompressiveTracker.h和RunTracker.cpp，其中因为RunTracker.cpp和TLD算法中的run_tld.cpp差不多，我这里就不注释了，大家可以参考我之前的：
TLD（Tracking-Learning-Detection）学习与源码理解之（四）
http://blog.csdn.net/zouxy09/article/details/7893032
**下面是具体的源码：**
CompressiveTracker.h
```cpp
/************************************************************************
* File:	CompressiveTracker.h
* Brief: C++ demo for paper: Kaihua Zhang, Lei Zhang, Ming-Hsuan Yang,"Real-Time Compressive Tracking," ECCV 2012.
* Version: 1.0
* Author: Yang Xian
* Email: yang_xian521@163.com
* Date:	2012/08/03
* History:
* Revised by Kaihua Zhang on 14/8/2012
* Email: zhkhua@gmail.com
* Homepage: http://www4.comp.polyu.edu.hk/~cskhzhang/
************************************************************************/
//这是一个比较常用的C/C++杂注，只要在头文件的最开始加入这条杂注，就能够保证头文件只被插入和编译一次
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using std::vector;
using namespace cv;
//---------------------------------------------------
class CompressiveTracker
{
public:
	CompressiveTracker(void);
	~CompressiveTracker(void);
private:
	int featureMinNumRect;
	int featureMaxNumRect;
	int featureNum;  //每个box的harr特征个数（也就是弱分类器个数）
	vector<vector<Rect>> features;
	vector<vector<float>> featuresWeight;
	int rOuterPositive; //在离上一帧跟踪到的目标位置的距离小于rOuterPositive的范围内采集 正样本
	vector<Rect> samplePositiveBox;  //采集的正样本box集
	vector<Rect> sampleNegativeBox;  //采集的负样本box集
	int rSearchWindow;   //扫描窗口的大小，或者说检测box的大小
	Mat imageIntegral;   //图像的积分图
	Mat samplePositiveFeatureValue;  //采集的正样本的harr特征值
	Mat sampleNegativeFeatureValue;  //采集的负样本的harr特征值
	//对每个样本z（m维向量），它的低维表示是v（n维向量，n远小于m）。假定v中的各元素是独立分布的。
	//假定在分类器H(v)中的条件概率p(vi|y=1)和p(vi|y=0)属于高斯分布，并且可以用以下四个参数来描述：
	//分别是描述正负样本的高斯分布的均值u和方差sigma
	vector<float> muPositive;
	vector<float> sigmaPositive;
	vector<float> muNegative;
	vector<float> sigmaNegative;
	float learnRate;   //学习速率，控制分类器参数更新的步长
	vector<Rect> detectBox;  //需要检测的box
	Mat detectFeatureValue;
	RNG rng;  //随机数
private:
	void HaarFeature(Rect& _objectBox, int _numFeature);
	void sampleRect(Mat& _image, Rect& _objectBox, float _rInner, float _rOuter, int _maxSampleNum, vector<Rect>& _sampleBox);
	void sampleRect(Mat& _image, Rect& _objectBox, float _srw, vector<Rect>& _sampleBox);
	void getFeatureValue(Mat& _imageIntegral, vector<Rect>& _sampleBox, Mat& _sampleFeatureValue);
	void classifierUpdate(Mat& _sampleFeatureValue, vector<float>& _mu, vector<float>& _sigma, float _learnRate);
	void radioClassifier(vector<float>& _muPos, vector<float>& _sigmaPos, vector<float>& _muNeg, vector<float>& _sigmaNeg,
						Mat& _sampleFeatureValue, float& _radioMax, int& _radioMaxIndex);
public:
	void processFrame(Mat& _frame, Rect& _objectBox);
	void init(Mat& _frame, Rect& _objectBox);
};
```

CompressiveTracker.cpp
```cpp
#include "CompressiveTracker.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;
//------------------------------------------------
//构造函数，初始化各参数
CompressiveTracker::CompressiveTracker(void)
{
	featureMinNumRect = 2;
	featureMaxNumRect = 4;	// number of rectangle from 2 to 4
	featureNum = 50;	// number of all weaker classifiers, i.e,feature pool
	rOuterPositive = 4;	// radical scope of positive samples
	rSearchWindow = 25; // size of search window
	muPositive = vector<float>(featureNum, 0.0f);
	muNegative = vector<float>(featureNum, 0.0f);
	sigmaPositive = vector<float>(featureNum, 1.0f);
	sigmaNegative = vector<float>(featureNum, 1.0f);
	learnRate = 0.85f;	// Learning rate parameter
}
CompressiveTracker::~CompressiveTracker(void)
{
}
//通过积分图来计算采集到的每一个样本的harr特征，这个特征通过与featuresWeight来相乘
//就相当于投影到随机测量矩阵中了，也就是进行稀疏表达了。这里不明白的话，可以看下
//论文中的图二，就比较直观了。
//还有一点：实际上这里采用的不属于真正的harr特征，我博客中翻译有误。这里计算的是
//在box中采样得到的不同矩形框的灰度加权求和（当权重是负数的时候就是灰度差）
//当为了表述方便，我下面都用harr特征来描述。
//每一个样本有50个harr特征，每一个harr特征是由2到3个随机选择的矩形框来构成的，
//对这些矩形框的灰度加权求和作为这一个harr特征的特征值。
void CompressiveTracker::HaarFeature(Rect& _objectBox, int _numFeature)
/*Description: compute Haar features
  Arguments:
  -_objectBox: [x y width height] object rectangle
  -_numFeature: total number of features. The default is 50.
*/
{
	//_numFeature是一个样本box的harr特征个数，共50个。而上面说到，
	//每一个harr特征是由2到3个随机选择的矩形框（vector<Rect>()类型）来构成的。
	features = vector<vector<Rect>>(_numFeature, vector<Rect>());
	//每一个反应特征的矩形框对应于一个权重，实际上就是随机测量矩阵中相应的元素，用它来与对应的特征
	//相乘，表示以权重的程度来感知这个特征。换句话说，featuresWeight就是随机测量矩阵。
	//这个矩阵的元素的赋值看论文中的第二部分。或者也可以参考下我的博文：（呵呵，好像博文也没说清楚）
	//http://blog.csdn.net/zouxy09/article/details/8118360
	featuresWeight = vector<vector<float>>(_numFeature, vector<float>());
	
	//numRect是每个特征的矩形框个数还是论文中说的随机测量矩阵中的s？还有兼备两种功能？
	//论文中说s取2或者3时，矩阵就满足Johnson-Lindenstrauss推论。
	int numRect;
	Rect rectTemp;
	float weightTemp;
      
	for (int i=0; i<_numFeature; i++)
	{
		//如何生成服从某个概率分布的随机数（或者说 sample）的问题。
		//比如，你想要从一个服从正态分布的随机变量得到 100 个样本，那么肯定抽到接近其均值的样本的
		//概率要大许多，从而导致抽到的样本很多是集中在那附近的。
		//rng.uniform()返回一个从[ 1，2）范围均匀采样的随机数，即在[ 1，2）内服从均匀分布（取不同值概率相同）
		//那么下面的功能就是得到[2，4）范围的随机数，然后用cvFloor返回不大于参数的最大整数值，那要么是2，要么是3。
		numRect = cvFloor(rng.uniform((double)featureMinNumRect, (double)featureMaxNumRect));
	    
		//int c = 1;
		for (int j=0; j<numRect; j++)
		{
			//我在一个box中随机生成一个矩形框，那和你这个box的x和y坐标就无关了，但我必须保证我选择
			//的这个矩形框不会超出你这个box的范围啊，是吧
			//但这里的3和下面的2是啥意思呢？我就不懂了，个人理解是为了避免这个矩形框太靠近box的边缘了
			//要离边缘最小2个像素，不知道这样理解对不对，恳请指导
			rectTemp.x = cvFloor(rng.uniform(0.0, (double)(_objectBox.width - 3)));
			rectTemp.y = cvFloor(rng.uniform(0.0, (double)(_objectBox.height - 3)));
			//cvCeil 返回不小于参数的最小整数值
			rectTemp.width = cvCeil(rng.uniform(0.0, (double)(_objectBox.width - rectTemp.x - 2)));
			rectTemp.height = cvCeil(rng.uniform(0.0, (double)(_objectBox.height - rectTemp.y - 2)));
			//保存得到的特征模板。注意哦，这里的矩形框是相对于box的相对位置哦，不是针对整幅图像的哦
			features[i].push_back(rectTemp);
			
			//weightTemp = (float)pow(-1.0, c);
			//pow(-1.0, c)也就是-1的c次方，而c随机地取0或者1，也就是说weightTemp是随机的正或者负。
			//随机测量矩阵中，矩阵元素有三种，sqrt(s)、-sqrt(s)和零。为正和为负的概率是相等的，
			//这就是为什么是[2，4）均匀采样的原因，就是取0或者1概率一样。
			//但是这里为什么是sqrt(s)分之一呢？还有什么时候是0呢？论文中是0的概率不是挺大的吗？
			//没有0元素，哪来的稀疏表达和压缩呢？不懂，恳请指导！（当然稀疏表达的另一个好处
			//就是只需保存非零元素。但这里和这个有关系吗？）
			weightTemp = (float)pow(-1.0, cvFloor(rng.uniform(0.0, 2.0))) / sqrt(float(numRect));
            
			//保存每一个特征模板对应的权重
			featuresWeight[i].push_back(weightTemp);
           
		}
	}
}
//在上一帧跟踪的目标box的周围采集若干正样本和负样本，来初始化或者更新分类器的
void CompressiveTracker::sampleRect(Mat& _image, Rect& _objectBox, float _rInner, float _rOuter, int _maxSampleNum, vector<Rect>& _sampleBox)
/* Description: compute the coordinate of positive and negative sample image templates
   Arguments:
   -_image:        processing frame
   -_objectBox:    recent object position 
   -_rInner:       inner sampling radius
   -_rOuter:       Outer sampling radius
   -_maxSampleNum: maximal number of sampled images
   -_sampleBox:    Storing the rectangle coordinates of the sampled images.
*/
{
	int rowsz = _image.rows - _objectBox.height - 1;
	int colsz = _image.cols - _objectBox.width - 1;
	//我们是在上一帧跟踪的目标box的周围采集正样本和负样本的，而这个周围是通过以
	//这个目标为中心的两个圆来表示，这两个圆的半径是_rInner和_rOuter。
	//我们在离上一帧跟踪的目标box的小于_rInner距离的范围内采集正样本，
	//在大于_rOuter距离的范围内采集负样本（论文中还有一个上界，但好像
	//这里没有，其实好像也没什么必要噢）
	float inradsq = _rInner*_rInner;
	float outradsq = _rOuter*_rOuter;
	
	int dist;
	//这四个是为了防止采集的框超出图像范围的，对采集的box的x和y坐标做限制
	int minrow = max(0,(int)_objectBox.y-(int)_rInner);
	int maxrow = min((int)rowsz-1,(int)_objectBox.y+(int)_rInner);
	int mincol = max(0,(int)_objectBox.x-(int)_rInner);
	int maxcol = min((int)colsz-1,(int)_objectBox.x+(int)_rInner);
    	
	
	int i = 0;
	//分母相当于x能采集的范围乘以y能采集的范围，也就是可以采集的最大box个数，
	//那么_maxSampleNum（我们需要采集的box的最大个数）肯定得小于或者等于它。
	//那这个prob是干嘛的呢？到下面用到它的地方说
	float prob = ((float)(_maxSampleNum))/(maxrow-minrow+1)/(maxcol-mincol+1);
	int r;
	int c;
    
    _sampleBox.clear();//important
    Rect rec(0,0,0,0);
	for( r=minrow; r<=(int)maxrow; r++ )
		for( c=mincol; c<=(int)maxcol; c++ ){
			//计算生成的box到目标box的距离
			dist = (_objectBox.y-r)*(_objectBox.y-r) + (_objectBox.x-c)*(_objectBox.x-c);
			//后两个条件是保证距离需要在_rInner和_rOuter的范围内
			//那么rng.uniform(0.,1.) < prob 这个是干嘛的呢？
			//连着上面看，如果_maxSampleNum大于那个最大个数，prob就大于1，这样，
			//rng.uniform(0.,1.) < prob这个条件就总能满足，表示在这个范围产生的
			//所以box我都要了（因为我本身想要更多的，但是你给不了我那么多，那么你能给的，我肯定全要了）。
			//那如果你给的太多了，我不要那么多，也就是prob<1，那我就随机地跳几个走好了
			if( rng.uniform(0.,1.) < prob && dist < inradsq && dist >= outradsq ){
                rec.x = c;
				rec.y = r;
				rec.width = _objectBox.width;  //没有做尺度不变？至此至终box的大小都没变化
				rec.height= _objectBox.height;
				
                _sampleBox.push_back(rec);				
				
				i++;
			}
		}
	
		_sampleBox.resize(i);
		
}
//这个sampleRect的重载函数是用来在上一帧跟踪的目标box的周围（距离小于_srw）采集若干box来待检测。
//与上面的那个不一样，上面那个是在这一帧已经检测出目标的基础上，采集正负样本来更新分类器的。
//上面那个属于论文中提到的算法的第四个步骤，这个是第一个步骤。然后过程差不多，没什么好说的了
void CompressiveTracker::sampleRect(Mat& _image, Rect& _objectBox, float _srw, vector<Rect>& _sampleBox)
/* Description: Compute the coordinate of samples when detecting the object.*/
{
	int rowsz = _image.rows - _objectBox.height - 1;
	int colsz = _image.cols - _objectBox.width - 1;
	float inradsq = _srw*_srw;		
	int dist;
	int minrow = max(0,(int)_objectBox.y-(int)_srw);
	int maxrow = min((int)rowsz-1,(int)_objectBox.y+(int)_srw);
	int mincol = max(0,(int)_objectBox.x-(int)_srw);
	int maxcol = min((int)colsz-1,(int)_objectBox.x+(int)_srw);
	int i = 0;
	int r;
	int c;
	Rect rec(0,0,0,0);
    _sampleBox.clear();//important
	for( r=minrow; r<=(int)maxrow; r++ )
		for( c=mincol; c<=(int)maxcol; c++ ){
			dist = (_objectBox.y-r)*(_objectBox.y-r) + (_objectBox.x-c)*(_objectBox.x-c);
			if( dist < inradsq ){
				rec.x = c;
				rec.y = r;
				rec.width = _objectBox.width;
				rec.height= _objectBox.height;
				_sampleBox.push_back(rec);				
				i++;
			}
		}
	
		_sampleBox.resize(i);
}
// Compute the features of samples
//通过积分图来计算采集到的每一个样本的harr特征，这个特征通过与featuresWeight来相乘
//就相当于投影到随机测量矩阵中了，也就是进行稀疏表达了。这里不明白的话，可以看下
//论文中的图二，就比较直观了。所以这里得到的是：每个样本的稀疏表达后的harr特征。
//还有一点：实际上这里采用的不属于真正的harr特征，我博客中翻译有误。这里计算的是
//在box中采样得到的不同矩形框的灰度加权求和
void CompressiveTracker::getFeatureValue(Mat& _imageIntegral, vector<Rect>& _sampleBox, Mat& _sampleFeatureValue)
{
	int sampleBoxSize = _sampleBox.size();
	_sampleFeatureValue.create(featureNum, sampleBoxSize, CV_32F);
	float tempValue;
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	for (int i=0; i<featureNum; i++)
	{
		for (int j=0; j<sampleBoxSize; j++)
		{
			tempValue = 0.0f;
			for (size_t k=0; k<features[i].size(); k++)
			{
				//features中保存的特征模板（矩形框）是相对于box的相对位置的，
				//所以需要加上box的坐标才是其在整幅图像中的坐标
				xMin = _sampleBox[j].x + features[i][k].x;
				xMax = _sampleBox[j].x + features[i][k].x + features[i][k].width;
				yMin = _sampleBox[j].y + features[i][k].y;
				yMax = _sampleBox[j].y + features[i][k].y + features[i][k].height;
				//通过积分图来快速计算一个矩形框的像素和，积分图不了解的话，可以看下我的这个博文：
				//http://blog.csdn.net/zouxy09/article/details/7929570
				//那么这里tempValue就是经过稀释矩阵加权后的灰度和了。
				//每一个harr特征是由2到3个矩形框来构成的，对这些矩形框的灰度加权求和
				//作为这一个harr特征的特征值。然后一个样本有50个harr特征
				tempValue += featuresWeight[i][k] * 
					(_imageIntegral.at<float>(yMin, xMin) +
					_imageIntegral.at<float>(yMax, xMax) -
					_imageIntegral.at<float>(yMin, xMax) -
					_imageIntegral.at<float>(yMax, xMin));
			}
			_sampleFeatureValue.at<float>(i,j) = tempValue;
		}
	}
}
// Update the mean and variance of the gaussian classifier
//论文中是通过用高斯分布去描述样本的每一个harr特征的概率分布的。高斯分布就可以通过期望和方差
//两个参数来表征。然后通过正负样本的每一个harr特征高斯概率分布的对数比值，来构建分类器决策
//该box属于目标还是背景。这里计算新采集到的正负样本的特征的期望和标准差，并用其来更新分类器
void CompressiveTracker::classifierUpdate(Mat& _sampleFeatureValue, vector<float>& _mu, vector<float>& _sigma, float _learnRate)
{
	Scalar muTemp;
	Scalar sigmaTemp;
    
	for (int i=0; i<featureNum; i++)
	{
		//计算所有正样本或者负样本的某个harr特征的期望和标准差
		meanStdDev(_sampleFeatureValue.row(i), muTemp, sigmaTemp);
	   
	    //这个模型参数更新的公式见论文的公式6
		_sigma[i] = (float)sqrt( _learnRate*_sigma[i]*_sigma[i]	+ (1.0f-_learnRate)*sigmaTemp.val[0]*sigmaTemp.val[0] 
		+ _learnRate*(1.0f-_learnRate)*(_mu[i]-muTemp.val[0])*(_mu[i]-muTemp.val[0]));	// equation 6 in paper
		_mu[i] = _mu[i]*_learnRate + (1.0f-_learnRate)*muTemp.val[0];	// equation 6 in paper
	}
}
// Compute the ratio classifier 
void CompressiveTracker::radioClassifier(vector<float>& _muPos, vector<float>& _sigmaPos, vector<float>& _muNeg, vector<float>& _sigmaNeg,
										 Mat& _sampleFeatureValue, float& _radioMax, int& _radioMaxIndex)
{
	float sumRadio;
	//FLT_MAX是最大的浮点数的宏定义，那么-FLT_MAX就是最小的浮点数了
	//这个是拿来存放 那么多box中最大的分类分数的
	_radioMax = -FLT_MAX; 
	//这个是对应于上面那个，是存放分类分数最大的那个box的
	_radioMaxIndex = 0;
	float pPos;
	float pNeg;
	int sampleBoxNum = _sampleFeatureValue.cols;
	for (int j=0; j<sampleBoxNum; j++)  //每帧采样得到的需要检测的box
	{
		sumRadio = 0.0f;
		for (int i=0; i<featureNum; i++)  //每个box的需要匹配的特征数
		{
			//计算每个特征的概率，特征分布近似于高斯分布，故将描述该特征的均值和标准差代入高斯模型就可以
			//得到，分别在正样本和负样本的基础上，出现该特征的概率是多少。如果正样本时候的概率大，那么
			//我们就说，这个特征对应的样本是正样本。数学上比较大小，就是减法或者除法了，这里是取对数比值
			pPos = exp( (_sampleFeatureValue.at<float>(i,j)-_muPos[i])*(_sampleFeatureValue.at<float>(i,j)-_muPos[i]) / -(2.0f*_sigmaPos[i]*_sigmaPos[i]+1e-30) ) / (_sigmaPos[i]+1e-30);
			pNeg = exp( (_sampleFeatureValue.at<float>(i,j)-_muNeg[i])*(_sampleFeatureValue.at<float>(i,j)-_muNeg[i]) / -(2.0f*_sigmaNeg[i]*_sigmaNeg[i]+1e-30) ) / (_sigmaNeg[i]+1e-30);
			
			//paper的方程4：计算分类结果，得到一个分数，这个分数是由一个样本或者box的50个特征（弱分类）
			//进入分类器分类得到的结果总和（强分类？）。表征的是目前这个box的特征属于正样本（目标）的
			//可能性大小。哪个分数最大，自然我就认为你是目标了。（当然，在具体应用中需要加一些策略去
			//改善误跟踪的情况。例如如果最高的分数都达不到一个阈值，那就不存在目标等）
			sumRadio += log(pPos+1e-30) - log(pNeg+1e-30);	// equation 4
		}
		if (_radioMax < sumRadio) //拿到最大的分数和相应的box索引
		{
			_radioMax = sumRadio;
			_radioMaxIndex = j;
		}
	}
}
//传入第一帧和要跟踪的目标box（由文件读入或者用户鼠标框选），来初始化分类器
void CompressiveTracker::init(Mat& _frame, Rect& _objectBox)
{
	// compute feature template
	//计算box的harr特征模板，先存着
	HaarFeature(_objectBox, featureNum);
	// compute sample templates
	//因为这是第一帧，目标box是由由文件读入或者用户鼠标框选的，是已知的，
	//所以我们通过在这个目标box周围，采集正样本和负样本来初始化我们的分类器
	sampleRect(_frame, _objectBox, rOuterPositive, 0, 1000000, samplePositiveBox);
	sampleRect(_frame, _objectBox, rSearchWindow*1.5, rOuterPositive+4.0, 100, sampleNegativeBox);
	//计算积分图，用以快速的计算harr特征
	integral(_frame, imageIntegral, CV_32F);
	//通过上面的积分图，计算我们采样到的正负样本的box的harr特征
	getFeatureValue(imageIntegral, samplePositiveBox, samplePositiveFeatureValue);
	getFeatureValue(imageIntegral, sampleNegativeBox, sampleNegativeFeatureValue);
	
	//通过上面的正负样本的特征来初始化分类器
	classifierUpdate(samplePositiveFeatureValue, muPositive, sigmaPositive, learnRate);
	classifierUpdate(sampleNegativeFeatureValue, muNegative, sigmaNegative, learnRate);
}
//传入上一帧跟踪到的box，来处理新的一帧
void CompressiveTracker::processFrame(Mat& _frame, Rect& _objectBox)
{
	// predict
	//在上一帧跟踪到的boxbox周围，采集需要检测的box框
	sampleRect(_frame, _objectBox, rSearchWindow, detectBox);
	//计算这一帧的积分图
	integral(_frame, imageIntegral, CV_32F);
	//用积分图来计算上面采集到的每个box的harr特征
	getFeatureValue(imageIntegral, detectBox, detectFeatureValue);
	int radioMaxIndex;
	float radioMax;
	//对上面的每个box进行匹配分类
	radioClassifier(muPositive, sigmaPositive, muNegative, sigmaNegative, detectFeatureValue, radioMax, radioMaxIndex);
	//得到分数最高的那个目标box
	_objectBox = detectBox[radioMaxIndex];
	// update
	//在新跟踪到的这个目标box的周围，采集正样本和负样本来更新我们的分类器
	sampleRect(_frame, _objectBox, rOuterPositive, 0.0, 1000000, samplePositiveBox);
	sampleRect(_frame, _objectBox, rSearchWindow*1.5, rOuterPositive+4.0, 100, sampleNegativeBox);
	
	//通过上面的积分图，计算我们采样到的正负样本的box的harr特征
	getFeatureValue(imageIntegral, samplePositiveBox, samplePositiveFeatureValue);
	getFeatureValue(imageIntegral, sampleNegativeBox, sampleNegativeFeatureValue);
	
	//通过上面的正负样本的特征来更新我们的分类器
	classifierUpdate(samplePositiveFeatureValue, muPositive, sigmaPositive, learnRate);
	classifierUpdate(sampleNegativeFeatureValue, muNegative, sigmaNegative, learnRate);
}
```


