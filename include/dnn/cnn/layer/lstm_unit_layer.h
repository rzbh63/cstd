



#include "caffe/layer.hpp"
#include "lstm_layer.hpp"




inline Dtype sigmoid(Dtype x) {
  return 1. / (1. + exp(-x));
}


inline Dtype tanh(Dtype x) {
  return 2. * sigmoid(2. * x) - 1.;
}


void LSTMUnitLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int num_instances = prevSize.c;
  for (i = 0; i < layer->i_size; ++i) {
    if (i == 2) {
      CHECK_EQ(2, IBLOB(i)->num_axes());
    } else {
      CHECK_EQ(3, IBLOB(i)->num_axes());
    }
    CHECK_EQ(1, IBLOB(i)->shape(0));
    CHECK_EQ(num_instances, IBLOB(i)->shape(1));
  }
  hidden_dim_ = bottom[0]->shape(2);
  CHECK_EQ(4 * hidden_dim_, bottom[1]->shape(2));
  top[0]->ReshapeLike(*bottom[0]);
  top[1]->ReshapeLike(*bottom[0]);
  X_acts_.ReshapeLike(*bottom[1]);
}


void LSTMUnitLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int num = prevSize.c;
  const int x_dim = hidden_dim_ * 4;
  const Dtype* C_prev = prevData;
  const Dtype* X = nextData;
  const Dtype* cont = getData(prev2);
  Dtype* C = nextData;
  Dtype* H = top[1]->mutable_data();
  for (n = 0; n < num; ++n) {
    for (d = 0; d < hidden_dim_; ++d) {
      const Dtype i = sigmoid(X[d]);
      const Dtype f = (*cont == 0) ? 0 :
          (*cont * sigmoid(X[1 * hidden_dim_ + d]));
      const Dtype o = sigmoid(X[2 * hidden_dim_ + d]);
      const Dtype g = tanh(X[3 * hidden_dim_ + d]);
      const Dtype c_prev = C_prev[d];
      const Dtype c = f * c_prev + i * g;
      C[d] = c;
      const Dtype tanh_c = tanh(c);
      H[d] = o * tanh_c;
    }
    C_prev += hidden_dim_;
    X += x_dim;
    C += hidden_dim_;
    H += hidden_dim_;
    ++cont;
  }
}


void LSTMUnitLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  CHECK(!propagate_down[2]) << "Cannot backpropagate to sequence indicators.";
  if (!propagate_down[0] && !propagate_down[1]) { return; }

  const int num = prevSize.c;
  const int x_dim = hidden_dim_ * 4;
  const Dtype* C_prev = prevData;
  const Dtype* X = nextData;
  const Dtype* cont = getData(prev2);
  const Dtype* C = top[0]->cpu_data();
  const Dtype* H = top[1]->cpu_data();
  const Dtype* C_diff = nextDiff;
  const Dtype* H_diff = top[1]->cpu_diff();
  Dtype* C_prev_diff = bottom[0]->mutable_diff();
  Dtype* X_diff = bottom[1]->mutable_diff();
  for (n = 0; n < num; ++n) {
    for (d = 0; d < hidden_dim_; ++d) {
      const Dtype i = sigmoid(X[d]);
      const Dtype f = (*cont == 0) ? 0 :
          (*cont * sigmoid(X[1 * hidden_dim_ + d]));
      const Dtype o = sigmoid(X[2 * hidden_dim_ + d]);
      const Dtype g = tanh(X[3 * hidden_dim_ + d]);
      const Dtype c_prev = C_prev[d];
      const Dtype c = C[d];
      const Dtype tanh_c = tanh(c);
      Dtype* c_prev_diff = C_prev_diff + d;
      Dtype* i_diff = X_diff + d;
      Dtype* f_diff = X_diff + 1 * hidden_dim_ + d;
      Dtype* o_diff = X_diff + 2 * hidden_dim_ + d;
      Dtype* g_diff = X_diff + 3 * hidden_dim_ + d;
      const Dtype c_term_diff =
          C_diff[d] + H_diff[d] * o * (1 - tanh_c * tanh_c);
      *c_prev_diff = c_term_diff * f;
      *i_diff = c_term_diff * g * i * (1 - i);
      *f_diff = c_term_diff * c_prev * f * (1 - f);
      *o_diff = H_diff[d] * tanh_c * o * (1 - o);
      *g_diff = c_term_diff * i * (1 - g * g);
    }
    C_prev += hidden_dim_;
    X += x_dim;
    C += hidden_dim_;
    H += hidden_dim_;
    C_diff += hidden_dim_;
    H_diff += hidden_dim_;
    X_diff += x_dim;
    C_prev_diff += hidden_dim_;
    ++cont;
  }
}

#ifdef CPU_ONLY
STUB_GPU(LSTMUnitLayer);
#endif

INSTANTIATE_CLASS(LSTMUnitLayer);
REGISTER_LAYER_CLASS(LSTMUnit);


