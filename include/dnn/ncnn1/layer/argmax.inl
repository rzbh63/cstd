

//#include "test/test_partial_sort.inl"

static int ArgMax(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_data = LBLOB(0)->data;
  img_t* top_blob = net->blobs[layer->blobs[1]].data;
  const float* ptr = NULL;
  int* vec = NULL;
  float* outptr = NULL;
  int i, size = pBox_total(bottom_data);
  int out_max_val = LARG(out_max_val);
  int topk = LARG(topk);

  if (out_max_val) {
    pBoxsetsize(top_blob, 2, topk, 1, 1);
  }
  else {
    pBoxsetsize(top_blob, 1, topk, 1, 1);
  }

  if (pBox_empty(top_blob)) {
    return -100;
  }

  ptr = pBox_datap(bottom_data);

  // partial sort topk with index
  // optional value
  MYREALLOC(vec, size);

  for (i = 0; i < size; i++) {
    vec[i] = i;
  }

#define GREATER(a, b)  ptr[a]>ptr[b]
  PARTIAL_SORT(size, vec, topk, GREATER, int);

  outptr = pBox_datap(top_blob);
  if (out_max_val) {
    float* valptr = outptr + topk;

    for (i = 0; i < topk; i++) {
      outptr[i] = ptr[vec[i]];
      valptr[i] = vec[i];
    }
  }
  else {
    for (i = 0; i < topk; i++) {
      outptr[i] = vec[i];
    }
  }
  FREE(vec);

  return 0;
}
