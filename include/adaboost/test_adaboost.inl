
//#include "debug.h"
#include "std/stddef_c.h"

#include <direct.h>
//#include "ui/window.inl"
#include "std/fileio_c.h"
#include "std/inifile.h"
#include "adaboost.inl"
#include "basefeatlib.inl"
//#include "lobofeatlib.inl"
#include "haarfeatlib.inl"
//#include "chifeatlib.inl"
#include "lutweak.inl"
#include "genweak.inl"
#include "cascade.inl"
//#include "cap.h"

//#include "img/color.inl"
//#include "std/imdraw.h"
#include "std/drawing_c.h"
#include "std/gui_c.h"
#include "std/log_c.h"
#include "std/objdetect.h"
#include "std/colorcvt.h"
#include "std/fileioex.h"
//#include "test_luobo.inl"

int detect_idcard(int h, int w, const uchar* data, int step, XRECT* out, int maxout) {
  static cascade_t ca[1] = { 0 };
  static buf_t bf1[1] = { 0 };
  buf_t bf[1] = { 0 };
  if (ca->w == 0) {
    const char* fn = "E:/OCR_Line/adaboost/cas.dat";
    INIT_CA_FUN(ca, HAAR, LUT);
    cascade_load(fn, ca);
    bfsetsize(bf1, 20 * _1M);
  }
  img_t gry[1] = {0};
  *bf = *bf1;
  IMINIT(gry, h, w, data, step, 1, 1);
  int n = cascade_detect(ca, gry, 1, 1000, 1.1, 1, out, maxout);
  return n;
}

int test_cascade1() {
  cascade_t ca[1] = {0};
  char* fn = NULL;
  vstr_t sv[1] = {0};
  img_t im[2] = {0};
  img_t* gry = im+1;
  int i, n;
  XRECT out[10000];
  //buf_t bf[1] = {0};
  sys_chdir("D:/pub/bin/adaboost/lobo");
  sys_chdir("D:/pub/faceplusplus/gender/out");
  sys_chdir("E:/OCR_Line/adaboost");
  sys_chdir("E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com");
  vstr_load("../backend_2.txt", sv);

  if (1) {
    fn = "haar_LUT_nesting_cas.dat";
    fn = "cas.dat";
    fn = "haar_gen_cas.dat";
    fn = "gender_cas.dat";
    fn = "idcard_cas.dat";
    fn = "idcard_cas.dat";
    fn = "cas.dat";
    INIT_CA_FUN(ca, HAAR, GEN);
    INIT_CA_FUN(ca, HAAR, LUT);
    //cascade_load(fn, ca);
	if (1) {
		fn = "cas.txt";
		cascade_load_txt(fn, ca);
		cascade_save_inl("cas.inl", ca);
	}
    //ca->w = 100, ca->h = 100;
    //ca->w = 30, ca->h = 20;
  } 
  if (0) {
#ifdef _LUOBO_INL_
    fn = "���_cas.dat";
    fn = "cas.dat";
    fn = "���_cas_nesting.dat";
    fn = "���+lut+cas.dat";
    fn = "lobogen_cas.dat";
    fn = "lobo_lut_cas.dat";
    INIT_CA_FUN(ca, LOBO, GEN);
    INIT_CA_FUN(ca, LOBO, LUT);
    cascade_load(fn, ca);
    //cascade_save_txt("lobo_lut_cas.txt", ca);
    ca->w = 80, ca->h = 80;
#endif
    //ca->stagelen = 3;
  }
 // bfsetsize(bf, 20*_1M);
#ifdef _CAP_H_
  if (0) {
    cap_t cap[1] = { 0 };
    if (capvfw_open(cap, 0, 640, 480) > 0) {
      int fram = 0;
      char ch = 0;
      img_t im[1] = { 0 };
      for (; 'q' != ch && 'Q' != ch; ++fram) {
        cap_getframe(cap, im, CAP_RGB);
        //printf("%d w=%d h=%d\n", fram, im->w, im->h);
        imshow_(im);
        ch = waitkey(10);
      }
      cap_close(cap);
      imfree(im);
    }
    return 0;
  }
#endif // _CAP_H_

  for (i=0; i<sv->n; ++i) {
    if (!imread((char*)sv->v[i].s, 3, 1, im)) continue;
	if (im->h < 400) {
		continue;
	}
	//imshow2("im1", im);
    if (im->h>400) {
      imresize1(im, 400./im->h, im);
    }
    imcolorcvt(im, gry, T_BGR, T_GRAY);
	n = 0;
	memset(out, 0, sizeof(out));
    if (ca->w == 80) {
      //n = lobo_detect(ca, im, out, countof(out));
    } else {
		double t = gry->w / (ca->w*3.);
		//t = MAX(t, 1);
      n = cascade_detect(ca, gry, 1, 1000, 1.1, 1, out, countof(out));
    }
    printf("%d\n", n);
    int j;
    for (j=0; j<n; ++j) {
      //imdraw_xrects(im, out, n);
      if (out[j].count>5) {
        Rect rc = iRect(out[j].x, out[j].y, out[j].w, out[j].h);
        //imdraw_rect(im, 0, NULL, rc, 0, _RGB(255, 0, 0), 1);
		rectangle(im, rc, _RGB(255, 0, 0), 1, 8, 0);
      }
    }
    imshow_(im);
    waitkey(-1);
  }
  //bffree(bf);
  freeims(im, 2);
  vstr_free(sv);
  return 0;
}

