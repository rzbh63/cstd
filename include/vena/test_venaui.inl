
#include "ui/uiwin.inl"

typedef struct test_vena_recog_t {
  ctrl_t c[1];
  button_t btn_del[1];
  slider_t sli0[1];
  layout_t lay0[1];

  layout_t lay_reg[1];
  button_t btn_reg[1];
  button_t btn_rec[1];
  button_t btn_recstop[1];
  editctrl_t edt_id[1];
  char str_id[32];

  button_t btn_opencap[1];
  
  layout_t lay_std[1];
  null_t nul_std[1];

  layout_t lay_rec[1];
  button_t btn_rec_clear[1];

  null_t nul0[1];

  listbox_t lb_reg[1];
  listbox_t lb_rec[1];
  buf_t* bf;

  capdev cap[1];
  img_t im[1];
  uchar imstd[VENA_STDH * VENA_STDW];
  int isreg;
  int isrec;
  int stdok;
  buf_t lib[1];
  int recid;
  int recdist;
  char buf[256];
} test_vena_recog_t;

static LRESULT test_vena_recog_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_vena_recog_t* s = (test_vena_recog_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  img_t imstd[1] = {0};
  IMINIT(imstd, VENA_STDH, VENA_STDW, s->imstd, VENA_STDW, 1, 1);

  if (WM_CTRLINIT == uMsg) {
    int n;
    button_set(s->btn_reg, c, "注册");
    button_set(s->btn_rec, c, "开始识别");
    button_set(s->btn_recstop, c, "停止识别");
    button_set(s->btn_del, c, "删除所有");
    button_set(s->btn_rec_clear, c, "清空记录");

    slider_set(s->sli0, c, "slider %6.3f", 0, 100, 10);
    listbox_set(s->lb_reg, c);
    listbox_set(s->lb_rec, c);
    editctrl_set_buf(s->edt_id, c, "id", s->str_id, countof(s->str_id), DT_SINGLELINE);
    
    ctrl_set(null_proc, s->nul0, s->nul0->c, c, WT_CHILD, 0);
    ctrl_set(null_proc, s->nul_std, s->nul_std->c, c, WT_CHILD, 0);
    
    layout_set(s->lay_std, c, 4, "归一化照片",
      " x;"
      "x%;",
      s->nul_std);

    layout_set(s->lay_reg, c, 4, "注册",
        " x;"
        "a%;"
        "a%;"
        "a%;"
        "a%;"
        "a%;"
        "x%;"
        , s->edt_id, s->btn_reg, s->btn_rec, s->btn_recstop, s->btn_del, s->lb_reg);

    layout_set(s->lay_rec, c, 4, "记录",
        " x;"
        "a%;"
        "x%;"
        , s->btn_rec_clear, s->lb_rec);

    layout_set(s->lay0, c, 4, "视频", 
        " xxxx;"
        "x%--%;"//nul grp_ipcap
        "x+  %;"
        , s->nul0, s->lay_reg, s->lay_std);
    
    ctrl_addsub_tail(c, s->lay0->c);
    //c->sub = s->lay0->c;
    n = cap_num(CAP_DSHOW);
    if (n>0) {
      cap_open(s->cap, n-1, CAP_DSHOW);
    }
    buf_load(VENA_LIBFILE, s->lib);
    listbox_set_item_count(s->lb_reg, s->lib->len>30 ? *(int*)s->lib->data/PICNUM : 0);
    return 0;
  }

  if (!s->cap->x) {
    static int ttt = 0;

    if (WM_TIMER == uMsg) {
      if (0 == ttt) {
        //hcnet_open(s->cap, "192.168.1.45", 8000, "admin", "123456");
        //capvfw_open(capvfw* s, int wIndex)
      }

      ++ttt;
    }
  }

  if (WM_CTRLFREE == uMsg) {
    imfree(s->im);
    cap_close(s->cap);
    c->ctrlproc = 0;
    free(s);
    return 0;
  }

  switch (uMsg) {
  case WM_TIMER:
    if (s->cap->x) {
      char buf[256];
      uchar feat[VENA_STEP] = {0};
      //printf("%d\n", s->str->l);
      cap_getframe(s->cap, s->im, 0);

      s->stdok = vena_recog(s->bf, s->im, s->lib, s->imstd, feat, &s->recid, &s->recdist);
      if (s->stdok && s->isreg) {
        if (1) {
          _snprintf(buf, 256, "reg\\%s", s->str_id);
          _mkdir("reg");
          _mkdir(buf);
          _snprintf(buf, 256, "reg\\%s\\std_%s_%03d.bmp", s->str_id, s->buf, s->isreg-1);
          imwrite(buf, imstd);
          _snprintf(buf, 256, "reg\\%s\\%s_%03d.jpg", s->str_id, s->buf, s->isreg-1);
          imwrite(buf, s->im);
        }
        if (1) {
          uchar* p;
          strcpy((char*)feat+VENA_FEATLEN, s->str_id);
          bf_pushback(s->lib, 8, feat, VENA_STEP);
          p = s->lib->data;
          (*GETARG(p, int))++;
          (*GETARG(p, int)) = VENA_STEP;
        }
        s->isreg = (s->isreg+1)%(PICNUM+1);
        if (0==s->isreg) {
          buf_save(VENA_LIBFILE, s->lib);
          listbox_set_item_count(s->lb_reg, *(int*)s->lib->data/PICNUM);
          MessageBox(NULL, "注册完毕!", "注册完毕!", 0);
        }
      }
      if (s->isrec) {
        if (s->recdist<VENA_THD) {
          char buf[256];
          uchar* finfo;
          s->isrec = 0;
          finfo = s->lib->data+8 + s->recid*VENA_STEP + VENA_FEATLEN;
          _snprintf(buf, 256, "%s : %s(%d)", s->recdist<VENA_THD?"通过":"拒绝", finfo, s->recdist);
          MessageBox(NULL, buf, "识别通过!", 0);
        }
      }

      force_redraw(c);
    }

    break;

  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    if (c->sub) {
      c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
    }
    mminfo->ptMinTrackSize.x += 520;
    mminfo->ptMinTrackSize.y += 220;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONUP:
    if (1) {
    }

    break;

  case WM_COMMAND:
    if (1) {
      if (sender == s->btn_reg->c) {
        s->isreg = 1;
        s->isrec = 0;
        sf_get_time();
        _snprintf(s->buf, 256, "%04d%02d%02d%02d%02d%02d%03d",
            sfyear(), sfmonth(), sfday(), sfhour(), sfminute() , sfsecond() , sfmillis());
      }
      if (sender == s->btn_rec->c) {
        s->isrec = 1;
      }
      if (sender == s->btn_recstop->c) {
        s->isrec = 0;
      }

      if (sender == s->btn_rec_clear->c) {

      }

      if (sender == s->btn_del->c) {
        bffree(s->lib);
        buf_save(VENA_LIBFILE, s->lib);
        listbox_set_item_count(s->lb_reg, 0);
      }

      if (sender == s->btn_opencap->c) {
        if (s->cap->x) {
          cap_close(s->cap);
        }

        //hcnet_open(s->cap, s->edt_ip->edit->str->s, 8000, s->edt_user->edit->str->s, s->edt_pass->edit->str->s);
      }
    }

    break;

  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }

    break;

  case WM_MOUSEMOVE:
    if (s) {
    }

    break;

  case WM_MEASUREITEM:
    if (sender == s->lb_reg->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = 20;
      ds->w = 100;
    }

    if (sender == s->lb_rec->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = 20;
      ds->w = 100;
    }

    break;

  case WM_DRAWITEM:
    if (sender == s->lb_reg->c && s->lib->len>50) {
      uchar* p = s->lib->data;
      int n = *GETARG(p, int);
      int k = wParam;
      char buf[256];
      int step = *GETARG(p, int);
      uchar* feat = s->lib->data + 8 + PICNUM*k*step + VENA_FEATLEN;
      drawitem_t* ds = (drawitem_t*)lParam;
      IRECT rcTxt = ds->itemrc;
      COLOR clrBk = _rgb(1, 1, 1);
      COLOR clrText = _rgb(0, 0, 0);
      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = _rgb(1, 1, 1);
      }
      
      _snprintf(buf, 256, "%s", feat);
      imdraw_rect(ds->im, ds->cliprc, ds->itemrc, clrBk, 0, 0, 0);
      imdraw_text(ds->im, ds->cliprc, rcTxt, buf, -1, c->fo, clrText, 0, DT_VCENTER);
    }

    if (sender == s->lb_rec->c) {
    }

    break;

  case WM_PAINT:
    if (c->recalcsize) {
      ctrl_setrect(c->sub, c->rc);
    }

    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      IRECT imrc = s->nul0->c->rc;
      //imdraw_fill(im, _rgb(1,1,1));
      imdraw_fill(im, _GetSysColor(CLR_3DFACE));
      //venaapp_draw(im, &s->nul0->c->rc, s->im, s->fa, s->fp, g_scr->font);
      imdraw_image(im, &imrc, &s->nul0->c->rc, s->im, 0);
      if (s->isreg) {
        IRECT rc;
        //rc = iRECT(0, 0, RCW(&imrc), 30);
        //imdraw_rect(im, &imrc, rc, 0, _rgba(0,0,0,0.3), 1, BDM_ALL);
        rc = iRECT(0, 0, RCW(&imrc)*(s->isreg)/PICNUM, 30);
        imdraw_rect(im, &imrc, rc, _rgba(1,1,1,0.3), 0, 0, 0);
      } else {
        if (s->recid>=0 && s->lib->data) {
          char buf[256];
          uchar* finfo = s->lib->data+8 + s->recid*VENA_STEP + VENA_FEATLEN;
          font_t* fo = sys_font_load_win32(c->sys, "宋体|72");
          COLOR clr = s->recdist<VENA_THD ? _rgba(0,1,0,1) : _rgba(1,0,0,0.2);
          _snprintf(buf, 256, "%s : %s(%d)", s->recdist<VENA_THD?"通过":"拒绝", finfo, s->recdist);
          imdraw_text(im, &imrc, imrc, buf, -1, fo, clr, 0, DT_SINGLELINE|DT_CENTER|DT_CENTER);
        }
      }
      if (s->stdok) {
        IRECT rc = iRECT(0, 0, 64, 96);
        COLOR clr = s->isreg ? _rgb(1,1,1) : _rgb(1,0,0);
        rc = s->nul_std->c->rc;
        imdraw_image_corner(im, 0, rc, imstd, 0, 0, 0);
        imdraw_rect(im, 0, rc, 0, clr, 1, BDM_ALL);
      }
      c->redraw = 0;
      //printf("WM_PAINT\n");
    }
    break;
  }

  return 0;
}

int test_venaui() {
  if (0) {
    sys_t sys[1] = {0};
    test_vena_recog_t test_vena_recog[1] = {0};
    test_vena_recog->bf = bf;
    ctrl_set(test_vena_recog_proc, test_vena_recog, test_vena_recog->c, 0, WT_POPUP, "指静脉演示demo");
    for (;;) {
      int ch = waitkey(-1);
      
      if (-2 == ch) {
        break;
      }
      
      //printf("--%08x\n", &ch);
    }
    
    sys_exit();
  }
  return 0;
}