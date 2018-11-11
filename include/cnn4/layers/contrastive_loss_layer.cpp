#include <algorithm>


#include "caffe/layers/contrastive_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void ContrastiveLossLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::LayerSetUp(bottom, top);
    CHECK_EQ(bottom[0]->channels(), bottom[1]->channels());
    CHECK_EQ(bottom[0]->height(), 1);
    CHECK_EQ(bottom[0]->width(), 1);
    CHECK_EQ(bottom[1]->height(), 1);
    CHECK_EQ(bottom[1]->width(), 1);
    CHECK_EQ(bottom[2]->channels(), 1);
    CHECK_EQ(bottom[2]->height(), 1);
    CHECK_EQ(bottom[2]->width(), 1);
    diff_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);
    diff_sq_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);
    dist_sq_.Reshape(bottom[0]->num(), 1, 1, 1);
    // vector of ones used to sum along channels
    summer_vec_.Reshape(bottom[0]->channels(), 1, 1, 1);
    for (int i = 0; i < bottom[0]->channels(); ++i) {
      summer_vec_.mutable_data<Context>()[i] = Dtype(1);
    }
  }

  template <typename Dtype>
  void ContrastiveLossLayer<Dtype>::Forward(_CONTEXT,
    const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    int count = bottom[0]->count();
    caffe_sub(
      count,
      bottom[0]->data<Context>(),  // a
      bottom[1]->data<Context>(),  // b
      diff_.mutable_data<Context>());  // a_i-b_i
    const int channels = bottom[0]->channels();
    Dtype margin = this->param_->contrastive_loss_param().margin();
    bool legacy_version =
      this->param_->contrastive_loss_param().legacy_version();
    Dtype loss(0.0);
    for (int i = 0; i < bottom[0]->num(); ++i) {
      dist_sq_.mutable_data<Context>()[i] = caffe_dot(channels,
                                       diff_.data<Context>() + (i * channels), diff_.data<Context>() + (i * channels));
      if (static_cast<int>(bottom[2]->data<Context>()[i])) {  // similar pairs
        loss += dist_sq_.data<Context>()[i];
      } else {  // dissimilar pairs
        if (legacy_version) {
          loss += std::max(margin - dist_sq_.data<Context>()[i], Dtype(0.0));
        } else {
          Dtype dist = std::max<Dtype>(margin - sqrt(dist_sq_.data<Context>()[i]),
                                       Dtype(0.0));
          loss += dist * dist;
        }
      }
    }
    loss = loss / static_cast<Dtype>(bottom[0]->num()) / Dtype(2);
    top[0]->mutable_data<Context>()[0] = loss;
  }

  template <typename Dtype>
  void ContrastiveLossLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
      const vector<Blob<Dtype>*> & bottom)
  {
    Dtype margin = this->param_->contrastive_loss_param().margin();
    bool legacy_version =
      this->param_->contrastive_loss_param().legacy_version();
    for (int i = 0; i < 2; ++i) {
      if (bottom[i]->propagate_down_) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * top[0]->diff<Context>()[0] /
                            static_cast<Dtype>(bottom[i]->num());
        int num = bottom[i]->num();
        int channels = bottom[i]->channels();
        for (int j = 0; j < num; ++j) {
          Dtype* bout = bottom[i]->mutable_diff<Context>();
          if (static_cast<int>(bottom[2]->data<Context>()[j])) {  // similar pairs
            caffe_axpby(
              channels,
              alpha,
              diff_.data<Context>() + (j * channels),
              Dtype(0.0),
              bout + (j * channels));
          } else {  // dissimilar pairs
            Dtype mdist(0.0);
            Dtype beta(0.0);
            if (legacy_version) {
              mdist = margin - dist_sq_.data<Context>()[j];
              beta = -alpha;
            } else {
              Dtype dist = sqrt(dist_sq_.data<Context>()[j]);
              mdist = margin - dist;
              beta = -alpha * mdist / (dist + Dtype(1e-4));
            }
            if (mdist > Dtype(0.0)) {
              caffe_axpby(
                channels,
                beta,
                diff_.data<Context>() + (j * channels),
                Dtype(0.0),
                bout + (j * channels));
            } else {
              caffe_set(channels, Dtype(0), bout + (j * channels));
            }
          }
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ContrastiveLossLayer);
#endif

  INSTANTIATE_CLASS(ContrastiveLossLayer);
  REGISTER_LAYER_CLASS(ContrastiveLoss);

}  // namespace