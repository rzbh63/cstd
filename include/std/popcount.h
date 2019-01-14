#ifndef _POPCNT_INL_
#define _POPCNT_INL_
// popcount �㷨
// population count����� popcount ��� sideways sum. �Ǽ���һ�����Ķ����Ʊ�ʾ�ж���λ��1��
// ��һЩ�����º����ã��������0-1ϡ�����(sparse matrix)��λ����(bit array)�з���Ԫ�ظ�����
// ������������ַ����ĺ�������(Hamming distance)��Ȼ�� Intel ֱ��2008��11�� Nehalem �ܹ��Ĵ�����Core i7 �������� SSE4.2 ָ���
// ���о��� CRC32 �� POPCNT ָ�POPCNT���Դ���16,32,64λ������
//
// ����popcount�кܶ��㷨���Ѽ����Ĵ��������¼��֣�
//
//     popcnt_iterated
//     popcnt_sparse
//     popcnt_dense
//     popcnt_lookup
//     popcnt_parallel
//     popcnt_nifty
//     popcnt_hacker
//     popcnt_hakmem
//     popcnt_assembly
//popcnt_iterated ͨ����������1�ۼӣ�����λ��ѭ�����ٴΣ�������Ҳ���������
int popcnt_iterated(unsigned int n)
{
  int count = 0;
  for (; n; n >>= 1) {
    count += n & 1u;
  }
  return count;
}
// popcnt_sparse �Ƕ� popcnt_iterated �ĸĽ���ÿ�ε������ǽ����ұߵķ���λ���㡣���������á�
// ����һ�£�һ�������λΪ1���������ô˷����Ļ�����һ�ε������� popcnt_iterated ���ǻᡰ�ԹԵġ�����64�Σ���64λ����������
int popcnt_sparse(unsigned int n)
{
  int count = 0;
  while (n) {
    ++count;
    n &= (n - 1);
  }
  return count;
}
// popcnt_dense ֻ�� popcnt_sparse ��һ�����֡�
// �������������������֪����n��bitλ�кܶ�ܶ��1(ȡ�� dense ������)��������ȡ���� popcount�������intλ��һ���͵õ������������������������1��0λ�����(not too sparse, not too dense)����ʧȥ���ƣ��� popcnt_sparse ����һ����������Ե���ʼ��һ��ȡ��������
// n&(n-1) ʽ�ӻ���һ����;�������ж�һ������ x �Ƿ��� 2 �� n ���ݡ������� x!=0 && (x&(x-1))==0 ��
int popcnt_dense(unsigned int n)
{
  int count = CHAR_BIT * sizeof(unsigned int);
  n ^= (unsigned int) - 1;
  while (n) {
    --count;
    n &= (n - 1);
  }
  return count;
}
// popcnt_lookup ͨ��������ÿռ任ʱ�䣬�������ÿ졣
// ����û���������� uint ����ģ���̫��ռ䣩������һ�� uchar �����Ĵ���ֵȻ����ӡ�����Գ��� ushort ��Ȼ��������(���������Ĵ�)�����᲻����죬�Ͼ�ֱ����ȡ���Ӧ�ñ��κμ��㶼Ҫ���ÿ죬����˵��
// ����Ĵ����г������ַ������� TABLE��
// 1. ���� f(n)=f(n/2)+n%2, f(0)=0��ֱ�۶��ҷ�����ֲ����Ҫ��charһ��Ϊ8λ����(CHAR_BIT==8)
// 2. ���ú���չ�ݹ�չ������Ư����д��������ٶ���charΪ8λ����Щ�ɵĻ���һ���ֽ�7λ������Ҫ��һЩ�޸ġ�
// CHAR_BIT �� UCHAR_MAX ��Ϊ�궨���� limit.h �ļ��У�������ֲ��
int popcnt_lookup(unsigned int n)
{
#define TBL_LEN (1<<CHAR_BIT)
#define BIT2(n)      n,       n+1,       n+1,       n+2
#define BIT4(n) BIT2(n), BIT2(n+1), BIT2(n+1), BIT2(n+2)
#define BIT6(n) BIT4(n), BIT4(n+1), BIT4(n+1), BIT4(n+2)
#define BIT8(n) BIT6(n), BIT6(n+1), BIT6(n+1), BIT6(n+2)
  static const unsigned char TABLE[TBL_LEN] = {
#if (CHAR_BIT==8)
    BIT8(0)
#elif (CHAR_BIT==7)
    BIT6(0), BIT6(1)
#else
# error "BITX to be added here."
#endif
  };
  return TABLE[n                 & UCHAR_MAX] +
         TABLE[(n >> CHAR_BIT)     & UCHAR_MAX] +
         TABLE[(n >> (CHAR_BIT * 2)) & UCHAR_MAX] +
         TABLE[(n >> (CHAR_BIT * 3)) & UCHAR_MAX];
#undef TBL_LEN
#undef BIT2
#undef BIT4
#undef BIT6
#undef BIT8
}

