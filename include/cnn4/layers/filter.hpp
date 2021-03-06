#ifndef CAFFE_FILTER_LAYER_HPP_
#define CAFFE_FILTER_LAYER_HPP_







namespace
{

  /**
   * @brief Takes two+ Blobs, interprets last Blob as a selector and
   *  filter remaining Blobs accordingly with selector data (0 means that
   * the corresponding item has to be filtered, non-zero means that corresponding
   * item needs to stay).
   */
  template <typename Dtype>
  class FilterLayer : public Layer
  {
  public:
    explicit FilterLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Filter"; }
    virtual inline int MinBottomBlobs() const { return 2; }
    virtual inline int MinTopBlobs() const { return 1; }

  public:
    /**
     * @param bottom input Blob vector (length 2+)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs to be filtered @f$ x_1 @f$
     *   -# ...
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs to be filtered @f$ x_K @f$
     *   -# @f$ (N \times 1 \times 1 \times 1) @f$
     *      the selector blob
     * @param top output Blob vector (length 1+)
     *   -# @f$ (S \times C \times H \times W) @f$ ()
     *        the filtered output @f$ x_1 @f$
     *        where S is the number of items
     *        that haven't been filtered
     *      @f$ (S \times C \times H \times W) @f$
     *        the filtered output @f$ x_K @f$
     *        where S is the number of items
     *        that haven't been filtered
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the forwarded inputs.
     *
     * @param top output Blob vector (length 1+), providing the error gradient with
     *        respect to the outputs
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length 2+), into which the top error
     *        gradient is copied
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    bool first_reshape_;
    vector<int> indices_to_forward_;
  };

}  // namespace

#endif  // CAFFE_FILTER_LAYER_HPP_
