//һ������ħ�������������, ��ɫ�ֱ���
//0 ��ɫ��W��
//1 ����Ϊ��ɫ��Y��
//2 ��ɫ��R��
//3 ����Ϊ��ɫ��O��
//4 ��ɫ��G��
//5 ����Ϊ��ɫ��B��
//����ֳ�ħ��, ��ɫ����, ��ɫ�����Լ�, ����ɫ�����ֲ�, ����, ����, �ȱ���
//��дһ������, ��һ�����ڻ�ԭ״̬���������ͬɫ����ħ��, ���в���,
//��ӡ�������ħ��״̬������ָ��Ϊ��������ָ����ɵ��ַ�����
//��������ָ�������
//U����ɫ��˳ʱ����ת90��
//D����ɫ��˳ʱ����ת90��
//L����ɫ��˳ʱ����ת90��
//R����ɫ��˳ʱ����ת90��
//F����ɫ��˳ʱ����ת90��
//B����ɫ��˳ʱ����ת90��
//˳ʱ��90
//6, 7, 0 4, 5, 6
//5, , 1 ==> 3, , 7
//4, 3, 2 2, 1, 0
//�������� => 0123
// ħ�� 48 С��չ��
// B��
// 46,47,40
// 45, ,41
// 44,43,42
//
// U��
// 22,23,16
// 21, ,17
// 20,19,18
//
// L�� F�� R��
// 14,15, 8 6, 7, 0 30,31,24
// 13, , 9 5, , 1 29, ,25
// 12,11,10 4, 3, 2 28,27,26
//
// D��
// 38,39,32
// 37, ,33
// 36,35,34
uchar opt_group[12 * 48] = {0};
int magic_cube_group(const unsigned char* input_pos, int opt, unsigned char* output_pos)
{
  static int inited = 0;
  if (!inited) {
    int i, j, k;
    const char* r90 = "67012345";
    const char* optstr = "FLURDB";
    // ��������
    const char* side[6] = {
      "UDRLDULR",//F
      "ULFLDLBL",//L
      "BDRUFULU",//U
      "URBRDRFR",//R
      "FDRDBULD",//D
      "DDRRUULL",//B
    };
    char aa[6 * 3] = {
      0, 0, 0, //ǰ
      4, 5, 6, //��
      6, 7, 0, //��
      0, 1, 2, //��
      2, 3, 4, //��
      0, 0, 0, //��
    };
    for (i = 0; i < 6; ++i) {
      unsigned char* g1 = opt_group + i * 6 * 8;
      for (j = 0; j < 6 * 8; ++j) {
        g1[j] = j;
      }
      for (j = 0; j < 8; ++j) {
        g1[i * 8 + j] = (r90[j] - '0') + i * 8;
      }
      {
        const char* ss = side[i];
        uchar mp[128];
        for (j = 0; j < 6; ++j) {
          mp[optstr[j]] = j;
        }
        // ��ת��Χ����
        for (j = 0; j < 8; j += 2) {
          int l = (j + 2) % 8;
          int j0 = mp[ss[j + 0]];
          int j1 = mp[ss[j + 1]];
          int l0 = mp[ss[l + 0]];
          int l1 = mp[ss[l + 1]];
          for (k = 0; k < 3; ++k) {
            g1[l0 * 8 + aa[l1 * 3 + k]] = j0 * 8 + aa[j1 * 3 + k];
          }
        }
      }
      {
        for (j = 0; j < 48; ++j) {
          printf("%d ", g1[j]);
        }
        printf("\n");
      }
    }
    inited = 1;
  }
  return 0;
}