CC_INLINE int popcnt_lookup2(unsigned int u)
{
  static const uchar poptable[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
  };
  return poptable[u & 0xFF] + poptable[(u >> 8) & 0xFF] + poptable[(u >> 16) & 0xFF] + poptable[(u >> 24) & 0xFF];
}
int popcnt_lookup3(unsigned int u)
{
  static const uchar poptable[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
  };
  return poptable [u & 0xFF] + poptable [(u >> 8) & 0xFF] + poptable [(u >> 16) & 0xFF] + poptable [(u >> 24) & 0xFF];
}
// popcnt_parallel ���ò��м��㡣����ͼƬչʾ��
//
// ����һ������õ�ÿkλһ��� bitcount����2*kλͦ����
// num=a*2k+b, aΪ��kλ��bΪ��kλ�����ǽ��Ϊ a+b��
// �ù���num�ı��ʽ����mask�ֱ����θߵ�λȡ��a��b�ټӡ�
// ��ˣ�32λ������5�ε�����64λ������6�ε�����
//
// ���ڵ�n�ε���MASK��ֵ
//     [0]. 01010101 01010101 01010101 01010101 = 55555555 = FFFFFFFF/(0x2+1)
//     [1]. 00110011 00110011 00110011 00110011 = 33333333 = FFFFFFFF/(0x4+1)
//     [2]. 00001111 00001111 00001111 00001111 = 0F0F0F0F = FFFFFFFF/(0x10+1)
//     [3]. 00000000 11111111 00000000 11111111 = 00FF00FF = FFFFFFFF/(0x100+1)
//     [4]. 00000000 00000000 11111111 11111111 = 0000FFFF = FFFFFFFF/(0x10000+1)
// ���������ߵ�һ���Ƕ������⣬�ұߵĶ���ʮ����������
// ��n�ε����Ĺ����� 2n ��0������ 2n ��1��Ȼ��ѭ����2^2^n��Ӧ��2nλ��1��Ӧ��2nλ��
// �����ұߵĳ����� 2^2^n+1���˴�^Ϊ�˷����ҽ�ϡ���ʵ���Ƿ�����(Fermat number)��
//
#define BITPOW2(c)    (1u<<(c))
#define BITMASK(c)    (((unsigned int)(-1))/(BITPOW2(BITPOW2(c))+1u))
#define BITCOUNT(x,c) ((x)&BITMASK(c)) + (((x)>>(BITPOW2(c)))&BITMASK(c))
int popcnt_parallel(unsigned int n)
{
  n = BITCOUNT(n, 0);
  n = BITCOUNT(n, 1);
  n = BITCOUNT(n, 2);
  n = BITCOUNT(n, 3);
  n = BITCOUNT(n, 4);
  /*  n=BITCOUNT(n,5);  for 64-bit integers */
  return n ;
}
#undef BITCOUNT
#undef BITMASK
#undef BITPOW2
// popcnt_nifty �� popcnt_parallel ��ʼ��һ����ֻ��д����ͬ����3�ε��������ÿ8λһ��ĺͣ���ͬ������ĩβ��ģ255һ�½��������㣬��֮Ư��(nifty)���������漰��һЩ��ѧ֪ʶ��
// K������B (BnBn-1...B1B0)��B=Bn*Kn-1+Bn-1*Kn-2+...+B1*K+B0
// Ki��1(mod K-1),�������ö���ʽKi=((K-1)+1)iչ����������ѧ���ɷ�֤���˽��ۡ�
//   B (mod K-1)
// =Bn*Kn-1+Bn-1*Kn-2+...+B1*K+B0 (mod K-1)
// =Bn+Bn-1+...+B1+B0 (mod K-1)
// ���ۣ�һ��K������ģ(K-1)�Ľ�������ڴ�K�������ĸ�λ�����ģK-1
// ���ǣ�popcnt(B)��B (mod K-1)
// ��Ȼ�������ܿ϶� Bn+Bn-1+...+B1+B0<K-1 �Ļ������Լ�ǿ���ۣ�popcnt(B)=B%(K-1)
// ����ÿ8λһ�飬�൱��28=256���ƣ���������255�������Ϊ��ʹ������ĵ�ʽ���㣬�������ٵ�3�ε�����
// 2�ε������� 24=16 ���ƣ�������һ��32λ���Σ�popcount ���ֵΪ32��32>16-1��32<256-1
// ����һ��64λ���Σ�popcount ���ܵ����ȡֵ��64������Ҫѡȡ������511��
#define MASK_01010101 (((unsigned int)(-1))/3)
#define MASK_00110011 (((unsigned int)(-1))/5)
#define MASK_00001111 (((unsigned int)(-1))/17)
int popcnt_nifty(unsigned int n)
{
  n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101) ;
  n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011) ;
  n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111) ;
  return n % 255 ;
}
// ȡ�� popcnt_hacker ����Ϊ������ Hacker's Delight �Ȿ�顣�����е��Ӱ����أ�ǿ���Ƽ����ġ�
// ���е�5��ר�Ž� Counting Bits��������Ƕδ������Ե�һ�ڵ� Figure 5-2 Counting 1-bits in a word.
// �ô���Ҳ�� Android ��Դ��Ŀ���ã�·�� <AOSP>libcore/luni/src/main/java/java/lang/
// FFmpeg ��Ҳ����δ��룬·�� libavutil/common.h ���� grep һ�¹ؼ��� popcount��
// ���е� Integer �� Long ��ķ��� bitCount���ֱ��Ӧ32λ��64λ�� popcount
// ��һ�еļ����������������ƶ�λ�� 2a+b ����һλ�� a������ (2a+b)-a=a+b
// ����ԭ��ͬ����� popcnt_nifty һ�������ﲻ��������
int popcnt_hacker1(unsigned int n)
{
  n -= (n >> 1) & 0x55555555;
  n  = (n & 0x33333333) + ((n >> 2) & 0x33333333);
  n  = ((n >> 4) + n) & 0x0F0F0F0F;
  n += n >> 8;
  n += n >> 16;
  return n & 0x0000003F;
}
int popcnt_hacker(unsigned int u)
{
  u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
  u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
  u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
  u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
  u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
  return u;
}
//  HAKMEM Popcount
//
//   Consider a 3 bit number as being
//         4a+2b+c
//   if we shift it right 1 bit, we have
//         2a+b
//   subtracting this from the original gives
//         2a+b+c
//   if we shift the original 2 bits right we get
//         a
//   and so with another subtraction we have
//         a+b+c
//   which is the number of bits in the original number.
//
//   Suitable masking  allows the sums of  the octal digits  in a 32 bit  number to
//   appear in  each octal digit.  This  isn't much help  unless we can get  all of
//   them summed together.   This can be done by modulo  arithmetic (sum the digits
//   in a number by  molulo the base of the number minus  one) the old "casting out
//   nines" trick  they taught  in school before  calculators were  invented.  Now,
//   using mod 7 wont help us, because our number will very likely have more than 7
//   bits set.   So add  the octal digits  together to  get base64 digits,  and use
//   modulo 63.   (Those of you  with 64  bit machines need  to add 3  octal digits
//   together to get base512 digits, and use mod 511.)
//
//   This is HACKMEM 169, as used in X11 sources.
//   Source: MIT AI Lab memo, late 1970's.
// ȡ�� popcnt_hakmem ����Ϊ������ HAKMEM (Hacks Memo)������ MIT AI Lab�������кܶ������ѧ�����ö���Ư�����㷨������Զ��ζһ������㷨��^_^
// �����õ���һ�����ɣ�popcnt(n) = n - (n>>1) - (n>>2) - ... - (n>>31) = a31+a30+...+a0
// ֤�����ѣ���ʾ��2k-2k-1-2k-2-...-21-20=1������������k������
// ������ triple һ�� popcnt(abc)=popcnt(4a+2b+c)=(4a+2b+c)-(2a+b)-a=a+b+c�������� double һ�£����ģ63��
// ������� popcnt_nifty ������֪����ʵ popcount(unsigned int)<=32��������� unsigned int��32λ���Ρ���63���
// ��ģ�͹��ˣ���Ϊ63������2n-1����С��������n=6����ʾ������Ҫ���ÿ6λһ��Ľ����6=3x2������������� triple
// �� double ��ԭ�򡣶� popcnt_nifty ���� double (x2)��
int popcnt_hakmem(unsigned int n)
{
  unsigned int tmp;
  tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
  return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}
