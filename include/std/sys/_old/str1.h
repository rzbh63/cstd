#ifndef _STR_H_
#define _STR_H_
#include <assert.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
//#include "cstd.h"

//#define str_cmp(s1, s2)  mystrcmp(s1.s, s1.l, s2.s, s2.l)
#define str_count_str(s1, s2, ignore_case)  cstr_count((s1).s, (s1).l, (s2).s, (s2).l, ignore_case)
#define str_remove_chr(s0, ch)  (s0)->l = STR_remove_chr((s0)->s, (s0)->l, ch)

// �ַ���Hash�����Ա�
// http://www.byvoid.com/blog/string-hash-compare/
//�Ҷ���Щhash��ɢ��������Ч������һ���򵥲��ԣ����Խ�����£�
//����1����100000���ɴ�Сд��ĸ�����������ANSI�ַ��������ظ���ÿ���ַ�����󳤶Ȳ�����64�ַ�������ɢ�У�
//�ַ�������    ��ͻ��  ��1000003ȡ���ĳ�ͻ��
//
// BKDRHash     0       4826
// SDBMHash     2       4814
// RSHash       2       4886
// APHash       0       4846
// ELFHash      1515    6120
// JSHash       779     5587
// DEKHash      863     5643
// FNVHash      2       4872
// DJBHash      832     5645
// DJB2Hash     695     5309
// PJWHash      1515    6120
//
// ����2����100000��������UNICODE�������ַ��������ظ���ÿ���ַ�����󳤶Ȳ�����64�ַ�������ɢ�У�
// �ַ�������   ��ͻ��  ��1000003ȡ���ĳ�ͻ��
// BKDRHash     3       4710
// SDBMHash     3       4904
// RSHash       3       4822
// APHash       2       4891
// ELFHash      16      4869
// JSHash       3       4812
// DEKHash      1       4755
// FNVHash      1       4803
// DJBHash      1       4749
// DJB2Hash     2       4817
// PJWHash      16      4869
//
// ����3����1000000�����ANSI�ַ��������ظ���ÿ���ַ�����󳤶Ȳ�����64�ַ�������ɢ�У�
// �ַ�������   ��ʱ�����룩
// BKDRHash     109
// SDBMHash     109
// RSHash       124
// APHash       187
// ELFHash      249
// JSHash       172
// DEKHash      140
// FNVHash      125
// DJBHash      125
// DJB2Hash     125
// PJWHash      234
//
// ���ۣ�Ҳ�����ҵ���������һЩ�����ԣ��ڶ�ASCII���ַ�������ɢ��ʱ��PJW��ELF Hash��������ʵ��ͬһ���㷨����������������Ч�ʣ����൱��⣻���磺"b5"�롰aE"���������ַ�������PJWɢ�г�����hashֵ����һ���ġ����⣬�����������������ɢ�еĹ�ϣ�������磺JS/DEK/DJB Hash���ڶ���ĸ��������ɵ��ַ�����ɢ��Ч��Ҳ����ô�á���Զ��ԣ�����BKDR��SDBM����򵥵�HashЧ����Ч�����á�
//���ߣ�icefireelf
//������http://blog.csdn.net/icefireelf/article/details/5796529

//�����ַ���Hash�����Ƚ�
//���õ��ַ���Hash��������ELFHash��APHash�ȵȣ�����ʮ�ּ���Ч�ķ�������Щ����ʹ��λ����ʹ��ÿһ���ַ��������ĺ���ֵ���� Ӱ�졣���⻹����MD5��SHA1Ϊ������Ӵպ�������Щ���������������ҵ���ײ��
//�����ַ�����ϣ������ BKDRHash��APHash��DJBHash��JSHash��RSHash��SDBMHash��PJWHash��ELFHash�ȵȡ��������ϼ��ֹ� ϣ�������Ҷ��������һ��СС�����⡣
// Hash����     ����1   ����2   ����3   ����4   ����1�÷�       ����2�÷�       ����3�÷�       ����4�÷�       ƽ����
// BKDRHash     2       0       4774    481     96.55   100     90.95   82.05   92.64
// APHash       2       3       4754    493     96.55   88.46   100     51.28   86.28
// DJBHash      2       2       4975    474     96.55   92.31   0       100     83.43
// JSHash       1       4       4761    506     100     84.62   96.83   17.95   81.94
// RSHash       1       0       4861    505     100     100     51.58   20.51   75.96
// SDBMHash     3       2       4849    504     93.1    92.31   57.01   23.08   72.41
// PJWHash      30      26      4878    513     0       0       43.89   0       21.95
// ELFHash      30      26      4878    513     0       0       43.89   0       21.95
//
// ��������1Ϊ100000����ĸ��������ɵ��������ϣ��ͻ����������2Ϊ100000���������Ӣ�ľ��ӹ�ϣ��ͻ����������3Ϊ����1�Ĺ�ϣֵ�� 1000003(������)��ģ��洢�����Ա��г�ͻ�ĸ���������4Ϊ����1�Ĺ�ϣֵ��10000019(��������)��ģ��洢�����Ա��г�ͻ�ĸ�����
//
// �����Ƚϣ��ó�����ƽ���÷֡�ƽ����Ϊƽ��ƽ���������Է��֣�BKDRHash��������ʵ��Ч�����Ǳ���ʵ���У�Ч��������ͻ���ġ�APHashҲ �ǽ�Ϊ������㷨��DJBHash,JSHash,RSHash��SDBMHash����ǧ�PJWHash��ELFHashЧ�������÷����ƣ����� �����������Ƶġ�

