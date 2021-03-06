#include <algorithm>


#include "caffe/layers/contrastive_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void ContrastiveLossLayer::LayerSetUp(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::LayerSetUp(bottom, top);
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
      summer_vec_.mdata()[i] = Dtype(1);
    }
  }

  template <typename Dtype>
  void ContrastiveLossLayer::Forward_(_CONTEXT,
    const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    int count = bottom[0]->count();
    caffe_sub(
      count,
      bottom[0]->data(),  // a
      bottom[1]->data(),  // b
      diff_.mdata());  // a_i-b_i
    const int channels = bottom[0]->channels();
    Dtype margin = this->param_->contrastive_loss_param().margin();
    bool legacy_version =
      this->param_->contrastive_loss_param().legacy_version();
    Dtype loss(0.0);
    for (int i = 0; i < bottom[0]->num(); ++i) {
      dist_sq_.mdata()[i] = caffe_dot(channels,
                                       diff_.data() + (i * channels), diff_.data() + (i * channels));
      if (static_cast<int>(bottom[2]->data()[i])) {  // similar pairs
        loss += dist_sq_.data()[i];
      } else {  // dissimilar pairs
        if (legacy_version) {
          loss += std::max(margin - dist_sq_.data()[i], Dtype(0.0));
        } else {
          Dtype dist = std::max<Dtype>(margin - sqrt(dist_sq_.data()[i]),
                                       Dtype(0.0));
          loss += dist * dist;
        }
      }
    }
    loss = loss / static_cast<Dtype>(bottom[0]->num()) / Dtype(2);
    top[0]->mdata()[0] = loss;
  }

  template <typename Dtype>
  void ContrastiveLossLayer::Backward_(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom)
  {
    Dtype margin = this->param_->contrastive_loss_param().margin();
    bool legacy_version =
      this->param_->contrastive_loss_param().legacy_version();
    for (int i = 0; i < 2; ++i) {
      if (bottom[i]->propagate_down_) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * top[0]->diff()[0] /
                            static_cast<Dtype>(bottom[i]->num());
        int num = bottom[i]->num();
        int channels = bottom[i]->channels();
        for (int j = 0; j < num; ++j) {
          Dtype* bout = bottom[i]->mdiff();
          if (static_cast<int>(bottom[2]->data()[j])) {  // similar pairs
            caffe_axpby(
              channels,
              alpha,
              diff_.data() + (j * channels),
              Dtype(0.0),
              bout + (j * channels));
          } else {  // dissimilar pairs
            Dtype mdist(0.0);
            Dtype beta(0.0);
            if (legacy_version) {
              mdist = margin - dist_sq_.data()[j];
              beta = -alpha;
            } else {
              Dtype dist = sqrt(dist_sq_.data()[j]);
              mdist = margin - dist;
              beta = -alpha * mdist / (dist + Dtype(1e-4));
            }
            if (mdist > Dtype(0.0)) {
              caffe_axpby(
                channels,
                beta,
                diff_.data() + (j * channels),
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
