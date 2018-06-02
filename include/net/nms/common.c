#include "common.h"

static int32_t fd_led = 0;
char* gsDebugType[] = {
  "EMERG",        // 紧急
  "ALERT",        // 警报
  "CRIT",
  "ERROR",        // 错误
  "WARNING",      // 警告
  "DNOTICE",
  "INFO",         // 信息
  "DEBUG",        // 调试
};

int32_t SprintfHex(char* pInHex, int32_t HexLen, char* pOutStr, int32_t buflen)
{
  int32_t i, offset;

  for (i = 0, offset = 0; i < HexLen; i++) {
    if (buflen < (offset + 4)) {
      break;
    }

    if (!i || i % 16) {
      sprintf(pOutStr + offset, " %02X", (unsigned char)pInHex[i]);
    }
    else {
      sprintf(pOutStr + offset, "\n%02X", (unsigned char)pInHex[i]);
    }

    offset += 3;
  }

  pOutStr[offset] = 0;

  return 0;
}

/**
  * @brief
  * @param
  * @retval None
  */
static const uint8_t u8CRC8Table[256] = {
  0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,//0x07
  0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,//0x0F
  0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
  0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,//0x1F
  0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,
  0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
  0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,
  0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
  0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
  0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
  0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,
  0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
  0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,
  0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
  0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
  0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
  0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,
  0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
  0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,
  0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
  0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
  0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
  0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,
  0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
  0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,
  0x76, 0x71, 0x78, 0x7f, 0x6A, 0x6d, 0x64, 0x63,
  0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
  0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
  0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,
  0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8D, 0x84, 0x83,
  0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,
  0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3,
};

/**
  * @brief  Get Buff CRC8
  * @param  *Buff Input the needs caulate
  *         BuffStart Can use buff Index
  *         DataLength
  * @retval Crc8 value
  */
uint8_t Crc8Calc(char* buf, uint32_t len)
{
  uint8_t i;
  uint8_t  crc;

  len &= 0xFF;
  crc = 0;

  for (i = 0; i < len; i++) {
    crc ^= buf[i];
    crc  = u8CRC8Table[crc];
  }

  return crc;
}

//crc16实现函数
uint16_t Crc16Calc(char* buf, uint32_t len)
{
  uint32_t i, j;
  uint32_t c;
  uint32_t crc = 0xFFFF;

  for (i = 0; i < len; i++) {
    c = *(buf + i) & 0x00FF;
    crc ^= c;

    for (j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else {
        crc >>= 1;
      }
    }
  }

  crc = (crc >> 8) + (crc << 8);
  return(crc);
}

//crc32初始化表
static void init_table(uint32_t* crcbuf)
{
  uint32_t   i, j;
  uint32_t   crc;

  for (i = 0; i < 256; i++) {
    crc = i;

    for (j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320;
      }
      else {
        crc = crc >> 1;
      }
    }

    crcbuf[i] = crc;
  }
}
//crc32实现函数
uint32_t Crc32Calc(char* buf, uint32_t len)
{
  static uint32_t CRC32[256];
  static char init = 0;
  uint32_t ret = 0xFFFFFFFF;
  uint32_t i;

  if (!init) {
    init_table(CRC32);
    init = 1;
  }

  for (i = 0; i < len; i++) {
    ret = CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
  }

  ret = ~ret;

  return ret;
}

int32_t BspDevOpen()
{
  if (fd_led > 0) {
    return -1;
  }

  fd_led = open("/dev/mrd_led", O_RDWR);

  if (fd_led < 0) {
    MDEBUG(DERR, "/dev/mrd_led:%s\n", strerror(errno));
    fd_led = 0;
    return -1;
  }

  return 0;
}

int32_t BspDevClose()
{
  if (fd_led) {
    close(fd_led);
    fd_led = 0;
  }

  return 0;
}

int32_t LedIoctrl(int32_t led, int32_t value)
{
  if (fd_led <= 0) {
    return -1;
  }

  return ioctl(fd_led, led, value);
}

void ThreadExitHandle(int32_t sig)
{
  (void)sig;
  pthread_exit(0);
}