/// @brief BKDR Hash Function
/// @detail ���㷨������Brian Kernighan��Dennis Ritchie�ġ�The C Programming Language��һ�鱻չʾ����������һ�ּ򵥿�ݵ�hash�㷨��Ҳ��JavaĿǰ���õ��ַ�����Hash�㷨���۳�����Ϊ31����
// Ҳ���Գ���31��131��1313��13131��131313..
// ����˵���˷��ֽ�Ϊλ���㼰�Ӽ����������Ч�ʣ��罫��ʽ���Ϊ��hash = hash << 7 + hash << 1 + hash + ch;
// ����ʵ��Intelƽ̨�ϣ�CPU�ڲ��Զ��ߵĴ���Ч�ʶ��ǲ��ģ�
// �ҷֱ������100�ڴε������������㣬���ֶ���ʱ�������Ϊ0�������Debug�棬�ֽ��λ�����ĺ�ʱ��Ҫ��1/3����
// ��ARM����RISCϵͳ��û�в��Թ�������ARM�ڲ�ʹ��Booth's Algorithm��ģ��32λ�����˷����㣬����Ч��������йأ�
// ������8-31λ��Ϊ1��0ʱ����Ҫ1��ʱ������
// ������16-31λ��Ϊ1��0ʱ����Ҫ2��ʱ������
// ������24-31λ��Ϊ1��0ʱ����Ҫ3��ʱ������
// ������Ҫ4��ʱ������
// ��ˣ���Ȼ��û��ʵ�ʲ��ԣ���������Ȼ��Ϊ����Ч���ϲ�𲻴�
static unsigned int BKDRHash(const char* str, unsigned int len)
{
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    /* 31 131 1313 13131 131313 etc.. */
    hash = (hash * 131) + (*str);
  }
  return hash;
}
/// @brief SDBM Hash Function
/// @detail ���㷨�������ڿ�Դ��ĿSDBM��һ�ּ򵥵����ݿ����棩�б�Ӧ�ö�����������BKDRHash˼��һ�£�ֻ�����Ӳ�ͬ���ѡ�
static unsigned int SDBMHash(const char* str, unsigned int len)
{
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = 65599 * hash + (*str);
    //hash = (*str) + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}
/// @brief RS Hash Function
/// @detail ��Robert Sedgwicks���䡶Algorithms in C��һ����չʾ��������
static unsigned int RSHash(const char* str, unsigned int len)
{
  unsigned int a = 63689;
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash * a + (*str);
    a = a * 378551;
  }
  return hash;
}
/// @brief AP Hash Function
/// @detail ��Arash Partow������һ��hash�㷨��
/* End Of FNV Hash Function */
static unsigned int APHash(const char* str, unsigned int len)
{
  unsigned int hash = 0xAAAAAAAA;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :  ((~((hash << 11)) + ((*str) ^ (hash >> 5))));
  }
  return hash;
}
/// @brief JS Hash Function
/// ��Justin Sobel������һ��hash�㷨��
static unsigned int JSHash(const char* str, unsigned int len)
{
  unsigned int hash = 1315423911;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash ^= ((hash << 5) + (*str) + (hash >> 2));
  }
  return hash;
}
/// @brief DEK Function
/// @detail ���㷨������Donald E. Knuth�ڡ�Art Of Computer Programming Volume 3����չʾ��������
static unsigned int DEKHash(const char* str, unsigned int len)
{
  unsigned int hash = 1315423911; // len
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
  }
  return hash;
}
/// @brief FNV Hash Function
/// @detail Unix systemϵͳ��ʹ�õ�һ������hash�㷨������΢��Ҳ����hash_map��ʵ�֡�
static unsigned int FNVHash(const char* str, unsigned int len)
{
  unsigned int hash = 0x811c9dc5;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash *= 16777619;
    hash ^= (*str);
  }
  return hash;
}
/* End Of BP Hash Function */
static unsigned int FNVHash1(const char* str, unsigned int len)
{
  const unsigned int fnv_prime = 0x811C9DC5;
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash *= fnv_prime;
    hash ^= (*str);
  }
  return hash;
}
/// @brief DJB Hash Function
/// @detail ��Daniel J. Bernstein���ڷ�����һ��hash�㷨��
/* End Of SDBM Hash Function */
static unsigned int DJBHash(const char* str, unsigned int len)
{
  unsigned int hash = 5381;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = ((hash << 5) + hash) + (*str);
  }
  return hash;
}
/// @brief DJB Hash Function 2
/// @detail ��Daniel J. Bernstein ��������һ��hash�㷨��
static unsigned int DJB2Hash(const char* str, unsigned int len)
{
  register unsigned int hash = 5381;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash * 33 ^ (*str);
  }
  return hash;
}
/// @brief PJW Hash Function
/// @detail ���㷨�ǻ���AT&T����ʵ���ҵ�Peter J. Weinberger�����Ķ�������һ��hash�㷨��
/* End Of JS Hash Function */
static unsigned int PJWHash(const char* str, unsigned int len)
{
  const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
  const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
  const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  unsigned int hash = 0;
  unsigned int test = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << OneEighth) + (*str);
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}
/// @brief ELF Hash Function
/// @detail ������Unix��Extended Library Function��������������һ��hash�㷨������ʵ����PJW Hash�ı��Ρ�
static unsigned int ELFHash(const char* str, unsigned int len)
{
  enum { TotalBits           = sizeof(unsigned int) * 8,
    ThreeQuarters       = (TotalBits * 3) / 4,
    OneEighth           = TotalBits / 8,
    HighBits            = ((unsigned int) - 1) << (TotalBits - OneEighth) 
  };
  register unsigned int hash = 0;
  unsigned int magic = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << OneEighth) + (*str);
    if ((magic = hash & HighBits) != 0) {
      hash ^= (magic >> ThreeQuarters);
      hash &= ~magic;
    }
  }
  return hash;
}
/* End Of P. J. Weinberger Hash Function */
static unsigned int ELFHash2(const char* str, unsigned int len)
{
  unsigned int hash = 0;
  unsigned int x = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = (hash << 4) + (*str);
    if ((x = hash & 0xF0000000L) != 0) {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }
  return hash;
}
/* End Of DEK Hash Function */
static unsigned int BPHash(const char* str, unsigned int len)
{
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash << 7 ^ (*str);
  }
  return hash;
}
/* End Of AP Hash Function */
//���־����Hash�㷨��ʵ��(Դ����)
//��PHP�г��ֵ��ַ���Hash����
static uint32_t hashpjw(char* arKey, unsigned int nKeyLength)
{
  uint32_t h = 0, g;
  char* arEnd = arKey + nKeyLength;
  while (arKey < arEnd) {
    h = (h << 4) + *arKey++;
    if ((g = (h & 0xF0000000))) {
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }
  return h;
}
//��OpenSSL�г��ֵ��ַ���Hash����
static uint32_t lh_strhash11(const char* str)
{
  int i, l;
  uint32_t ret = 0;
  unsigned short* s;
  if (str == NULL) {
    return(0);
  }
  l = ((int)strlen(str) + 1) / 2;
  s = (unsigned short*)str;
  for (i = 0; i < l; ++i) {
    ret ^= (s[i] << (i & 0x0f));
  }
  return(ret);
}
/* The following hash seems to work very well on normal text strings
* no collisions on /usr/dict/words and it distributes on %2^n quite
* well, not as good as MD5, but still good.
*/
static uint32_t lh_strhash1(const char* c)
{
  uint32_t ret = 0;
  long n;
  uint32_t v;
  int r;
  if ((c == NULL) || (*c == '\0')) {
    return(ret);
  }
  /*
  unsigned char b[16];
  MD5(c,strlen(c),b);
  return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24));
  */
  n = 0x100;
  while (*c) {
    v = n | (*c);
    n += 0x100;
    r = (int)((v >> 2) ^ v) & 0x0f;
    ret = (ret * (32 - r));
    ret &= 0xFFFFFFFFL;
    ret ^= v * v;
    c++;
  }
  return((ret >> 16) ^ ret);
}
//��MySql�г��ֵ��ַ���Hash����
#ifndef NEW_HASH_FUNCTION
/* Calc hashvalue for a key */
static uint32_t calc_hashnr(const char* key, uint32_t length)
{
  register uint32_t nr = 1, nr2 = 4;
  while (length--) {
    nr ^= (((nr & 63) + nr2) * ((uint32_t)(uint8_t) * key++)) + (nr << 8);
    nr2 += 3;
  }
  return((uint32_t) nr);
}
/* Calc hashvalue for a key, case indepenently */
static uint32_t calc_hashnr_caseup(const char* key, uint32_t length)
{
  register uint32_t nr = 1, nr2 = 4;
  while (length--) {
    nr ^= (((nr & 63) + nr2) * ((uint32_t)(uint8_t) toupper(*key++))) + (nr << 8);
    nr2 += 3;
  }
  return((uint32_t) nr);
}
#else
/*
* Fowler/Noll/Vo hash
*
* The basis of the hash algorithm was taken from an idea sent by email to the
* IEEE Posix P1003.2 mailing list from Phong Vo (kpv@research.att.com) and
* Glenn Fowler (gsf@research.att.com). Landon Curt Noll (chongo@toad.com)
* later improved on their algorithm.
*
* The magic is in the interesting relationship between the special prime
* 16777619 (2^24 + 403) and 2^32 and 2^8.
*
* This hash produces the fewest collisions of any function that we've seen so
* far, and works well on both numbers and strings.
*/
static uint32_t calc_hashnr(const byte* key, uint32_t len)
{
  const byte* end = key + len;
  uint32_t hash;
  for (hash = 0; key < end; key++) {
    hash *= 16777619;
    hash ^= (uint32_t) * (uint8_t*) key;
  }
  return (hash);
}
static uint32_t calc_hashnr_caseup(const byte* key, uint32_t len)
{
  const byte* end = key + len;
  uint32_t hash;
  for (hash = 0; key < end; key++) {
    hash *= 16777619;
    hash ^= (uint32_t)(uint8_t) toupper(*key);
  }
  return (hash);
}
#endif
//Mysql�ж��ַ���Hash�����������˴�Сд
//����һ�������ַ���Hash����
static unsigned int MysqlHash(const char* str, uint32_t len)
{
  register unsigned int h = 0;
  register const unsigned char* p = (const unsigned char*)str;
  uint32_t i = 0;
  for (; i<len; ++i, ++p) {
    h = 31 * h + *p;
  }
  return h;
}
//���Կ������¼�����ϣ������
//MurmurHash��CityHash��FarmHash��XXHash��SpookyHash��lookup3��SimHash��SuperFastHash��FNV��CRC32��
//������ϣ�����Ĺ�ϣֵһ����32bit��64bit��128bit������Ҳ�����ñ���MD5��SHA1�ȼ��ܹ�ϣ������
//��Ȼ���ܹ�ϣ��������ڷǼ��ܹ�ϣ�����ٶ�Ҫ��Щ���������ǵĹ�ϣֵҪ������������ͻ�ĸ��ʻ��С��
//һ����ΪSHA�����г�ͻ����Ϊ��������ͻ�ĸ���ԶԶС��Ӳ����������ĸ��ʡ�
//���⣬������һ��ʵ�飬��ϣ������javaʵ�֣�������һǧ���ٶ������ַ�������ϣֵ��int�ͣ���������Χ��0~0x7fffffff��
//����SuperFastHash�������Ĺ�ϣ�����ĳ�ͻ����0.3%���ң�����Ҷ�ĳ�������Լ��޸ģ�
//�����murmurhash�еĳ˷�������Ϊ�����������߰�XXHash��ĳһ�д���ȥ������ͻ��һ������٣��Ƚ����档 

// MurMurHash �㷨���ǷǼ���HASH�㷨�����ܸܺߣ� 
// �ȴ�ͳ��CRC32,MD5��SHA-1���������㷨���Ǽ���HASH�㷨�����Ӷȱ���ͺܸߣ������������ϵ���Ҳ���ɱ��⣩ 
// ��HASH�㷨Ҫ��ܶ࣬���Ҿ�˵����㷨����ײ�ʺܵ�. 
// http://murmurhash.googlepages.com/ 
static uint64_t MurMurHash(const char* key, int len) {  
  uint8_t* buf = (uint8_t*)key;  
  uint32_t seed = 0x1234ABCD;
  uint64_t m = 0xc6a4a7935bd1e995L;  
  int r = 47, remaining = len;
  uint64_t h = seed ^ (len * m); 
  uint64_t k;
  while (remaining >= 8) {
    k = *(uint64_t*)buf;
    k *= m;
    k ^= k >> r;  
    k *= m;
    h ^= k;  
    h *= m;
    remaining -= sizeof(k);
    buf += sizeof(k);
  }  
  
  if (remaining > 0) {
    uint64_t finish = 0;
    memcpy(&finish, buf, remaining);
    // for big-endian version, do this first:   
    // finish.position(8-buf.remaining());  
    h ^= finish;  
    h *= m;  
  }  
  
  h ^= h >> r;  
  h *= m;  
  h ^= h >> r; 
  return h;  
}  
static unsigned int murMurHash(const void *key, int len)
{
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  const int seed = 97;
  unsigned int h = seed ^ len;
  // Mix 4 bytes at a time into the hash
  const unsigned char *data = (const unsigned char *)key;
  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;
    k *= m; 
    k ^= k >> r; 
    k *= m; 
    h *= m; 
    h ^= k;
    data += 4;
    len -= 4;
  }
  // Handle the last few bytes of the input array
  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
    h *= m;
  };
  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}
