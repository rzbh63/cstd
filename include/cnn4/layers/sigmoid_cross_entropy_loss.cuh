

#include "caffe/layers/sigmoid_cross_entropy_loss_layer.hpp"


namespace {

template <typename Dtype>
void SigmoidCrossEntropyLossLayer::Backward_gpu(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (bottom[1]->propagate_down_) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (bottom[0]->propagate_down_) {
    // First, compute the diff
    const int count = bottom[0]->count();
    const int num = bottom[0]->num();
    const Dtype* sigmoid_output_data = sigmoid_output_->data();
    const Dtype* target = bottom[1]->data();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    caffe_copy(count, sigmoid_output_data, bottom_diff);
    caffe_gpu_axpy(count, Dtype(-1), target, bottom_diff);
    // Scale down gradient
    const Dtype loss_weight = top[0]->diff()[0];
    caffe_gpu_scal(count, loss_weight / num, bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_BACKWARD(SigmoidCrossEntropyLossLayer);


}  // namespace
