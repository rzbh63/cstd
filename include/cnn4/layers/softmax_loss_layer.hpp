#ifndef CAFFE_SOFTMAX_WITH_LOSS_LAYER_HPP_
#define CAFFE_SOFTMAX_WITH_LOSS_LAYER_HPP_

template <typename Dtype>
class SoftmaxWithLossLayer : public LossLayer<Dtype>
{
public:
  /// The internal SoftmaxLayer used to map predictions to a distribution.
  Layer<Dtype>* softmax_layer_;
  /// prob stores the output probability predictions from the SoftmaxLayer.
  Blob<Dtype> prob_;
  /// bottom vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob<Dtype>*> softmax_bottom_vec_;
  /// top vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob<Dtype>*> softmax_top_vec_;
  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  /// How to normalize the output loss.
  NormalizationMode normalization_;

  int softmax_axis_, outer_num_, inner_num_, label_num_;

  virtual inline const char* type() const { return "SoftmaxWithLoss"; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

  virtual void LayerSetUp( const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::LayerSetUp(bottom, top);
    //LayerParameter softmax_param(this->layer_param_);
    //softmax_param.set_type("Softmax");
    softmax_layer_ = new SoftmaxLayer<Dtype>();
    softmax_layer_->init();
    softmax_bottom_vec_.clear();
    softmax_bottom_vec_.push_back(bottom[0]);
    softmax_top_vec_.clear();
    softmax_top_vec_.push_back(&prob_);
    softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);
    has_ignore_label_ = this->param_->has("ignore_label");
    if (has_ignore_label_) {
      ignore_label_ = this->param_->getint("ignore_label", 0);
    }
    if (!this->param_->has("normalization") && this->param_->has("normalize")) {
      normalization_ = this->param_->getbool("normalize", true) ?
        NormalizationMode_VALID :
        NormalizationMode_BATCH_SIZE;
    }
    else {
      normalization_ = param_->getenum("normalization", NormalizationMode_VALID, NormalizationMode_Name, countof(NormalizationMode_Name));
    }
  }

  int axis_;
  virtual void Reshape(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::Reshape(bottom, top);
    softmax_layer_->Reshape(softmax_bottom_vec_, softmax_top_vec_);
    axis_  = this->param_->getint("axis_", 1);
    softmax_axis_ = bottom[0]->CanonicalAxisIndex(axis_);
    outer_num_ = bottom[0]->count(0, softmax_axis_);
    inner_num_ = bottom[0]->count(softmax_axis_ + 1);
    CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
      << "Number of labels must match number of predictions; "
      << "e.g., if softmax axis == 1 and prediction shape is (N, C, H, W), "
      << "label count (number of labels) must be N*H*W, "
      << "with integer values in {0, 1, ..., C-1}.";
    if (top.size() >= 2) {
      // softmax output
      top[1]->ReshapeLike(*bottom[0]);
    }
  }

  int get_normalizer(NormalizationMode normalization_mode, int valid_count)
  {
    int normalizer;

    switch (normalization_mode) {
    case NormalizationMode_FULL:
      normalizer = int(outer_num_ * inner_num_);
      break;
    case NormalizationMode_VALID:
      if (valid_count == -1) {
        normalizer = int(outer_num_ * inner_num_);
      }
      else {
        normalizer = int(valid_count);
      }
      break;
    case NormalizationMode_BATCH_SIZE:
      normalizer = int(outer_num_);
      break;
    case NormalizationMode_NONE:
      normalizer = int(1);
      break;
    default:
      LOG(FATAL) << "Unknown normalization mode: "
        << NormalizationMode_Name[normalization_mode];
    }
    return std::max(int(1), normalizer);
  }
  virtual void Forward(Context* context,const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) {
    // The forward pass computes the softmax prob values.
    int prob_count = prob_.count();
    softmax_layer_->Forward(context, softmax_bottom_vec_, softmax_top_vec_);
    const Dtype* prob_data = prob_.data<Context>();
    const Dtype* label = bottom[1]->data<Context>();
    const int dim = prob_count / outer_num_;
    Dtype* top_data = top[0]->mutable_data<CPUContext>();
    Dtype loss = 0;
    int valid_count = softmaxloss_forward(context, prob_data, label, outer_num_, dim, inner_num_, has_ignore_label_, ignore_label_, &loss);
    top_data[0] = loss / get_normalizer(normalization_, valid_count);
    if (top.size() == 2) {
      top[1]->ShareData(prob_);
    }
  }

  
  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top, const vector<Blob<Dtype>*> & bottom)
  {
    if (bottom[1]->propagate_down_) {
      //LOG(FATAL) << this->type() << " Layer cannot backpropagate to label inputs.";
    }
    if (bottom[0]->propagate_down_) {
      int prob_count = prob_.count();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      const Dtype* prob_data = prob_.data<Context>();
      const Dtype* top_data = top[0]->data<Context>();
      caffe_memcpy(context, prob_count * sizeof(Dtype), prob_data, bottom_diff);
      const Dtype* label = bottom[1]->data<Context>();
      const int dim = prob_count / outer_num_;
      // Since this memory is never used for anything else,
      // we use to to avoid allocating new GPU memory.
      // NOLINT_NEXT_LINE(whitespace/operators)

      int valid_count = softmaxloss_backward(context, top_data, label, bottom_diff,
        outer_num_, dim, inner_num_, has_ignore_label_, ignore_label_);

      const Dtype* top_diff = top[0]->diff<CPUContext>();
      // Only launch another CUDA kernel if we actually need the count of valid outputs.
      const Dtype loss_weight = top_diff[0] / get_normalizer(normalization_, valid_count);
      caffe_scal(context, prob_count, loss_weight, bottom_diff);

    }
  }
};

INSTANTIATE_CLASS(SoftmaxWithLoss);


#endif  // CAFFE_SOFTMAX_WITH_LOSS_LAYER_HPP_