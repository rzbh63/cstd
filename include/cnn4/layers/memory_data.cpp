#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif  // USE_OPENCV



#include "caffe/layers/memory_data_layer.hpp"

namespace
{

  template <typename Dtype>
  void MemoryDataLayer::DataLayerSetUp(const vector<Blob*> & bottom,
      const vector<Blob*> & top)
  {
    batch_size_ = this->param_->memory_data_param().batch_size();
    channels_ = this->param_->memory_data_param().channels();
    height_ = this->param_->memory_data_param().height();
    width_ = this->param_->memory_data_param().width();
    label_size_ = this->param_->memory_data_param().label_size();
    size_ = channels_ * height_ * width_;
    CHECK_GT(batch_size_ * size_, 0) <<
                                     "batch_size, channels, height, and width must be specified and"
                                     " positive in memory_data_param";
    vector<int> label_shape;
    top[0]->Reshape(batch_size_, channels_, height_, width_);
    top[1]->Reshape(label_shape);
    added_data_.Reshape(batch_size_, channels_, height_, width_);
    added_label_.Reshape(label_shape);
    data_ = NULL;
    labels_ = NULL;
    added_data_.data();
    added_label_.data();
  }

  template <typename Dtype>
  void MemoryDataLayer::AddDatumVector(const vector<Datum> & datum_vector)
  {
    CHECK(!has_new_data_) <<
                          "Can't add data until current data has been consumed.";
    size_t num = datum_vector.size();
    CHECK_GT(num, 0) << "There is no datum to add.";
    CHECK_EQ(num % batch_size_, 0) <<
                                   "The added data must be a multiple of the batch size.";
    CHECK_EQ(label_size_, datum_vector[0].label_size()) <<
        "The label size for input and prototxt setting is not match";
    added_data_.Reshape(num, channels_, height_, width_);
    added_label_.Reshape(num, 1, 1, 1);
    // Apply data transformations (mirror, scale, crop...)
    this->data_transformer_->Transform(datum_vector, &added_data_);
    // Copy Labels
    Dtype* top_label = added_label_.mdata();
//   for (int item_id = 0; item_id < num; ++item_id) {
//     top_label[item_id] = datum_vector[item_id].label();
//   }
    for (int item_id = 0; item_id < num; ++item_id) {
      for (int i = 0; i < label_size_; i++) {
        top_label[item_id * label_size_ + i] = labels_[item_id * label_size_ + i];
      }
    }
    // num_images == batch_size_
    Dtype* top_data = added_data_.mdata();
    Reset(top_data, top_label, num);
    has_new_data_ = true;
  }

#ifdef USE_OPENCV
  template <typename Dtype>
  void MemoryDataLayer::AddMatVector(const vector<cv::Mat> & mat_vector,
      const vector<int> & labels)
  {
    size_t num = mat_vector.size();
    CHECK(!has_new_data_) <<
                          "Can't add mat until current data has been consumed.";
    CHECK_GT(num, 0) << "There is no mat to add";
    CHECK_EQ(num % batch_size_, 0) <<
                                   "The added data must be a multiple of the batch size.";
    added_data_.Reshape(num, channels_, height_, width_);
    added_label_.Reshape(num, 1, 1, 1);
    // Apply data transformations (mirror, scale, crop...)
    this->data_transformer_->Transform(mat_vector, &added_data_);
    // Copy Labels
    Dtype* top_label = added_label_.mdata();
    for (int item_id = 0; item_id < num; ++item_id) {
      top_label[item_id] = labels[item_id];
    }
    // num_images == batch_size_
    Dtype* top_data = added_data_.mdata();
    Reset(top_data, top_label, num);
    has_new_data_ = true;
  }
#endif  // USE_OPENCV

  template <typename Dtype>
  void MemoryDataLayer::Reset(Dtype* data, Dtype* labels, int n)
  {
    CHECK(data);
    CHECK(labels);
    CHECK_EQ(n % batch_size_, 0) << "n must be a multiple of batch size";
    // Warn with transformation parameters since a memory array is meant to
    // be generic and no transformations are done with Reset().
    if (this->param_->has_transform_param()) {
      LOG(WARNING) << this->type() << " does not transform array data on Reset()";
    }
    data_ = data;
    labels_ = labels;
    n_ = n;
    pos_ = 0;
  }

  template <typename Dtype>
  void MemoryDataLayer::set_batch_size(int new_size)
  {
    CHECK(!has_new_data_) <<
                          "Can't change batch_size until current data has been consumed.";
    batch_size_ = new_size;
    added_data_.Reshape(batch_size_, channels_, height_, width_);
    added_label_.Reshape(batch_size_, label_size_, 1, 1);
  }

  template <typename Dtype>
  void MemoryDataLayer::Forward_(CPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top)
  {
    CHECK(data_) << "MemoryDataLayer needs to be initialized by calling Reset";
    top[0]->Reshape(batch_size_, channels_, height_, width_);
    top[1]->Reshape(batch_size_, label_size_, 1, 1);
    top[0]->set_data(data_ + pos_ * size_);
    top[1]->set_data(labels_ + pos_);
    pos_ = (pos_ + batch_size_) % n_;
    if (pos_ == 0) {
      has_new_data_ = false;
    }
  }

  INSTANTIATE_CLASS(MemoryDataLayer);
  REGISTER_LAYER_CLASS(MemoryData);

}  // namespace
