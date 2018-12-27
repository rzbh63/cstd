
#include <string>
#include <vector>

const char* tempdir() {
#ifdef _WIN32
  static char _tmppath[256] = "";
  if (0 == *_tmppath) {
    GetTempPathA(256, _tmppath);
  }
  return _tmppath;
#else
  return "/tmp";
#endif
}
#include "std/dir_c.h"

inline char* tempfilename(const char* dirname, const char* name, char* buf, int len)
{
  char dir[256] = "";
  for (int i = 0; i < 10; i++) {
    _snprintf(dir, 256, "%s/%s.%s", tempdir(), name, "%%%%-%%%%");
    bool done = mkdirs(dir);
    _snprintf(buf, len, "%s/%s", dir, name);
    return buf;
  }
  return buf;
}

class DataLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;
public:
  DBMethod backend_;
  char filename_[256];
  Blob* const blob_top_data_;
  Blob* const blob_top_label_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
  int seed_;

  DataLayerTest()
    : backend_(DBMethod_LEVELDB),
    blob_top_data_(new Blob()),
    blob_top_label_(new Blob()),
    seed_(1701) {}
  virtual void SetUp() {
    tempfilename("cnn4_DataLayerTest", "testdb", filename_, 256);
    blob_top_vec_.push_back(blob_top_data_);
    blob_top_vec_.push_back(blob_top_label_);
  }

  // Fill the DB with data: if unique_pixels, each pixel is unique but
  // all images are the same; else each image is unique but all pixels within
  // an image are the same.
  void Fill(const bool unique_pixels, DBMethod backend) {
    backend_ = backend;
    LOG(INFO) << "Using temporary dataset " << filename_;
    DB* db = (GetDB(backend));
    db->Open(filename_, NEW);
    uchar data[2 * 3 * 4] = { 0 };
    Transaction* txn(db->NewTransaction());
    for (int i = 0; i < 5; ++i) {
      Datum datum;
      datum.resize(2);
      for (int j = 0; j < 24; ++j) {
        int datum = unique_pixels ? j : i;
        data[j] = datum;
      }
      datum[0].set(NCHW, TF_U8, data, 4, 3, 2);
      datum[1].set(NCHW, TF_U32, &i, 1);
      string key_str = format_int(i, 8);
      string value;
      str_append_datum(&value, datum);
      txn->Put(key_str, value);
    }
    txn->Commit();
    db->Close();
  }

  void TestRead() {
    const Dtype scale = 3;
    DataLayer layer;
    DataLayer* data_param = &layer;
    layer.phase_ = (TRAIN);
    data_param->batch_size_ = (5);
    data_param->source_ = (filename_);
    data_param->backend_ = (backend_);
    DataTransformerInfo* transform_param = &layer.transform_;
    transform_param->scale = (scale);
    layer.SetUp(blob_bottom_vec_, blob_top_vec_);
    EXPECT_EQ(blob_top_data_->num(), 5);
    EXPECT_EQ(blob_top_data_->channels(), 2);
    EXPECT_EQ(blob_top_data_->height(), 3);
    EXPECT_EQ(blob_top_data_->width(), 4);
    EXPECT_EQ(blob_top_label_->num(), 5);
    EXPECT_EQ(blob_top_label_->channels(), 1);
    EXPECT_EQ(blob_top_label_->height(), 1);
    EXPECT_EQ(blob_top_label_->width(), 1);
    for (int iter = 0; iter < 100; ++iter) {
      layer.Forward(blob_bottom_vec_, blob_top_vec_);
      for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
      }
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 24; ++j) {
          EXPECT_EQ(scale * i, blob_top_data_->cpu_data()[i * 24 + j])
            << "debug: iter " << iter << " i " << i << " j " << j;
        }
      }
    }
  }

  void TestReshape(DBMethod backend) {
    const int num_inputs = 5;
    // Save data of varying shapes.
    LOG(INFO) << "Using temporary dataset " << filename_;
    DB* db(GetDB(backend));
    db->Open(filename_, NEW);
    Transaction* txn(db->NewTransaction());
    for (int i = 0; i < num_inputs; ++i) {
      Datum datum;
      int channels = (2);
      int height = (i % 2 + 1);
      int width = (i % 4 + 1);
      int data_size = channels * height * width;
      uchar* data = (uchar*)malloc(data_size);
      for (int j = 0; j < data_size; ++j) {
        data[j] = (static_cast<uint8_t>(j));
      }
      datum.resize(2);
      datum[0].set(NCHW, TF_U8, data, width, height, channels);
      datum[1].set(NCHW, TF_U32, &i, 1);
      free(data);
      string key_str = format_int(i, 8);
      string value;
      str_append_datum(&value, datum);
      txn->Put(key_str, value);
    }
    txn->Commit();
    db->Close();
    // Load and check data of various shapes.
    DataLayer layer;
    DataLayer * data_param = &layer;
    layer.phase_ = (TEST);
    data_param->batch_size_ = (1);
    data_param->source_ = (filename_);
    data_param->backend_ = (backend);

    layer.SetUp(blob_bottom_vec_, blob_top_vec_);
    EXPECT_EQ(blob_top_data_->num(), 1);
    EXPECT_EQ(blob_top_data_->channels(), 2);
    EXPECT_EQ(blob_top_label_->num(), 1);
    EXPECT_EQ(blob_top_label_->channels(), 1);
    EXPECT_EQ(blob_top_label_->height(), 1);
    EXPECT_EQ(blob_top_label_->width(), 1);
    for (int iter = 0; iter < num_inputs; ++iter) {
      layer.Forward(blob_bottom_vec_, blob_top_vec_);
      EXPECT_EQ(blob_top_data_->height(), iter % 2 + 1);
      EXPECT_EQ(blob_top_data_->width(), iter % 4 + 1);
      EXPECT_EQ(iter, blob_top_label_->cpu_data()[0]);
      const int channels = blob_top_data_->channels();
      const int height = blob_top_data_->height();
      const int width = blob_top_data_->width();
      for (int c = 0; c < channels; ++c) {
        for (int h = 0; h < height; ++h) {
          for (int w = 0; w < width; ++w) {
            const int idx = (c * height + h) * width + w;
            EXPECT_EQ(idx, static_cast<int>(blob_top_data_->cpu_data()[idx]))
              << "debug: iter " << iter << " c " << c
              << " h " << h << " w " << w;
          }
        }
      }
    }
  }

  void TestReadCrop(Phase phase) {
    const Dtype scale = 3;
    DataLayer layer;
    DataLayer * data_param = &layer;
    layer.phase_ = (phase);
    set_random_seed(1701);

    data_param->batch_size_ = (5);
    data_param->source_ = (filename_);
    data_param->backend_ = (backend_);
    DataTransformerInfo* transform_param =
      &layer.transform_;
    transform_param->scale = (scale);
    transform_param->crop_size = (1);

    layer.SetUp(blob_bottom_vec_, blob_top_vec_);
    EXPECT_EQ(blob_top_data_->num(), 5);
    EXPECT_EQ(blob_top_data_->channels(), 2);
    EXPECT_EQ(blob_top_data_->height(), 1);
    EXPECT_EQ(blob_top_data_->width(), 1);
    EXPECT_EQ(blob_top_label_->num(), 5);
    EXPECT_EQ(blob_top_label_->channels(), 1);
    EXPECT_EQ(blob_top_label_->height(), 1);
    EXPECT_EQ(blob_top_label_->width(), 1);
    for (int iter = 0; iter < 2; ++iter) {
      layer.Forward(blob_bottom_vec_, blob_top_vec_);
      for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
      }
      int num_with_center_value = 0;
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
          const Dtype center_value = scale * (j ? 17 : 5);
          num_with_center_value +=
            (center_value == blob_top_data_->cpu_data()[i * 2 + j]);
          // At TEST time, check that we always get center value.
          if (phase == TEST) {
            EXPECT_EQ(center_value, this->blob_top_data_->cpu_data()[i * 2 + j])
              << "debug: iter " << iter << " i " << i << " j " << j;
          }
        }
      }
      // At TRAIN time, check that we did not get the center crop all 10 times.
      // (This check fails with probability 1-1/12^10 in a correct
      // implementation, so we call set_random_seed.)
      if (phase == TRAIN) {
        EXPECT_LT(num_with_center_value, 10);
      }
    }
  }

  void TestReadCropTrainSequenceSeeded() {
    DataLayer layer;
    DataLayer * data_param = &layer;
    layer.phase_ = (TRAIN);

    data_param->batch_size_ = (5);
    data_param->source_ = (filename_);
    data_param->backend_ = (backend_);
    DataTransformerInfo* transform_param =
      &layer.transform_;
    transform_param->crop_size = (1);
    transform_param->do_mirror = (true);
    // Get crop sequence with Caffe seed 1701.
    set_random_seed(seed_);
    vector<vector<Dtype> > crop_sequence;
    {
      DataLayer layer1(layer);
      layer1.SetUp(blob_bottom_vec_, blob_top_vec_);
      for (int iter = 0; iter < 2; ++iter) {
        layer1.Forward(blob_bottom_vec_, blob_top_vec_);
        for (int i = 0; i < 5; ++i) {
          EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
        }
        vector<Dtype> iter_crop_sequence;
        for (int i = 0; i < 5; ++i) {
          for (int j = 0; j < 2; ++j) {
            iter_crop_sequence.push_back(
              blob_top_data_->cpu_data()[i * 2 + j]);
          }
        }
        crop_sequence.push_back(iter_crop_sequence);
      }
    }  // destroy 1st data layer and unlock the db
    // Get crop sequence after reseeding Caffe with 1701.
    // Check that the sequence is the same as the original.
    set_random_seed(seed_);
    DataLayer layer2(layer);
    layer2.SetUp(blob_bottom_vec_, blob_top_vec_);
    for (int iter = 0; iter < 2; ++iter) {
      layer2.Forward(blob_bottom_vec_, blob_top_vec_);
      for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
      }
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
          EXPECT_EQ(crop_sequence[iter][i * 2 + j],
            blob_top_data_->cpu_data()[i * 2 + j])
            << "debug: iter " << iter << " i " << i << " j " << j;
        }
      }
    }
  }

  void TestReadCropTrainSequenceUnseeded() {
    DataLayer layer;
    layer.phase_ = (TRAIN);
    DataLayer * data_param = &layer;

    data_param->batch_size_ = (5);
    data_param->source_ = (filename_);
    data_param->backend_ = (backend_);
    DataTransformerInfo* transform_param =
      &layer.transform_;
    transform_param->crop_size = (1);
    transform_param->do_mirror = (true);
    // Get crop sequence with Caffe seed 1701, srand seed 1701.
    set_random_seed(seed_);
    srand(seed_);
    vector<vector<Dtype> > crop_sequence;
    {
      DataLayer layer1(layer);
      layer1.SetUp(blob_bottom_vec_, blob_top_vec_);
      for (int iter = 0; iter < 2; ++iter) {
        layer1.Forward(blob_bottom_vec_, blob_top_vec_);
        for (int i = 0; i < 5; ++i) {
          EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
        }
        vector<Dtype> iter_crop_sequence;
        for (int i = 0; i < 5; ++i) {
          for (int j = 0; j < 2; ++j) {
            iter_crop_sequence.push_back(
              blob_top_data_->cpu_data()[i * 2 + j]);
          }
        }
        crop_sequence.push_back(iter_crop_sequence);
      }
    }  // destroy 1st data layer and unlock the db
    // Get crop sequence continuing from previous Caffe RNG state; reseed
    // srand with 1701. Check that the sequence differs from the original.
    srand(seed_);
    DataLayer layer2(layer);
    layer2.SetUp(blob_bottom_vec_, blob_top_vec_);
    for (int iter = 0; iter < 2; ++iter) {
      layer2.Forward(blob_bottom_vec_, blob_top_vec_);
      for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, blob_top_label_->cpu_data()[i]);
      }
      int num_sequence_matches = 0;
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
          num_sequence_matches += (crop_sequence[iter][i * 2 + j] ==
            blob_top_data_->cpu_data()[i * 2 + j]);
        }
      }
      EXPECT_LT(num_sequence_matches, 10);
    }
  }

  virtual ~DataLayerTest() { delete blob_top_data_; delete blob_top_label_; }

};

