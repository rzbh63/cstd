简单的TEA加密算法
TEA(Tiny Encrypt Algorithm)是一种极为简单的对称加密算法，运用比较普遍，它不是通过算法的复杂性来保证的，而是依赖加密的轮数来保证。这种算法采用一个128位的密钥来加密64位的数据明文，能产生一个64位的密文。具有较好的抗差分性能。
加密算法如下：
#define ROUNDS 32 /*加密轮数*/
#define DELTA 0x9e3779b9 /* sqr(5)-1 * 2^31 */
#include "ctypes.h"
/**********************************************************
 Input values: k[4] 128位密钥
 v[2] 加密时64位明文，解密时64位密文
 Output values: v[2] 加密时64位密文，解密时64位明文
 **********************************************************/
void tea(word32* k, word32* v, long N) //如果N为负值就是解密过程，相应的v就为密文，密钥k一共就有k[0]、k[1]、k[2]、k[3]四个元素
{
  word32 y = v[0], z = v[1]; //y为明文或密文高32位，z为明文或密文低32位
  word32 limit, sum = 0; //sum为部分和
  if (N > 0) { /* 加密过程*/
    limit = DELTA * N;
    while (sum != limit) { //注意：高位和地位交叉运算，利用sum操作的低两位进行密钥的部分选择
      y += ((z << 4) ^ (z >> 5)) + (z ^ sum) + k[sum & 3];
      sum += DELTA;
      z += ((y << 4) ^ (y >> 5)) + (y ^ sum) + k[(sum >> 11) & 3];
    }
  }
  else { /* 解密过程 ，就是加密算法简单的反向运算*/
    sum = DELTA * (-N);
    while (sum) {
      z -= ((y << 4) ^ (y >> 5)) + (y ^ sum) + k[(sum >> 11) & 3];
      sum -= DELTA;
      y -= ((z << 4) ^ (z >> 5)) + (z ^ sum) + k[sum & 3];
    }

