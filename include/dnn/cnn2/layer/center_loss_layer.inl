

#include "caffe/filler.hpp"
#include "center_loss_layer.hpp"




  
  void CenterLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                          const vector<Blob*>& top) {
    const int num_output = this->layer_param_.center_loss_param().num_output();
    distance_type_ = this->layer_param_.center_loss_param().distance_type();
    N_ = num_output;
    const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.center_loss_param().axis());
    // Dimensions starting from "axis" are "flattened" into a single
    // length K_ vector. For example, if bottom[0]'s shape is (N, C, H, W),
    // and axis == 1, N inner products with dimension CHW are performed.
    K_ = bottom[0]->count(axis);
    // Check if we need to set up the weights
    if (this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else {
      this->blobs_.resize(1);
      // Intialize the weight
      vector<int> center_shape(2);
      center_shape[0] = N_;
      center_shape[1] = K_;
      this->blobs_[0].reset(new Blob(center_shape));
      // fill the weights
      shared_ptr<Filler > center_filler(GetFiller(
        this->layer_param_.center_loss_param().center_filler()));
      center_filler->Fill(this->blobs_[0].get());
    }  // parameter initialization
    this->param_propagate_down_.resize(this->blobs_.size(), true);
  }

  
  void CenterLossLayer::Reshape(const vector<Blob*>& bottom,
                                       const vector<Blob*>& top) {
    CHECK_EQ(bottom[1]->channels(), 1);
    CHECK_EQ(bottom[1]->height(), 1);
    CHECK_EQ(bottom[1]->width(), 1);
    M_ = bottom[0]->num();
    // The top shape will be the bottom shape with the flattened axes dropped,
    // and replaced by a single axis with dimension num_output (N_).
    LossLayer::Reshape(bottom, top);
    distance_.ReshapeLike(*bottom[0]);
    variation_sum_.ReshapeLike(*this->blobs_[0]);
    vector<int> count_shape(1);
    count_shape[0] = N_;
    count_.Reshape(count_shape);
  }

  
  void CenterLossLayer::Forward_cpu(const vector<Blob*>& bottom,
                                           const vector<Blob*>& top) {
    const Dtype* bottom_data = Blob_datap(bottom_blob->data);
    const Dtype* label = Blob_datap(top_blob->data);
    const Dtype* center = this->blobs_[0]->cpu_data();
    Dtype* distance_data = distance_.mutable_cpu_data();
    // the i-th distance_data
    for (int i = 0; i < M_; i++) {
      const int label_value = static_cast<int>(label[i]);
      // D(i,:) = X(i,:) - C(y(i),:)
      caffe_sub(K_, bottom_data + i * K_,
                center + label_value * K_, distance_data + i * K_);
    }
    Dtype dot;
    Dtype loss;
    if (distance_type_ == "L1") {
      dot = caffe_cpu_asum(M_ * K_, distance_.cpu_data());
      loss = dot / M_;
    }
    else if (distance_type_ == "L2") {
      dot = caffe_cpu_dot(M_ * K_, distance_.cpu_data(),
                          distance_.cpu_data());
      loss = dot / M_ / Dtype(2);
    }
    else {
      LOG(FATAL) << "distance_type must be L1 or L2!";
    }
    top[0]->mutable_cpu_data()[0] = loss;
  }

  
  void CenterLossLayer::Backward_cpu(const vector<Blob*>& top,
                                            const vector<bool>& propagate_down,
                                            const vector<Blob*>& bottom) {
    // Gradient with respect to centers
    if (this->param_propagate_down_[0]) {
      const Dtype* label = Blob_datap(top_blob->data);
      Dtype* center_diff = this->blobs_[0]->mutable_cpu_diff();
      Dtype* variation_sum_data = variation_sum_.mutable_cpu_data();
      int* count_data = count_.mutable_cpu_data();

      const Dtype* distance_data = distance_.cpu_data();

      if (distance_type_ == "L1") {
        caffe_cpu_sign(M_ * K_, distance_data, distance_.mutable_cpu_data());
      }

      // \sum_{y_i==j}
      caffe_set(N_ * K_, (Dtype)0., variation_sum_.mutable_cpu_data());
      caffe_set(N_, 0, count_.mutable_cpu_data());

      for (int m = 0; m < M_; m++) {
        const int label_value = static_cast<int>(label[m]);
        caffe_sub(K_, variation_sum_data + label_value * K_,
                  distance_data + m * K_, variation_sum_data + label_value * K_);
        count_data[label_value]++;
      }
      for (int m = 0; m < M_; m++) {
        const int n = static_cast<int>(label[m]);
        caffe_cpu_axpby(K_, (Dtype)1. / (count_data[n] + (Dtype)1.),
                        variation_sum_data + n * K_,
                        (Dtype)1., center_diff + n * K_);
      }
    }
    // Gradient with respect to bottom data
    if (propagate_down[0]) {
      caffe_copy(M_ * K_, distance_.cpu_data(),
                 bottom[0]->mutable_cpu_diff());
      caffe_scal(M_ * K_, top[0]->cpu_diff()[0] / M_,
                 bottom[0]->mutable_cpu_diff());
    }
    if (propagate_down[1]) {
      LOG(FATAL) << this->type()
        << " Layer cannot backpropagate to label inputs.";
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(CenterLossLayer);
#endif

  INSTANTIATE_CLASS(CenterLossLayer);
  REGISTER_LAYER_CLASS(CenterLoss);