TYPED_TEST_CASE(DataLayerTest, TestDtypesAndDevices);

#ifdef USE_LEVELDB
TYPED_TEST(DataLayerTest, TestReadLevelDB)
{
  const bool unique_pixels = false;  // all pixels the same; images different
  this->Fill(unique_pixels, DBMethod_LEVELDB);
  this->TestRead();
}

TYPED_TEST(DataLayerTest, TestReshapeLevelDB)
{
  this->TestReshape(DBMethod_LEVELDB);
}

TYPED_TEST(DataLayerTest, TestReadCropTrainLevelDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LEVELDB);
  this->TestReadCrop(TRAIN);
}

// Test that the sequence of random crops is consistent when using
// set_random_seed.
TYPED_TEST(DataLayerTest, TestReadCropTrainSequenceSeededLevelDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LEVELDB);
  this->TestReadCropTrainSequenceSeeded();
}

// Test that the sequence of random crops differs across iterations when
// set_random_seed isn't called (and seeds from srand are ignored).
TYPED_TEST(DataLayerTest, TestReadCropTrainSequenceUnseededLevelDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LEVELDB);
  this->TestReadCropTrainSequenceUnseeded();
}

TYPED_TEST(DataLayerTest, TestReadCropTestLevelDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LEVELDB);
  this->TestReadCrop(TEST);
}
#endif  // USE_LEVELDB

