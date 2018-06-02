
#if 0
#include "init.inl"
#include "img/color.inl"

#include "cascadearr.h"

unsigned int vis_nesting_face0701a[] = {
#include "face/cas/vis_nesting_face0701.inl"
};
unsigned int VisAllEye_nesting[] = {
#include "face/cas/VisAllEye_nesting.inl"
};
uint asmTrainData_all[] = {
#include "asmTrainData_all.inl"
};

int asmfit_process2( void* p0 ) {
  enum { BUFLEN = 100 };
  XRECT out[ BUFLEN ];
  asmfit_param_t* p = (asmfit_param_t*)p0;
  int ret;
  
  int face_cnt;
  int height = p->height, width = p->width;
  const unsigned char* gry = p->data;
  //imwrite("AAAA.bmp",height, width, gry, width, 1);
  face_cnt = CPM_FaceDetect( vis_nesting_face0701, height, width, gry, width,
    0, 0, 0.1f, 1.f, 15.f, 1.1f, 0, 0.5f, 4, out, BUFLEN );
  ret = face_cnt > 0;
  if ( ret ) {
    XRECT * face = out;
    XRECT *eye = out + face_cnt;
    int len = BUFLEN - face_cnt;
    const void * eyecas = VisAllEye_nesting;
    const void * alleye = 0;
    const void * eyewg = 0; //CPM_GetCascade(NIR_EYE_WITHOUT_GLASS_CASCADE);
    const void * deyecas = 0;  //CPM_GetCascade(NIR_DEYE_CASCADE);
    ret = 3 == CPM_EyesDetect( deyecas, eyecas, eyecas, alleye, alleye, eyewg, eyewg, gry, width, 1, face, eye, len );
#define RECTCENTER(_RT, _PT) ((_PT).x = (_RT).x + (_RT).w / 2, (_PT).y = (_RT).y + (_RT).h / 2)
    
    RECTCENTER( eye[ 1 ], p->leye );
    RECTCENTER( eye[ 2 ], p->reye );
    if (ret) {
      asmfit_process(p);
      return 1;
    }
  }
#undef BUFLEN
  
  return 0;
}
#endif
