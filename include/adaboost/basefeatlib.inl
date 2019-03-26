
#include "std/algo.h"

typedef float base_feat_type;
// h_feat
typedef struct BASE_FEAT_T {
  base_feat_type* samp; // featnum * sampnum �ľ���
  int* allfeat;        // ���е���������
}
BASE_FEAT_T;

typedef int BASEFEAT;

#define BASE_FEAT_ELEMSIZE  sizeof(base_feat_type)

#define BASE_FEAT_VAL(A1, pw, IV) (*((A1) + (pw)))

STATIC double BASE_feat_val(double* A, int al, int* chi, int i, double IV)
{
  return BASE_FEAT_VAL(A, chi[ i ], IV);
}

#define BASE_CN 0
#define BASE_setoffset 0
#define BASE_integral 0

STATIC int BASE_print(FILE* pf, const int* chi)
{
  fprintf(pf, " %6d ", chi[ 0 ]);
  return 0;
}
STATIC int BASE_scanf(FILE* pf, int* chi)
{
  fscanf(pf, "%d", chi);
  return 0;
}

STATIC int BASE_uninit(BOOST* hh)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  free(fs->allfeat);
  free(fs->samp);
  free(hh->h_feat);
  hh->h_feat = 0;
  return 0;
}

STATIC int BASE_samp_data(BOOST* hh, char** buf, int* size)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = hh->feat_num * BASE_FEAT_ELEMSIZE;
  return 0;
}

STATIC int BASE_save_feat(BOOST* hh, int id)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  int es = BASE_FEAT_ELEMSIZE;
  char* dst = (char*)(hh->ca->featptr) + hh->ca->weaklen * es;

  if (id >= 0) {
    char* src = (char*)(fs->allfeat) + id * es;
    memcpy(dst, src, es);
  }
  else {
    memset(dst, 0, es);
  }

  hh->ca->weaklen++;
  return 0;
}
STATIC double* BASE_feat(BOOST* hh, int i)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  int nSampNum = hh->samp_num;
  int nFeatNum = hh->feat_num;
  const base_feat_type* samp = (fs->samp) + nSampNum * i;
  COPY(nSampNum, samp, hh->fi);
  return hh->fi;
}

// HAAR ����
// ÿ��ͼƬռ�ռ� 2*4*33*33=8712 �ֽ�
STATIC int BASE_find_samp(BOOST* hh, int type)
{
  BASE_FEAT_T* fs = (BASE_FEAT_T*)(hh->h_feat);
  // ��������������
  int i, j, k, ispass, cnt;
  int nFeatNum = hh->feat_num;
  int nSampNum = hh->samp_num;
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  const base_feat_type* Samp = (fs->samp);
  int Num = (type > 0) ? (GetPosNum(hh)) : (hh->samp_num - GetPosNum(hh));
  double* wi = hh->wi + offset;
  double* vi = hh->vi + offset;
  int id = type;
  double* tmpFeat = MALLOC(double, hh->feat_num);
  double score;

  for (cnt = 0, k = 0, i = offset; i < (offset + Num) && (k < Num);) {
    if (wi[ k ] >= 0.f) {
      ++k;
      continue;
    }

    for (j = 0; j < nFeatNum; ++j) {
      tmpFeat[ j ] = Samp[ nSampNum * j + i ];
    }

    // ���ͨ����������
    score = 0;
    //CASCADE_PASS(hh->ca, tmpFeat, hh->ca->id, FEAT_VAL_ID, 0, ispass, score);
    cascade_pass(hh->ca, tmpFeat, 1, 0, &ispass, &score);

    if (ispass) {
      ++cnt;
      printf("%s�����ҵ�%6d ��! \r", id > 0 ? "��" : "��", cnt);

      //LOGPRINTF(hh->log, "%d��%s����ͨ����"ENDL, i, ff[(idx[i].id+1)&3]);
      //hh->samp_idx[k] = idx[i].id;
      wi[ k ] = 1.f;
      vi[ k ] = score;
    }
    else {
      wi[ k ] = -1.f;
    }

    ++k;
    ++i;
  }

  free(tmpFeat);
  return 0;
}

int read_matrix(int rows, int cols, void* A, int elemsize, int step, FILE* pf, int tr)
{
  int i, j;
  char* m = (char*)A;

  if (tr) {
    for (j = 0; j < cols; ++j) {
      for (i = 0; i < rows; ++i) {
        fread(m + i * step + j * elemsize, elemsize, 1, pf);
      }
    }
  }
  else {
    for (i = 0; i < rows; ++i) {
      for (j = 0; j < cols; ++j) {
        fread(m + i * step + j * elemsize, elemsize, 1, pf);
      }
    }
  }

  return 0;
}

STATIC int BASE_init(BOOST* hh)
{
  int i, samp_size, nn;
  char* sampfile;    // �����ļ� txt �� dat �ļ�
  base_feat_type* samp;
  BASE_FEAT_T* fs = MALLOC(BASE_FEAT_T, 1);
  sampfile = inigetstr("�����ļ�");
  hh->feat_num = inigetint("������Ŀ");
  samp_size = hh->feat_num * hh->samp_num;
  samp = MALLOC(base_feat_type, samp_size);
  fs->samp = samp;
  hh->h_feat = fs;
  nn = hh->feat_num * BASE_FEAT_ELEMSIZE;
  fs->allfeat = (int*) MALLOC(char, nn);

  for (i = 0; i < hh->feat_num; ++i) {
    ((int*)(fs->allfeat)) [ i ] = i;
  }

  {
    char* ext = strrchr(sampfile, '.');
    FILE* pf = fopen(sampfile, "rb");

    if (pf) {
      if (memcmp(ext, ".dat", 4) == 0) {
        //fread(fs->samp, samp_size * sizeof(base_feat_type), 1, pf);
        read_matrix(hh->feat_num, hh->samp_num, fs->samp, sizeof(base_feat_type), hh->samp_num * sizeof(base_feat_type), pf, 0);
      }
      else if (memcmp(ext, ".txt", 4) == 0) {
        FSCANF_ALL(pf, "%f", samp_size, samp);
      }

      fclose(pf);
    }
  }

  return 1;
}
