#include <algorithm>


#include "caffe/layers/elu_layer.hpp"

namespace {

template <typename Dtype>
__global__ void ELUForward(const int n, const Dtype* in, Dtype* out,
    Dtype alpha) {
  CUDA_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] :
        alpha * (exp(in[index]) - 1);
  }
}

template <typename Dtype>
void ELULayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
  const int count = bottom[0]->count();
  Dtype alpha = this->param_->elu_param().alpha();
  // NOLINT_NEXT_LINE(whitespace/operators)
  ELUForward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
      count, bottom_data, top_data, alpha);
  CUDA_POST_KERNEL_CHECK;
}

template <typename Dtype>
__global__ void ELUBackward(const int n, const Dtype* in_diff,
    const Dtype* out_data, const Dtype* in_data,
    Dtype* out_diff, Dtype alpha) {
  CUDA_KERNEL_LOOP(index, n) {
    out_diff[index] = in_data[index] > 0 ? in_diff[index] :
        in_diff[index] * (out_data[index] + alpha);
  }
}

template <typename Dtype>
void ELULayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (bottom[0]->propagate_down_) {
    const Dtype* bottom_data = bottom[0]->data();
    const Dtype* top_diff = top[0]->gpu_diff();
    const Dtype* top_data = top[0]->data();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    const int count = bottom[0]->count();
    Dtype alpha = this->param_->elu_param().alpha();
    // NOLINT_NEXT_LINE(whitespace/operators)
    ELUBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, top_diff, top_data, bottom_data, bottom_diff, alpha);
    CUDA_POST_KERNEL_CHECK;
  }
}


INSTANTIATE_LAYER_GPU_FUNCS(ELULayer);


}  // namespace
