// WMF图像读写模块实现文件
// 读取功能：  可读取Windows标准元文件、Aldus可放置元文件。
// 保存功能：  不支持保存功能。

/* 文件结构定义 */
#define ALDUSKEY        0x9AC6CDD7
#define ALDUSMETAHEADERSIZE   22

typedef struct {
  unsigned int  key;        /* 必需等于ALDUSKEY */
  unsigned short  hmf;
  short     left;
  short     top;
  short     right;
  short     bottom;
  unsigned short  inch;
  unsigned int  reserved;
  unsigned short  checksum;
} ALDUSMETAHEADER;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断指定图象是否是WMF */
static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  is_aldus, filesize, wmf_offset = 0;
  METAHEADER    meta_hdr;

  __try {
    __try {
      /* 首先验证文件长度，不应该小于METAHEADER结构，外加一个6字节的尾记录长度 */
      if (isio_seek(pfile, 0, SEEK_END) == -1
          || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < (sizeof(METAHEADER) + 6)) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* 读取文件的头四字节，判断是不是placeable元文件 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER aldus_hdr;
        WORD* pw;
        WORD checksum = 0;

        /*  如果是，再次验证文件长度 */
        if (filesize < (ALDUSMETAHEADERSIZE + sizeof(METAHEADER) + 6)) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        /* 读取placeable元文件头，并作代码和检查。 */
        if (isio_seek(pfile, 0, SEEK_SET) == -1
            || isio_read(&aldus_hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        for (pw = (WORD*)&aldus_hdr; pw < (WORD*)&aldus_hdr.checksum; pw++) {
          checksum ^= *pw;
        }

        /* 如失败，表示文件已损坏 */
        if (checksum != aldus_hdr.checksum) {
          b_status = ER_BADIMAGE;
          __leave;
        }

        wmf_offset = ALDUSMETAHEADERSIZE;
      }

      /* 读取windows标准元文件头，其中记录有元文件大小（不包括placeable元文件
         头在内，以WORD为单位），元文件长度不应该小于此 */
      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(&meta_hdr, sizeof(METAHEADER), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < (meta_hdr.mtSize * 2 - wmf_offset)) {
        b_status = ER_BADIMAGE;
        __leave;
      }
    }
    __finally {

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 获取图象信息 */
static int _get_info(ISFILE* pfile, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  is_aldus;
  HDC       hdc = NULL;

  assert(pfile);
  assert(pinfo_str);

  __try {
    __try {
      hdc = GetDC(GetDesktopWindow());

      if (hdc == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* 设置图像宽、高度，标准wmf文件未记录相关信息，默认为屏幕宽、高度 */
      pinfo_str->width  = GetDeviceCaps(hdc, HORZRES);
      pinfo_str->height = GetDeviceCaps(hdc, VERTRES);

      /* 设置图像为倒向放置 */
      pinfo_str->order = 1;

      /* 设置位深度,由于wmf文件并未记录相关信息，直接设定为24bit */
      pinfo_str->bitcount = 24;

      /* 设置各颜色分量掩码 */
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->b_mask = 0xff;
      pinfo_str->a_mask = 0x0;

      /* 如果是可放置元文件，从文件头中获得宽度、高度 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER hdr;
        float fx, fy;

        if (isio_seek(pfile, 0, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        if (isio_read(&hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
        }

        /* 首先计算图像的单位尺寸 */
        fx = (float)(hdr.right - hdr.left);
        fy = (float)(hdr.bottom - hdr.top);

        /* hdr.inch表示一英寸相当于多少单位，由此计算出以英寸表示的图像尺寸，
           再乘以屏幕的DPI值，从而得到图像以象素为单位的尺寸*/
        fx = (fx / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSX) + 0.5f;
        fy = (fy / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSY) + 0.5f;
        pinfo_str->width = (unsigned long)fx;
        pinfo_str->height = (unsigned long)fy;
      }

      /* 设置图像类型、存储格式和数据压缩方式 */
      pinfo_str->imgtype     = IMT_VECTORSTATIC;
      pinfo_str->imgformat   = is_aldus == ALDUSKEY ? IMF_APM : IMF_WMF;
      pinfo_str->compression = ICS_GDIRECORD;
    }
    __finally {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;

}


/* 读WMF数据，并创建HMETAFILE句柄 */
static int _load_metafile(ISFILE* pfile, HMETAFILE* phout)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  METAHEADER    meta_hdr;
  unsigned long is_aldus;
  unsigned char* meta_bits = NULL;
  int       wmf_offset = 0;

  assert(pfile);

  __try {
    __try {
      /* 读取文件头标识 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 如果是可放置元文件（APM），跳过APM文件头 */
      if (is_aldus == ALDUSKEY) {
        wmf_offset = ALDUSMETAHEADERSIZE;
      }

      /* 读入元文件数据，创建元文件handle */
      memset(&meta_hdr, 0, sizeof(meta_hdr));

      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(&meta_hdr, sizeof(meta_hdr), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      meta_bits = (unsigned char*)isirw_malloc(2 * meta_hdr.mtSize);

      if (meta_bits == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
          || isio_read(meta_bits, 2, meta_hdr.mtSize, pfile) != meta_hdr.mtSize) {
        b_status = ER_FILERWERR;
        __leave;
      }

      *phout = SetMetaFileBitsEx(2 * meta_hdr.mtSize, meta_bits);

      if (*phout == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

    }
    __finally {
      if (meta_bits != NULL) {
        isirw_free(meta_bits);
      }

      if (b_status != ER_SUCCESS || AbnormalTermination()) {
        *phout = NULL;
      }

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 将WMF数据转至数据包（矢量至光栅）*/
static int _meta_to_raster(HMETAFILE hwmf, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned char* dib_bits = NULL;
  HDC       hdc = NULL, hComDC = NULL;
  HBITMAP     bmp = NULL, oldbmp = NULL;
  BITMAPINFO    bmi;
  RECT      rect;
  int       linesize;
  unsigned    i;
  unsigned char** ppt;

  assert(hwmf);
  assert(pinfo_str);

  __try {
    __try {
      /* 创建与桌面兼容的DC，将元文件播放到该DC */
      if ((hdc = GetDC(GetDesktopWindow())) == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      memset(&bmi, 0, sizeof(bmi));
      bmi.bmiHeader.biSize          = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth         = pinfo_str->width;
      bmi.bmiHeader.biHeight        = pinfo_str->height;
      bmi.bmiHeader.biPlanes        = 1;
      bmi.bmiHeader.biBitCount      = (WORD)pinfo_str->bitcount;
      bmi.bmiHeader.biCompression   = BI_RGB;
      bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdc, HORZRES) * 1000 / GetDeviceCaps(hdc, HORZSIZE);
      bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdc, VERTRES) * 1000 / GetDeviceCaps(hdc, VERTSIZE);

      /* 创建位图句柄 */
      bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dib_bits, NULL, 0);

      if (bmp == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* 创建兼容DC */
      if ((hComDC = CreateCompatibleDC(hdc)) == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      oldbmp = SelectObject(hComDC, bmp);

      /* 将图象背景填充为白色 */
      rect.left = rect.top = 0;
      rect.right = pinfo_str->width;
      rect.bottom = pinfo_str->height;
      FillRect(hComDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

      /* 向兼容DC播放WMF数据 */
      SetMapMode(hComDC, MM_ANISOTROPIC);
      EnumMetaFile(hComDC, hwmf, _EnumMetaFileProc, (LPARAM)pinfo_str);
      SelectObject(hComDC, oldbmp);

      /* 利用得到的位图数据填充数据包相关域 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* 申请象素缓冲区 */
      if ((pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == NULL
          || (pinfo_str->pp_line_addr = isirw_malloc(sizeof(unsigned char*)*pinfo_str->height)) == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 将象素数据拷贝至数据包 */
      memcpy(pinfo_str->p_bit_data, dib_bits, linesize * pinfo_str->height);

      /* 填写行首地址数组 */
      ppt = (unsigned char**)pinfo_str->pp_line_addr;

      for (i = 0; i < pinfo_str->height; i++) {
        ppt[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }

      /* 填写数据包的其他域 */
      pinfo_str->pal_count  = 0;
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = NULL;
    }
    __finally {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }

      if (hComDC != NULL) {
        DeleteDC(hComDC);
      }

      if (bmp != NULL) {
        DeleteObject(bmp);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* WMF播放过程回调函数 */
static int CALLBACK _EnumMetaFileProc(HDC hDC,  HANDLETABLE* lpHTable,  METARECORD* lpMFR, int nObj, LPARAM lpClientData)
{
  static int viewport_ext_set = 0;
  LPINFOSTR  pinfo_str = (LPINFOSTR)lpClientData;

  PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);

  switch (lpMFR->rdFunction) {
  case 0x20e: /* SetViewportExt() */
    viewport_ext_set = !0;
    break;

  case 0x20c: /* SetWindowExt() */

    /* 如果元文件在设定窗口坐标(逻辑坐标）范围时，此前没有修改
       视坐标（设备坐标）范围，我们就自行设定。否则什么也不做，
       以防修改文件的自身设置 */
    if (!viewport_ext_set) {
      HDC hdc = GetDC(GetDesktopWindow());
      unsigned int cx, cy;
      float ratio;

      switch (pinfo_str->imgformat) {
      case IMF_APM:
        cx = pinfo_str->width;
        cy = pinfo_str->height;
        break;

        /* 如果是标准元文件，调整视坐标范围，使设备坐标纵横比
           与逻辑坐标一致，并且不超出图象的宽、高尺寸 */
      case IMF_WMF:
        cx = pinfo_str->width;
        cy = pinfo_str->height;

        /* ! 注意这里计算纵横比（y/x)的方法: 在元文件记录中，参数是
           与以函数形参表相反的顺序储存的，并且不储存hdc，因此对于
           SetWindowExt(),rdParm[0]储存Y范围，rdParm[1]储存X范围。*/
        ratio = (float)lpMFR->rdParm[0] / (float)lpMFR->rdParm[1];
        cy = (unsigned int)((float)cx * ratio);

        if (cy > pinfo_str->height) {
          cy = pinfo_str->height;
          cx = (unsigned int)((float)cy / ratio);
        }

        break;

      default:
        assert(0);
      }

      SetViewportExtEx(hDC, cx, cy, NULL);
      ReleaseDC(GetDesktopWindow(), hdc);
    }

    break;

  default:
    break;
  }

  return !0;
}

/* 读取图像位数据 */
WMF_API int CALLAGREEMENT wmf_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
# ifdef WIN32

  ISFILE*      pfile = (ISFILE*)0;
  enum EXERESULT  b_status = ER_SUCCESS;
  HMETAFILE   hwmf = NULL;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&wmf_load_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 打开流 */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 验证wmf文件的有效性 */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* 如果是空的数据包，首先获取图像概要信息，成功后再读取图像 */
      if (pinfo_str->data_state == 0) {
        b_status = _get_info(pfile, pinfo_str);

        if (b_status != ER_SUCCESS) {
          __leave;
        }

        pinfo_str->data_state = 1;
      }

      assert(pinfo_str->data_state == 1);

      /* 读入wmf数据 */
      if ((b_status = _load_metafile(pfile, &hwmf)) != ER_SUCCESS) {
        __leave;
      }

      assert(hwmf);

      /* 转至数据包 */
      if ((b_status = _meta_to_raster(hwmf, pinfo_str)) != ER_SUCCESS) {
        __leave;
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 一次行进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 主图象进度结束 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (hwmf) {
        DeleteMetaFile(hwmf);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (unsigned char*)0;
        }

        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (void**)0;
        }

        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1;  /* 自动降级 */
        }

        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
      }
      else {
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          break;

        case  2:
          b_status = ER_SYSERR;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }

        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (unsigned char*)0;
          }

          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (void**)0;
          }

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;  /* 自动降级 */
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&wmf_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

# else

  b_status = ER_NOTSUPPORT;

# endif /* WIN32 */

  return (int)b_status;
}
