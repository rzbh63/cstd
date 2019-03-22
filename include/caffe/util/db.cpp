<<<<<<< HEAD
#include "caffe/util/db.hpp"
#ifdef USE_LMDB
#include "caffe/util/db_leveldb.hpp"
#endif
#ifdef USE_LMDB
#include "caffe/util/db_lmdb.hpp"
#endif

#include <string>

namespace caffe
{
  namespace db
  {

    DB* GetDB(DataParameter::DB backend)
    {
      switch (backend) {
#ifdef USE_LEVELDB
      case DataParameter_DB_LEVELDB:
        return new LevelDB();
#endif  // USE_LEVELDB
#ifdef USE_LMDB
      case DataParameter_DB_LMDB:
        return new LMDB();
#endif  // USE_LMDB
      default:
        LOG(FATAL) << "Unknown database backend";
        return NULL;
      }
    }

    DB* GetDB(const string & backend)
    {
#ifdef USE_LEVELDB
      if (backend == "leveldb") {
        return new LevelDB();
      }
#endif  // USE_LEVELDB
#ifdef USE_LMDB
      if (backend == "lmdb") {
        return new LMDB();
      }
#endif  // USE_LMDB
      LOG(FATAL) << "Unknown database backend";
      return NULL;
    }

  }  // namespace db
}  // namespace caffe
=======
#include "caffe/util/db.hpp"
#ifdef USE_LMDB
#include "caffe/util/db_leveldb.hpp"
#endif
#ifdef USE_LMDB
#include "caffe/util/db_lmdb.hpp"
#endif

#include <string>

namespace caffe
{
  namespace db
  {

    DB* GetDB(DataParameter::DB backend)
    {
      switch (backend) {
#ifdef USE_LEVELDB
      case DataParameter_DB_LEVELDB:
        return new LevelDB();
#endif  // USE_LEVELDB
#ifdef USE_LMDB
      case DataParameter_DB_LMDB:
        return new LMDB();
#endif  // USE_LMDB
      default:
        LOG(FATAL) << "Unknown database backend";
        return NULL;
      }
    }

    DB* GetDB(const string & backend)
    {
#ifdef USE_LEVELDB
      if (backend == "leveldb") {
        return new LevelDB();
      }
#endif  // USE_LEVELDB
#ifdef USE_LMDB
      if (backend == "lmdb") {
        return new LMDB();
      }
#endif  // USE_LMDB
      LOG(FATAL) << "Unknown database backend";
      return NULL;
    }

  }  // namespace db
}  // namespace caffe
>>>>>>> aa5c8bea907c2c9ef806cc4a153cc214ef54cb5d