// ���������˻��ָ�� POPCNT���������������style��Intel �� AT&T���﷨һ����ֻ��д����ͬ��֮��������Լ� Google��
// Visual C++ �õ��� Intel style����UNIX ʹ�� AT&T style����ѧѧ����ǰ��һ�֡����˾��� Intel �ļ��׶��������һ��ָ������֤����C/C++�����ķ���ֵ���Ǵ��� EAX �Ĵ����ġ������ܼ�̣����Ա����õ�ʱ�����ֱ�� inline ������������
//
// ���ڻ��ָ����÷�������� Intel �ٷ��ֲᣬ��ַ�������Ļ�����һ��֧�֣�����ͨ�������г�������������ȼ��һ�¡� Before an application attempts to use the POPCNT instruction, it must check that the processor supports SSE4.2 (if CPUID.01H:ECX.SSE4_2[bit 20] = 1) and POPCNT (if CPUID.01H:ECX.POPCNT[bit 23] = 1).
//
// asm ������ C++ �Ĺؼ��ֵģ�MSDN��˵ The __asm keyword replaces C++ asm syntax. asm is reserved for compatibility with other C++ implementations, but not implemented. Use__asm. ����� GCC �������Ļ�������ʹ��asm���������ʱ��Ҫ�� gcc popcnt.c -o bitcnt -std=c99 -fasm��-fasm���ñ�������"asm", "inline" or "typeof"Ϊ�ؼ��֡�
// GNU������Ҳ�����˺ܶຯ����Ҳ���� int __builtin_popcount (unsigned int x);�������Ļ����ܹ�֧�ֵĻ�����ֱ�����һ��CPUָ�
// int popcnt_assembly(unsigned int n)
// {
// #ifdef _MSC_VER /* Intel style assembly */
//     __asm popcnt eax,n;
// #elif __GNUC__ /* AT&T style assembly */
//     __asm__("popcnt %0,%%eax"::"r"(n));
// }
// �ܽ᣺
// �Ա� popcount �ĸ����㷨����Ч���������ò��м��㣬ȥ��ѭ����ʹ�ü�����ģ���㡣
// ͨ����1��ѭ���㷨(parse/dense)��֪����ֻ������λΪ1����0���Ļ�����ʵЧ��Ҳ������
// �����Ч��ͦ����ģ����û��Ӳ��ָ���֧�֣�ѡ������ǿ��Եġ�
// Hacker's Delight �е��㷨���ڿ�Դ��Ŀ�й�Ϊ���á�
// �㷨��Ҫ����ھ��������ѧ֪ʶ��
//
// �ο���
//
//     The Aggregate Magic Algorithms by enry Gordon Dietz at University of Kentucky
//     Puzzle: Fast Bit Counting Delightful Puzzles
//     HAKMEM (bit counting is memo number 169), MIT AI Lab, Artificial Intelligence Memo No. 239, February 29, 1972.
//     Bit Twiddling Hacks by Sean Anderson at Stanford University.
//     Bitwise Tricks and Techniques by Donald E. Knuth (The Art of Computer Programming, Part IV).
//     Hacker's Delight Hacker's_Delight_2nd.pdf
//


static int hamming_distance(const void* a, const void* b, int n)
{
  int i, n2 = n / 4, dis = 0;
  const unsigned int* ua = (const unsigned int*)a;
  const unsigned int* ub = (const unsigned int*)b;
  const unsigned char* ca = (const unsigned char*)(ua + n2);
  const unsigned char* cb = (const unsigned char*)(ub + n2);
  for (i = 0; i < n2; ++i) {
    dis += popcnt_lookup2(ua[i] ^ ub[i]);
  }
  for (i = n2 * 4; i < n; ++i) {
    dis += popcnt_lookup2(ca[i] ^ cb[i]);
  }
  return dis;
}


#endif // _POPCNT_INL_

