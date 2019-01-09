

#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

#include <filesystem>
#include <time.h>
#include <math.h>
#include <io.h>
#include <stdio.h>
#include "os.hpp"
#include "string.hpp"

//using namespace 
using namespace std::experimental;
using filesystem::path;

namespace wstd {
  namespace filesystem {
    static path unique_path(const path& model) {
      std::wstring s(model.wstring());  // std::string ng for MBCS encoded POSIX
      const wchar_t hex[] = L"0123456789abcdef";
      char ran[] = "123456789abcdef";  // init to avoid clang static analyzer message
                                       // see ticket #8954
      assert(sizeof(ran) == 16);
      srand((unsigned int)time(NULL));
      for (std::wstring::size_type i = 0; i < s.size(); ++i)
      {
        if (s[i] == L'%')                        // digit request
        {
          int c = rand();
          s[i] = hex[c & 0xf];             // convert to hex digit and replace
        }
      }

      return s;
    }
  }

  static string readfile(const char* testfile) {
    FILE* pf = NULL;
    pf = fopen(testfile, "rb");
    string str;
    if (pf) {
      fseek(pf, 0, SEEK_END);   ///���ļ�ָ���ƶ��ļ���β
      int size = ftell(pf); ///�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
      str.resize(size);
      fseek(pf, 0, SEEK_SET);   ///���ļ�ָ���ƶ��ļ���β
      fread(&str[0], size, 1, pf);
      fclose(pf);
    }
    return str;
  }
  static int readlines(const char* testfile, std::vector<string>& strs) {
    string str = readfile(testfile);
    wstd::split(strs, str, "\n");
    return strs.size();
  }
  static int readlines(const char* testfile, std::vector<string>& strs, int begline, int maxline) {
    FILE* pf = NULL;
    pf = fopen(testfile, "rb");
    if (pf) {
      char buf[1024];
      int i = 0;
      int endline = begline + maxline;
      for (; fgets(buf, 1024, pf) > 0; ++i) {
        if (begline>0 && i < begline) {
          continue;
        }
        if (maxline>0 && i >= endline) {
          break;
        }
        string str = trim(buf, "\r\n");
        strs.push_back(str);
      }
      fclose(pf);
    }
    return (int)strs.size();
  }

}


#endif // _FILESYSTEM_HPP_
