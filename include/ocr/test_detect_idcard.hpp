


#include "std/time_c.h"
#include "utime.h"
#include "detect_idcard.hpp"
#include "parser/cjson.hpp"
#include "idcard.h"
#include "std/iconv_c.h"
#include "std/dir_c.h"
#include "wstd/string.hpp"
#include "ocr_caffe.hpp"
using namespace wstd;


#if 0
#include "ocr_char.hpp"
#endif

template <typename T>
int argmax(int n, const T* arr) {
  if (n > 0) {
    int j = 0;
    for (int i = 1; i < n; ++i) {
      if (arr[i] > arr[j]) j = i;
    }
    return j;
  }
  return -1;
}

string g_idcard;
string g_imgfn;

#if 0
#include "hanz_charset.h"
#define DEF(a, b, c, d, e, f)   L##a L##c L##e L##f
static const wchar_t hanz_charset[] = HANZ_CHARSET_DEF(DEF);
#undef DEF
#endif

#ifdef _OCR_CHAR_HPP_
ocr_char_recog ocr_c;
ocr_hans_recog ocr_h;
#endif

struct ocr_seghans {
  cv::Ptr<cv::MSER> mesr1;
  Mat color_edge;
  vector<Rect> rrect2;

  ocr_seghans() {
    cjson_Clear;
    mesr1 = cv::MSER::create(2, 80, 300, 0.2, 0.3);
  }
  char idnumber[20];
  wchar_t whans[256];
  char hans[256];
  int run_h(const Mat& src) {
    Mat gray;
    Mat bin;
    int wlen = 0;
    hans[0] = 0;
    whans[0] = 0;
    std::vector<cv::Rect> bboxes1;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
    color_edge = src.clone();
    for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
      // 
      const std::vector<cv::Point>& p = regContours[i];
      Rect rc = bboxes1[i];
      if (1) {
        //RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //cv::rectangle(color_edge, bboxes1[i], color, 1);
      }
    }
    std::vector<vector<cv::Rect> > rrects2;
    if (bboxes1.size() > 0) {
      vector<int> labels;
      vector<int> cnt;
      int nclasses = cv::partition(bboxes1, labels, [](const cv::Rect& r1, const cv::Rect& r2) {
        double hd = 0.7;
        double a1 = r1.x;
        double b1 = r1.x + r1.width;
        double a2 = r2.x;
        double b2 = r2.x + r2.width;
        double l = MAX(a1, a2);
        double r = MIN(b1, b2);
        double d = r - l;
        double u = r1.width + r2.width - d;
        u = MIN(r1.width, r2.width);
        double iou = d / u;
        return iou > hd;
      });
      cnt.assign(nclasses, 0);
      for (int i = 0; i < labels.size(); ++i) {
        int j = labels[i];
        ++cnt[j];
      }
      std::vector<int> index = argsort(cnt);
      //nclasses = min(nclasses, 15);
      for (int i = 0; i < nclasses; ++i) {
        int idx = index[i];
        int c = cnt[idx];
        if (c > 3) {
          vector<Rect> rrects;
          for (int j = 0; j < bboxes1.size(); ++j) {
            if (labels[j] == idx) {
              rrects.push_back(bboxes1[j]);
            }
          }
          rrects2.push_back(rrects);
        }
      }
      int avgh = 0;
      int avgw = 0;
      nclasses = rrects2.size();
      vector<RotatedRect> rrect3(nclasses);
      for (int i = 0; i < nclasses; ++i) {
        RotatedRect r = minAreaRect(rrects2[i]);
        r = curr(r);
        rrect3[i] = r;
        //Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawRotatedRect(color_edge, r, color, 1);
        Rect boundRect = r.boundingRect();
        avgh += boundRect.height;
        avgw += boundRect.height;
      }
      sort(rrect3.begin(), rrect3.end(), [](const RotatedRect& a, const RotatedRect& b) {
        return a.center.x < b.center.x;
      });
      nclasses = rrect3.size();
      int j = 0;
      vector<Rect> rects1;
      for (int i = 0; i < nclasses; ++i) {
        Rect r = rrect3[i].boundingRect();
        int n = int(r.width *1.9 / r.height);
        if (0 && r.height>src.rows*0.5 && n>1) {
          for (int j = 0; j < n; ++j) {
            Rect rr = rectSplitH(r, j, n);
            rects1.push_back(rr);
          }
        }
        else {
          rects1.push_back(r);
        }
      }
      nclasses = rects1.size();
      for (int i = 0; i < nclasses; ++i) {
        Rect boundRect = rects1[i];
        //rrect2[j++] = boundRect;
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::rectangle(color_edge, boundRect, color, 1);
        boundRect = rectExt(boundRect, 4, 4, src.rows, src.cols);

        if (boundRect.width > 0 && boundRect.height > 0) {
          //Scalar color = Scalar(230, 0, 0);
          //rectangle(color_edge, boundRect, color, 1);
#ifdef _OCR_CHAR_HPP_
          Mat im4 = src(boundRect).clone();
          Mat imchar = gray(boundRect).clone();
          if (boundRect.height > boundRect.width*1.2) {
            static const wchar_t char_charset[] = L"0123456789X";
            int idx = ocr_c.run(imchar.data, imchar.rows, imchar.cols, imchar.channels());
            whans[j++] = char_charset[idx];
          }
          else {
            int idx = ocr_h.run(imchar.data, imchar.rows, imchar.cols, imchar.channels());
            //assert(idx < strlen(chs));
            whans[j++] = hanz_charset[idx];
          }
          //imshow("imchar", imchar); waitKey(-1);
#endif
        }
      }
      wlen = j;
      whans[j] = 0;
      int len = iconv_c(ICONV_UCS2LE, ICONV_GB2312, (char*)whans, j * 2, hans, 256);
      hans[len] = 0;
      //printf("%s\n", hans);
      //imshow("color_edge", color_edge); waitKey(-1);
    }
    return wlen;
  }
};

