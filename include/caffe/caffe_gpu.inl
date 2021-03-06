
#include "caffe_config.h"


#ifdef USE_CUDNN

#include "../caffe/layers/absval_layer.cu"
#include "../caffe/layers/base_data_layer.cu"
#include "../caffe/layers/batch_norm_layer.cu"
#include "../caffe/layers/batch_reindex_layer.cu"
#include "../caffe/layers/bias_layer.cu"
#include "../caffe/layers/bnll_layer.cu"
#include "../caffe/layers/concat_layer.cu"
#include "../caffe/layers/contrastive_loss_layer.cu"
#include "../caffe/layers/conv_layer.cu"
#include "../caffe/layers/crop_layer.cu"
#include "../caffe/layers/ctcpp_entrypoint.cu"
#include "../caffe/layers/cudnn_conv_layer.cu"
#include "../caffe/layers/cudnn_lcn_layer.cu"
#include "../caffe/layers/cudnn_lrn_layer.cu"
#include "../caffe/layers/cudnn_pooling_layer.cu"
#include "../caffe/layers/cudnn_relu_layer.cu"
#include "../caffe/layers/cudnn_sigmoid_layer.cu"
#include "../caffe/layers/cudnn_softmax_layer.cu"
#include "../caffe/layers/cudnn_tanh_layer.cu"
#include "../caffe/layers/deconv_layer.cu"
#include "../caffe/layers/denseblock_layer.cu"
#include "../caffe/layers/dropout_layer.cu"
#include "../caffe/layers/eltwise_layer.cu"
#include "../caffe/layers/elu_layer.cu"
#include "../caffe/layers/embed_layer.cu"
#include "../caffe/layers/euclidean_loss_layer.cu"
#include "../caffe/layers/exp_layer.cu"
#include "../caffe/layers/filter_layer.cu"
//#include "../caffe/layers/hdf5_data_layer.cu"
//#include "../caffe/layers/hdf5_output_layer.cu"
#include "../caffe/layers/im2col_layer.cu"
#include "../caffe/layers/inner_product_layer.cu"
#include "../caffe/layers/log_layer.cu"
#include "../caffe/layers/lrn_layer.cu"
#include "../caffe/layers/lstm_layer_Junhyuk.cu"
#include "../caffe/layers/lstm_unit_layer.cu"
#include "../caffe/layers/mvn_layer.cu"
#include "../caffe/layers/pooling_layer.cu"
#include "../caffe/layers/power_layer.cu"
#include "../caffe/layers/prelu_layer.cu"
#include "../caffe/layers/recurrent_layer.cu"
#include "../caffe/layers/reduce.cu"
#include "../caffe/layers/reduction_layer.cu"
#include "../caffe/layers/relu_layer.cu"
#include "../caffe/layers/reverse_layer.cu"
#include "../caffe/layers/reverse_time_layer.cu"
#include "../caffe/layers/scale_layer.cu"
#include "../caffe/layers/sigmoid_cross_entropy_loss_layer.cu"
#include "../caffe/layers/sigmoid_layer.cu"
#include "../caffe/layers/silence_layer.cu"
#include "../caffe/layers/slice_layer.cu"
#include "../caffe/layers/softmax_layer.cu"
#include "../caffe/layers/softmax_loss_layer.cu"
#include "../caffe/layers/softmax_loss_layer_multi_label.cu"
#include "../caffe/layers/split_layer.cu"
#include "../caffe/layers/tanh_layer.cu"
#include "../caffe/layers/threshold_layer.cu"
#include "../caffe/layers/tile_layer.cu"
#include "../caffe/layers/transpose_layer.cu"
#include "../caffe/layers/warp_ctc_loss_layer.cu"
#include "../caffe/solvers/adadelta_solver.cu"
#include "../caffe/solvers/adagrad_solver.cu"
#include "../caffe/solvers/adam_solver.cu"
#include "../caffe/solvers/nesterov_solver.cu"
#include "../caffe/solvers/rmsprop_solver.cu"
#include "../caffe/solvers/sgd_solver.cu"
#include "../caffe/util/im2col.cu"
#include "../caffe/util/interp.cu"
#include "../caffe/util/math_functions.cu"


#endif
