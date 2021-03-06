#ifndef CAFFE_CONCAT_LAYER_HPP_
#define CAFFE_CONCAT_LAYER_HPP_







namespace
{

  /**
   * @brief Takes at least two Blob%s and concatenates them along either the num
   *        or channel dimension, outputting the result.
   */
  template <typename Dtype>
  class ConcatLayer : public Layer
  {
  public:
    explicit ConcatLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Concat"; }
    virtual inline int MinBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    /**
     * @param bottom input Blob vector (length 2+)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x_1 @f$
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x_2 @f$
     *   -# ...
     *   - K @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x_K @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (KN \times C \times H \times W) @f$ if axis == 0, or
     *      @f$ (N \times KC \times H \times W) @f$ if axis == 1:
     *      the concatenated output @f$
     *        y = [\begin{array}{cccc} x_1 & x_2 & ... & x_K \end{array}]
     *      @f$
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the concatenate inputs.
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *        respect to the outputs
     *   -# @f$ (KN \times C \times H \times W) @f$ if axis == 0, or
     *      @f$ (N \times KC \times H \times W) @f$ if axis == 1:
     *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
     *      with respect to concatenated outputs @f$ y @f$
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length K), into which the top gradient
     *        @f$ \frac{\partial E}{\partial y} @f$ is deconcatenated back to the
     *        inputs @f$
     *        \left[ \begin{array}{cccc}
     *          \frac{\partial E}{\partial x_1} &
     *          \frac{\partial E}{\partial x_2} &
     *          ... &
     *          \frac{\partial E}{\partial x_K}
     *        \end{array} \right] =
     *        \frac{\partial E}{\partial y}
     *        @f$
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    int count_;
    int num_concats_;
    int concat_input_size_;
    int concat_axis_;
  };

}  // namespace

#endif  // CAFFE_CONCAT_LAYER_HPP_