Mat mesr_bin(const Mat& mat) {
	cv::Ptr<cv::MSER> mesr1;
	std::vector<std::vector<cv::Point> > regContours;
	std::vector<cv::Rect> bboxes1;
	mesr1 = cv::MSER::create(2, 50, 200, 0.2, 0.3);
	Mat gray = mastbegray(mat);
	mesr1->detectRegions(gray, regContours, bboxes1);
	cv::Mat mserMapMat = cv::Mat::zeros(mat.size(), CV_8UC1);
	const Mat& src = mat;
	for (int i = (int)regContours.size() - 1; i >= 0; i--)
	{
		// 根据检测区域点生成mser+结果
		const std::vector<cv::Point>& p = regContours[i];
		Rect rc = bboxes1[i];
		//if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
		RotatedRect r(Point2f(rc.x + rc.width / 2, rc.y + rc.height / 2), Size2f(rc.width, rc.height), 0);
		r = minAreaRect(p);
		//int k = 10;
		//if (rr.size.height < k || rr.size.width < k) { continue; }
		double aa = r.size.width*1. / r.size.height;
		double d = ptdist(r.center, Point(src.cols*0.8, src.rows*0.4))*1. / src.cols;
		int angle = mymodi(r.angle - 90, 90);
		if (aa>0.7 && r.size.width>5 && r.size.height>5) {
			RotatedRect rectPoint = minAreaRect(p);
			//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//drawRotatedRect(color_edge, rectPoint, color, 1);
			//vrr.push_back(r);
			for (int j = 0; j < (int)p.size(); j++)
			{
				cv::Point pt = p[j];
				//if (ptInRect(pt.x, pt.y, r))
				mserMapMat.at<unsigned char>(pt) = 255;
			}
		}
	}
	return mserMapMat;
}

struct ocr_segchar {
  cv::Ptr<cv::MSER> mesr1;
  Mat color_edge;
  vector<Rect> rrect2;

