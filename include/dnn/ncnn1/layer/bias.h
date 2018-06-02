

#ifndef LAYER_BIAS_H
#define LAYER_BIAS_H

#include "layer.h"

namespace ncnn
{

struct Bias : public Layer
{
public:
  Bias();
  virtual ~Bias();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
  // param
  int bias_data_size;

  // model
  img_t bias_data;
};

} // namespace ncnn

#endif // LAYER_BIAS_H
