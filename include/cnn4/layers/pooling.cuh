
__global__ void FUN(MaxPoolForward)(const int nthreads,
    const Dtype* const bottom_data, const int num, const int channels,
    const int height, const int width, const int pooled_height,
    const int pooled_width, const int kernel_h, const int kernel_w,
    const int stride_h, const int stride_w, const int pad_h, const int pad_w,
    Dtype* const top_data, int* mask, Dtype* top_mask) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int pw = index % pooled_width;
    const int ph = (index / pooled_width) % pooled_height;
    const int c = (index / pooled_width / pooled_height) % channels;
    const int n = index / pooled_width / pooled_height / channels;
    int hstart = ph * stride_h - pad_h;
    int wstart = pw * stride_w - pad_w;
    const int hend = min(hstart + kernel_h, height);
    const int wend = min(wstart + kernel_w, width);
    hstart = max(hstart, 0);
    wstart = max(wstart, 0);
    Dtype maxval = -FLT_MAX;
    int maxidx = -1;
    const Dtype* const bottom_slice =
        bottom_data + (n * channels + c) * height * width;
    for (int h = hstart; h < hend; ++h) {
      for (int w = wstart; w < wend; ++w) {
        if (bottom_slice[h * width + w] > maxval) {
          maxidx = h * width + w;
          maxval = bottom_slice[maxidx];
        }
      }
    }
    top_data[index] = maxval;
    if (mask) {
      mask[index] = maxidx;
    } else {
      top_mask[index] = maxidx;
    }
  }
}

__global__ void FUN(AvePoolForward)(const int nthreads,
    const Dtype* const bottom_data, const int num, const int channels,
    const int height, const int width, const int pooled_height,
    const int pooled_width, const int kernel_h, const int kernel_w,
    const int stride_h, const int stride_w, const int pad_h, const int pad_w,
    Dtype* const top_data) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int pw = index % pooled_width;
    const int ph = (index / pooled_width) % pooled_height;
    const int c = (index / pooled_width / pooled_height) % channels;
    const int n = index / pooled_width / pooled_height / channels;
    int hstart = ph * stride_h - pad_h;
    int wstart = pw * stride_w - pad_w;
    int hend = min(hstart + kernel_h, height + pad_h);
    int wend = min(wstart + kernel_w, width + pad_w);
    const int pool_size = (hend - hstart) * (wend - wstart);
    hstart = max(hstart, 0);
    wstart = max(wstart, 0);
    hend = min(hend, height);
    wend = min(wend, width);
    Dtype aveval = 0;
    const Dtype* const bottom_slice =
        bottom_data + (n * channels + c) * height * width;
    for (int h = hstart; h < hend; ++h) {
      for (int w = wstart; w < wend; ++w) {
        aveval += bottom_slice[h * width + w];
      }
    }
    top_data[index] = aveval / pool_size;
  }
}

__global__ void FUN(StoPoolForwardTrain)(const int nthreads,
    const Dtype* const bottom_data,
    const int num, const int channels, const int height,
    const int width, const int pooled_height, const int pooled_width,
    const int kernel_h, const int kernel_w, const int stride_h,
    const int stride_w, Dtype* const rand_idx, Dtype* const top_data) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int pw = index % pooled_width;
    const int ph = (index / pooled_width) % pooled_height;
    const int c = (index / pooled_width / pooled_height) % channels;
    const int n = index / pooled_width / pooled_height / channels;
    const int hstart = ph * stride_h;
    const int hend = min(hstart + kernel_h, height);
    const int wstart = pw * stride_w;
    const int wend = min(wstart + kernel_w, width);
    Dtype cumsum = 0.;
    const Dtype* const bottom_slice =
        bottom_data + (n * channels + c) * height * width;
    // First pass: get sum
    for (int h = hstart; h < hend; ++h) {
      for (int w = wstart; w < wend; ++w) {
        cumsum += bottom_slice[h * width + w];
      }
    }
    const float thres = rand_idx[index] * cumsum;
    // Second pass: get value, and set index.
    cumsum = 0;
    for (int h = hstart; h < hend; ++h) {
      for (int w = wstart; w < wend; ++w) {
        cumsum += bottom_slice[h * width + w];
        if (cumsum >= thres) {
          rand_idx[index] = ((n * channels + c) * height + h) * width + w;
          top_data[index] = bottom_slice[h * width + w];
          return;
        }
      }
    }
  }
}

__global__ void FUN(StoPoolForwardTest)(const int nthreads,
    const Dtype* const bottom_data,
    const int num, const int channels, const int height,
    const int width, const int pooled_height, const int pooled_width,
    const int kernel_h, const int kernel_w, const int stride_h,
    const int stride_w, Dtype* const top_data) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int pw = index % pooled_width;
    const int ph = (index / pooled_width) % pooled_height;
    const int c = (index / pooled_width / pooled_height) % channels;
    const int n = index / pooled_width / pooled_height / channels;
    const int hstart = ph * stride_h;
    const int hend = min(hstart + kernel_h, height);
    const int wstart = pw * stride_w;
    const int wend = min(wstart + kernel_w, width);
    // We set cumsum to be 0 to avoid divide-by-zero problems
    Dtype cumsum = FLT_MIN;
    Dtype cumvalues = 0.;
    const Dtype* const bottom_slice =
        bottom_data + (n * channels + c) * height * width;
    // First pass: get sum
    for (int h = hstart; h < hend; ++h) {
      for (int w = wstart; w < wend; ++w) {
        cumsum += bottom_slice[h * width + w];
        cumvalues += bottom_slice[h * width + w] * bottom_slice[h * width + w];
      }
    }
    top_data[index] = cumvalues / cumsum;
  }
}