  ocr_segchar() {
    cjson_Clear;
    mesr1 = cv::MSER::create(2, 50, 200, 0.2, 0.3);
  }
  Rect idnumber_rect;
  int run(const Mat& src, int iline) {
    Mat gray;
    Mat bin;
    std::vector<cv::Rect> bboxes1;
	idnumber_rect = Rect();
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
    color_edge = src.clone();

    for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
      // ���ݼ�����������mser+���
      const std::vector<cv::Point>& p = regContours[i];
      Rect rc = bboxes1[i];
      if (1) {
        //RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //cv::rectangle(color_edge, bboxes1[i], color, 1);
      }
    }
    std::vector<vector<cv::Rect> > rrects2;
    if (bboxes1.size() > 10) {
      vector<int> labels;
      vector<int> cnt;
      int nclasses = cv::partition(bboxes1, labels, [](const cv::Rect& r1, const cv::Rect& r2) {
        double hd = 0.7;
        double a1 = r1.x;
        double b1 = r1.x + r1.width;
        double a2 = r2.x;
        double b2 = r2.x + r2.width;
        double l = MAX(a1, a2);
        double r = MIN(b1, b2);
        double d = r - l;
        double u = r1.width + r2.width - d;
        u = MIN(r1.width, r2.width);
        double iou = d / u;
        return iou > hd;
      });
      cnt.assign(nclasses, 0);
      for (int i = 0; i < labels.size(); ++i) {
        int j = labels[i];
        ++cnt[j];
      }
      std::vector<int> index = argsort(cnt);
      //nclasses = min(nclasses, 15);
      for (int i = 0; i < nclasses; ++i) {
        int idx = index[i];
        int c = cnt[idx];
        if (c > 1) {
          vector<Rect> rrects;
          for (int j = 0; j < bboxes1.size(); ++j) {
            if (labels[j] == idx) {
              rrects.push_back(bboxes1[j]);
            }
          }
          rrects2.push_back(rrects);
        }
      }
      int avgh = 0;
      int avgw = 0;
      nclasses = rrects2.size();
      vector<RotatedRect> rrect3(nclasses);
      for (int i = 0; i < nclasses; ++i) {
        RotatedRect r = minAreaRect(rrects2[i]);
        r = curr(r);
        rrect3[i] = r;
        //drawRotatedRect(color_edge, r, color, 1);
        Rect boundRect = r.boundingRect();
        avgh += boundRect.height;
        avgw += boundRect.height;
      }
      sort(rrect3.begin(), rrect3.end(), [](const RotatedRect& a, const RotatedRect& b) {
        return a.center.x < b.center.x;
      });
      int hist[64] = {0};
      vector<int> score(nclasses);
      for (int i = 0; i < nclasses; ++i) {
        if (i > 0) {
          double dist = rrect3[i].center.x - rrect3[i - 1].center.x;
          int t = (int)(dist);
          if (t>10 && t < 64) {
            hist[t]++;
          }
        }
      }
      int maxgap = argmax(64, hist);
      for (int i = 0; i < nclasses; ++i) {
        for (int j = i + 1; j < nclasses; ++j) {
          double dist = fabs(rrect3[i].center.x - rrect3[j].center.x);
          double t = dist / (maxgap + 0.0001);
          double fx = t - round(t);
          if (t<3 && fabs(fx) < 0.2) {
            score[i]++;
            score[j]++;
          }
        }
      }
      int j = 0;
      for (int i = 0; i < nclasses; ++i) {
        if (score[i] > 0) {
          rrect3[j++] = rrect3[i];
        }
      }
      nclasses = j;
      rrect3.resize(nclasses);
      rrect2.resize(nclasses);
      for (int i = 0; i < nclasses; ++i) {
        rrect2[i] = rrect3[i].boundingRect();
      }
      for (int i = 0; i < nclasses; ++i) {
        Rect r = rrect2[i];
        r.y = 1;
        r.height = src.rows-1;
      }
      if (rrect3.size() > 2) {
        //if (rrect3[rrect3.size() - 1].center.x - rrect3[rrect3.size() - 2].center.x)
      }
      //rrect2.resize(rrects2.size());
      sort(rrect2.begin(), rrect2.end(), [](const Rect& a, const Rect& b) {
        return a.x < b.x;
      });
      if (1) {
        j = 17;
        //rrect2.erase(rrect2.begin() + 14);
        //nclasses = rrect2.size();
        double halfw = maxgap*0.5;
        vector<Rect> rrect1(18);
        for (int i = nclasses - 1; i >= 1 && j >= 0; --i) {
          double cx0 = rrect2[i].x + rrect2[i].width*0.5;
          double cx1 = rrect2[i - 1].x + rrect2[i - 1].width*0.5;
          double cx2 = cx1;
          if (i + 1 < nclasses) {
            cx2 = rrect2[i + 1].x + rrect2[i + 1].width*0.5;
          }
          rrect1[j] = rrect2[i];
          --j;
          while (j>=0 && fabs(cx0 - cx1) / maxgap > 1.2) {
            int i = j + 1;
            rrect1[j] = rrect1[i];
            rrect1[j].x -= maxgap;
            cx0 -= maxgap;
            --j;
          }
        }
        while (j >= 0) {
          int i = j + 1;
          rrect1[j] = rrect1[i];
          rrect1[j].x -= maxgap;
          --j;
        }
        rrect2 = rrect1;
        nclasses = rrect1.size();
      }
      if (0 == iline) {
        int i = 0;
        int j = 0;
        i = MAX(nclasses - 18, 0);
        Rect boundrc = rrect2[0];
        
        if (1) {
          boundrc.width = rrect2[nclasses - 1].x + rrect2[nclasses - 1].width - boundrc.x;
          boundrc = rectExt(boundrc, 20, 6, src.rows, src.cols);
		  idnumber_rect = boundrc;
          //printf("%s\n", idnumber);   imshow("im4", im4); waitKey(-1);
        }
        //printf("\n");
        //imshow("color_edge", color_edge); waitKey(-1);
        //imshow("color_edge", color_edge); waitKey(-1);
        return i > 0;
      }
      //printf("\n");
    }
    //threshold(gray, bin, 0, 255, CV_THRESH_OTSU);
    //imshow("color_edge", color_edge); waitKey(-1);
    //imshow("color_edge", color_edge); 
    //imshow("bin", bin);
    //waitKey(-1);
    return 0;
  }

};

