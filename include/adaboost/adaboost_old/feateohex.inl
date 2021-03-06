/*
EHOG特征计算步骤
1.把原图分成N块，大小，尺度都不一样
椋篸o：
2.计算每个像素点的梯度幅值与方向
3.把梯度方向0-180分成9个bin方向
4.分成三大类特征，每类特征都有9个候选特征
�0-20，20-40，40-60，60-80，80-100，100-120，120-140，140-160，160-180 9个bin方向
   特征=每个bin方向的梯度幅值和/总的梯度幅值和
�0-40，20-60，40-80，60-100，80-120，100-140，120-160，140-180，160-180-20 9个bin方向
   特征=每个bin方向的梯度幅值和/总的梯度幅值和
�0-60，20-80，40-100，60-120，80-140，100-160，120-180，140-180-20，160-180-40 9个bin方向
  特征=每个bin方向的梯度幅值和/总的梯度幅值和
荻确较蚍殖烧�27种，则每一快有27个候选特征。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "boost.h"
#include "macro.h"
#include "xerror.h"
#include "inifile.h"
#include "console.h"
#include "cstd.h"
#include "imresize.h"
#include "cascade.h"
int EOHEX_save_feat(BOOST* hh, int id)
{
  if (id < 0) {
    hh->ca->weaklen++;
  }
  else {
    hh->ca->eoh[ hh->ca->weaklen++ ] = hh->eoh[ id ];
  }

  return 0;
}

int EOHEX_init(BOOST* hh, const char* inifile, const char* featsect)
{
  char buf[ 256 ];
  hh->postxtfile = MALLOC(char, 256);
  hh->negtxtfile = MALLOC(char, 256);
  IniGetString(inifile, featsect, "正样本txt文件", "", hh->postxtfile, 256);
  IniGetString(inifile, featsect, "负样本txt文件", "", hh->negtxtfile, 256);
  IniGetFloat(inifile, featsect, "缩放比", hh->zoom);
  hh->ca->w = IniGetInt(inifile, featsect, "样本宽", 0);
  hh->ca->h = IniGetInt(inifile, featsect, "样本高", 0);
  hh->stepxy = IniGetInt(inifile, featsect, "样本检测步进", 2);
  IniGetString(inifile, featsect, "特征矩形文件", "", buf, 256);

  if (!hh->samp) {
    int cx = hh->ca->w, cy = hh->ca->h;
    hh->samp_size = hh->samp_num * (cy + 1) * (cx + 1) * EOHEX_CN;
    hh->samp = MALLOCSET(real, hh->samp_size, 0);
  }

  {
    hh->feat_num = read_rect_from_file(buf, NULL, 27);

    if (hh->feat_num) {
      hh->eoh = MALLOC(EOHFEAT, hh->feat_num);
      read_rect_from_file(buf, hh->eoh, 27);
    }
  }

  return 0;
}

int EOHEX_uninit(BOOST* hh)
{
  SAFEFREE(hh->eoh);
  SAFEFREE(hh->postxtfile);
  SAFEFREE(hh->negtxtfile);
  return 0;
}

int EOHEX_feat(BOOST* hh, int i, real* tmpSamp)
{
  int j, cx = hh->ca->w;
  const real* samp = hh->samp;
  EOHFEAT eoh = hh->eoh[ i ];
  int stp = (cx + 1) * EOHEX_CN;
  EOHEX_SETOFFSET(eoh, stp, EOHEX_CN, 1.f);

  for (j = 0; j < hh->samp_num; ++j, samp += hh->samp_step) {
    if (hh->wi[ j ] < 0.f) {
      continue;
    }

    tmpSamp[ j ] = EOHEX_FEAT_VAL(samp, eoh, 0);
  }

  return 0;
}

// EOH 样本
// 每张图片占空间 10*4*33*33=43560 字节
int EOHEX_find_samp(BOOST* hh, int type)
{
  int i = 0, j = 0, n = 0, cx = hh->ca->w, cy = hh->ca->h;
  char buf[ 256 ];
  int _step = (cx + 1) * EOHEX_CN;
  int _size = (cy + 1) * _step;
  int SampNum = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  real* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  real* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  real zoom = hh->zoom;
  const char* piclist = type > 0 ? hh->postxtfile : hh->negtxtfile;
  int picnum = 0;
  FILE* pf = fopen(piclist, "r");
  hh->samp_step = _size;

  if (!pf) {
    error(ERR_FOPEN);
    return 0;
  }

  if (1 == type) {
    FEAT_SETOFFSET(hh->ca->eoh, hh->ca->weaklen, _step, EOHEX_CN, 1.f, EOHEX);
  }

  for (i = 0, j = 0; j < SampNum && fscanf(pf, "%s", buf) > 0; ++i) {
    if (wi[ j ] >= 0.f) {
      ++j;
      continue;
    }

    {
      int height, width, channels = 1;
      struct fmtimgreader* reader = new_reader(buf, &height, &width, 0);

      if (reader) {
        int step = width * channels;
        unsigned char* data = (unsigned char*) malloc(height * step);
        int ispass = 0;
        real score;
        imread(reader, data, step, 1);
        ++picnum;

        if (1 == type) {
          real* tmpSamp = hh->samp + j * _size;
          assert(cy == height && cx == width);
          EOH_grads_integral(height, width, data, step, tmpSamp, _step, EOHEX_CN);
          //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->eoh, EOHEX_FEAT_VAL, 0, ispass, score);
          cascade_pass(hh->ca, tmpSamp, 0, &ispass, &score);

          if (ispass) {
            wi[ j ] = 1.f;
            vi[ j ] = score;
            ++j;
          }
        }

        if (-1 == type) {
          real ss = 1.0;
          int w = width, h = height;
          unsigned char* tmpimg = MALLOC(unsigned char, w * h);

          for (; j < SampNum && w >= cx && h >= cy; ss *= zoom, w = XFLOOR(width / ss), h = XFLOOR(height / ss)) {
            int ranx = w - cx + 1;
            int rany = h - cy + 1;
            int stp = (w + 1) * EOHEX_CN;
            int siz = (h + 1) * stp;
            int x, y, k = 0;
            int stepx = hh->stepxy, stepy = hh->stepxy;
            real* inter;
            inter = MALLOCSET(real, siz, 0);
            IMRESIZE(height, width, data, step, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY);
            EOH_grads_integral(h, w, tmpimg, w, inter, stp, EOHEX_CN);
            FEAT_SETOFFSET(hh->ca->eoh, hh->ca->weaklen, stp, EOHEX_CN, 1.f, EOHEX);

            //for (i=0; i<times; ++i)
            for (x = 0; j < SampNum && x < ranx; x += stepx) {
              for (y = 0; j < SampNum && y < rany; y += stepy) {
                real* tmpSamp = inter + y * stp + x * EOHEX_CN;
                //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->eoh, EOHEX_FEAT_VAL, 0, ispass, score);
                cascade_pass(hh->ca, tmpSamp, 0, &ispass, &score);

                if (j < SampNum && ispass) {
                  int cy1 = hh->ca->h + 1;
                  real* Samp = hh->samp + (j + hh->pos_num) * _size;
                  SKIP_COPY(cy1, _step, tmpSamp, stp, 1, Samp, _step, 1);

                  //imwrite("aa.bmp", hh->h, cx, data+step*y+x, step, 1);
                  wi[ j ] = 1.f;
                  vi[ j ] = score;
                  ++j;
                  ++k;
                }
              }
            }

            SAFEFREE(inter);
          }

          SAFEFREE(tmpimg);
        }

        free(data);
        free_reader(reader);
        printf("找了%5d张图片，共找到%5d个%s样本！\r", picnum, j, type > 0 ? "正" : "负");
      }
    }
  }

  fclose(pf);
  logprintf("找了%5d张图片，共找到%5d个%s样本！\n", picnum, j, type > 0 ? "正" : "负");
  return n;
}

