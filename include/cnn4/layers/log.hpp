#ifndef CAFFE_LOG_LAYER_HPP_
#define CAFFE_LOG_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Computes @f$ y = log_{\gamma}(\alpha x + \beta) @f$,
   *        as specified by the scale @f$ \alpha @f$, shift @f$ \beta @f$,
   *        and base @f$ \gamma @f$.
   */
  template <typename Dtype>
  class LogLayer : public NeuronLayer
  {
  public:
    /**
     * @param param provides LogParameter log_param,
     *     with LogLayer options:
     *   - scale (\b optional, default 1) the scale @f$ \alpha @f$
     *   - shift (\b optional, default 0) the shift @f$ \beta @f$
     *   - base (\b optional, default -1 for a value of @f$ e \approx 2.718 @f$)
     *         the base @f$ \gamma @f$
     */
    explicit LogLayer()
      : NeuronLayer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual inline const char* type() const { return "Log"; }

  public:
    /**
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the computed outputs @f$
     *        y = log_{\gamma}(\alpha x + \beta)
     *      @f$
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the exp inputs.
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
     *        \frac{\partial E}{\partial x} =
     *            \frac{\partial E}{\partial y} y \alpha \log_e(gamma)
     *      @f$ if bottom[0]->propagate_down_
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    Dtype base_scale_;
    Dtype input_scale_, input_shift_;
    Dtype backward_num_scale_;
  };

}  // namespace

#endif  // CAFFE_LOG_LAYER_HPP_
