#ifndef CAFFE_PRELU_LAYER_HPP_
#define CAFFE_PRELU_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Parameterized Rectified Linear Unit non-linearity @f$
   *        y_i = \max(0, x_i) + a_i \min(0, x_i)
   *        @f$. The differences from ReLULayer are 1) negative slopes are
   *        learnable though backprop and 2) negative slopes can vary across
   *        channels. The number of axes of input blob should be greater than or
   *        equal to 2. The 1st axis (0-based) is seen as channels.
   */
  template <typename Dtype>
  class PReLULayer : public NeuronLayer
  {
  public:
    /**
     * @param param provides PReLUParameter prelu_param,
     *     with PReLULayer options:
     *   - filler (\b optional, FillerParameter,
     *     default {'type': constant 'value':0.25}).
     *   - channel_shared (\b optional, default false).
     *     negative slopes are shared across channels.
     */
    explicit PReLULayer()
      : NeuronLayer() {}

    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "PReLU"; }

  public:
    /**
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times ...) @f$
     *      the inputs @f$ x @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (N \times C \times ...) @f$
     *      the computed outputs for each channel @f$i@f$ @f$
     *        y_i = \max(0, x_i) + a_i \min(0, x_i)
     *      @f$.
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the PReLU inputs.
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *      respect to the outputs
     *   -# @f$ (N \times C \times ...) @f$
     *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
     *      with respect to computed outputs @f$ y @f$
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times ...) @f$
     *      the inputs @f$ x @f$; For each channel @f$i@f$, backward fills their
     *      diff with gradients @f$
     *        \frac{\partial E}{\partial x_i} = \left\{
     *        \begin{array}{lr}
     *            a_i \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i \le 0 \\
     *            \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i > 0
     *        \end{array} \right.
     *      @f$.
     *      If blobs_[0]->propagate_down_ is true, it fills the diff with gradients
     *      @f$
     *        \frac{\partial E}{\partial a_i} = \left\{
     *        \begin{array}{lr}
     *            \sum_{x_i} x_i \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i \le 0 \\
     *            0 & \mathrm{if} \; x_i > 0
     *        \end{array} \right.
     *      @f$.
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    bool channel_shared_;
    Blob multiplier_;  // dot multiplier for backward computation of params
    Blob backward_buff_;  // temporary buffer for backward computation
    Blob bottom_memory_;  // memory for in-place computation
  };

}  // namespace

#endif  // CAFFE_PRELU_LAYER_HPP_
