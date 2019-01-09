
// 
#include "cstd.h"
#include "cfile.h"

//dirһ��Ŀ¼�������ļ�����
// FindFile.cpp : Defines the entry point for the console application.
//

const int MAXLEN = 1024;                //�������Ŀ¼����
unsigned long FILECOUNT = 0;            //��¼�ļ�����

int listdir(const char* dirpath, fileinfo_t** finfo)
{
  return 0;
}

int test_findfile() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/book");
  dirlist(dir, "E:/book", "*.pdf", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}

int test_rename() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/www/mv/vip.aqdyh.net/htm_rmvb_have");
  dirlist(dir, "E:/www/mv/vip.aqdyh.net/htm_rmvb_have", "*.html", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}

int strv_include1(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos;
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  for (i=0; i<sv->n; ++i) {
    pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    if (pos>=0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
#include "str\strstr.inl"
int strv_include(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos;
  int charStep[MAX_CHAR_SIZE] = {0};
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  setCharStep(incstr, l_incstr, charStep);
  for (i=0; i<sv->n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = strstr_sunday_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, charStep);
    if (pos>=0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
int strv_includes(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos, k;
  vstr_t sv3[1] = {0};
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  strv_split_str(sv3, STR2(incstr, l_incstr), " ", " ", 2);
  for (i=0; i<sv->n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = -1;
    for (k=0; k<sv3->n; ++k) {
      pos = cstr_find(sv->v[i].s, sv->v[i].l, sv3->v[k].s, sv3->v[k].l, 0, ignore_case);
      if (pos>=0) {
        str_t* s1 = sv2->v + j++;
        str_setstr(s1, sv->v[i].s, sv->v[i].l);
        break;
      }
    }
  }
  strv_setsize(sv2, j);
  strv_free(sv3);
  return j;
}
int strs_include_find(const str_t* s, int n, const char* incstr, int l_incstr, int i, int ignore_case) {
  int j=0, pos;
  int charStep[MAX_CHAR_SIZE] = {0};
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  setCharStep(incstr, l_incstr, charStep);
  for (; i<n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = strstr_sunday_find(s[i].s, s[i].l, incstr, l_incstr, charStep);
    if (pos>=0) {
      return i;
    }
  }
  return -1;
}
int strv_uniq_filename(const vstr_t* sv, vstr_t* sv2, int ignore_case) {
  int i, j=0, pos;
  strv_setsize(sv2, sv->n);
  for (i=0; i<sv->n; ++i) {
    char* name = GetFileNameExt(sv->v[i].s);
    pos = strs_include_find(sv->v, sv->n, name, -1, i+1, ignore_case);
    if (pos<0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
int strv_tostr(const vstr_t* sv, str_t* s, const char* sp) {
  int i, n = 0, spl = strlen(sp);
  for (i=0; i<sv->n; ++i) {
    n += sv->v[i].l;
  }
  n += sv->n * spl;
  str_setsize(s, n);
  n = 0;
  for (i=0; i<sv->n; ++i) {
    memcpy(s->s + n, sv->v[i].s, sv->v[i].l);
    memcpy(s->s + n + sv->v[i].l, sp, spl);
    n += sv->v[i].l + spl;
  }
  return 0;
}
int str_include_split(vstr_t* sv, str_t s, str_t incstr, int ignore_case, const char* trimstr) {
  int i=0;
  str_t s2[1] = {0};
  strv_setsize(sv, 0);
  for (;(i = STRsplit_find(s, incstr, i, ignore_case, "\n", trimstr, s2))>=0;) {
    strv_push_str(sv, (*s2));
  }
  return 0;
}
int test_have_rmvb() {
  dirlist_t dir[1] = {0};
  int i, j;
  vstr_t sv[5] = {0};
  //str_t s[5] = {0};
  vstr_t* sv1 = sv+1;
  vstr_t* sv2 = sv+2;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/www/mv/vip.aqdys.net");
  //test_strstr();
  vstr_load("info.txt", sv);
  if (0) {
    {
      utime_start(_start_time);
      strv_include(sv, sv2, "����ɳ��", -1, 1);
      printf("%f\n", utime_elapsed(_start_time));
    }
    {
      utime_start(_start_time);
      strv_include1(sv, sv2, "����ɳ��", -1, 1);
      printf("%f\n", utime_elapsed(_start_time));
    }
  }
  //strv_tostr(sv, s, "\n");
  //str_include_split(sv2, *s, STR1("MIMK"), 1, " ");
  strv_include(sv, sv, "������", -1, 1);
  // ������ ԰����ӣ ֯������ �⏎ �ɍ�� ��ԭ���� MMND PURIN ��Ұ��ѩ OHO CEAD DASD ���� �������� KTDS ��Ҷ���� �ߵ����� ��ɽ��ӣ �ѳι��� �����Է ���ȹ��� �߲�ǧ�� ��ʯ���� ���˰��� ��ӣ ϣ�D���� ������� ��Ұ�椫�� ���D��� HEYZO NITR NATR ����� ���� �ɹ����� EKDV SPRD ����ϣ �������� KAORI ��ɽ���� ������ �����ɼ� ��Ұ���� ľ���ղ� �ɱ��� ����ľ�� ľ�ºͽ�ʵ ERIKA �ٱ����� ������ ����ˮ�� ��ɽ�̲� С���� ���˻� �ҤФ���� �����ϣ ˮԪ�{���� ����˩`�� ��D���� �������� Ҷ������ ��ϲ�� ��Ұ���� ��ľ�Ĵ� HND ��ʯ���� β����Ҷ �\�� �������� ���ȹ��� �W �椤�� �ϳ������� �������� ˫Ҷ���� ��ԭ�ۤΤ� ������ BOBB ���S���� ���Ż� ��Ұ������ ��ӣ �������� СȪ��ϣ ������ ͩԭ������ �ྮ���� �������� ����ľ����
  dirlist(dir, "H:/MV", "*.jpg", FF_SUBDIR);
  for (j=0; j<sv->n; ++j) {
    char* p;
    int len = 10;
    strv_split_str(sv2, sv->v[j], "*", "\" \n", 0);
    p = sv2->v[3].s;
    len = MIN(len, sv2->v[3].l);
    if (len<10) {
      continue;
    }
    for (i=0; i<dir->n; ++i) {
      char* name = GetFileNameExt(dir->v[i].name);
      int len2 = len;
      int len1 = strlen(name);
      len2 = MIN(len2, len1);
      if (0==cstr_icmp(p, len2, name, len2, 1)) {
        break;
      }
    }
    if (i==dir->n) {
      strv_split_str_add(sv1, sv2->v[1], "#", " \n\"", 5);
    }
    //printf("%d %s\n", (i==dir->n), p);
  }
  strv_include(sv1, sv1, "rmvb", -1, 1);
  strv_uniq_filename(sv1, sv1, 1);
  for (j=0; j<sv1->n; ++j) {
    logprintf("%s\n", sv1->v[j].s);
  }
  strv_frees(sv, countof(sv));
  dirlist_free(dir);
  return 0;
}

