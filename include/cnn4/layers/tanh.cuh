// TanH neuron activation function layer.
// Adapted from ReLU layer code written by Yangqing Jia



#include "caffe/layers/tanh_layer.hpp"

namespace {

template <typename Dtype>
__global__ void TanHForward(const int n, const Dtype* in, Dtype* out) {
  CUDA_KERNEL_LOOP(index, n) {
    out[index] = tanh(in[index]);
  }
}

template <typename Dtype>
void TanHLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
  const int count = bottom[0]->count();
  // NOLINT_NEXT_LINE(whitespace/operators)
  TanHForward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
      count, bottom_data, top_data);
  CUDA_POST_KERNEL_CHECK;
}

template <typename Dtype>
__global__ void TanHBackward(const int n, const Dtype* in_diff,
    const Dtype* out_data, Dtype* out_diff) {
  CUDA_KERNEL_LOOP(index, n) {
    Dtype tanhx = out_data[index];
    out_diff[index] = in_diff[index] * (1 - tanhx * tanhx);
  }
}

template <typename Dtype>
void TanHLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (bottom[0]->propagate_down_) {
    const Dtype* top_data = top[0]->data();
    const Dtype* top_diff = top[0]->gpu_diff();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    const int count = bottom[0]->count();
    // NOLINT_NEXT_LINE(whitespace/operators)
    TanHBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, top_diff, top_data, bottom_diff);
    CUDA_POST_KERNEL_CHECK;
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(TanHLayer);


}  // namespace
