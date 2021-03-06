#ifndef CAFFE_CONTRASTIVE_LOSS_LAYER_HPP_
#define CAFFE_CONTRASTIVE_LOSS_LAYER_HPP_







#include "caffe/layers/loss_layer.hpp"

namespace
{

  /**
   * @brief Computes the contrastive loss @f$
   *          E = \frac{1}{2N} \sum\limits_{n=1}^N \left(y\right) d^2 +
   *              \left(1-y\right) \max \left(margin-d, 0\right)^2
   *          @f$ where @f$
   *          d = \left| \left| a_n - b_n \right| \right|_2 @f$. This can be
   *          used to train siamese networks.
   *
   * @param bottom input Blob vector (length 3)
   *   -# @f$ (N \times C \times 1 \times 1) @f$
   *      the features @f$ a \in [-\infty, +\infty]@f$
   *   -# @f$ (N \times C \times 1 \times 1) @f$
   *      the features @f$ b \in [-\infty, +\infty]@f$
   *   -# @f$ (N \times 1 \times 1 \times 1) @f$
   *      the binary similarity @f$ s \in [0, 1]@f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
   *      the computed contrastive loss: @f$ E =
   *          \frac{1}{2N} \sum\limits_{n=1}^N \left(y\right) d^2 +
   *          \left(1-y\right) \max \left(margin-d, 0\right)^2
   *          @f$ where @f$
   *          d = \left| \left| a_n - b_n \right| \right|_2 @f$.
   * This can be used to train siamese networks.
   */
  template <typename Dtype>
  class ContrastiveLossLayer : public LossLayer
  {
  public:
    explicit ContrastiveLossLayer()
      : LossLayer(param), diff_() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual inline int ExactNumBottomBlobs() const { return 3; }
    virtual inline const char* type() const { return "ContrastiveLoss"; }
    /**
     * Unlike most loss layers, in the ContrastiveLossLayer we can backpropagate
     * to the first two inputs.
     */
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return bottom_index != 2;
    }

  public:
    /// @copydoc ContrastiveLossLayer
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the Contrastive error gradient w.r.t. the inputs.
     *
     * Computes the gradients with respect to the two input vectors (bottom[0] and
     * bottom[1]), but not the similarity label (bottom[2]).
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *      respect to the outputs
     *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
     *      This Blob's diff will simply contain the loss_weight* @f$ \lambda @f$,
     *      as @f$ \lambda @f$ is the coefficient of this layer's output
     *      @f$\ell_i@f$ in the overall Net loss
     *      @f$ E = \lambda_i \ell_i + \mbox{other loss terms}@f$; hence
     *      @f$ \frac{\partial E}{\partial \ell_i} = \lambda_i @f$.
     *      (*Assuming that this top Blob is not used as a bottom (input) by any
     *      other layer of the Net.)
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length 2)
     *   -# @f$ (N \times C \times 1 \times 1) @f$
     *      the features @f$a@f$; Backward_ fills their diff with
     *      gradients if bottom[0]->propagate_down_
     *   -# @f$ (N \times C \times 1 \times 1) @f$
     *      the features @f$b@f$; Backward_ fills their diff with gradients if
     *      bottom[1]->propagate_down_
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    Blob diff_;  // cached for backward pass
    Blob dist_sq_;  // cached for backward pass
    Blob diff_sq_;  // tmp storage for gpu forward pass
    Blob summer_vec_;  // tmp storage for gpu forward pass
  };

}  // namespace

#endif  // CAFFE_CONTRASTIVE_LOSS_LAYER_HPP_
