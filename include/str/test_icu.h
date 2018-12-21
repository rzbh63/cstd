#include <unicode/ucnv.h>

#ifdef _WIN32
#pragma comment(lib, "icuuc.lib")
#endif

//����0Ϊ�ɹ���������붨�������
int convert(const char * toConverterName, const char * fromConverterName, char * target,
  int32_t targetCapacity, const char * source, int32_t sourceLength)
{
  UErrorCode error = U_ZERO_ERROR;
  ucnv_convert(toConverterName, fromConverterName, target, targetCapacity, source, sourceLength, &error);
  return error;
}

int test_icu()
{
  wchar_t aaa[] = L"�й��˵�ϵͳ��123323";
  int alen = wcslen(aaa);
  int blen = alen * 2 + sizeof(int);
  char *abuff = new char[blen];
  int result = convert("gb2312", "utf-16le", abuff, blen, (const char *)aaa, alen);
  printf("%s %d\n", abuff, strlen(abuff));
  delete[]abuff;
  return 0;
}