__global__ void FUN(MaxPoolBackward)(const int nthreads, const Dtype* const top_diff,
    const int* const mask, const Dtype* const top_mask, const int num,
    const int channels, const int height, const int width,
    const int pooled_height, const int pooled_width, const int kernel_h,
    const int kernel_w, const int stride_h, const int stride_w, const int pad_h,
    const int pad_w, Dtype* const bottom_diff) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    // find out the local index
    // find out the local offset
    const int w = index % width;
    const int h = (index / width) % height;
    const int c = (index / width / height) % channels;
    const int n = index / width / height / channels;
    const int phstart =
         (h + pad_h < kernel_h) ? 0 : (h + pad_h - kernel_h) / stride_h + 1;
    const int phend = min((h + pad_h) / stride_h + 1, pooled_height);
    const int pwstart =
         (w + pad_w < kernel_w) ? 0 : (w + pad_w - kernel_w) / stride_w + 1;
    const int pwend = min((w + pad_w) / stride_w + 1, pooled_width);
    Dtype gradient = 0;
    const int offset = (n * channels + c) * pooled_height * pooled_width;
    const Dtype* const top_diff_slice = top_diff + offset;
    if (mask) {
      const int* const mask_slice = mask + offset;
      for (int ph = phstart; ph < phend; ++ph) {
        for (int pw = pwstart; pw < pwend; ++pw) {
          if (mask_slice[ph * pooled_width + pw] == h * width + w) {
            gradient += top_diff_slice[ph * pooled_width + pw];
          }
        }
      }
    } else {
      const Dtype* const top_mask_slice = top_mask + offset;
      for (int ph = phstart; ph < phend; ++ph) {
        for (int pw = pwstart; pw < pwend; ++pw) {
          if (top_mask_slice[ph * pooled_width + pw] == h * width + w) {
            gradient += top_diff_slice[ph * pooled_width + pw];
          }
        }
      }
    }
    bottom_diff[index] = gradient;
  }
}

__global__ void FUN(AvePoolBackward)(const int nthreads, const Dtype* const top_diff,
    const int num, const int channels, const int height,
    const int width, const int pooled_height, const int pooled_width,
    const int kernel_h, const int kernel_w, const int stride_h,
    const int stride_w, const int pad_h, const int pad_w,
    Dtype* const bottom_diff) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    // find out the local index
    // find out the local offset
    const int w = index % width + pad_w;
    const int h = (index / width) % height + pad_h;
    const int c = (index / width / height) % channels;
    const int n = index / width / height / channels;
    const int phstart = (h < kernel_h) ? 0 : (h - kernel_h) / stride_h + 1;
    const int phend = min(h / stride_h + 1, pooled_height);
    const int pwstart = (w < kernel_w) ? 0 : (w - kernel_w) / stride_w + 1;
    const int pwend = min(w / stride_w + 1, pooled_width);
    Dtype gradient = 0;
    const Dtype* const top_diff_slice =
        top_diff + (n * channels + c) * pooled_height * pooled_width;
    for (int ph = phstart; ph < phend; ++ph) {
      for (int pw = pwstart; pw < pwend; ++pw) {
        // figure out the pooling size
        int hstart = ph * stride_h - pad_h;
        int wstart = pw * stride_w - pad_w;
        int hend = min(hstart + kernel_h, height + pad_h);
        int wend = min(wstart + kernel_w, width + pad_w);
        int pool_size = (hend - hstart) * (wend - wstart);
        gradient += top_diff_slice[ph * pooled_width + pw] / pool_size;
      }
    }
    bottom_diff[index] = gradient;
  }
}

__global__ void FUN(StoPoolBackward)(const int nthreads,
    const Dtype* const rand_idx, const Dtype* const top_diff,
    const int num, const int channels, const int height,
    const int width, const int pooled_height, const int pooled_width,
    const int kernel_h, const int kernel_w, const int stride_h,
    const int stride_w, Dtype* const bottom_diff) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    // find out the local index
    // find out the local offset
    const int w = index % width;
    const int h = (index / width) % height;
    const int c = (index / width / height) % channels;
    const int n = index / width / height / channels;
    const int phstart = (h < kernel_h) ? 0 : (h - kernel_h) / stride_h + 1;
    const int phend = min(h / stride_h + 1, pooled_height);
    const int pwstart = (w < kernel_w) ? 0 : (w - kernel_w) / stride_w + 1;
    const int pwend = min(w / stride_w + 1, pooled_width);
    Dtype gradient = 0;
    const Dtype* const rand_idxslice =
        rand_idx + (n * channels + c) * pooled_height * pooled_width;
    const Dtype* const top_diff_slice =
        top_diff + (n * channels + c) * pooled_height * pooled_width;
    for (int ph = phstart; ph < phend; ++ph) {
      for (int pw = pwstart; pw < pwend; ++pw) {
        gradient += top_diff_slice[ph * pooled_width + pw] *
            (index == static_cast<int>(rand_idxslice[ph * pooled_width + pw]));
      }
    }
    bottom_diff[index] = gradient;
  }
}



