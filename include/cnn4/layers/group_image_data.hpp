#ifndef CAFFE_GROUP_IMAGE_DATA_LAYER_HPP_
#define CAFFE_GROUP_IMAGE_DATA_LAYER_HPP_

#include <string>
#include <utility>



#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"

#include "caffe/layers/base_data_layer.hpp"


namespace
{

  /**
   * @brief Provides data to the Net from image files.
   *
   * TODO(dox): thorough documentation for Forward_ and proto params.
   */
  template <typename Dtype>
  class GroupImageDataLayer : public BasePrefetchingDataLayer
  {
  public:
    explicit GroupImageDataLayer()
      : BasePrefetchingDataLayer() {}
    virtual ~GroupImageDataLayer();
    virtual void DataLayerSetUp(const vector<Blob*> & bottom,
                                const vector<Blob*> & top);

    virtual inline const char* type() const { return "GroupImageData"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int ExactNumTopBlobs() const { return 2; }

  public:
    //SHARED_PTR<RNG> prefetch_rng_;
    virtual void ShuffleImages();
    virtual void load_batch(Batch<Dtype>* batch);

    vector<std::vector<std::string> > lines_;
    int lines_id_;
  };


}  // namespace

#endif  // CAFFE_GROUP_IMAGE_DATA_LAYER_HPP_
