
#include "string_c.h"
#include <stdio.h>

int test_strsep()
{
  char str[] = "abcdefg";
  char* p = str;
  char* key_point;
  while (p) {
    //�ؼ���Ϊc��d����������������
    while (key_point = strsep_c(&p, "cd")) {
      //��������cʱ��key_pointָ��c���أ�����dʱkey_pointָ��d���أ�ע���ʱd�Ѿ�����дΪ'\0'�ˣ�
      if (*key_point == 0) {
        continue;  //���������Ĺؼ��֣�����һ��ָ��\0��ָ�룬���������Ҿ���
      } else {
        break;  //�ָ��һ���������ַ�������ȥ��ӡ�ɣ�
      }
    }
    printf("%s\n", key_point);
  }
  return 0;
}
