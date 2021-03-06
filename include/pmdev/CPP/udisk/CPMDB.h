#ifndef CPMINTERFACE_H
#define CPMINTERFACE_H

/********************************************************************/
/*工程名称: CPMDB.lib
/*功    能: 用于解析在DSP端使用U盘下载下来的数据
/*说    明: 实现U盘用户下载, U盘记录下载
            此U盘中存有多台DSP中的用户数据和记录数据
/*时    间: 2011/04/14
/********************************************************************/
#ifdef CPMDB_EXPORTS
#define CPMDB_API
#else
#define CPMDB_API
#endif


#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*OperCallBack)(void* handle, const char* buf, unsigned int len, void* param);

  //功能：打开 flag = true，关闭 flag = false数据库
  //参数：数据库的所在路径名全称
  //返回：成功true，失败false
  CPMDB_API BOOL  CPM_DBOpen(BOOL flag , const char* DB_AbsPath);

  //功能：注册回调
  //参数：数据库的所在路径名全称
  //返回：成功true，失败false
  CPMDB_API void  CPM_DBSetOperCallBack(OperCallBack cb , void* param);

  //功能：打开 flag = true，关闭 flag = false数据库
  //参数：数据库的所在路径名全称
  //返回：成功true，失败false
  CPMDB_API void  OperDB(const char* buf, unsigned int len, const char* DB_AbsPath);

#ifdef __cplusplus
};
#endif

#endif