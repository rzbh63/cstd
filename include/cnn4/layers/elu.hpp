#ifndef CAFFE_ELU_LAYER_HPP_
#define CAFFE_ELU_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Exponential Linear Unit non-linearity @f$
   *        y = \left\{
   *        \begin{array}{lr}
   *            x                  & \mathrm{if} \; x > 0 \\
   *            \alpha (\exp(x)-1) & \mathrm{if} \; x \le 0
   *        \end{array} \right.
   *      @f$.
   */
  template <typename Dtype>
  class ELULayer : public NeuronLayer
  {
  public:
    /**
     * @param param provides ELUParameter elu_param,
     *     with ELULayer options:
     *   - alpha (\b optional, default 1).
     *     the value @f$ \alpha @f$ by which controls saturation for negative inputs.
     */
    explicit ELULayer()
      : NeuronLayer() {}

    virtual inline const char* type() const { return "ELU"; }

  public:
    /**
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the computed outputs @f$
     *        y = \left\{
     *        \begin{array}{lr}
     *            x                  & \mathrm{if} \; x > 0 \\
     *            \alpha (\exp(x)-1) & \mathrm{if} \; x \le 0
     *        \end{array} \right.
     *      @f$.
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the ELU inputs.
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *      respect to the outputs
     *   -# @f$ (N \times C \times H \times W) @f$
     *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
     *      with respect to computed outputs @f$ y @f$
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$; Backward_ fills their diff with
     *      gradients @f$
     *        \frac{\partial E}{\partial x} = \left\{
     *        \begin{array}{lr}
     *            1           & \mathrm{if} \; x > 0 \\
     *            y + \alpha  & \mathrm{if} \; x \le 0
     *        \end{array} \right.
     *      @f$ if bottom[0]->propagate_down_.
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  };


}  // namespace

#endif  // CAFFE_ELU_LAYER_HPP_
