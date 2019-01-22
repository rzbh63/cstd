#include <thread>
#include <mutex>
#include "caffe/layer.hpp"

namespace caffe
{
  template <typename Dtype>
  void Layer<Dtype>::InitMutex()
  {
    forward_mutex_.reset(new std::mutex());
  }

  template <typename Dtype>
  void Layer<Dtype>::Lock()
  {
    if (IsShared()) {
      forward_mutex_->lock();
    }
  }

  template <typename Dtype>
  void Layer<Dtype>::Unlock()
  {
    if (IsShared()) {
      forward_mutex_->unlock();
    }
  }

  INSTANTIATE_CLASS(Layer);

}  // namespace caffe