#if 0
#include "face/face_recog.inl"

int test_cascade2() {
  cascade_t ca[1] = {0};
  char* fn = NULL;
  vstr_t sv[1] = {0};
  img_t im[2] = {0};
  img_t* gry = im+1;
  int i, n;
  char buf[256];
  sys_chdir("D:/pub/bin/adaboost/lobo");
  sys_chdir("D:/pub/faceplusplus/gender/out");
  //sys_chdir("D:/pub/faceplusplus/out");
  //vstr_load("test_image_list.txt", sv);
  sys_chdir("D:/pub/faceplusplus/out/std");
  sys_chdir("D:/pub/faceplusplus/out/M");
  sys_chdir("D:/pub/faceplusplus/jpg");
  vstr_load("list.txt", sv);
  mkdirs("M");
  mkdirs("F");
  mkdirs("U");
  //vstr_load("poslist.ini", sv);
  INIT_CA_FUN(ca, HAAR, LUT);
  fn = "D:/pub/faceplusplus/out/std1/cas.dat";
  fn = "D:/pub/faceplusplus/out/std/cas.dat";
  cascade_load(fn, ca);
  ca->w = 100, ca->h = 100;
  int cnt=0, err=0;
  int is1 = !!strstr(fn, "std1");
  for (i=0; i<sv->n; ++i) {
    if (imread((char*)sv->v[i].s, 1, 1, im)) {
      double score = 0;
      char* name = GetFileNameExt(sv->v[i].s);
      int label = !strstr(sv->v[i].s, "\\fff\\");
      printf("%s\n", name);
      //imshow(im);cvWaitKey(-1);
      //ASSERT(im->s==100);
      if (0) {
        int pass = cascade_pass_img(ca, im->tt.data, im->s, &score);
        ++cnt;
        err += label!=pass;
        printf("%d: %d %f %f\n", cnt, pass, score, err*1./cnt);
      }
      if (1) {
        int xywh[12] = {0};
        uchar stdface[10000];
        int ret = face_recog(im->h, im->w, im->tt.data, im->s, im->c, "gray", "df", 0, 1, xywh, stdface, NULL, NULL, NULL, 0, NULL, NULL);
        if (ret) {
          //img_t* gry = im;
          //imcolorcvt(im, gry, T_BGR, T_GRAY);
          int* xywh1 = xywh+4;
          int* xywh2 = xywh+8;
          IPOINT leye = iPOINT(xywh1[0]+xywh1[2]/2, xywh1[1]+xywh1[3]/2), reye = iPOINT(xywh2[0]+xywh2[2]/2, xywh2[1]+xywh2[3]/2);
          if (is1) {
            CPM_FaceStd( gry->h, gry->w, gry->tt.data, gry->s, gry->c, STDIMGH, STDIMGW, stdface, STDIMGW, 1, leye, reye, 0.35, 0.5, 0.65, 0.5 );
          }
          double score = 0;
          //cvShowImage("asdf", 100, 100, stdface, 100, 1); cvWaitKey(-1);
          int pass = cascade_pass_img(ca, stdface, 100, &score);
          //cvShowImage("asdf", 100, 100, stdface, 100, 1);
          printf("%s %s\n", pass ? "��" : "Ů", name);
          if (1) {
            sys_mvfile(sv->v[i].s, _format("%s/%s", pass ? "./M/" : "./F/", name));
          }
          if (0) {
            ++cnt;
            err += label!=pass;
            printf("%d: %d %f %f\n", cnt, pass, score, err*1./cnt);
          }
          if (label!=pass) {
            if (0) {
              _snprintf(buf, countof(buf), label ? "D:/pub/faceplusplus/out/F/%s" : "D:/pub/faceplusplus/out/M/%s", name);
              sys_mvfile(sv->v[i].s, buf);
            }
            if (0) {
              printf("��%s����?", label ? "��":"Ů");
              imread((char*)sv->v[i].s, 3, 1, im);
              imshow(im);
              int ch;
              ch = cvWaitKey(-1);
              ch = tolower(ch);
              if ('n'==ch) {
                //ca->stage[0].thd;
                _snprintf(buf, countof(buf), "D:/pub/faceplusplus/out/mm/%s", name);
                sys_mvfile(sv->v[i].s, buf);
              } else {
                _snprintf(buf, countof(buf), "D:/pub/faceplusplus/out/ff/%s", name);
                sys_mvfile(sv->v[i].s, buf);
              }
            }
          }
        } else {
          sys_mvfile(sv->v[i].s, _format("U/%s", name));
        }
      }
    }
  }
  freeims(im, 2);
  strv_free(sv);
  return 0;
}

