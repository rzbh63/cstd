

#include "std/string_c.h"

int test_str() {
  char aa[256] = "�Ķ��߷���㷢��������";
  replace_str(aa, -1, "�Ķ�", -1, "�Ĵ�", -1, 1);
  replace_str(aa, 10, "��", -1, "��", -1, 1);
  replace_str(aa, 10, "��", -1, "��", -1, 1);
  replace_str(aa, 10, "��", -1, "��", -1, 1);
  return 0;
}