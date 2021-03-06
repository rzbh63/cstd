#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include <vector>

#include "blob.hpp"
#include "data_transformer.hpp"
#include "internal_thread.hpp"
#include "layer.hpp"
#include "layers/base_data_layer.hpp"
#include "proto.h"
#include "db.hpp"

namespace caffe
{

  template <typename Dtype>
  class DataLayer : public BasePrefetchingDataLayer<Dtype>
  {
  public:
    explicit DataLayer(CJSON* param);
    virtual ~DataLayer();
    virtual void DataLayerSetUp(const vector<Blob<Dtype>*> & bottom,
                                const vector<Blob<Dtype>*> & top);
    // DataLayer uses DataReader instead for sharing for parallelism
    virtual inline bool ShareInParallel() const { return false; }
    virtual inline const char* type() const { return "Data"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int MinTopBlobs() const { return 1; }
    virtual inline int MaxTopBlobs() const { return 10; }

    vector< std::pair<std::string, vector<int> > > lines_;
    int line_id_;

  protected:
    void Next();
    bool Skip();
    virtual void load_batch(Batch<Dtype>* batch);

    SHARED_PTR(db::DB) db_;
    SHARED_PTR(db::Cursor) cursor_;
    uint64_t offset_;

    unsigned int rand_skip_num_;
  };

}  // namespace caffe

#endif  // CAFFE_DATA_LAYER_HPP_
