
#ifndef __OCR_CAFFE_HPP__
#define __OCR_CAFFE_HPP__

#include <string>
#include "caffe/classification/ICNNPredict.h"
using namespace std;

#ifdef _DEBUG
#define MODELPATH "E:/OCR_Line/bin/model/"
#else
#define MODELPATH "./model/"
#endif


//#include "classification/libClassification.hpp"
//#pragma comment (lib,"libClassification.lib")

string GetPredictString(const vector<float>& fm, int idxBlank, const vector<string>& labels)
{
  string str;
  for (size_t t = 0; t < fm.size(); t++)
  {
    int idx = t;
    int label = (int)fm[idx] + 0.5f;
    if (label >= 0 && label != idxBlank)
    {
      str += labels[label];
    }
  }
  return str;
}

#if 1
struct ocr_caffe {
  ICNNPredict* pCNN;
  int wstd = 0, hstd = 0;
  vector<string> alphabets;
  int idxBlank = 0;
  ocr_caffe() {
    const char* model_folder;
    model_folder = MODELPATH"densenet-sum-blstm-full-res-blstm2/";
    model_folder = MODELPATH"densenet-sum-blstm-full-res-blstm8000";
    pCNN = CreatePredictInstance(model_folder, true);
    pCNN->GetInputImageSize(wstd, hstd);
    alphabets  = pCNN->GetLabels();
    vector<string>::const_iterator it = find(alphabets.begin(), alphabets.end(), "blank");
    if (it != alphabets.end())
      idxBlank = (int)(it - alphabets.begin());
  }
  string run(const Mat& im) {
    Mat img = im;
    int w = img.cols, h = img.rows;
	if (h*w == 0)return "";
    int w1 = hstd*w / h;
	if (w1 <10)return "";
    if (w1 != w && h != hstd)
      cv::resize(img, img, cv::Size(w1, hstd));
	img = mastbegray(img);
    //cvtColor(img, img, CV_GRAY2BGR);

    //imshow("asdfasdf", img); waitKey(-1);

    vector<int> shape;
    vector<float> pred = pCNN->GetOutputFeatureMap(img, shape);

    string strpredict0 = GetPredictString(pred, idxBlank, alphabets);
    return strpredict0;
  }
};

struct ocrnum_caffe {
  ICNNPredict* pCNN;
  int wstd = 0, hstd = 0;
  vector<string> alphabets;
  int idxBlank = 0;
  ocrnum_caffe() {
    const char* model_folder;
    model_folder = MODELPATH"densenet-sum-blstm-full-res-blstm-09X";
    pCNN = CreatePredictInstance(model_folder, true);
    pCNN->GetInputImageSize(wstd, hstd);
    alphabets = pCNN->GetLabels();
    vector<string>::const_iterator it = find(alphabets.begin(), alphabets.end(), "blank");
    if (it != alphabets.end())
      idxBlank = (int)(it - alphabets.begin());
  }
  string run(const Mat& im) {
    Mat img = im;
    int w = img.cols, h = img.rows;
	if (h*w == 0)return "";
    int w1 = hstd*w / h;
	if (w1 <10)return "";
    if (w1 != w && h != hstd)
      cv::resize(img, img, cv::Size(w1, hstd));

	img = mastbegray(img);
    //cvtColor(img, img, CV_GRAY2BGR);

    //imshow("asdfasdf", img); waitKey(-1);

    vector<int> shape;
    vector<float> pred = pCNN->GetOutputFeatureMap(img, shape);

    string strpredict0 = GetPredictString(pred, idxBlank, alphabets);
    return strpredict0;
  }
};

string run_ocrnum_caffe(const Mat& im4) {
	static ocrnum_caffe ocrnum_caffe1;
	string ss = ocrnum_caffe1.run(im4);
	return ss;
}

string run_ocr_caffe(const Mat& im4) {
	static ocr_caffe ocr_caffe1;
	string ss = ocr_caffe1.run(im4);
	return ss;
}

int ocr_caffe_init() {
	Mat mat(32, 32, CV_8UC3);
	run_ocr_caffe(mat);
	run_ocrnum_caffe(mat);
	return 0;
}

#else
struct ocr_caffe {
	string run(const Mat& im) {
		return "";
	}
};
struct ocrnum_caffe {
	string run(const Mat& im) {
		return "433022198202080916";
	}
};
#endif




#endif // __OCR_CAFFE_HPP__