#if 0
struct oct_tess {
  tesseract::TessBaseAPI tess;
  oct_tess() {
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
  }
  int run(const Mat& im)
  {
    cv::Mat gray;
    cv::cvtColor(im, gray, CV_BGR2GRAY);
    // ...other image pre-processing here...

    // Pass it to Tesseract API
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    printf("out = %s\n", out);
    return 0;
  }
};
#endif

#ifndef _OCR_CTC_INL_
struct ocr_net {
  int loadjson(const char* fn) { return 0; }
  int run(wchar_t* wstr, int len, const uchar* data, int w) { return 0; }
};
#endif

struct ocr_idcard_reg {
  ocr_net ocr;
  ocr_segchar seg;
  ocr_seghans segh;
  Mat color_edge;
  //oct_tess tess;
  int init() {
    // Load the network. 
    if (ocr.loadjson(0)) {
      //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
      //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";
      //set_debug_info(2);
      return 1;
    }
    return 0;
  }
  int run(const Mat& mat, const RotatedRect* r, int n, idcard* out) {
    int i, j=0;
    if (n <= 0) return 0;
    Mat im;
    Mat im2;
    double ss = 1.9;
    wchar_t wstr[256] = { 0 };
    char str[256] = { 0 };
    int stdh = 48;
    //imshow("mat", mat);
    memset(out, 0, sizeof(*out));
    float ridnum_ss = 0;
    Rect ridnum_rect;
	vector<Rect> r_ok2;
	out->type = 1;
    if (1) {
      for (i = 0; i < 1; ++i) {
        RotatedRect r2 = r[i];
        r2.size.width += 40;
        r2.size.height *= ss;
        im = getSubImage(mat, r2);
        int w = (im.cols) * stdh / im.rows;
        w += 3;
        ridnum_rect = r2.boundingRect();
        ridnum_ss = ridnum_rect.width*1./w;
        w &= ~3;
        resize(im, im2, Size(w, stdh));
        if (0 == i) {
          seg.run(im2, i);
          str[0] = 0;
        }
        //printf("%s\n", str);
        //waitKey(-1);
        //int n = seg.rrect2.size();
      }
    }
    Rect ridnum = seg.rrect2[0];
    int ridnum_w = (seg.rrect2[17].x + seg.rrect2[17].width - ridnum.x);
    int ridnum_x = ridnum_rect.x + (ridnum.x - ridnum_w*0.35)*ridnum_ss;
    int ridnum_r = ridnum_rect.x + (ridnum.x + ridnum_w*0.5)*ridnum_ss;
    vector<Rect> r_ok;
    ss = 1.2;
    if (1) {
      for (i = 0; i < n; ++i) {
        RotatedRect r2 = r[i];
        r2.size.width += 20;
        r2.size.height *= ss;
        {
          Rect r = r2.boundingRect();
          int right = r.x + r.width;
          r.x = MAX(r.x, 0);
          if (i > 3) {
            right = MAX(right, ridnum_r);
            right = MIN(right, mat.cols);
          }
          r.x = MAX(r.x, ridnum_x);
          r.x = MAX(r.x, 0);
          r.y = MAX(r.y, 0);
          r.width = right - r.x;
          if (r.width > 10 && r.height > 10) {
            r_ok.push_back(r);
          }
        }
      }
      n = r_ok.size();
    }
	if (1) {
		Rect r = r_ok[2];
		r = curr(r);
		int off1 = MIN(r.height * 4, r.width- r.height);
		off1 = MAX(0, off1);
		r.x += off1;
		r.width -= off1;
		r = curr(r);
		Mat gray = mat(r);
		gray = mesr_bin(gray);
		//imshow("gray", gray); waitKey(0);
		vector<float> ph = project_H(gray, 20);
		//Mat ph = projectHistogram(mserMapMat, 1);
		//drawHist(color_edge, ph, 1);
		int xpos = 0;
		int nnn = MIN(r.height, r.width);
		//int begpos = gray.rows * 4;
		for (int i = 1; i < nnn; ++i) {
			if (ph[i] < ph[xpos]) {
				xpos = i;
			}
		}
		r.x += xpos;
		r.width -= xpos;
		r = curr(r);
		r_ok[2] = r;
		gray = mat(r);
		//imshow("gray", gray); waitKey(0);
	}
    if (0) {
      color_edge  = mat.clone();
      for (i = 0; i < seg.rrect2.size(); ++i) {
        Scalar color = Scalar(230, 230, 0);
        Rect r = seg.rrect2[i];
        rectangle(color_edge, r, color, 1);
      }
      for (i = 0; i < n; ++i) {
        Rect r = r_ok[i];
        Scalar color = Scalar(230, 0, 0);
        rectangle(color_edge, r, color, 2);
      }
      imshow("color_edge", color_edge); waitKey(-1);
    }
	if (1) {
		char idnumber[32] = { 0 };
		if (1) {
			Rect boundrc = seg.idnumber_rect;
			Mat im4 = im2(boundrc).clone();
			string ss = run_ocrnum_caffe(im4);
			strcpy(idnumber, ss.c_str());
			if (17 == strlen(idnumber)) {
				int cd = get_check_digit(idnumber);
				idnumber[17] = cd < 10 ? ('0' + cd) : 'X';
			}
			idnumber[18] = 0;
		}
		if (!idcard_get_info(idnumber, out)) {
			printf("idcard_get_info fail! number:%s\n", idnumber);
			return 0;
		}
		printf("%s %s %s\n", idnumber, out->birthday, out->gender);
		strcpy(out->number, idnumber);
	}
    if (1) {
      for (i = 1; i < n; ++i) {
		  //if (i == 2) continue;
		  if (i == 3) continue;
        str[0] = 0;
        if (0) {
          Rect r = r_ok[i];
          im = mat(r);
          int w = (im.cols) * 32 / im.rows;
          w += 3;
          w &= ~3;
          resize(im, im2, Size(w, 32));
          //imshow("im", im); waitKey(-1);
          int len = 0;
          //seg.run(im2);
		  //imshow("im2", im2);  waitKey(-1);
		  len = ocr.run(wstr, 256, im2.data, w);
		  //n = ocr.run(wstr, 256, img_data, 280);
		  //tess.run(im2);
		  len = iconv_cs("UCS-2LE", "gb2312", (char*)wstr, len * 2, str, 256);
		  str[len] = 0;
        }
		if (i == 20) {
			int r = r_ok[i].x + r_ok[i].width;
			r_ok[i].x = ridnum.x+ ridnum.height;
			r_ok[i].width = r - r_ok[i].x;
		}
        if (1) {
          Rect r = r_ok[i];
		  r = curr(r);
          im = mat(r);
          //imshow("im", im); waitKey(-1);
          int len = 0;
		  if (i == 20) {
			  cv::imshow("asdf", im);
			  cv::waitKey(-1);
		  }
          //seg.run(im2);
          //imshow("im2", im2);  waitKey(-1);
          string ss = run_ocr_caffe(im);

          //n = ocr.run(wstr, 256, img_data, 280);
          //tess.run(im2);
          strcpy(str, ss.c_str());
        }
        if (0) {
          Rect r = r_ok[i];
          r.width += 20;
          im2 = mat(r);
          int len = segh.run_h(im2);
          if (len > 0) {
            strcpy(str, segh.hans);
          }
        }
        printf("%s\n", str);
        if (strlen(str) > 0) {
          out->type = 1;
          strncpy(out->side, "front", 32);
          switch (i)
          {
          case 0:
            strncpy(out->number, str, 32);
            break;
          case 1:
            strncpy(out->name, str, 16);
            break;
          case 2:
		  {
			  strncpy(out->race, str, 32);
		  }
            break;
          case 3:
            strncpy(out->birthday, str, 32);
            break;
          default:
            strcat(out->address, str);
            break;
          }
          ++j;
        }
      }
    }
    //waitKey(-1);
    return 1;
  }
};

