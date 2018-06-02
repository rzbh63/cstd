#include "StdAfx.h"
#include "Base64Convert.h"


CBase64Convert::CBase64Convert(void)
{
}


CBase64Convert::~CBase64Convert(void)
{
}

void CBase64Convert::_enBase64Help(unsigned char chasc[3], unsigned char chuue[4])
{
  int i, k = 2;
  unsigned char t = 0;

  for (i = 0; i < 3; i++) {
    *(chuue + i) = *(chasc + i) >> k;
    *(chuue + i) |= t;
    t = *(chasc + i) << (8 - k);
    t >>= 2;
    k += 2;
  }

  *(chuue + 3) = *(chasc + 2) & 63;

  for (i = 0; i < 4; i++) {
    if ((*(chuue + i) <= 128) && (*(chuue + i) <= 25)) {
      *(chuue + i) += 65; // 'A'-'Z'
    }
    else if ((*(chuue + i) >= 26) && (*(chuue + i) <= 51)) {
      *(chuue + i) += 71; // 'a'-'z'
    }
    else if ((*(chuue + i) >= 52) && (*(chuue + i) <= 61)) {
      *(chuue + i) -= 4; // 0-9
    }
    else if (*(chuue + i) == 62) {
      *(chuue + i) = 43; // +
    }
    else if (*(chuue + i) == 63) {
      *(chuue + i) = 47; // /
    }
  }
}

void CBase64Convert::_deBase64Help(unsigned char chuue[4], unsigned char chasc[3])
{
  int i, k = 2;
  unsigned char t = 0;

  for (i = 0; i < 4; i++) {
    if ((*(chuue + i) >= 65) && (*(chuue + i) <= 90)) {
      *(chuue + i) -= 65;  // 'A'-'Z' -> 0-25
    }
    else if ((*(chuue + i) >= 97) && (*(chuue + i) <= 122)) {
      *(chuue + i) -= 71;  // 'a'-'z' -> 26-51
    }
    else if ((*(chuue + i) >= 48) && (*(chuue + i) <= 57)) {
      *(chuue + i) += 4;  // '0'-'9' -> 52-61
    }
    else if (*(chuue + i) == 43) {
      *(chuue + i) = 62;  // + -> 62
    }
    else if (*(chuue + i) == 47) {
      *(chuue + i) = 63;  // / -> 63
    }
    else if (*(chuue + i) == 61) {
      *(chuue + i) = 0;  // = -> 0  Note: 'A'和'='都对应了0
    }
  }

  for (i = 0; i < 3; i++) {
    *(chasc + i) = *(chuue + i) << k;
    k += 2;
    t = *(chuue + i + 1) >> (8 - k);
    *(chasc + i) |= t;
  }
}

string CBase64Convert::enBase64(const char* inbuf, size_t inbufLen)
{
  string outStr;
  unsigned char in[8];
  unsigned char out[8];
  out[4] = 0;
  size_t blocks = inbufLen / 3;

  for (size_t i = 0; i < blocks; i++) {
    in[0] = inbuf[i * 3];
    in[1] = inbuf[i * 3 + 1];
    in[2] = inbuf[i * 3 + 2];
    _enBase64Help(in, out);
    outStr += out[0];
    outStr += out[1];
    outStr += out[2];
    outStr += out[3];
  }

  if (inbufLen % 3 == 1) {
    in[0] = inbuf[inbufLen - 1];
    in[1] = 0;
    in[2] = 0;
    _enBase64Help(in, out);
    outStr += out[0];
    outStr += out[1];
    outStr += '=';
    outStr += '=';
  }
  else if (inbufLen % 3 == 2) {
    in[0] = inbuf[inbufLen - 2];
    in[1] = inbuf[inbufLen - 1];
    in[2] = 0;
    _enBase64Help(in, out);
    outStr += out[0];
    outStr += out[1];
    outStr += out[2];
    outStr += '=';
  }

  return string(outStr);
}

string CBase64Convert::enBase64(const string& inbuf)
{
  return CBase64Convert::enBase64(inbuf.c_str(), inbuf.size());
}

int CBase64Convert::deBase64(const string& src, char* outbuf)
{

  // Break when the incoming base64 coding is wrong
  if ((src.size() % 4) != 0) {
    return 0;
  }

  unsigned char in[4];
  unsigned char out[3];

  size_t blocks = src.size() / 4;

  for (size_t i = 0; i < blocks; i++) {
    in[0] = src[i * 4];
    in[1] = src[i * 4 + 1];
    in[2] = src[i * 4 + 2];
    in[3] = src[i * 4 + 3];
    _deBase64Help(in, out);
    outbuf[i * 3]   = out[0];
    outbuf[i * 3 + 1] = out[1];
    outbuf[i * 3 + 2] = out[2];
  }

  int length = src.size() / 4 * 3;

  if (src[src.size() - 1] == '=') {
    length--;

    if (src[src.size() - 2] == '=') {
      length--;
    }
  }

  return length;
}

int CBase64Convert::deBase64(const char* pSrc , int len , char* outbuf)
{
  // Break when the incoming base64 coding is wrong
  if (pSrc == NULL || len == 0 || (len % 4) != 0) {
    return 0;
  }

  DEBUG_INFO1(_T("deBase64 len = %d") , len);
  unsigned char inData[4];
  size_t blocks = len / 4;

  for (size_t i = 0; i < blocks; i++) {
    memcpy(inData , pSrc + i * 4 , 4);
    _deBase64Help(inData , (unsigned char*)outbuf + i * 3);
  }

  int length = len / 4 * 3;

  if (pSrc[len - 1] == '=') {
    length--;

    if (pSrc[len - 2] == '=') {
      length--;
    }
  }

  return length;
}

string CBase64Convert::deBase64(const string& src)
{
  char* buf = new char[src.length() * 2 + 1];
  int len = deBase64(src, buf);
  buf[len] = '\0';
  string result = string(buf, len);
  delete [] buf;
  return result;
}
