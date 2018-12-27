#ifndef CAFFE_XXX_LAYER_HPP_
#define CAFFE_XXX_LAYER_HPP_

#define ReshapeParameter_DEF(DEF) \
DEF##Int(axis, 0, 0) \
DEF##Int(num_axes, -1, 0) \

class ReshapeLayer : public Layer
{
public:
  ReshapeParameter_DEF(Def);
  /// @brief vector of axes indices whose dimensions we'll copy from the bottom
  vector<int> copy_axes_;
  vector<int> shape_;
  /// @brief the index of the axis whose dimension we infer, or -1 if none
  int inferred_axis_;
  /// @brief the product of the "constant" output dimensions
  int constant_count_;
  virtual inline const char* type() const { return "Reshape"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

public:
  ReshapeLayer() {
    ReshapeParameter_DEF(Init);
  }
  virtual void init() {
    ReshapeParameter_DEF(Init);
  }
  virtual void fromJson(cjson* param) {
    ReshapeParameter_DEF(Get);
    cjson_GetObjectNumberArray(param, "shape", shape_);
  }
  virtual void toJson(cjson* param) {
    ReshapeParameter_DEF(Set);
    cjson_AddNumberArrayToObject(param, "shape", shape_);
  }


  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
      "allow in-place computation.";
    inferred_axis_ = -1;
    copy_axes_.clear();
    const vector<int>& top_blob_shape = shape_;
    const int top_num_axes = (int)top_blob_shape.size();
    constant_count_ = 1;
    for (int i = 0; i < top_num_axes; ++i) {
      const int top_dim = top_blob_shape[i];
      if (top_dim == 0) {
        copy_axes_.push_back(i);
      }
      else if (top_dim == -1) {
        CHECK_EQ(inferred_axis_, -1) << "new shape contains multiple "
          << "-1 dims; at most a single (1) value of -1 may be specified";
        inferred_axis_ = i;
      }
      else {
        constant_count_ *= top_dim;
      }
    }
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const int input_start_axis = axis_;
    const int start_axis = (input_start_axis >= 0) ? input_start_axis :
      bottom[0]->num_axes() + input_start_axis + 1;
    CHECK_GE(start_axis, 0) << "axis " << input_start_axis << " out of range";
    CHECK_LE(start_axis, bottom[0]->num_axes()) << "axis " << input_start_axis
      << " out of range for " << bottom[0]->num_axes() << "-D input blob";
    const int num_axes = num_axes_;
    CHECK_GE(num_axes, -1) << "num_axes must be >= 0, or -1 for all";
    const int end_axis =
      (num_axes == -1) ? bottom[0]->num_axes() : (start_axis + num_axes);
    CHECK_LE(end_axis, bottom[0]->num_axes())
      << "end_axis = axis + num_axes is out of range";
    const int num_axes_replaced = end_axis - start_axis;
    const int num_axes_retained = bottom[0]->num_axes() - num_axes_replaced;
    const vector<int>& top_blob_shape = shape_;
    const int num_new_axes = (int)top_blob_shape.size();
    vector<int> top_shape(num_axes_retained + num_new_axes);
    int top_shape_index = 0;
    for (int i = 0; i < start_axis; ++i) {
      top_shape[top_shape_index++] = bottom[0]->shape(i);
    }
    for (int i = 0; i < num_new_axes; ++i) {
      top_shape[top_shape_index++] = top_blob_shape[i];
    }
    for (int i = end_axis; i < bottom[0]->num_axes(); ++i) {
      top_shape[top_shape_index++] = bottom[0]->shape(i);
    }
    CHECK_EQ(top_shape_index, top_shape.size());
    for (int i = 0; i < copy_axes_.size(); ++i) {
      const int copy_axis_index = copy_axes_[i];
      CHECK_GT(bottom[0]->num_axes(), start_axis + copy_axis_index)
        << "new shape contains a 0, but there was no corresponding bottom axis "
        << "to copy";
      top_shape[start_axis + copy_axis_index] =
        bottom[0]->shape(start_axis + copy_axis_index);
    }
    if (inferred_axis_ >= 0) {
      // A -1 dim was specified; infer the correct dimension by computing the
      // product of the other dimensions.
      int explicit_count = constant_count_;
      explicit_count *= bottom[0]->count(0, start_axis);
      explicit_count *= bottom[0]->count(end_axis);
      for (int i = 0; i < copy_axes_.size(); ++i) {
        const int copy_axis_index = copy_axes_[i];
        explicit_count *= top_shape[start_axis + copy_axis_index];
      }
      CHECK_EQ(0, bottom[0]->count() % explicit_count) << "bottom count ("
        << bottom[0]->count() << ") must be divisible by the product of "
        << "the specified dimensions (" << explicit_count << ")";
      const int inferred_dim = bottom[0]->count() / explicit_count;
      top_shape[start_axis + inferred_axis_] = inferred_dim;
    }
    top[0]->Reshape(top_shape);
    CHECK_EQ(top[0]->count(), bottom[0]->count())
      << "output count must match input count";
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    top[0]->ShareData(bottom[0]);
    top[0]->ShareDiff(bottom[0]);
  }
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
    bottom[0]->ShareData(top[0]);
    bottom[0]->ShareDiff(top[0]);
  }
};


INSTANTIATE_CLASS(Reshape, ReshapeLayer);

#endif  // CAFFE_XXX_LAYER_HPP_