static int str_setsize(str_t* s, int l)
{
  if (l > s->l) {
    s->s = (char*)prealloc(s->s, l + 1);
  }
  s->l = l;
  if (s->s) {
    s->s[l] = 0;
  }
  return 0;
}
#define str_setstr1(_s, ss) str_setstr(_s, (char*)(ss).s, (ss).l)
#define str_setstr2(_s, ss) str_setstr(_s, ss, -1)
static int str_setstr(str_t* s, const char* s0, int l)
{
  l = (l >= 0) ? l : (int)strlen(s0);
  str_setsize(s, l);
  memcpy(s->s, s0, l);
  return 0;
}
#define str_isempty(_s)  0==_s->l
#define str_trimleft(_s)  _s->l=STRtrimleft(_s->s, _s->l)
#define str_trimright(_s)  _s->l=STRtrimright(_s->s, _s->l)
static int str_free(str_t* s)
{
  if (s->s) {
    pfree(s->s);
    s->s = 0, s->l = 0;
  }
  return 0;
}
static int str_frees(str_t* s, int n)
{
  for (; n-- > 0; ++s) {
    if (s->s) {
      pfree(s->s);
      s->s = 0, s->l = 0;
    }
  }
  return 0;
}
static int str_del(str_t* s, int i, int l)
{
  ASSERT((i + l) <= s->l);
  memcpy(s->s + i, s->s + i + l, s->l - i - l + 1);
  s->l -= l;
  return 0;
}
static int str_delchr(str_t* s, int ch) {
  s->l = cstr_delchr(s->s, s->l, ch);
  s->s[s->l] = 0;
  return 0;
}
// �� [i, i+n] ֮����� [s0 l0]
static int str_ins(str_t* s, int i, int n, const char* s0, int l0)
{
#if 0
  str_t ss[1] = {0};
  str_setsize(ss, s->l + l0 - n);
  memrep(s->s, s->l, i, n, s0, l0, ss->s, ss->l);
  str_free(s);
  s->s = ss->s, s->l = ss->l;
#else
#endif
  return 0;
}
static IPOINT str_get_word(const str_t* s, int k)
{
  IPOINT pt;
  pt = iPOINT(k, k);
  for (; pt.x > 0; pt.x--) {
    if (s->s[pt.x - 1] > 0 && !isalnum(s->s[pt.x - 1])) {
      break;
    }
  }
  for (; pt.y < s->l; pt.y++) {
    if (s->s[pt.y] > 0 && !isalnum(s->s[pt.y])) {
      break;
    }
  }
  return pt;
}
static IPOINT str_get_line(const str_t* s, int k)
{
  IPOINT pt;
  pt = iPOINT(k, k);
  for (; pt.x > 0; pt.x--) {
    if ('\n' == s->s[pt.x - 1]) {
      break;
    }
  }
  for (; pt.y < s->l; pt.y++) {
    if ('\n' == s->s[pt.y]) {
      break;
    }
  }
  return pt;
}
static int str_pt2pos(const str_t* s, IPOINT pt0)
{
  IPOINT pt = {0};
  int i = 0;
  int ch = '\n';
  for (i = 0; i < s->l && pt.y <= pt0.y; ++i) {
    if (pt.y == pt0.y && pt.x >= pt0.x) {
      return i;
    }
    ++pt.x;
    if (ch == s->s[i]) {
      ++pt.y;
      pt.x = 0;
      if (pt.y > pt0.y) {
        return i;
      }
    }
  }
  return i;
}
#define str_cat_cstr(s, ss)  str_cat(s, STR2(ss, strlen(ss)))
static int str_catc(str_t* s, int c)
{
  int l = s->l;
  str_setsize(s, s->l + 1);
  s->s[l] = c;
  return 0;
}
static int str_cat(str_t* s, str_t s1)
{
  int l = s->l;
  str_setsize(s, s->l + s1.l);
  memcpy(s->s + l, s1.s, s1.l);
  return 0;
}
static int str_cat2(str_t* s, str_t s1, str_t s2)
{
  int l = s->l;
  str_setsize(s, s->l + s1.l + s2.l);
  memcpy(s->s + l, s1.s, s1.l);
  memcpy(s->s + l + s1.l, s2.s, s2.l);
  return 0;
}
static int str_cat3(str_t* s, str_t s1, str_t s2, str_t s3)
{
  int l = s->l;
  str_setsize(s, s->l + s1.l + s2.l + s3.l);
  memcpy(s->s + l, s1.s, s1.l);
  memcpy(s->s + l + s1.l, s2.s, s2.l);
  memcpy(s->s + l + s1.l + s2.l, s3.s, s3.l);
  return 0;
}
static int str_cat4(str_t* s, str_t s1, str_t s2, str_t s3, str_t s4)
{
  int l = s->l;
  str_setsize(s, s->l + s1.l + s2.l + s3.l + s4.l);
  memcpy(s->s + l, s1.s, s1.l);
  memcpy(s->s + l + s1.l, s2.s, s2.l);
  memcpy(s->s + l + s1.l + s2.l, s3.s, s3.l);
  memcpy(s->s + l + s1.l + s2.l + s3.l, s4.s, s4.l);
  return 0;
}
static int str_cat_nstr(str_t* s, str_t s1, int n)
{
  int i, l = s->l;
  str_setsize(s, s->l + s1.l * n);
  for (i = 0; i < n; ++i) {
    memcpy(s->s + l + i * s1.l, s1.s, s1.l);
  }
  return 0;
}
static int str_cat_nchr(str_t* s, int c1, int n)
{
  int i, l = s->l;
  str_setsize(s, s->l + n);
  for (i = 0; i < n; ++i) {
    s->s[l + i] = c1;
  }
  return 0;
}
static int str_trim(str_t* str, const char* sp)
{
  str_t s = _cstrtrim_c(str->s, str->l, sp);
  memcpy(str->s, s.s, s.l);
  str->s[s.l] = 0;
  return s.l;
}
#define STRleft(str, pos)  STRmid(str, 0, (pos))
#define STRright(str, pos) STRmid(str, pos, str.l)
static str_t STRmid(str_t str, int beg, int end)
{
  _cstrmid0(str.s, str.l, &beg, &end);
  return STR2(str.s+beg, end-beg);
}
static int STRsplitc(str_t s, const char* delims, const char* trimstr, str_t* out, int maxout, str_t* ps) {
  int i=0;
  uint8_t delimsmap[256] = {0};
  get_delimsmap(delimsmap, delims);
  for (;i<maxout && s.l>0;++i) {
    str_t s0 = s;
    char* e = s.s + s.l;
    for (; s.s < e; ++s.s) {
      if (delimsmap[(uint8_t)(*s.s)]) {
        s0.l = s.s - s0.s;
        ++s.s;
        break;
      }
    }
    s.l = e - s.s;
    if (ps) {
      *ps = s;
    }
    if (trimstr) {
      s0 = _cstrtrim_c(s0.s, s0.l, trimstr);
    }
    out[i] = s0;
  }
  return i;
}
static str_t STRsplit(str_t s, const char* delims, const char* trimstr, str_t* ps)
{
  str_t s0 = s;
  char* e = s.s + s.l;
  uint8_t delimsmap[256] = {0};
  get_delimsmap(delimsmap, delims);
  for (; s.s < e; ++s.s) {
    if (delimsmap[(uint8_t)(*s.s)]) {
      s0.l = s.s - s0.s;
      ++s.s;
      break;
    }
  }
  s.l = e - s.s;
  if (ps) {
    *ps = s;
  }
  if (trimstr) {
    s0 = _cstrtrim_c(s0.s, s0.l, trimstr);
  }
  return s0;
}
static int STRsplit_find(str_t s, str_t s2, int i, int ignore_case, const char* delims, const char* trimstr, str_t* ps)
{
  //str_t s0 = s;
  //char* e = s.s + s.l;
  uint8_t delimsmap[256] = {0};
  int j, k;
  get_delimsmap(delimsmap, delims);
  j = cstr_find(s.s, s.l, s2.s, s2.l, i, ignore_case);
  if (j>=0 && ps) {
    for (k=j; k>i; --k) {
      if (delimsmap[(uint8_t)s.s[k]]) {
        ++k;
        break;
      }
    }
    ps->s = s.s + k;
    for (k=j+s2.l; k<s.l; ++k) {
      if (delimsmap[(uint8_t)s.s[k]]) {
        break;
      }
    }
    ps->l = s.s + k - ps->s;
    if (trimstr) {
      *ps = _cstrtrim_c(ps->s, ps->l, trimstr);
    }
    for (; k<s.l; ++k) {
      if (!delimsmap[(uint8_t)s.s[k]]) {
        break;
      }
    }
    j = k;
  }
  return j;
}
static int STRsplit_key(str_t s, int i, const uint8_t* trimstrmap, str_t* key)
{
  //int prech = 0;
  int ibeg = -1, iend = -1;
  for (; i < s.l && trimstrmap[(uint8_t)s.s[i]]; ++i) {}
  ibeg = i;
  for (; i < s.l && 0 == trimstrmap[(uint8_t)s.s[i]]; ++i) {}
  iend = i;
  for (; i < s.l && trimstrmap[(uint8_t)s.s[i]]; ++i) {}
  if (ibeg >= 0 && iend >= ibeg) {
    key->s = s.s + ibeg;
    key->l = iend - ibeg;
    return i;
  }
  return -1;
}
static int STRsplit_string(str_t s, int i, int k_beg_chr, int k_end_chr, int slash, const uint8_t* trimstrmap, str_t* value)
{
  int prech = 0;
  int ibeg = -1, iend = -1;
  for (; i < s.l && trimstrmap[(uint8_t)s.s[i]]; ++i) {}
  prech = 0;
  for (; i < s.l; ++i) {
    if (s.s[i] == k_beg_chr && prech != slash) {
      ibeg = ++i;
      break;
    }
    prech = s.s[i];
  }
  prech = 0;
  for (; i < s.l; ++i) {
    if (s.s[i] == k_end_chr && prech != slash) {
      iend = i++;
      break;
    }
    prech = s.s[i];
  }
  for (; i < s.l && trimstrmap[(uint8_t)s.s[i]]; ++i) {}
  if (ibeg >= 0 && iend >= ibeg) {
    value->s = s.s + ibeg;
    value->l = iend - ibeg;
    return i;
  }
  return -1;
}
static int STRsplit_pair(str_t s, int i, int eqchr, int v_beg, int v_end, const char* trimstr, str_t* key, str_t* value)
{
  int i0 = i;
  uint8_t trimstrmap[256] = {0};
  get_delimsmap(trimstrmap, trimstr);
  for (; i < s.l; ++i) {
    if (s.s[i] == eqchr) {
      str_t s1 = STR2(s.s + i0, i - i0);
      ++i;
      STRsplit_key(s1, 0, trimstrmap, key) > 0 && (i0 = STRsplit_string(s, i, v_beg, v_end, '\\', trimstrmap, value));
      return i0 + i;
    }
  }
  return -1;
}
static str_t STRsplit_str(str_t s, const char* delims, const char* trimstr, str_t* ps, char* buf, int buflen)
{
  str_t s0 = STRsplit(s, delims, trimstr, ps);
  if (buf) {
    cstr_cpy((char*)buf, buflen, s0.s, s0.l);
  }
  return s0;
}
static int STRsplit_int(str_t s, const char* delims, const char* trimstr, str_t* ps)
{
  str_t s0 = STRsplit(s, delims, trimstr, ps);
  return atoi((char*)s0.s);
}
static int STRsplitn(str_t s, const char* delims, const char* trimstr, int minlen)
{
  int i;
  char* e = s.s + s.l;
  for (i = 0; s.s < e; ) {
    str_t ss = STRsplit(s, delims, trimstr, &s);
    if (ss.l>=minlen) {
      ++i;
    }
  }
  return i;
}
static int STRsplitv(str_t s, const char* delims, const char* trimstr, str_t* ps, str_t* v, int vl)
{
  int i;
  char* e = s.s + s.l;
  //uint8_t trimstrmap[256] = {0};
  //get_delimsmap(trimstrmap, trimstr);
  for (i = 0; s.s < e && i < vl; ++i) {
    v[i] = STRsplit(s, delims, trimstr, &s);
  }
  if (ps) {
    *ps = s;
  }
  return i;
}
static int str_splitv(str_t s, const char* delims, const char* trimstr, str_t* ps, str_t* v, int vl, int minlen)
{
  int i;
  char* e = s.s + s.l;
  //uint8_t trimstrmap[256] = {0};
  //get_delimsmap(trimstrmap, trimstr);
  for (i = 0; s.s < e && i < vl;) {
    str_t ss = STRsplit(s, delims, trimstr, &s);
    if (ss.l >= minlen) {
      str_setstr1(v + i, ss);
      ++i;
    }
  }
  if (ps) {
    *ps = s;
  }
  return i;
}
typedef struct vstr_t {
  str_t* v;
  int n;
} vstr_t;
static int vstr_free(vstr_t* sv)
{
  if (sv->v) {
    str_frees(sv->v, sv->n);
    pfree(sv->v);
  }
  sv->v = 0;
  sv->n = 0;
  return 0;
}
static int vstr_frees(vstr_t* sv, int n) {
  int i;
  for (i=0; i<n; ++i) {
    vstr_free(sv+i);
  }
  return 0;
}
static int vstr_setsize(vstr_t* sv, int n)
{
#if 0
  int i, old_n = sv->n;
  if (n<old_n) {
    for (i = n; i < sv->n; ++i) {
      str_free(sv->v + i);
    }
  } else if (n>old_n) {
    //myrealloc((void**)&(sv->v), n * sizeof(str_t), __FILE__, __LINE__);
    sv->v = (str_t*)prealloc(sv->v, sizeof(str_t)*n);
    memset(sv->v+old_n, 0, (n-old_n)*sizeof(str_t));
  }
  sv->n = n;
#else
  VEC_SETSIZE(sv->v, sv->n, n, str_free);
#endif
  return 0;
}
static int vstr_set_str(vstr_t* sv, int i, str_t s0)
{
  return str_setstr1(sv->v + i, s0);
}
static int vstr_copy(vstr_t* sv, const vstr_t* sv2)
{
  int i;
  vstr_setsize(sv, sv2->n);
  for (i = 0; i < sv->n; ++i) {
    str_setstr(sv->v + i, sv2->v[i].s, sv2->v[i].l);
  }
  return 0;
}
static int vstr_set_cstrs(vstr_t* sv, int n, const char** ps)
{
  int i;
  vstr_setsize(sv, n);
  if (ps) {
    for (i = 0; i < n; ++i) {
      int l = (int)strlen(ps[i]);
      str_setstr(sv->v + i, ps[i], l);
    }
  }
  return 0;
}
#define vstr_ins_c_str(sv, i, c_str)  vstr_ins_str(sv, i, STR1(c_str))
static int vstr_del_str(vstr_t* sv, int i)
{
  if (0 <= i && i < sv->n) {
    str_free(sv->v + i);
    MEMMOVE(sv->v + i, sv->v + i + 1, sv->n - i - 1);
    return 1;
  }
  return 0;
}
#define vstr_ins_str(sv, i, s0)  vstr_ins_cstr(sv, i, s0.s, s0.l)
#define vstr_push_str(sv, s0)  vstr_ins_cstr(sv, sv->n, s0.s, s0.l)
#define vstr_push_str2(sv, s, l)  vstr_ins_cstr(sv, sv->n, s, l)
#define vstr_push_cstr(sv, s0, l0)  vstr_ins_cstr(sv, sv->n, s0, l0)
static int vstr_ins_cstr(vstr_t* sv, int i, const char* s0, int l0)
{
  int n = sv->n;
  str_t s[1] = {0};
  i = (i < 0 || i > n) ? n : i;
  //i = BOUND(i, 0, n);
  vstr_setsize(sv, sv->n + 1);
  MEMMOVE(sv->v + i + 1, sv->v + i, n - i);
  str_setstr(s, s0, l0);
  sv->v[i] = *s;
  return 0;
}
static int strv_find(const str_t* sv, int n, str_t s, int i, int ignore_case) {
  ASSERT(i>=0);
  for (; i < n; ++i) {
    if (0 == str_cmp(sv[i], s, ignore_case)) {
      return i;
    }
  }
  return -1;
}
static int vstr_find(const vstr_t* sv, str_t s, int i, int ignore_case)
{
  ASSERT(i>=0);
  for (; i < sv->n; ++i) {
    if (0 == str_cmp(sv->v[i], s, ignore_case)) {
      return i;
    }
  }
  return -1;
}
// ����s���б�
static int vstr_find_instr(vstr_t* sv, str_t s, int i, int ignore_case)
{
  ASSERT(i>=0);
  for (; i < sv->n; ++i) {
    if (0 <= cstr_find(sv->v[i].s, sv->v[i].l, s.s, s.l, 0, ignore_case)) {
      return i;
    }
  }
  return -1;
}
#define vstr_join vstr_merge
static int vstr_merge(const vstr_t* sv, str_t* s, const char* delims)
{
  int i, k = s->l, n = s->l, l = (int)strlen(delims);
  for (i = 0; i < sv->n; ++i) {
    n += l;
    n += sv->v[i].l;
  }
  str_setsize(s, n);
  for (i = 0; i < sv->n; ++i) {
    memcpy(s->s + k, sv->v[i].s, sv->v[i].l);
    k += sv->v[i].l;
    memcpy(s->s + k, delims, l);
    k += l;
  }
  return k;
}
static int vstr_split_str(vstr_t* sv, str_t s, const char* delims, const char* trimstr, int minlen)
{
  int n = STRsplitn(s, delims, trimstr, minlen);
  vstr_setsize(sv, n);
  n = str_splitv(s, delims, trimstr, 0, sv->v, n, minlen);
  vstr_setsize(sv, n);
  return 0;
}
static int vstr_split_str_add(vstr_t* sv, str_t s, const char* delims, const char* trimstr, int minlen)
{
  int old_n = sv->n;
  int n = STRsplitn(s, delims, trimstr, minlen);
  vstr_setsize(sv, old_n+n);
  n = str_splitv(s, delims, trimstr, 0, sv->v+old_n, n, minlen);
  vstr_setsize(sv, old_n+n);
  return 0;
}
static int vstr_getstr(vstr_t* sv, IPOINT beg, IPOINT pos, str_t* s)
{
  int i, k, n;
  if (iPOINT_cmp(beg, pos) > 0) {
    T_SWAP(IPOINT, beg, pos);
  }
  ASSERT(beg.y < sv->n && pos.y < sv->n);
  n = pos.x - beg.x + (pos.y - beg.y) * 1;
  for (i = beg.y; i < pos.y; ++i) {
    n += sv->v[i].l;
  }
  str_setsize(s, n);
  k = sv->v[beg.y].l - beg.x;
  memcpy(s->s, sv->v[beg.y].s + beg.x, k);
  for (i = beg.y; i < pos.y; ++i) {
    s->s[k++] = '\n';
    memcpy(s->s + k, sv->v[i].s, sv->v[i].l);
    k += sv->v[i].l;
  }
  return 0;
}
static int memfind_block(const void* s, const void* s1, int n, int ns1, int step)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (0 == memcmp((char*)s + i * step, s1, ns1)) {
      return i;
    }
  }
  return -1;
}
static char* memfind(const char* buf, unsigned int buf_len, const char* byte_sequence, unsigned int byte_sequence_len)
{
  char* bf = (char*)buf;
  char* bs = (char*)byte_sequence;
  char* p  = bf;
  while (byte_sequence_len <= (buf_len - (p - bf))) {
    unsigned int b = *bs & 0xFF;
    if ((p = (char*) memchr(p, b, buf_len - (p - bf))) != NULL) {
      if ((memcmp(p, byte_sequence, byte_sequence_len)) == 0) {
        return p;
      }
      else {
        ++p;
      }
    }
    else {
      break;
    }
  }
  return NULL;
}
static int str_find_chr(str_t s, int ch, int i)
{
  return cstr_findchr(s.s, s.l, ch, i);
}
static int str_replace_str(str_t* s1, str_t s2, str_t s3, int ignore_case)
{
  int j = 0;
  if (s2.l >= s3.l) {
    s1->l = cstr_replacestr(s1->s, s1->l, s1->l, s2.s, s2.l, s3.s, s3.l, ignore_case, 0, &j);
  }
  else {
    int pl1, l = s1->l;
    pl1 = cstr_replacestr(s1->s, l, l, s2.s, s2.l, 0, 0, ignore_case, -1, &j);
    str_setsize(s1, pl1);
    s1->l = cstr_replacestr(s1->s, l, s1->l, s2.s, s2.l, s3.s, s3.l, ignore_case, pl1, &j);
  }
  return j;
}
// CString formatting
static int str_formatv(str_t* s, int isadd, const char* lpszFormat, va_list argList)
{
  int nMaxLen = 0;
  str_t s2[1] = {0}, t;
  nMaxLen = cstr_vnprintf(0, 0, lpszFormat, argList);
  str_setsize(s2, nMaxLen + 1);
  nMaxLen = cstr_vnprintf((char*)s2->s, nMaxLen, lpszFormat, argList);
  ASSERT(nMaxLen <= s->l);
  str_setsize(s2, nMaxLen);
  if (isadd) {
    str_cat(s, *s2);
  }
  else {
    CC_SWAP(*s2, *s, t);
  }
  str_free(s2);
  return nMaxLen;
}
// formatting (using wsprintf style formatting)
static int str_format(str_t* s, int isadd, const char* lpszFormat, ...)
{
  int ret = 0;
  va_list argList;
  va_start(argList, lpszFormat);
  ret = str_formatv(s, isadd, lpszFormat, argList);
  va_end(argList);
  return ret;
}
static str_t* str_tolower(str_t* str)
{
  TRANSFORM(str->s, str->s + str->l, str->s, tolower);
  return str;
}
static str_t* str_toupper(str_t* str)
{
  TRANSFORM(str->s, str->s + str->l, str->s, toupper);
  return str;
}
#define str_toi(str)   str2i(str.s, str.l, 0, 0, 0)
#define str_tof(str)   str2f(str.s, str.l, 0, 0, 0)
static int str_skip_nline(const char* s, int l, int i, int n, const char* trimstr, int* pn)
{
  char trimstrmap[256] = {0};
  int j = 0;
  if (n > 0) {
    for (; *trimstr;) {
      trimstrmap[(uint8_t)(*trimstr++)] = 1;
    }
    for (; i < l; ++i) {
      if (trimstrmap[(uint8_t)s[i]]) {
        ++j;
        if (j >= n) {
          ++i;
          break;
        }
      }
    }
  }
  if (pn) {
    *pn = j;
  }
  return i;
}
static IPOINT str_pos2pt(const char* s, int l, const char* trimstr)
{
  char trimstrmap[256] = {0};
  IPOINT pt = {0};
  int i = 0, i0=0;
  if (l > 0) {
    for (; *trimstr;) {
      trimstrmap[(uint8_t)(*trimstr++)] = 1;
    }
    for (; i < l;) {
      if (trimstrmap[(uint8_t)s[i]]) {
        ++pt.y;
        i0 = i;
        pt.x = 0;
      }
      else {
        pt.x = i-i0;
      }
      chr_next(s, i, &i);
    }
  }
  pt.x = i-i0;
  return pt;
}
static int test_rev_word()
{
  char s[100] = "today is a good day";
  char s2[100] = "today is a good day";
  int n = (int)strlen(s);
  rev_word(n, s);
  rev_word(n, s);
  ASSERT(0 == strcmp(s, s2));
  return 0;
}
static int test_strv()
{
  vstr_t sv[1] = {0};
  int i, j;
  vstr_setsize(sv, 1000);
  for (j = 0; j < 100; ++j) {
    for (i = 0; i < sv->n; ++i) {
      str_setsize(sv->v + i, 100 * j);
    }
  }
  vstr_free(sv);
  return 0;
}
static int STRpath_split(const char* dir, int len, const char* flag, char* buf, int buflen)
{
  int i = 0, l = 0, len1 = buflen - 1;
  if (buflen > 0) {
    int flaglen = cstr_len(flag);
    const char* driver_end = cstr_chr(dir, len, ':');// ������
    const char* path = driver_end ? driver_end : dir;
    const char* filename1 = cstr_rchr(dir, len, '\\');
    const char* filename2 = cstr_rchr(dir, len, '/');
    const char* filename = MAX(filename1, filename2);
    const char* ext = 0;
    filename = filename ? (filename + 1) : (dir + len);
    ext = cstr_rchr(filename, dir + len - filename, '.');
    ext = ext ? ext : (dir + len);
    filename = MIN(filename, ext);
    l = driver_end - dir;
    if ((cstr_chr(flag, flaglen, 'd')) && driver_end) {
      if (l < (len1 - i)) {
        memcpy(buf + i, dir, l);
        i += l;
      }
    }
    if (cstr_chr(flag, flaglen, 'p')) {
      l = filename - path;
      if (l < (len1 - i)) {
        memcpy(buf + i, path, l);
        i += l;
      }
    }
    if (cstr_chr(flag, flaglen, 'n')) {
      l = ext - filename;
      if (l < (len1 - i)) {
        memcpy(buf + i, filename, l);
        i += l;
      }
    }
    if (cstr_chr(flag, flaglen, 'x')) {
      l = dir + len - ext;
      if (l < (len1 - i)) {
        memcpy(buf + i, ext, l);
        i += l;
      }
    }
  }
  if (i < buflen) {
    buf[i] = 0;
  }
  return i;
}
static int str_skip_delims(const char* s, int l, int i, const char* delims)
{
  int delimslen = (int)strlen(delims);
  for (; i < l; ++i) {
    if (!cstr_chr(delims, delimslen, s[i])) {
      break;
    }
  }
  return i;
}
enum {
  STROPT_INSERT,
    STROPT_DELETE,
};
static int str_interface(void* s0, int opt, int param1, int* param2)
{
  str_t* s = (str_t*)s0;
  switch (opt) {
  case STROPT_INSERT: {
    str_t* s1 = (str_t*)param2;
    str_ins(s, param1, 0, s1->s, s1->l);
                      }
    break;
  case STROPT_DELETE:
    //str_del(s, param1, (int)param2);
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}
//#include "cfile.h"

static int STRisempty(const char* s)
{
  return NULL == s || 0 == *s;
}

static int isutf16(const uint8_t* src, int srclen)
{
  int i;
  const uint8_t* _PCS;
  enum { GB2312_MATRIX = (94),
    DELTA = (0xA0),
    FONT_ROW_BEGIN = (16 + DELTA),
    FONT_ROW_END = (87 + DELTA),
    FONT_COL_BEGIN = (1 + DELTA),
    FONT_COL_END = (GB2312_MATRIX + DELTA),
    FONT_TOTAL = (72 * GB2312_MATRIX),
  };
  for (i = 0; i < srclen; i += 2) {
    _PCS = src + i;
    if (0 == *(_PCS) || 0 == (*(_PCS) & 0x80)) {
      return 0;
    }
    if (!(*(_PCS) & 0xff80)) {
      if (0 == (*(_PCS) & 0x80) ||
        (FONT_ROW_BEGIN <= *(_PCS) && *(_PCS) <= FONT_ROW_END && FONT_COL_BEGIN <= *(_PCS + 1) && *(_PCS + 1) <= FONT_COL_END)) {}
      else {
        return 0;
      }
    }
  }
  return 1;
}
static int istutf8(const uint8_t* str, int length)
{
  int i, nBytes = 0; //UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
  uint8_t chr;
  BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8
  for (i = 0; i < length; i++) {
    chr = *(str + i);
    // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
    if ((chr & 0x80) != 0) {
      bAllAscii = FALSE;
    }
    if (nBytes == 0) { //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD) {
          nBytes = 6;
        }
        else if (chr >= 0xF8) {
          nBytes = 5;
        }
        else if (chr >= 0xF0) {
          nBytes = 4;
        }
        else if (chr >= 0xE0) {
          nBytes = 3;
        }
        else if (chr >= 0xC0) {
          nBytes = 2;
        }
        else {
          return FALSE;
        }
        nBytes--;
      }
    }
    else { //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
      if ((chr & 0xC0) != 0x80) {
        return FALSE;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) { //Υ������
    return FALSE;
  }
  if (bAllAscii) { //���ȫ������ASCII, ˵������UTF-8
    return FALSE;
  }
  return TRUE;
}
static bool is_utf8(const char* str, unsigned int len)
{
  const unsigned char* ustr = (const unsigned char*)(str);
  const unsigned char* end = ustr + len;
  
  while (ustr < end)
  {
    const int uv = *ustr;
    if (uv < 0x80)
    {
      ++ustr;
      continue;
    }
    
    if (uv < 0xC0)
      return false;
    
    if ((uv < 0xE0) && (end - ustr > 1))
      ustr += 2;
    else if ((uv < 0xF0) && (end - ustr > 2))
      ustr += 3;
    else if ((uv < 0x1F) && (end - ustr > 3))
      ustr += 4;
    else
      return false;
  }
  return true;
}

/// return a pointer to the code unit of the character at pos
static const char* utf_char_ptr(const char* text, int pos)
{
  const unsigned char* ustr = (const unsigned char*)(text);
  const unsigned char* const end = ustr + strlen(text);
  int i;
  
  for (i = 0; i != pos; ++i)
  {
    const unsigned char uch = *ustr;
    if (uch < 0x80)
    {
      ++ustr;
      continue;
    }
    
    if (uch < 0xC0)        // use police ?
      return NULL;
    
    if ((uch < 0xE0) && (ustr + 1 < end)) //? *(ustr + 1) < 0xE0 
      ustr += 2;
    else if (uch < 0xF0 && (ustr + 2 <= end))
      ustr += 3;
    else if (uch < 0x1F && (ustr + 3 <= end))
      ustr += 4;
    else
      return NULL;
  }
  
  return (const char*)(ustr);
}

/// return a code point (max 16 bits?) and the len in code units of the character at pos
static ushort utf_char_at(const char* text_utf8, unsigned pos, unsigned * len)
{
  const char* end;
  ushort uch;
  if (!text_utf8)
    return 0;
  
  if (pos)
  {
    text_utf8 = utf_char_ptr(text_utf8, pos);
    if (!text_utf8)
      return 0;
  }
  
  uch = *(const ushort*)(text_utf8);
  if (uch < 0x80)
  {
    if (len)
      *len = 1;
    
    return *text_utf8;  // uch ?
  }
  
  if (uch < 0xC0)    // use police or ??
  {
    if (len)
      *len = 0;
    
    return 0;
  }
  
  end = text_utf8 + strlen(text_utf8);
  
  if (uch < 0xE0 && (text_utf8 + 1 <= end))
  {
    if (len)
      *len = 2;
    return ((wchar_t)(uch & 0x1F) << 6) | ((text_utf8)[1] & 0x3F);
  }
  else if (uch < 0xF0 && (text_utf8 + 2 <= end))
  {
    if (len)
      *len = 3;
    
    return ((((uch & 0xF) << 6) | ((text_utf8)[1] & 0x3F)) << 6) | ((text_utf8)[2] & 0x3F);
  }
  else if (uch < 0x1F && (text_utf8 + 3 <= end))
  {
    if (len)
      *len = 4;
    return ((((((uch & 0x7) << 6) | ((text_utf8)[1] & 0x3F)) << 6) | ((text_utf8)[2] & 0x3F)) << 6) | ((text_utf8)[3] & 0x3F);
  }
  
  if (len)
    *len = 0;
  
  return 0;
}
static uint32_t nextCharA(const void* str, int len, int* i) {
  if (*i<len) {
    uint8_t* s = (uint8_t*)str;
    uint32_t code = s[(*i)++];
    if (code < 0x80) {
    } else {
      code = (code<<8) | s[(*i)++];
    }
    return code;
  }
  return 0;
}
/// return the first code point and move the pointer to next character, springing to the end by errors
static uint32_t utf8char(const unsigned char** pp, const unsigned char* end)
{
  const unsigned char* p = *pp;
  uint32_t ch, code = 0;
  if(p != end)
  {
    if(*p < 0x80)        // ASCII char   0-127 or 0-0x80
    {
      *pp = p;
      return *(p++);
    }
    ch = *p;
    if(ch < 0xC0)       // error? - move to end. Posible ANSI or ISO code-page 
    {
      //return *(p++); // temp: assume equal
      //p = end;
      //return 0;
      return 0;//def_encoding_error_police->next_code_point(p, end);
    }
    else if(ch < 0xE0 && (p + 1 <= end))      // two byte chararcter
    {
      code = ((ch & 0x1F) << 6) | (p[1] & 0x3F);
      p += 2;
    }
    else if(ch < 0xF0 && (p + 2 <= end))     // 3 byte character
    {
      code = ((((ch & 0xF) << 6) | (p[1] & 0x3F)) << 6) | (p[2] & 0x3F);
      p += 3;
    }
    else if(ch < 0x1F && (p + 3 <= end))   // 4 byte character
    {
      code = ((((((ch & 0x7) << 6) | (p[1] & 0x3F)) << 6) | (p[2] & 0x3F)) << 6) | (p[3] & 0x3F);
      p += 4;
    }
    else    //  error, go to end
    {
      p = end;
      code = 0;
    }
  }
  *pp = p;
  return code;
}

static uint32_t utf16char(const unsigned char** pbytes, const unsigned char* end, bool le_or_be)
{
  const unsigned char* bytes = *pbytes;
  uint32_t code = 0;
  if(le_or_be)
  {
    if((end - bytes >= 4) && ((bytes[1] & 0xFC) == 0xD8))
    {
      //32bit encoding
      uint32_t ch0 = bytes[0] | (bytes[1] << 8);
      uint32_t ch1 = bytes[2] | (bytes[3] << 8);
      
      code = ((ch0 & 0x3FF) << 10) | (ch1 & 0x3FF);
      bytes += 4;
    }
    else if(end - bytes >= 2)
    {
      code = bytes[0] | (bytes[1] << 8);
      bytes += 2;
    }
    else
    {
      bytes = end;
    }
  }
  else
  {
    if((end - bytes >= 4) && ((bytes[0] & 0xFC) == 0xD8))
    {
      //32bit encoding
      uint32_t ch0 = (bytes[0] << 8) | bytes[1];
      uint32_t ch1 = (bytes[2] << 8) | bytes[3];
      code = (((ch0 & 0x3FF) << 10) | (ch1 & 0x3FF)) + 0x10000;
      bytes += 4;
    }
    else if(end - bytes >= 2)
    {
      code = (bytes[0] << 8) | bytes[1];
      bytes += 2;
    }
    else
    {
      bytes = end;
    }
  }
  *pbytes = bytes;
  return code;
}

static uint32_t utf32char(const unsigned char** pbytes, const unsigned char* end, bool le_or_be)
{
  const unsigned char* bytes = *pbytes;
  uint32_t code = 0;
  if(end - bytes >= 4)
  {
    if(le_or_be)
      code = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    else
      code = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
    bytes += 4;
  } else {
    bytes = end;
  }
  *pbytes = bytes;
  return code;
}

static int put_utf8char(char* s, int i, uint32_t code)
{
  if(code < 0x80)
  {
    s[i++] = (char)(code);
  }
  else if(code < 0x800)
  {
    s[i++] = (char)(0xC0 | (code >> 6));
    s[i++] = (char)(0x80 | (code & 0x3F));
  }
  else if(code < 0x10000)
  {
    s[i++] = (char)(0xE0 | (code >> 12));
    s[i++] = (char)(0x80 | ((code >> 6) & 0x3F));
    s[i++] = (char)(0x80 | (code & 0x3F));
  }
  else
  {
    s[i++] = (char)(0xF0 | (code >> 18));
    s[i++] = (char)(0x80 | ((code >> 12) & 0x3F));
    s[i++] = (char)(0x80 | ((code >> 6) & 0x3F));
    s[i++] = (char)(0x80 | (code & 0x3F));
  }
  return i;
}

//le_or_be, true = le, false = be
static int put_utf16char(char* s, int i, uint32_t code, bool le_or_be)
{
  if(code <= 0xFFFF)
  {
    if(le_or_be)
    {
      s[i++] = (char)(code & 0xFF);
      s[i++] = (char)((code & 0xFF00) >> 8);
    }
    else
    {
      s[i++] = (char)((code & 0xFF00) >> 8);
      s[i++] = (char)(code & 0xFF);
    }
  }
  else
  {
    uint32_t ch0 = (0xD800 | ((code - 0x10000) >> 10));
    uint32_t ch1 = (0xDC00 | ((code - 0x10000) & 0x3FF));
    
    if(le_or_be)
    {
      s[i++] = (char)(ch0 & 0xFF);
      s[i++] = (char)((ch0 & 0xFF00) >> 8);
      
      s[i++] = (char)(ch1 & 0xFF);
      s[i++] = (char)((ch1 & 0xFF00) >> 8);
    }
    else
    {
      s[i++] = (char)((ch0 & 0xFF00) >> 8);
      s[i++] = (char)(ch0 & 0xFF);
      
      s[i++] = (char)((ch1 & 0xFF00) >> 8);
      s[i++] = (char)(ch1 & 0xFF);
    }
  }
  return i;
}

static int put_utf32char(char* s, int i, uint32_t code, bool le_or_be)
{
  if(le_or_be)
  {
    s[i++] = (char)(code & 0xFF);
    s[i++] = (char)((code & 0xFF00) >> 8);
    s[i++] = (char)((code & 0xFF0000) >> 16);
    s[i++] = (char)((code & 0xFF000000) >> 24);
  }
  else
  {
    s[i++] = (char)((code & 0xFF000000) >> 24);
    s[i++] = (char)((code & 0xFF0000) >> 16);
    s[i++] = (char)((code & 0xFF00) >> 8);
    s[i++] = (char)(code & 0xFF);
  }
  return i;
}

static int utf8_to_utf16(const char* s, bool le_or_be, char** putf16str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + s_len;
  
  MYREALLOC(*putf16str, s_len*2);
  
  //If there is a BOM, ignore it.
  if(s_len >= 3)
  {
    if(bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
    {
      bytes += 3;
      i = put_utf16char(*putf16str, i, 0xFEFF, le_or_be);
    }
  }
  
  while(bytes != end)
  {
    i = put_utf16char(*putf16str, i, utf8char(&bytes, end), le_or_be);
  }
  (*putf16str)[i] = 0;
  return i;
}

static int utf8_to_utf32(const char* s, bool le_or_be, char** putf32str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + s_len;
  
  MYREALLOC(*putf32str, s_len*2);
  //If there is a BOM, ignore it.
  if(s_len >= 3)
  {
    if(bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
    {
      bytes += 3;
      i = put_utf32char(*putf32str, i, 0xFEFF, le_or_be);
    }
  }
  
  while(bytes != end)
  {
    i = put_utf32char(*putf32str, i, utf8char(&bytes, end), le_or_be);
  }
  return i;
}

static int utf16_to_utf8(const char* s, char** putf8str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + s_len;
  bool le_or_be = true;
  MYREALLOC(*putf8str, s_len*2);
  //If there is a BOM, ignore it
  if(s_len >= 2)
  {
    if(bytes[0] == 0xFF && bytes[1] == 0xFE)
    {
      bytes += 2;
      le_or_be = true;
      
      (*putf8str)[i++] = (char)0xEF;
      (*putf8str)[i++] = (char)0xBB;
      (*putf8str)[i++] = (char)0xBF;
    }
    else if(bytes[0] == 0xFE && bytes[1] == 0xFF)
    {
      bytes += 2;
      le_or_be = false;
      (*putf8str)[i++] = (char)(0xEF);
      (*putf8str)[i++] = (char)(0xBB);
      (*putf8str)[i++] = (char)(0xBF);
    }
  }
  
  while(bytes != end)
  {
    i = put_utf8char(*putf8str, i, utf16char(&bytes, end, le_or_be));
  }
  return i;
}

static int utf16_to_utf32(const char* s, char** putf32str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + s_len;
  bool le_or_be = true;
  
  MYREALLOC(*putf32str, s_len*2);
  //If there is a BOM, ignore it
  if(s_len >= 2)
  {
    if(bytes[0] == 0xFF && bytes[1] == 0xFE)
    {
      bytes += 2;
      le_or_be = true;
      i = put_utf32char(*putf32str, i, 0xFEFF, true);
    }
    else if(bytes[0] == 0xFE && bytes[1] == 0xFF)
    {
      bytes += 2;
      le_or_be = false;
      i = put_utf32char(*putf32str, i, 0xFEFF, false);
    }
  }
  
  while(bytes != end)
  {
    i = put_utf32char(*putf32str, i, utf16char(&bytes, end, le_or_be), le_or_be);
  }
  return i;
}

static int utf32_to_utf8(const char* s, char** putf8str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + (s_len & (~4 + 1));
  
  bool le_or_be = true;
  MYREALLOC(*putf8str, s_len*2);
  //If there is a BOM, ignore it
  if(s_len >= 4)
  {
    if(bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0xFE && bytes[3] == 0xFF)
    {
      le_or_be = false;
      bytes += 4;
      (*putf8str)[i++] = (char)0xEF;
      (*putf8str)[i++] = (char)0xBB;
      (*putf8str)[i++] = (char)0xBF;
    }
    else if(bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0 && bytes[3] == 0)
    {
      le_or_be = true;
      bytes += 4;
      (*putf8str)[i++] = (char)0xEF;
      (*putf8str)[i++] = (char)0xBB;
      (*putf8str)[i++] = (char)0xBF;
    }
  }
  
  while(bytes < end)
  {
    i = put_utf8char(*putf8str, i, utf32char(&bytes, end, le_or_be));
  }
  return i;
}

static int utf32_to_utf16(const char* s, char** putf16str)
{
  int i=0, s_len = (int)strlen(s);
  const unsigned char * bytes = (const unsigned char*)(s);
  const unsigned char * end = bytes + (s_len & (~4 + 1));
  
  bool le_or_be = true;
  MYREALLOC(*putf16str, s_len*2);
  //If there is a BOM, ignore it
  if(s_len >= 4)
  {
    if(bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0xFE && bytes[3] == 0xFF)
    {
      le_or_be = false;
      bytes += 4;
      i = put_utf16char(*putf16str, i, 0xFEFF, false);
    }
    else if(bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0 && bytes[3] == 0)
    {
      le_or_be = true;
      bytes += 4;
      i = put_utf16char(*putf16str, i, 0xFEFF, true);
    }
  }
  
  while(bytes < end)
  {
    i = put_utf16char(*putf16str, i, utf32char(&bytes, end, le_or_be), le_or_be);
  }
  (*putf16str)[i] = 0;
  return i;
}

//һ����˵Ҫ���ַ������ʽ�����жϾ��Ǹ��ݸ��ֱ��������ֽڵ���ֹ��Χ���жϣ�
//���������ֹ��Χ����Ϊ��ĳ���ַ����롣����������˵��ͬ���ַ������п��ܻ����ͬ�����ֽڷ�Χ��
//�������ַ�����������ȫ��ĳ��˫�ֽ��ַ���Ӧ��Ψһ��һ�ֱ����ʽ��
//������˵�����Ҷ��� utf-8 ������������徲�ˣ���
//���ְ����ż������չ�����������ĸ���˫�ֽ������ַ�����ֻ���Ǿ�����취�����������������������
//�ж��Ƿ���GB2312
/* Support for Chinese(GB2312) characters */
// #define isgb2312head(c) (0xa1<=(uint8_t)(c) && (uint8_t)(c)<=0xf7)
// #define isgb2312tail(c) (0xa1<=(uint8_t)(c) && (uint8_t)(c)<=0xfe)
static int isGB2312(byte head, byte tail)
{
  int iHead = head & 0xff;
  int iTail = tail & 0xff;
  return ((iHead >= 0xa1 && iHead <= 0xf7 && iTail >= 0xa1 && iTail <= 0xfe) ? TRUE : FALSE);
}
/* Support for Chinese(GBK) characters */
// #define isgbkhead(c) (0x81<=(uint8_t)(c) && (uint8_t)(c)<=0xfe)
// #define isgbktail(c) ((0x40<=(uint8_t)(c) && (uint8_t)(c)<=0x7e)
// || (0x80<=(uint8_t)(c) && (uint8_t)(c)<=0xfe))
static int isGBK(byte head, byte tail)
{
  int iHead = head & 0xff;
  int iTail = tail & 0xff;
  return ((iHead >= 0x81 && iHead <= 0xfe && ((iTail >= 0x40 && iTail <= 0x7e) || (iTail >= 0x80 && iTail <= 0xfe))) ? TRUE : FALSE);
}
/* Support for Chinese(BIG5) characters */
// #define isbig5head(c) (0xa1<=(uint8_t)(c) && (uint8_t)(c)<=0xf9)
// #define isbig5tail(c) ((0x40<=(uint8_t)(c) && (uint8_t)(c)<=0x7e)
// || (0xa1<=(uint8_t)(c) && (uint8_t)(c)<=0xfe))
static int isBIG5(byte head, byte tail)
{
  int iHead = head & 0xff;
  int iTail = tail & 0xff;
  return ((iHead >= 0xa1 && iHead <= 0xf9 &&
    ((iTail >= 0x40 && iTail <= 0x7e) ||
    (iTail >= 0xa1 && iTail <= 0xfe))) ? TRUE : FALSE);
}
static int chr_mbtowc(const void* s, int slen, wchar_t* d, int dlen)
{
  return mb2wc(CE_GB2312, (const uint8_t*)s, slen, d, dlen);
}
static int chr_wctomb(const wchar_t* s, int slen, void* d, int dlen)
{
  return wc2mb(CE_GB2312, s, slen, (uint8_t*)d, dlen);
}
static int str_mbtowc(const str_t szNarrow, str_t* szWide)
{
  str_t dd[1] = {0};
  int size, size2;
  size = chr_mbtowc(szNarrow.s, szNarrow.l, NULL, 0);
  str_setsize(dd, size * 2);
  size2 = chr_mbtowc(szNarrow.s, szNarrow.l, (wchar_t*)dd->s, size);
  str_free(szWide), *szWide = *dd;
  return size2;
}
static int str_wctomb(const str_t szWide, str_t* szNarrow)
{
  str_t dd[1] = {0};
  int size, size2;
  size = chr_wctomb((const wchar_t*)szWide.s, szWide.l / 2, NULL, 0);
  str_setsize(dd, size);
  size2 = chr_wctomb((const wchar_t*)szWide.s, szWide.l / 2, dd->s, size);
  str_free(szNarrow), *szNarrow = *dd;
  return size2;
}
enum {
  XMLFILE_ENCODING_UTF8 = CE_UTF8,
    XMLFILE_ENCODING_UNICODE = CE_UTF16,
    XMLFILE_ENCODING_ASNI = CE_GB2312
};
static int isTextUTF8(const void* str0, int length)
{
  int i;
  uint32_t nBytes = 0;
  const uint8_t* str = (const uint8_t*)str0;
  BOOL bAllAscii = TRUE;
  for (i = 0; i < length; i++) {
    uint8_t chr = *(str + i);
    if ((chr & 0x80) != 0) {
      bAllAscii = FALSE;
    }
    if (nBytes == 0) {
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD) {
          nBytes = 6;
        }
        else if (chr >= 0xF8) {
          nBytes = 5;
        }
        else if (chr >= 0xF0) {
          nBytes = 4;
        }
        else if (chr >= 0xE0) {
          nBytes = 3;
        }
        else if (chr >= 0xC0) {
          nBytes = 2;
        }
        else {
          return FALSE;
        }
        nBytes--;
      }
    }
    else {
      if ((chr & 0xC0) != 0x80) {
        return FALSE;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) {
    return FALSE;
  }
  if (bAllAscii) {
    return FALSE;
  }
  return TRUE;
}
static CodePage GetCodePage(const void* str, int len)
{
  if (isTextUTF8(str, len)) {
    return CE_UTF8;
  }
  return CE_GB2312;
  return (CodePage)XMLFILE_ENCODING_ASNI;
}
static int iconv_s(CodePage scode, const void* s, int sl, CodePage dcode, void* d, int dl)
{
  wchar_t* t;
  int dlen, tlen;
  if (scode < 0) {
    scode = GetCodePage(s, sl);
  }
  tlen = mb2wc(scode, (const uint8_t*)s, sl, 0, 0);
  if (NULL == d) {
    return 3 * tlen;
  }
  t = (wchar_t*)pmalloc(tlen * sizeof(wchar_t) + 10);
  mb2wc(scode, (const uint8_t*)s, sl, t, tlen);
  dlen = wc2mb(dcode, t, tlen, 0, 0);
  dlen = MIN(dlen, dl);
  dlen = wc2mb(dcode, t, tlen, d, dl);
  pfree(t);
  return dlen;
}
static int iconv(CodePage scode, const str_t s, CodePage dcode, str_t* d)
{
  wchar_t* t;
  int dlen, tlen;
  if (scode==CE_UNKNOW) {
    scode = GetCodePage((const uint8_t*)s.s, s.l);
  }
  tlen = mb2wc(scode, (const uint8_t*)s.s, s.l, 0, 0);
  t = (wchar_t*)pmalloc(tlen * sizeof(wchar_t) + 10);
  tlen = mb2wc(scode, (const uint8_t*)s.s, s.l, t, tlen);
  dlen = wc2mb(dcode, t, tlen, 0, 0);
  str_setsize(d, dlen);
  d->l = dlen = wc2mb(dcode, t, tlen, (uint8_t*)d->s, d->l);
  pfree(t);
  return dlen;
}
static int str_toasni(const str_t* s, str_t* d)
{
  const BYTE* pByte = (BYTE*)s->s;
  uint32_t dwSize = s->l;
  str_t t, dd[1] = {0};
  wchar_t* w_str;
  int encoding = GetCodePage(pByte, dwSize);
  if (encoding == XMLFILE_ENCODING_UTF8) {
    uint32_t wide, nWide;
    nWide = mb2wc(CE_UTF8, pByte, dwSize, NULL, 0);
    w_str = (wchar_t*)(pmalloc((nWide + 1) * sizeof(wchar_t)));
    mb2wc(CE_UTF8, pByte, dwSize, w_str, nWide);
    w_str[ nWide ] = L'\0';
    wide = wc2mb(CE_GB2312, w_str, nWide, NULL, 0);
    str_setsize(dd, wide);
    wc2mb(CE_GB2312, w_str, nWide, dd->s, wide);
    pfree(w_str);
  }
  else if (encoding == XMLFILE_ENCODING_UNICODE) {
    if (dwSize >= 2 && ((pByte[ 0 ] == 0xFE && pByte[ 1 ] == 0xFF) || (pByte[ 0 ] == 0xFF && pByte[ 1 ] == 0xFE))) {
      uint32_t nWide;
      BYTE* pByte1 = (BYTE*)memdup1(pByte, dwSize);
      BYTE* pByte2 = pByte1;
      dwSize = dwSize / 2 - 1;
      if (pByte2[ 0 ] == 0xFE && pByte2[ 1 ] == 0xFF) {
        uint32_t nSwap;
        pByte2 += 2;
        for (nSwap = 0 ; nSwap < dwSize ; nSwap ++) {
          register CHAR nTemp = pByte2[(nSwap << 1) + 0 ];
          pByte2[(nSwap << 1) + 0 ] = pByte2[(nSwap << 1) + 1 ];
          pByte2[(nSwap << 1) + 1 ] = nTemp;
        }
      }
      else {
        pByte2 += 2;
      }
      nWide = wc2mb(CE_GB2312, (const wchar_t*) pByte2, dwSize, NULL, 0);
      str_setsize(dd, nWide);
      wc2mb(CE_GB2312, (const wchar_t*) pByte2, dwSize, dd->s, nWide);
      pfree(pByte1);
    }
  }
  else {
    str_setsize(dd, dwSize);
    memcpy(dd->s, pByte, dwSize);
  }
  t = dd[0], dd[0] = d[0], d[0] = t;
  str_free(dd);
  return 0;
}
static const char* hexVals = "0123456789ABCDEF";
// UNSAFE String
static const char* csUnsafeString = "\"<>%\\^[]`+$,@:;/!#?=&";
// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
#define DEC2HEX(dec, Hex) { *(Hex) = hexVals[(dec)&16]; *(Hex+1) = hexVals[((dec)>>4) & 16]; }
#define ISUNSAFE(_CH) (_CH & (1<<7))
static int url2gb(const char* url, int urllen, char* gb)
{
  int i, j = 0;
  if (gb) {
    for (i = 0; i < urllen; ++i) {
      if (ISUNSAFE(url[ i ])) {
        // get Hex Value of the Character
        gb[ j++ ] = '%';
        DEC2HEX(url[ i ], gb + j);
        j += 2;
      }
      else {
        // Safe Character
        gb[ j ] = url[ i ];
        ++j;
      }
    }
    gb[ j ] = '\0';
  }
  else {
    for (i = 0; i < urllen; ++i) {
      if (ISUNSAFE(url[ i ])) {
        j += 3;
      }
      else {
        ++j;
      }
    }
  }
  return j;
}
static BYTE SZHEX[] = {
  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 10, 11, 12, 13, 14, 15, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 10, 11, 12, 13, 14, 15, 0
}; //sizeof(SZHEX)=0x68
#define HEXChar(c) ((c)>0x68 ? 0 : SZHEX[(uint8_t)c])
// UrlDecode URL���뺯��
static int url2any(const void* src, int srclen, char* dst)
{
  char* srcc = (char*) src;
  char* dstc = (char*) dst;
  char* srce = srcc + srclen;
  if (!srcc || !dstc) {
    return 0;
  }
  for (; srcc < srce;) {
    char ch = *srcc++;
    if (ch == '%') {
      *dstc++ = ((HEXChar(srcc[ 0 ]) << 4) | HEXChar(srcc[ 1 ]));
      srcc += 2;
    }
    else if (ch == '+') {
      *dstc++ = ' ';
    }
    else {
      *dstc++ = ch;
    }
  }
  *dstc = 0;
  return dstc - (char*) dst;
}
static int any2url(const void* src, int srclen, void* dst)
{
  int i;
  unsigned char* srcc = (unsigned char*) src;
  unsigned char* dstc = (unsigned char*) dst;
  for (i = 0; i < srclen; ++i) {
    *dstc++ = '%';
    *dstc++ = hexVals[(srcc[ i ] >> 4) ];
    *dstc++ = hexVals[ srcc[ i ] & 0x0f ];
  }
  *dstc++ = '\0';
  return srclen * 3;
}
static int str_url2str(str_t s, str_t* d)
{
  str_setsize(d, s.l);
  d->l = url2any(s.s, s.l, d->s);
  return d->l;
}
static int str_str2url(str_t s, str_t* d)
{
  str_setsize(d, s.l * 3);
  d->l = any2url(s.s, s.l, d->s);
  return d->l;
}
static int gb2uni_code(int code)
{
  char str[8] = {0};
  wchar_t uni[2] = {0};
  //const char* aa1 = "��";
  //const ushort* aa = L"��";
  if ((code >> 8) & 0x80) {
    str[1] = code & 0xff;
    str[0] = (code >> 8) & 0xff;
  }
  else {
    str[0] = code & 0xff;
  }
  //MultiByteToWideChar(CP_ACP, 0, str, 2, uni, 2);
  mb2wc(CE_GB2312, str, 2, uni, 2);
  return uni[0];
}
typedef struct trie_node_t {
  int v1;   //������Ҫ�仯
  int id;
  int cnt;   //�ӽڵ���Ŀ
  int endcnt; // ���������Ŀ
  struct trie_node_t* next[1];
} trie_node_t;
typedef struct trie_t {
  int maxnode; // ����ӽڵ���Ŀ
  uint8_t smap[256];
  int count; // �ܵ�����
  trie_node_t* root;
} trie_t;
#if 0
next�Ǳ�ʾÿ���ж���������������ֻ��Сд��ĸ����26���ɣ�����Ϊ��Сд��ĸ������52�����ټ������֣�����62�ˣ��������������ȷ����
v���Ա�ʾһ���ֵ��������ж�����ͬǰ׺����Ŀ�����������ҪӦ��ѧ�����ɱ仯��
trie_t�Ĳ��ң�����Ҫ�Ĳ�������
(1) ÿ�δӸ���㿪ʼһ��������
(2) ȡ��Ҫ���ҹؼ��ʵĵ�һ����ĸ�������ݸ���ĸѡ���Ӧ��������ת���������������м����� ����(3) ����Ӧ�������ϣ�ȡ��Ҫ���ҹؼ��ʵĵڶ�����ĸ, ����һ��ѡ���Ӧ���������м����� ����
(4) �������̡��� ����
(5) ��ĳ����㴦���ؼ��ʵ�������ĸ�ѱ�ȡ�������ȡ���ڸý���ϵ���Ϣ������ɲ��ҡ�
������������ֵ����Ͳ��ҵ�ģ�棺
�����ֵ�����
#endif
static int trie_freenode(int maxnode, trie_node_t* node)
{
  int i;
  if (node) {
    for (i = 0; i < maxnode; ++i) {
      trie_freenode(maxnode, node->next[i]);
    }
    pfree(node);
  }
  return 0;
}
static int trie_free(trie_t* t)
{
  trie_freenode(t->maxnode, t->root);
  memset(t, 0, sizeof(trie_t));
  return 0;
}
static trie_node_t* trie_newnode(trie_t* t)
{
  int sz = sizeof(trie_node_t) + sizeof(trie_node_t*) * (t->maxnode - 1);
  trie_node_t* q = (trie_node_t*)pmalloc(sz);
  memset(q, 0, sz);
  q->cnt = 0;
  q->id = -1;
  return q;
}
// str ��ĸ���� ����ΪNULL
static int trie_set(trie_t* t, const char* str)
{
  int i;
  trie_free(t);
  if (str) {
    const uint8_t* ustr = (const uint8_t*)str;
    t->maxnode = (int)strlen(str);
    for (i = 0; i < t->maxnode; ++i) {
      t->smap[ustr[i]] = i;
    }
  }
  else {
    t->maxnode = 256;
    for (i = 0; i < t->maxnode; ++i) {
      t->smap[i] = i;
    }
  }
  return 0;
}
// ����id
static trie_node_t* trie_insert(trie_t* t, const void* str, int len)
{
  int i;
  const uint8_t* ustr = (const uint8_t*)str;
  trie_node_t** p = &t->root;
  len = len < 0 ? (int)strlen((const char*)str) : len;
  if (t->maxnode <= 0) {
    trie_set(t, 0);
  }
  for (i = 0; i < len; ++i) {
    int id = t->smap[ustr[i]];
    ASSERT(id < t->maxnode);
    if (*p == NULL) {
      *p = trie_newnode(t);
    }
    (*p)->cnt++;
    p = (*p)->next + id;
  }
  //��Ϊ��β����v�ĳ�-1��ʾ
  if (NULL == (*p)) {
    *p = trie_newnode(t);
  }
  (*p)->cnt++;
  (*p)->endcnt++;
  if ((*p)->id == -1) {
    (*p)->id = t->count++;
  }
  return (*p);
}
// ���ҵĹ���
static trie_node_t* trie_find(const trie_t* t, const void* str, int len)
{
  int i;
  const uint8_t* ustr = (const uint8_t*)str;
  trie_node_t* p = t->root;
  for (i = 0; i < len && p == NULL; ++i) {
    int id = t->smap[ustr[i]];
    p = p->next[id];
    //�ַ��������д��Ǵ˴���ǰ׺
    //if (p->v != -1) { return p; }
  }
  return (p && (p->id != -1)) ? p : NULL;   //�˴����ַ�����ĳ����ǰ׺
}
static int trie_have_instr(const trie_t* t, const void* str, int len) {
  int i, j;
  const uint8_t* ustr = (const uint8_t*)str;
  trie_node_t* ps[256];
  int np=0;
  if (t->root) {
    ps[np++] = t->root;
    for (;np>0;) {
      trie_node_t* p = ps[--np];
      for (i = 0; i < len && p == NULL; ++i) {
        int id = t->smap[ustr[i]];
        p = p->next[id];
      }
      if (p) {
        for (; p && p->id==-1; ) {
          for (j=0; j<t->maxnode && NULL==p->next[j]; ++j) {}
          p = p->next[j];
        }
        return p->id;
      }
    }
  }
  return -1;
}
//-----------------------------------------------------------------------------
// ImText* helpers
//-----------------------------------------------------------------------------
// Convert UTF-8 to 32-bits character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
static int TextCharFrom(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
  unsigned int c = 0;
  const unsigned char* str = (const unsigned char*)in_text;
  if (in_text<in_text_end) {
    if (!(*str & 0x80)) { 
      c = (unsigned int)(*str++);
      *out_char = c;
      return 1;
    }
    else {
      c = (*str++) << 8;
      c |= (*str++);
      *out_char = c;
      return 2;
    }
  }
  return 0;
}
static int TextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
  unsigned int c = (unsigned int) - 1;
  const unsigned char* str = (const unsigned char*)in_text;
  return TextCharFrom(out_char, in_text, in_text_end);
  if (!(*str & 0x80)) {
    c = (unsigned int)(*str++);
    *out_char = c;
    return 1;
  }
  if ((*str & 0xe0) == 0xc0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 2) {
      return 1;
    }
    if (*str < 0xc2) {
      return 2;
    }
    c = (unsigned int)((*str++ & 0x1f) << 6);
    if ((*str & 0xc0) != 0x80) {
      return 2;
    }
    c += (*str++ & 0x3f);
    *out_char = c;
    return 2;
  }
  if ((*str & 0xf0) == 0xe0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 3) {
      return 1;
    }
    if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) {
      return 3;
    }
    if (*str == 0xed && str[1] > 0x9f) {
      return 3; // str[1] < 0x80 is checked below
    }
    c = (unsigned int)((*str++ & 0x0f) << 12);
    if ((*str & 0xc0) != 0x80) {
      return 3;
    }
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80) {
      return 3;
    }
    c += (*str++ & 0x3f);
    *out_char = c;
    return 3;
  }
  if ((*str & 0xf8) == 0xf0) {
    *out_char = 0xFFFD; // will be invalid but not end of string
    if (in_text_end && in_text_end - (const char*)str < 4) {
      return 1;
    }
    if (*str > 0xf4) {
      return 4;
    }
    if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) {
      return 4;
    }
    if (*str == 0xf4 && str[1] > 0x8f) {
      return 4; // str[1] < 0x80 is checked below
    }
    c = (unsigned int)((*str++ & 0x07) << 18);
    if ((*str & 0xc0) != 0x80) {
      return 4;
    }
    c += ((unsigned int)(*str++ & 0x3f) << 12);
    if ((*str & 0xc0) != 0x80) {
      return 4;
    }
    c += (unsigned int)((*str++ & 0x3f) << 6);
    if ((*str & 0xc0) != 0x80) {
      return 4;
    }
    c += (*str++ & 0x3f);
    // utf-8 encodings of values used in surrogate pairs are invalid
    if ((c & 0xFFFFF800) == 0xD800) {
      return 4;
    }
    *out_char = c;
    return 4;
  }
  *out_char = 0;
  return 0;
}
static int TextStrFromUtf8(wchar_t* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
  wchar_t* buf_out = buf;
  wchar_t* buf_end = buf + buf_size;
  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c;
    in_text += TextCharFromUtf8(&c, in_text, in_text_end);
    if (c == 0) {
      break;
    }
    if (c < 0x10000) { // FIXME: Losing characters that don't fit in 2 bytes
      *buf_out++ = (wchar_t)c;
    }
  }
  *buf_out = 0;
  if (in_text_remaining) {
    *in_text_remaining = in_text;
  }
  return (int)(buf_out - buf);
}
static int TextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
  int char_count = 0;
  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c;
    in_text += TextCharFromUtf8(&c, in_text, in_text_end);
    if (c == 0) {
      break;
    }
    if (c < 0x10000) {
      char_count++;
    }
  }
  return char_count;
}
// Based on stb_to_utf8() from github.com/nothings/stb/
static int TextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
  if (c < 0x80) {
    buf[0] = (char)c;
    return 1;
  }
  if (c < 0x800) {
    if (buf_size < 2) {
      return 0;
    }
    buf[0] = (char)(0xc0 + (c >> 6));
    buf[1] = (char)(0x80 + (c & 0x3f));
    return 2;
  }
  if (c >= 0xdc00 && c < 0xe000) {
    return 0;
  }
  if (c >= 0xd800 && c < 0xdc00) {
    if (buf_size < 4) {
      return 0;
    }
    buf[0] = (char)(0xf0 + (c >> 18));
    buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
    buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[3] = (char)(0x80 + ((c) & 0x3f));
    return 4;
  }
  //else if (c < 0x10000)
  {
    if (buf_size < 3) {
      return 0;
    }
    buf[0] = (char)(0xe0 + (c >> 12));
    buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
    buf[2] = (char)(0x80 + ((c) & 0x3f));
    return 3;
  }
}
static int ImTextCountUtf8BytesFromChar(unsigned int c)
{
  if (c < 0x80) {
    return 1;
  }
  if (c < 0x800) {
    return 2;
  }
  if (c >= 0xdc00 && c < 0xe000) {
    return 0;
  }
  if (c >= 0xd800 && c < 0xdc00) {
    return 4;
  }
  return 3;
}
static int ImTextStrToUtf8(char* buf, int buf_size, const wchar_t* in_text, const wchar_t* in_text_end)
{
  char* buf_out = buf;
  const char* buf_end = buf + buf_size;
  while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c = (unsigned int)(*in_text++);
    if (c < 0x80) {
      *buf_out++ = (char)c;
    }
    else {
      buf_out += TextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
    }
  }
  *buf_out = 0;
  return (int)(buf_out - buf);
}
static int ImTextCountUtf8BytesFromStr(const wchar_t* in_text, const wchar_t* in_text_end)
{
  int bytes_count = 0;
  while ((!in_text_end || in_text < in_text_end) && *in_text) {
    unsigned int c = (unsigned int)(*in_text++);
    if (c < 0x80) {
      bytes_count++;
    }
    else {
      bytes_count += ImTextCountUtf8BytesFromChar(c);
    }
  }
  return bytes_count;
}
//utf8�ַ�����1-6�����Ը���ÿ���ַ���һ���ֽ��ж������ַ�����
//0xxxxxxx
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//
//������ұ�����256�����е���ֵ��ʾ�Դ�Ϊ��ʼ�ֽڵ�utf8�ַ�����
//����str�ַ���Ŀ
static const unsigned char utf8_look_for_table[256] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};
#define UTFLEN(x) utf8_look_for_table[(x)]
static int utf8_len(const char* str)
{
  int i = 0;
  unsigned char* ustr = (uint8_t*)str;
  unsigned char ch;
  for (; (ch = ustr[i])!=0; i += UTFLEN(ch));
  return i;
}
static int utf8_cnt(const char* str)
{
  int clen = (int)strlen(str);
  int len = 0;
  const char* ptr = str;
  for (; *ptr != 0 && len < clen; len++, ptr += UTFLEN((unsigned char) * ptr));
  return len;
}
//get�Ӵ�
static char* utf8_substring(const char* str, int start, int end)
{
  int i, retLen, len = utf8_cnt(str);
  char* retStr = NULL;
  const char* sptr;
  const char* eptr;
  if (start >= len) {
    return NULL;
  }
  if (end > len) {
    end = len;
  }
  sptr = str;
  for (i = 0; i < start; ++i, sptr += UTFLEN((unsigned char) * sptr));
  eptr = sptr;
  for (i = start; i < end; ++i, eptr += UTFLEN((unsigned char) * eptr));
  retLen = eptr - sptr;
  retStr = (char*)pmalloc(retLen + 1);
  memcpy(retStr, sptr, retLen);
  retStr[retLen] = 0;
  return retStr;
}
static bool ImCharIsSpace(int c)
{
  return c == ' ' || c == '\t' || c == 0x3000;
}
static int test_utf8()
{
  const char* str = "�ҵ�a���Թ��߰�˹��aaab123��˹�ٸ���å�˿�˹��";
  char* sub;
  const char *ptr;
  printf("%s\n", str);
  for (ptr = str; *ptr != 0;) {
    unsigned char c = (unsigned char) * ptr;
    printf("str[%d] is a word character with %d bytes\n", c, UTFLEN(c));
    ptr += UTFLEN(c);
  }
  printf("%d\n", utf8_cnt(str));
  sub = utf8_substring(str, 2, 100);
  if (sub) {
    printf("%s\n", sub);
    pfree(sub);
    sub = NULL;
  }
  return 0;
}
static int vcstr_len(const char*const* strs) {
  int i=0;
  if (strs) {
    for (; *strs;++i);
  }
  return i;
}
static int vcstr_setsize(char*** vstrs, int newn) {
  char** p = *vstrs;
  int n = vcstr_len(p);
  if (newn<n) {
    int i;
    for (i = newn; i < n; ++i) {
      if ((*vstrs)[i]) {
        FREE((*vstrs)[i]);
        (*vstrs)[i] = NULL;
      }
    }
  } else if (newn>n) {
    ++newn;
    MYREALLOC((*vstrs), newn);
    memset((*vstrs)+n, 0, (newn-n)*sizeof(char*));
  } 
  return 0;
}
static int vcstr_free(char** vstrs) {
  if (vstrs) {
    int i=0;
    for (; vstrs[i];++i) {
      FREE(vstrs[i]);
    }
    FREE(vstrs[i]);
  }
  return 0;
}

#endif // _STR_H_