struct ocr_detect {
  FaceDetector face_etector;
  cv::Mat srcImage, color_edge, gray, dst;
  ocr_idcard_reg ocr;
  get_angle_t get_angle;
  vector<idcard> vid;
  vector<idcard_back> vidb;
  int time_used;
  ocr_detect() {
    ocr.init();
  }
  std::string jsonstr;
  idcard getidcard(int index) {
    idcard id = vid[index];
    char* number = id.number;
    int i, j = 0;
    for (i = 0, j = 0; id.number[i]; ++i) {
      char c = number[i];
      if (('0' <= c && c <= '9') || 'x' == c || 'X' == c) {
        number[j++] = c;
      }
    }
    number[j] = 0;
    char* name = id.name;
    char* p;
    if (p = strstr(name, "��")) {
      strcpy(name, p + 2);
    }
    return id;
  }

  std::string getjson(const cv::Mat& src) {
    const char* nowstr = timenow();
    int ret = 0;
    utime_start(a);
    ret = run(src);
    time_used = 1000 * utime_elapsed(a);
    switch (ret) {
    case 1:
    {
      cjson * root = cjson_CreateObject();
      cjson * cards = cjson_CreateArray();
      cjson_AddItemToObject(root, "cards", cards);
      for (int i = 0; i < vid.size(); ++i) {
        cjson * card = cjson_CreateObject();
        idcard id = getidcard(i);
        cjson_AddItemToObject(card, "name", cjson_CreateString(id.name));
        cjson_AddItemToObject(card, "gender", cjson_CreateString(id.gender));
        cjson_AddItemToObject(card, "id_card_number", cjson_CreateString(id.number));
        cjson_AddItemToObject(card, "birthday", cjson_CreateString(id.birthday));
        cjson_AddItemToObject(card, "race", cjson_CreateString(id.race));
        cjson_AddItemToObject(card, "address", cjson_CreateString(id.address));
        cjson_AddItemToObject(card, "type", cjson_CreateNumber(1));
        cjson_AddItemToObject(card, "side", cjson_CreateString(id.side));
        cjson_AddItemToArray(cards, card);
      }
	  for (int i = 0; i < vidb.size(); ++i) {
		  cjson * card = cjson_CreateObject();
		  idcard_back idb = vidb[i];
		  cjson_AddItemToObject(card, "issued_by", cjson_CreateString(idb.issued_by));
		  cjson_AddItemToObject(card, "valid_date", cjson_CreateString(idb.valid_date));
		  cjson_AddItemToObject(card, "type", cjson_CreateNumber(1));
		  cjson_AddItemToObject(card, "side", cjson_CreateString(idb.side));
		  cjson_AddItemToArray(cards, card);
	  }
      cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
      cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
      jsonstr = cjson_Print(root);
      printf("%s\n", jsonstr.c_str());
      cjson_Delete(root);
    }
    break;
    case -1:
    {
      cjson * root = cjson_CreateObject();
      cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
      cjson_AddItemToObject(root, "error_message", cjson_CreateString("IMAGE_TOO_VAGUE"));
      cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
      jsonstr = cjson_Print(root);
      printf("%s\n", jsonstr.c_str());
      cjson_Delete(root);
    }
    break;
    default:
    {
      cjson * root = cjson_CreateObject();
      cjson_AddItemToObject(root, "time_used", cjson_CreateNumber(time_used));
      cjson_AddItemToObject(root, "error_message", cjson_CreateString("MISSING_ARGUMENTS: image_url, image_file, image_base64"));
      cjson_AddItemToObject(root, "request_id", cjson_CreateString(nowstr));
      jsonstr = cjson_Print(root);
      printf("%s\n", jsonstr.c_str());
      cjson_Delete(root);
    }
    break;
  }
  return jsonstr;
}
  int run(const cv::Mat& src) {
    vector<Mat> vecdst;
    int ret = 0;
    int aa_angle = 10;
    //imRotation(src, src, iline &1 ? aa_angle :-aa_angle);
    //cv::resize(src, src, cv::Size(), 1, 1.5, cv::INTER_LANCZOS4);
    srcImage = src;
    color_edge = src.clone();
    cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
    face_etector.run(gray);
    vector<RotatedRect> cardrect;
    vecdst.clear();
	vid.clear();
	vidb.clear();

	for (int i = 0; i < face_etector.rect_pairs.size(); i+=2) {
		RotatedRect r0 = face_etector.rect_pairs[i + 0];
		RotatedRect r1 = face_etector.rect_pairs[i + 1];
		r0 = scale_size(r0, 1.1, 1.3);
		r1 = scale_size(r1, 1.1, 1.3);
		Mat im0 = getSubImage(src, r0);
		Mat im1 = getSubImage(src, r1);
		string ss0 = run_ocr_caffe(im0);
		//string ss1 = run_ocrnum_caffe(im1);
		string ss1 = run_ocr_caffe(im1);
		idcard_back idb;
		strcpy(idb.side, "back");
		strcpy(idb.issued_by, ss0.c_str());
		strcpy(idb.valid_date, ss1.c_str());
		int ret = valid_date_curr(idb.valid_date);
		printf("issued_by: %s  valid_date:%s\n", idb.issued_by, idb.valid_date);
		if (ret) {
			vidb.push_back(idb);
			if (0) {
				imshow("im0", im0);
				imshow("im1", im1);
				waitKey(0);
			}
		}
	}
    for (int i = 0; i < face_etector.rects.size(); ++i) {
      RotatedRect& face_rect = face_etector.rects[i];
      RotatedRect r = face_rect_to_card_rect(face_rect, 0.1, 1.2, 1.2);
      if (Rect(0, 0, gray.cols, gray.rows).contains(r.center)) {
        Mat dst = getSubImage(src, r, Size(300, 200));
        if (get_angle.run(dst)) {
          int iangle = get_angle.iangle;
          r.angle += iangle;
          face_rect.angle += iangle;
          double tt = 70;
          double a = 0.1;
          double b = 1.1;
          //double scale, double y_off
          if (get_angle.centor_dis < tt) {
            a *= pow(tt / get_angle.centor_dis, 1);
          }
          else {
            //b *= pow(get_angle.centor_dis*1. / tt, 0.5);
            double mm = 200;
            if (get_angle.max_dis < mm) {
              //b *= 1.2;
            }
          }
          double dd = 250;
          if (get_angle.max_dis > 200) {
            b *= pow(get_angle.max_dis / dd, 0.5);
          }
          if (get_angle.centor_dis > 90) {
            b *= 1.2;
          }
          if (get_angle.max_dis < 150 && get_angle.centor_dis<60) {
            //b *= 60./ get_angle.centor_dis;
          }
          double bot_maxx = MAX(get_angle.botton_line[2], get_angle.botton_line[0]);
          double bot_minx = MIN(get_angle.botton_line[2], get_angle.botton_line[0]);
          if (bot_maxx < 200 && get_angle.max_dis<200 && get_angle.botton_line[0]) {
            //b *= 1.5;
          }
          r = face_rect_to_card_rect(face_rect, a, b, b);
          Mat dst_ok = getSubImage(src, r, Size(300, 200));
          get_rect_t gr;
          ret = gr.run(dst_ok);
          if (!ret && 0) {
            b *= 1.2;
            r = face_rect_to_card_rect(face_rect, a, b, b);
            dst_ok = getSubImage(src, r, Size(300, 200));
            //imshow("dst_ok", dst_ok); waitKey(0);
            ret = gr.run(dst_ok);
          }
          //if (ret)
          {
            vecdst.push_back(get_angle.color_edge.clone());
            //imshow("color_edge1", get_angle.color_edge);
            vecdst.push_back(gr.color_edge);
            if (0) {
              string fn = wstd::format("C://outpic//dst_ok_%03d_%d.jpg", iline, i);
              cv::imwrite(fn, dst_ok);
            }
            Scalar color = Scalar(0, 0, 255);
            //drawRotatedRects(dst_ok, gr.lines_ok, 2, &color);
            //ocr.run();
            if (0) {
              double s = 0;
              for (int i = 0; i < gr.lines_ok.size(); ++i) {
                s += gr.lines_ok[i].center.x;
                s += gr.lines_ok[i].center.y;
                s += gr.lines_ok[i].size.width;
                s += gr.lines_ok[i].size.height;
                s += gr.lines_ok[i].angle;
              }
              printf("lines_ok avg = %lf\n", s);
            }
            Mat dst_ok2 = getSubImage(src, r, Size(300*2, 200*2));
            double tt_b = imArticulation(dst_ok2);
            if (tt_b > 1.) {// ������
              vector<RotatedRect> rr_ok;
              for (int i = 0; i < gr.lines_ok.size(); ++i) {
                rr_ok.push_back(scale(gr.lines_ok[i], 2, 2));
              }
              idcard out;
              ret = ocr.run(dst_ok2, rr_ok.data(), rr_ok.size(), &out);
              //vecdst.push_back(dst_ok);
              vecdst.push_back(ocr.color_edge);
              if (ret) {
                vid.push_back(out);
              }
              //drawDetectLines(dst_ok, gr.lines, 4);
              cardrect.push_back(r);
            } else {
              ret = -1;
            }
          }
        }
      }
      if (0) {
        Point2f fourPoint2f[4];
        r.points(fourPoint2f);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        circle(color_edge, fourPoint2f[0], 5, color, -1);
        circle(color_edge, fourPoint2f[1], 5, color, -1);
      }
    }
    drawRotatedRects(color_edge, face_etector.rects, 2);
    drawRotatedRects(color_edge, cardrect, 2);
    vecdst.push_back(color_edge);
    dst  = mergeImgs(03, 0, 600, vecdst);
#ifdef _DEBUG
#endif
	if (vidb.size() + vid.size()) {
		ret = 1;
	}
    return ret;
  }
};

