

#include "interp.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Interp);

Interp::Interp()
{
  one_blob_only = true;
}

Interp::~Interp()
{
}

int Interp::load_param(const ParamDict& pd)
{
  resize_type = pd.get(0, 0);
  height_scale = pd.get(1, 1.f);
  width_scale = pd.get(2, 1.f);
  output_height = pd.get(3, 0);
  output_width = pd.get(4, 0);

  return 0;
}

int Interp::forward(const Blob& bottom_blob, Blob& top_blob) const
{
  int h = bottom_blob->h;
  int w = bottom_blob->w;
  int c = Blob_channels(bottom_blob);
  int oh = output_height;
  int ow = output_width;

  if (ow == 0 || ow == 0) {
    oh = h * height_scale;
    ow = w * width_scale;
  }

  if (oh == h && ow == w) {
    top_blob = bottom_blob;
    return 0;
  }

  top_blob->create(ow, oh, c);

  if (top_blob->empty()) {
    return -100;
  }

  if (resize_type == 1) { //nearest
    #pragma omp parallel for
    for (int q = 0; q < c; ++q) {
      const float* ptr = Blob_channel_p(bottom_blob, q);
      float* output_ptr = Blob_channel_p(top_blob, q);

      for (int y = 0; y < oh; ++y) {
        const int in_y = MIN((int)(y / height_scale), (h - 1));

        for (int x = 0; x < ow; ++x) {
          const int in_x = MIN((int)(x / width_scale), (w - 1));
          output_ptr[ow * y + x] = ptr[in_y * w + in_x];
        }
      }
    }

    return 0;

  }
  else if (resize_type == 2) { // bilinear
    resize_bilinear(bottom_blob, top_blob, ow, oh);
    return 0;

  }
  else {
    fprintf(stderr, "unsupported resize type %d %d %d\n", resize_type, oh, ow);
    return -233;
  }
}


} // namespace ncnn