void FUN(pooling_forward)(PoolMethod pool, Phase phase, const Dtype* bottom_data,
  const int num, const int channels, const int height, const int width, const int pooled_height, const int pooled_width,
  const int kernel_h, const int kernel_w, const int stride_h, const int stride_w, const int pad_h, const int pad_w,
  Dtype* rand_idx, Dtype* top_data, int* mask, Dtype* top_mask ) {
  int count = num * channels * pooled_height * pooled_width;
  //int count = top[0]->count();
  // We'll output the mask to top[1] if it's of size >1.
  //const bool use_top_mask = top.size() > 1;
  //int* mask = NULL;
  //Dtype* top_mask = NULL;
  switch (pool) {
  case PoolMethod_MAX:
    // NOLINT_NEXT_LINE(whitespace/operators)
    FUN(MaxPoolForward) << <CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >> >(
      count, bottom_data, num, channels,
      height, width, pooled_height, pooled_width, kernel_h,
      kernel_w, stride_h, stride_w, pad_h, pad_w, top_data,
      mask, top_mask);
    break;
  case PoolMethod_AVE:
    // NOLINT_NEXT_LINE(whitespace/operators)
    FUN(AvePoolForward) << <CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >> >(
      count, bottom_data, num, channels,
      height, width, pooled_height, pooled_width, kernel_h,
      kernel_w, stride_h, stride_w, pad_h, pad_w, top_data);
    break;
  case PoolMethod_STOCHASTIC:
    if (phase == TRAIN) {
      // We need to create the random index as well.
      FUN(caffe_rng_uniform)(count, Dtype(0), Dtype(1), rand_idx);
      // NOLINT_NEXT_LINE(whitespace/operators)
      FUN(StoPoolForwardTrain) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >>>(
          count, bottom_data, num, channels, height, width, pooled_height, pooled_width, kernel_h,
          kernel_w, stride_h, stride_w, rand_idx, top_data);
    }
    else {
      // NOLINT_NEXT_LINE(whitespace/operators)
      FUN(StoPoolForwardTest) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >> >(
          count, bottom_data, num, channels, height, width, pooled_height, pooled_width, kernel_h,
          kernel_w, stride_h, stride_w, top_data);
    }
    break;
  default:
    LOG(FATAL) << "Unknown pooling method.";
  }
  CUDA_POST_KERNEL_CHECK;
}

void FUN(pooling_backward)(PoolMethod pool, const Dtype* const rand_idx,
  const Dtype* top_diff, const int* mask, const Dtype* top_mask, const int num,
  const int channels, const int height, const int width, const int pooled_height, const int pooled_width, const int kernel_h,
  const int kernel_w, const int stride_h, const int stride_w, const int pad_h, const int pad_w, Dtype* bottom_diff) {
  const int count = num * channels * height * width;
#if 0
  if (!bottom[0]->propagate_down_) {
    return;
  }
  const Dtype* top_diff = top[0]->gpu_diff();
  Dtype* bottom_diff = bottom[0]->gpu_mdiff();
  const int count = bottom[0]->count();
  caffe_gpu_set(count, Dtype(0.), bottom_diff);
  // We'll output the mask to top[1] if it's of size >1.
  const bool use_top_mask = top.size() > 1;
  const int* mask = NULL;
  const Dtype* top_mask = NULL;
    if (use_top_mask) {
      top_mask = top[1]->data();
    } else {
      mask = max_idx_;
    }
#endif
  switch (pool) {
  case PoolMethod_MAX:
    // NOLINT_NEXT_LINE(whitespace/operators)
    FUN(MaxPoolBackward) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, top_diff, mask, top_mask, num, channels,
        height, width, pooled_height, pooled_width,
        kernel_h, kernel_w, stride_h, stride_w, pad_h, pad_w,
        bottom_diff);
    break;
  case PoolMethod_AVE:
    // NOLINT_NEXT_LINE(whitespace/operators)
    FUN(AvePoolBackward)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, top_diff, num, channels,
        height, width, pooled_height, pooled_width, kernel_h,
        kernel_w, stride_h, stride_w, pad_h, pad_w, bottom_diff);
    break;
  case PoolMethod_STOCHASTIC:
    // NOLINT_NEXT_LINE(whitespace/operators)
    FUN(StoPoolBackward)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, rand_idx, top_diff,
        num, channels, height, width, pooled_height,
        pooled_width, kernel_h, kernel_w, stride_h, stride_w,
        bottom_diff);
    break;
  default:
    LOG(FATAL) << "Unknown pooling method.";
  }
  CUDA_POST_KERNEL_CHECK;
}