//�㷨
int ldistance1(const char* source, int n, const char* target, int m)
{
  //step 1
  if (m == 0) return n;
  if (n == 0) return m;
  //Construct a matrix
  typedef vector< vector<int> >  Tmatrix;
  Tmatrix matrix(n + 1);
  for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);

  //step 2 Initialize

  for (int i = 1; i <= n; i++) matrix[i][0] = i;
  for (int i = 1; i <= m; i++) matrix[0][i] = i;

  //step 3
  for (int i = 1; i <= n; i++)
  {
    const char si = source[i - 1];
    //step 4
    for (int j = 1; j <= m; j++)
    {

      const char dj = target[j - 1];
      //step 5
      int cost;
      if (si == dj) {
        cost = 0;
      }
      else {
        cost = 1;
      }
      //step 6
      const int above = matrix[i - 1][j] + 1;
      const int left = matrix[i][j - 1] + 1;
      const int diag = matrix[i - 1][j - 1] + cost;
      matrix[i][j] = min(above, min(left, diag));

    }
  }//step7
  return matrix[n][m];
}


int test_detect_idcard()
{
  //return test_hsv_bin();
  //return test_myfindContours();
  //int k;
  RNG rng(12345);
  //Mat::convertTo;
  Rect r;
  //r.center;
  /// ����Դͼ��
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  const char* txtfn;
  txtfn = "E:/data/ew_id/list1.txt";
  txtfn = "E:/data/ew_id/t1_0911.txt";
  int pic_index = 1;
  if (0) {
	  txtfn = "E:/data/ew_id/backend_2.txt";
	  pic_index = 0;
  }
  wstd::readlines(txtfn, list, 20000, 200000);
#ifdef _DEBUG
  iline = 1182;
  iline = 1192;
  iline = 1222;
  iline = 1229;
  iline = 113;
#endif
  //iline = 30;
  //iline = 2;
  int c = 0;
  int n = list.size();
  //n = min(iline+300, n);
  ocr_detect od;
  srand(11);
  int errcnt = 0;
  int allcnt = 0;
  char linebuf[512];
  for (; iline < n; ) {
    vector<string> strs;
    int len = iconv_cs("utf-8", "gb2312", list[iline].c_str(), -1, linebuf, 512);
    linebuf[len] = 0;
    wstd::split(strs, linebuf, "\t");
    g_imgfn = strs[pic_index];
    string fn = im_dir + g_imgfn;
    cv::Mat src;
    if (1) {
		fn = "E:/OCR_Line/bin/20190312091804.jpg";
		fn = "E:/OCR_Line/bin/285133_235119.jpg";
    }
    //if (!strstr(fn.c_str(), "20161005101521422-4375")) { continue; }
    src = cv::imread(fn, 1);

	if (src.rows < 200) {
		continue;
	}
	if (1 == pic_index) {
		string idcard_no = strs[2];
		g_idcard = idcard_no;
		printf("%d %s %d %d %s\n", iline, wstd::path_split_filename(fn.c_str()).c_str(), src.rows, src.cols, idcard_no.c_str());
		if (src.empty()) {
			proc_key(c, list.size(), iline);
			continue;
		}
	}

    int ret = od.run(src);
#ifdef _DEBUG
#endif
    if (pic_index==1 && ret==1) {
      if (1) {
        if (od.vid.size() == 1) {
          const idcard* id = od.vid.data();
          const char* addr = strs[6].c_str();
          //int levenshtein(char * s1, int l1, char * s2, int l2, int threshold);
          //int levenshtein_distance(const char *s, int n, const char*t, int m, int noop);
          allcnt += strlen(id->address) + strlen(addr);
          errcnt += ldistance1(id->address, strlen(id->address), (char*)addr, strlen(addr));
        }
      }
      if (0) {
        if (od.vid.size() == 1) {
          const idcard* id = od.vid.data();
          const char* a = strs[2].c_str();
          const char* b = id->number;
          allcnt += strlen(b) + strlen(a);
          errcnt += ldistance1(b, strlen(b), (char*)a, strlen(a));
        }
      }
    }
    printf("%d --------------- %lf\n", iline, errcnt*1./allcnt);
    isdebug = 1;
    if (1) {
      string fn = wstd::format("C://outpic//dst_ok//src_%03d_z.jpg", iline);
      cv::imwrite(fn, src);
    }
    if (isdebug) {
      if (1) {
        cv::imshow("vecdst", od.dst);
        c = cv::waitKey(0);
      }
      proc_key(c, list.size(), iline);
    }
    else {
      string fn = wstd::format("C://outpic//out_%03d_z.jpg", iline);
      cv::imwrite(fn, od.dst);
      ++iline;
    }
  }
  return 0;
}