#ifdef USE_LMDB
TYPED_TEST(DataLayerTest, TestReadLMDB)
{
  const bool unique_pixels = false;  // all pixels the same; images different
  this->Fill(unique_pixels, DBMethod_LMDB);
  this->TestRead();
}

TYPED_TEST(DataLayerTest, TestReshapeLMDB)
{
  this->TestReshape(DBMethod_LMDB);
}
#if 0
TYPED_TEST(DataLayerTest, TestReadCropTrainLMDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LMDB);
  this->TestReadCrop(TRAIN);
}

// Test that the sequence of random crops is consistent when using
// set_random_seed.
TYPED_TEST(DataLayerTest, TestReadCropTrainSequenceSeededLMDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LMDB);
  this->TestReadCropTrainSequenceSeeded();
}

// Test that the sequence of random crops differs across iterations when
// set_random_seed isn't called (and seeds from srand are ignored).
TYPED_TEST(DataLayerTest, TestReadCropTrainSequenceUnseededLMDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LMDB);
  this->TestReadCropTrainSequenceUnseeded();
}

TYPED_TEST(DataLayerTest, TestReadCropTestLMDB)
{
  const bool unique_pixels = true;  // all images the same; pixels different
  this->Fill(unique_pixels, DBMethod_LMDB);
  this->TestReadCrop(TEST);
}
#endif

#endif  // USE_LMDB

