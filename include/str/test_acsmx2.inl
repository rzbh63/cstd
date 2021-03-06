/*
Author: wangyao
Email: wangyao@cs.hit.edu.cn
*/
#include "acsmx.h"
/*
* Text Data Buffer
*/
unsigned char text[MAXLEN];
extern int nline;
main(int argc, char** argv)
//int main (int argc, char **argv)
{
  int i, nocase = 0;
  FILE* fd;
  // char filename[20];
  ACSM_STRUCT* acsm;
  // if (argc < 3)
  // {
  // fprintf (stderr,"Usage: acsmx filename pattern1 pattern2 ... -nocase\n");
  // exit (0);
  // }
  acsm = acsmNew();
  // strcpy (filename, argv[1]);
  // fd = fopen(filename,"r");
  fd = fopen("word-list.txt", "r");
  if (fd == NULL) {
    fprintf(stderr, "Open file error!\n");
    exit(1);
  }
  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-nocase") == 0) { //strcmp 串比较
      nocase = 1;
    }
  for (i = 2; i < argc; i++) {
    if (argv[i][0] == '-') { // ? ? ? ? ?
      continue;
    }
    acsmAddPattern(acsm, argv[i], strlen(argv[i]), nocase);
  }
  /* Generate GtoTo Table and Fail Table */
  acsmCompile(acsm); // han shu
  /*Search Pattern*/
  while (fgets(text, MAXLEN, fd)) { //成行读。 从流stream读n-1个字符，或遇换行符'\n'为止，把读出的内容，存入s中。与gets不同，fgets在s未尾保留换行符。一个空字节被加入到s，用来标记串的结束。 成功时返回s所指的字符串；在出错或遇到文件结束时返回NULL
    acsmSearch(acsm, text, strlen(text), PrintMatch);
    nline++; // nline 加到什么数值为止
  }
  PrintSummary(acsm->acsmPatterns);
  acsmFree(acsm);
  printf("\n### AC Match Finished ###\n");
  // system("pause");
  return (0);
}