int test_cascade() {
  cascade_t ca[1] = {0};
  char* fn = NULL;
  vstr_t sv[1] = {0};
  img_t im[2] = {0};
  int i, n;
  sys_chdir("D:/pub/faceplusplus");
  vstr_load("list.txt", sv);
  INIT_CA_FUN(ca, HAAR, LUT);
  fn = "gender_cas.dat";
  cascade_load(fn, ca);
  ca->w = 100, ca->h = 100;

  mkdirs("./out/F/");
  mkdirs("./out/M/");
  int xywh[12] = {0};
  uchar stdface[10000];
  char buf[256];
  char outpathF[256];
  char outpathM[256];
  char outpathD[256];
  for (i=236179; i<sv->n; ++i) {
    char* path = GetPathFileNameExtN(sv->v[i].s, 3);
    strcpy(buf, path);
    cstr_replacechr(buf, -1, '\\', '_');
    cstr_replacechr(buf, -1, '/', '_');
    strcpy(outpathF, "./out/F/");
    strcat(outpathF, buf);
    strcpy(outpathM, "./out/M/");
    strcat(outpathM, buf);
    strcpy(outpathD, "./out/D/");
    strcat(outpathD, buf);
    if (!file_exist(outpathF) && !file_exist(outpathF) && !file_exist(outpathD)) {
      if (imread(sv->v[i].s, 1, 1, im)) {
        char* name = GetFileNameExt(sv->v[i].s);
        //imshow(im);cvWaitKey(10);
        int ret = face_recog(im->h, im->w, im->tt.data, im->s, im->c, "GRAY", "df", 0, 1, xywh, stdface, NULL, NULL, NULL, 0, NULL, NULL);
        if (ret) {
          double score = 0;
          //cvShowImage("asdf", 100, 100, stdface, 100, 1); cvWaitKey(-1);
          int pass = cascade_pass_img(ca, stdface, 100, &score);
          if (0) {
            _snprintf(buf, countof(buf), "./out/%s/%s.bmp", pass ? "F" : "M", name);
            imsave(buf, 100, 100, stdface, 100, 1);
          }
          //_snprintf(buf, countof(buf), "./out/%s/%s", pass ? "F" : "M", name);
          char* outpath = pass ? outpathF : outpathM;
          printf("%d: %f, %s\n", i, score, outpath);
          sys_cpfile(sv->v[i].s, outpath, 1);
        } else {
          sys_cpfile(sv->v[i].s, outpathD, 1);
        }
      }
    } else {
      printf("exist %s\n", buf);
    }
  }
  freeims(im, 2);
  strv_free(sv);
  return 0;
}
#include "cap.h"
#include "draw/imdraw.inl"
int test_cascade_cam() {
  int high = 0;
  capdev cap[1] = {0};
  
  if (caperoad_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[10] = {0};
    cascade_t ca[1] = {0};
    char* fn = NULL;
    int xywh[12] = {0};
    uchar stdface[10000];
#ifdef _DEBUG
    sys_chdir("D:/pub/faceplusplus/out/std/");
#endif // _DEBUG
    fn = "cas.dat";
    INIT_CA_FUN(ca, HAAR, LUT);
    cascade_load(fn, ca);
    ca->w = 100, ca->h = 100;
    
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      int ret = face_recog(im->h, im->w, im->tt.data, im->s, im->c, "rgb", "df", 0, 0.5, xywh, stdface, NULL, NULL, NULL, 0, NULL, NULL);
      if (ret) {
        img_t* gry = im+1;
        imcolorcvt(im, gry, T_BGR, T_GRAY);
        int* xywh1 = xywh+4;
        int* xywh2 = xywh+8;
        IPOINT leye = iPOINT(xywh1[0]+xywh1[2]/2, xywh1[1]+xywh1[3]/2), reye = iPOINT(xywh2[0]+xywh2[2]/2, xywh2[1]+xywh2[3]/2);
        //CPM_FaceStd( gry->h, gry->w, gry->tt.data, gry->s, gry->c, STDIMGH, STDIMGW, stdface, STDIMGW, 1, leye, reye, 0.35, 0.5, 0.65, 0.5 );
        double score = 0;
        //cvShowImage("asdf", 100, 100, stdface, 100, 1); cvWaitKey(-1);
        int pass = cascade_pass_img(ca, stdface, 100, &score);
        //cvShowImage("asdf", 100, 100, stdface, 100, 1);
        printf("%s\n", pass ? "��" : "Ů");
        imdrawaa_circle(im, NULL, leye.x, leye.y, 5, _rgba(0,1,0,0.5), 0, 0);
        imdrawaa_circle(im, NULL, reye.x, reye.y, 5, _rgba(0,1,0,0.5), 0, 0);
        imdrawaa_textv(im, 0, NULL, pass ? "Male" : "Female", 50, 100, 0, 2, _rgb(1,0,0), 5);
      }
      imshow(im);
      ch = cvWaitKey(10);
    }
    
    cap_close(cap);
    imfrees(im, 10);
  }
  return 0;
}
#endif
int test_imread() {
  img_t im[1] = { 0 };
  char* fn = "E:/www/news.ifeng.com/jpg/c1.ifengimg.com/iamsImg/2017/09/01/ee8dab9516b0566114d538648fbd7b86_w180_h30.jpg";
  if (imread(fn, 1, 1, im)) {
    int asdf = 0;
  }
  imfree(im);
  return 0;
}
int test_adaboost() {
  adaboost_t hh[1] = {0};
  //test_imread();
  //return test_cascade_cam();
  //return test_cascade2();
#ifdef _DEBUG
  
  if (1) {
    _chdir("D:/pub/bin/iris/adaboost");
    _chdir("D:/pub/bin/iris/adaboost_om");
    _chdir("D:/pub/bin/iris/adaboost_om2");
    _chdir("D:/pub/bin/iris/testPic/feat");
    _chdir("D:/pub/bin/iris/adaboost");
    _chdir("D:/pub/bin/adaboost/lobo");
    _chdir("D:/pub/bin/adaboost/data");
    _chdir("D:/pub/bin/adaboost/gender");
    _chdir("D:/pub/faceplusplus/gender/out");
    _chdir("E:/OCR_Line/adaboost");
  }
  
#endif // _DEBUG
  
  //INIT_FUN(hh, BASE, LUT);
  //INIT_FUN(hh, LOBO, LUT);
  //INIT_FUN(hh, LOBO, GEN);
  //INIT_FUN(hh, HAAR, GEN);
  //INIT_FUN(hh, HAAR, LUT);
  //INIT_FUN(hh, CHI, GEN);
  //INIT_FUN(hh, BASE, GEN);
  INIT_FUN(hh, HAAR, LUT);
  adaboost_train(hh);
  return 0;
}