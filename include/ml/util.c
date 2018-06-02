
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ml.h"
#include "matmul.h"
#include "matrix.h"
#include "macro.h"
#include "rand.h"

/* Calculates upper triangular matrix S, where A is a symmetrical matrix A=S'*S */
void cvChol( int dim, const real* A, int astep, real* S, int sstep ) {
  int i, j, k;
  real sum;
  
  for ( i = 0; i < dim; i++ ) {
    for ( j = 0; j < i; j++ )
      *( S + sstep * i + j ) = 0;
      
    sum = 0;
    for ( k = 0; k < i; k++ )
      sum += *( S + sstep * k + i ) * *( S + sstep * k + i );
      
    *( S + sstep * i + i ) = ( real ) sqrt( *( A + astep * i + i ) - sum );
    
    for ( j = i + 1; j < dim; j++ ) {
      sum = 0;
      for ( k = 0; k < i; k++ )
        sum += *( S + sstep * k + i ) * *( S + sstep * k + j );
        
      *( S + sstep * i + j ) = ( *( A + astep * i + j ) - sum ) / *( S + sstep * i + i );
    }
  }
}

/* Generates <sample> from multivariate normal distribution, where <mean> - is an
   average row vector, <cov> - symmetric covariation matrix */
void cvRandMVNormal(
  real* mean, int mean_step,
  real* cov, int cov_step,
  int sample_rows, int sample_cols,
  real* sample, int sample_step,
  CvRNG* rng
) {
  int i, dim = sample_cols, amount = sample_rows;
  real* utmat = MALLOC( real, dim * dim );
  real* vect = MALLOC( real, dim );
  
  CvRNG state = rng ? *rng : cvRNG( time( 0 ) );
  cvRandArr( sample_rows, sample_cols, sample, sample_step,
             ( 0 ), ( 1 ), state, CV_RAND_NORMAL, real );
             
  cvChol( dim, cov, cov_step, utmat, dim );
  
  // vect = vect*utmat+mean
  for ( i = 0; i < amount; i++ ) {
    real* dst = sample + sample_step * i;
    memcpy( vect, dst, dim * sizeof( real ) );
    memcpy( dst, mean, dim * sizeof( real ) );
    MAT_ABC( 1, dim, dim, vect, dim, utmat, dim, dst, sample_step );
  }
  
  SAFEFREE( vect );
  SAFEFREE( utmat );
}

/* Generates <sample> of <amount> points from a discrete variate xi,
   where Pr{xi = k} == probs[k], 0 < k < len - 1. */
void cvRandSeries( real* probs, int len, int* sample, int amount ) {
  real * univals = MALLOC( real, amount );
  real* knots = ( real* ) cvAlloc( len * sizeof( real ) );
  
  int i, j;
  
  CvRNG state = cvRNG( -1 );
  cvRandArr( 1, amount, univals, amount, 0, 1, state, CV_RAND_UNI, real );
  
  knots[ 0 ] = probs[ 0 ];
  for ( i = 1; i < len; i++ )
    knots[ i ] = knots[ i - 1 ] + probs[ i ];
    
  for ( i = 0; i < amount; i++ )
    for ( j = 0; j < len; j++ ) {
      if ( univals[ i ] <= knots[ j ] ) {
        sample[ i ] = j;
        break;
      }
    }
    
  cvFree( &knots );
}

/* Generates <sample> from gaussian mixture distribution */
void cvRandGaussMixture( int sample_rows, int sample_cols,
                         real* means[], int* means_step,
                         real* covs[], int* covs_step,
                         real weights[],
                         int clsnum,
                         real* sample, int sample_step,
                         real* sampClasses ) {
  int dim = sample_cols;
  int amount = sample_rows;
  
  int i, clss;
  CvRNG state = cvRNG( -1 );
  
  int* sample_clsnum = ( int* ) cvAlloc( amount * sizeof( int ) );
  real** utmats = ( real** ) cvAlloc( clsnum * sizeof( real* ) );
  real* vect;
  
  real* classes;
  if ( sampClasses )
    classes = sampClasses;
  else
    classes = MALLOC( real, amount );
    
  cvRandArr( sample_rows, sample_cols, sample, sample_step, 0, 1, state, CV_RAND_NORMAL, real );
  
  cvRandSeries( weights, clsnum, sample_clsnum, amount );
  
  for ( i = 0; i < clsnum; i++ ) {
    utmats[ i ] = MALLOC( real, dim * dim );
    cvChol( dim, covs[ i ], covs_step[ i ], utmats[ i ], dim );
  }
  
  for ( i = 0; i < amount; i++ ) {
    classes[ i ] = ( real ) sample_clsnum[ i ];
    vect = sample + i * sample_step;
    clss = sample_clsnum[ i ];
    //cvMatMulAdd(vect, utmats[clss], means[clss], vect);
    cvMatMulAdd( 1, dim, dim, vect, sample_step, utmats[ clss ], dim, means[ clss ], means_step[ clss ], vect, sample_step );
  }
  
  if ( !sampClasses )
    SAFEFREE( classes );
    
  for ( i = 0; i < clsnum; i++ )
    SAFEFREE( utmats[ i ] );
  cvFree( &utmats );
  cvFree( &sample_clsnum );
  SAFEFREE( vect );
}

real* icvGenerateRandomClusterCenters ( int seed, int data_rows, int data_cols, const real* data, int data_step,
                                        int num_of_clusters, real* centers, int centers_step ) {
  CvRNG rng;
  real minVal, maxVal;
  int i, minId, maxId;
  int dim = data_cols;
  
  rng = cvRNG( seed );
  for ( i = 0; i < dim; i++ ) {
    const real* data_comp = data + i * data_step;
    real* centers_comp = centers + i * centers_step;
    MINMAX_ELEM( dim, data_comp, 1, minId, maxId );
    minVal = data_comp[ minId ];
    maxVal = data_comp[ maxId ];
    cvRandArr( 1, num_of_clusters, centers_comp, centers_step, minVal, maxVal, rng, CV_RAND_UNI, real );
  }
  
  return centers;
} // end of icvGenerateRandomClusterCenters

// By S. Dilman - begin -

#define ICV_RAND_MAX    4294967296 // == 2^32
//#define CV_ASSERT assert

void cvRandRoundUni( real* center, int center_step,
                     real radius_small,
                     real radius_large,
                     int desired_matrix_rows,
                     int desired_matrix_cols,
                     real* desired_matrix,
                     int desired_matrix_step,
                     CvRNG* rng_state_ptr ) {
  real rad, norm, coefficient;
  int dim, size, i, j;
  real *cov;
  real *sample;
  CvRNG rng_local;
  
  rng_local = *rng_state_ptr;
  
  CV_ASSERT( ( radius_small >= 0 ) && ( radius_large > 0 ) && ( radius_small <= radius_large ) );
  CV_ASSERT( center && desired_matrix && rng_state_ptr );
  //CV_ASSERT(center->cols == desired_matrix->cols);
  
  dim = desired_matrix_cols;
  size = desired_matrix_rows;
  cov = MALLOC( real, dim * dim );
  cvSetIdentity( dim, dim, cov, dim, 1.f );
  cvRandMVNormal( center, center_step,
                  cov, dim,
                  desired_matrix_rows,
                  desired_matrix_cols,
                  desired_matrix,
                  desired_matrix_step,
                  &rng_local );
                  
  for ( i = 0; i < size; i++ ) {
    rad = ( real ) ( cvRandReal( rng_local ) * ( radius_large - radius_small ) + radius_small );
    sample = desired_matrix + desired_matrix_step * i;
    NORM2D_L2 ( 1, desired_matrix_cols, sample, desired_matrix_step, 1, norm );
    coefficient = rad / norm;
    for ( j = 0; j < dim; j++ )
      sample[ j ] *= coefficient;
  }
  SAFEFREE( cov );
}
// By S. Dilman - end -

/* Aij <- Aji for i > j if lower_to_upper != 0
              for i < j if lower_to_upper = 0 */
void cvCompleteSymm( int rows, int cols, real* matrix, int step, int lower_to_upper ) {
  int i, j;
  real* dst = matrix;
  if ( !lower_to_upper ) {
    for ( i = 1; i < rows; i++ ) {
      const real* src = matrix + i;
      dst += step;
      for ( j = 0; j < i; j++, src += step )
        dst[ j ] = src[ 0 ];
    }
  } else {
    for ( i = 0; i < rows - 1; i++, dst += step ) {
      const real * src = matrix + ( i + 1 ) * step + i;
      for ( j = i + 1; j < cols; j++, src += step )
        dst[ j ] = src[ 0 ];
    }
  }
}

static int CV_CDECL
icvCmpIntegers( const void* a, const void* b ) {
  return * ( const int* ) a - *( const int* ) b;
}

static int CV_CDECL
icvCmpIntegersPtr( const void* _a, const void* _b ) {
  int a = **( const int** ) _a;
  int b = **( const int** ) _b;
  return ( a < b ? -1 : 0 ) | ( a > b );
}


static int icvCmpSparseVecElems( const void* a, const void* b ) {
  return ( ( CvSparseVecElem32f* ) a ) ->idx - ( ( CvSparseVecElem32f* ) b ) ->idx;
}


int*
cvPreprocessIndexArray( int idx_total, const void* idx_arr, int step, int data_arr_size, BOOL check_for_duplicates , int idx_is_int ) {
  int i, idx_selected = 0, prev = INT_MIN, is_sorted = 1;
  unsigned char* srcb = ( unsigned char* ) idx_arr;
  int* srci = ( int* ) idx_arr;
  int* dsti;
  
  if ( idx_is_int ) {
    // idx_arr is array of 1's and 0's -
    // i.e. it is a mask of the selected components
    if ( idx_total != data_arr_size )
      CV_ERROR( CV_StsUnmatchedSizes,
                "Component mask should contain as many elements as the total number of input variables" );
                
    for ( i = 0; i < idx_total; i++ )
      idx_selected += srcb[ i * step ] != 0;
      
    if ( idx_selected == 0 )
      CV_ERROR( CV_StsOutOfRange, "No components/input_variables is selected!" );
      
    if ( idx_selected == idx_total )
      return 0;
  } else {
    // idx_arr is array of integer indices of selected components
    if ( idx_total > data_arr_size )
      CV_ERROR( CV_StsOutOfRange,
                "index array may not contain more elements than the total number of input variables" );
    idx_selected = idx_total;
    // check if sorted already
    for ( i = 0; i < idx_total; i++ ) {
      int val = srci[ i * step ];
      if ( val >= prev ) {
        is_sorted = 0;
        break;
      }
      prev = val;
    }
  }
  
  dsti = MALLOC( int, idx_selected );
  
  if ( idx_is_int ) {
    for ( i = 0; i < idx_total; i++ )
      if ( srcb[ i * step ] )
        * dsti++ = i;
  } else {
    for ( i = 0; i < idx_total; i++ )
      dsti[ i ] = srci[ i * step ];
      
    if ( !is_sorted )
      qsort( dsti, idx_total, sizeof( dsti[ 0 ] ), icvCmpIntegers );
      
    if ( dsti[ 0 ] < 0 || dsti[ idx_total - 1 ] >= data_arr_size )
      CV_ERROR( CV_StsOutOfRange, "the index array elements are out of range" );
      
    if ( check_for_duplicates ) {
      for ( i = 1; i < idx_total; i++ )
        if ( dsti[ i ] <= dsti[ i - 1 ] )
          CV_ERROR( CV_StsBadArg, "There are duplicated index array elements" );
    }
  }
  
  //SAFEFREE( dsti );
  return dsti;
}



unsigned char*
cvPreprocessVarType( int var_size, const unsigned char* var_type, int var_step, int idx_size, const int* var_idx,
                     int var_all, int* response_type ) {
  unsigned char * out_var_type = 0;
  int i, tm_size, tm_step;
  const int* map = 0;
  const unsigned char* src;
  unsigned char* dst;
  int var_count = var_all;
  
  if ( response_type )
    * response_type = -1;
    
  tm_size = var_size;
  tm_step = var_step;
  
  if (  /*tm_size != var_count &&*/ tm_size != var_count + 1 )
    CV_ERROR( CV_StsBadArg,
              "type mask must be of <input var count> + 1 size" );
              
  if ( response_type && tm_size > var_count )
    * response_type = var_type[ var_count * tm_step ] != 0;
    
  if ( var_idx ) {
    if ( var_size > var_count )
      CV_ERROR( CV_StsBadSize, "var index array is too large" );
    map = var_idx;
    var_count = idx_size;
  }
  
  out_var_type = MALLOC( unsigned char, var_count );
  src = var_type;
  dst = out_var_type;
  
  for ( i = 0; i < var_count; i++ ) {
    int idx = map ? map[ i ] : i;
    assert( ( unsigned ) idx < ( unsigned ) tm_size );
    dst[ i ] = ( unsigned char ) ( src[ idx * tm_step ] != 0 );
  }
  
  return out_var_type;
}

real*
cvPreprocessOrderedResponses( void* responses, int r_step, int sample_idx_size, const int* sample_idx, int sample_all, int is_int_type ) {
  real * out_responses = 0;
  int i;
  const int* map = 0;
  real* dst;
  int sample_count = sample_all;
  
  if ( !is_int_type && 1 == r_step && !sample_idx ) {
    return responses;
  }
  
  if ( sample_idx ) {
    if ( sample_idx_size > sample_count )
      CV_ERROR( CV_StsBadSize, "sample index array is too large" );
    map = sample_idx;
    sample_count = sample_idx_size;
  }
  
  out_responses = MALLOC( real, sample_count );
  
  dst = out_responses;
  if ( !is_int_type ) {
    const real * src = responses;
    for ( i = 0; i < sample_count; i++ ) {
      int idx = map ? map[ i ] : i;
      assert( ( unsigned ) idx < ( unsigned ) sample_all );
      dst[ i ] = src[ idx * r_step ];
    }
  } else {
    const int* src = responses;
    for ( i = 0; i < sample_count; i++ ) {
      int idx = map ? map[ i ] : i;
      assert( ( unsigned ) idx < ( unsigned ) sample_all );
      dst[ i ] = ( real ) src[ idx * r_step ];
    }
  }
  
  return out_responses;
}

#if 0

CvMat*
cvPreprocessCategoricalResponses( const CvMat* responses,
                                  const CvMat* sample_idx, int sample_all,
                                  CvMat** out_response_map, CvMat** class_counts ) {
  CvMat * out_responses = 0;
  int** response_ptr = 0;
  
  CV_FUNCNAME( "cvPreprocessCategoricalResponses" );
  
  if ( out_response_map )
    * out_response_map = 0;
    
  if ( class_counts )
    * class_counts = 0;
    
  __BEGIN__;
  
  int i, r_type, r_step;
  int cls_count = 1, prev_cls, prev_i;
  const int* map = 0;
  const int* srci;
  const real* srcfl;
  int* dst;
  int* cls_map;
  int* cls_counts = 0;
  int sample_count = sample_all;
  
  if ( !CV_IS_MAT( responses ) )
    CV_ERROR( CV_StsBadArg, "Invalid response array" );
    
  if ( responses->rows != 1 && responses->cols != 1 )
    CV_ERROR( CV_StsBadSize, "Response array must be 1-dimensional" );
    
  if ( responses->rows + responses->cols - 1 != sample_count )
    CV_ERROR( CV_StsUnmatchedSizes,
              "Response array must contain as many elements as the total number of samples" );
              
  r_type = CV_MAT_TYPE( responses->type );
  if ( r_type != CV_32FC1 && r_type != CV_32SC1 )
    CV_ERROR( CV_StsUnsupportedFormat, "Unsupported response type" );
    
  r_step = responses->step ? responses->step / CV_ELEM_SIZE( responses->type ) : 1;
  
  if ( sample_idx ) {
    if ( !CV_IS_MAT( sample_idx ) || CV_MAT_TYPE( sample_idx->type ) != CV_32SC1 ||
         sample_idx->rows != 1 && sample_idx->cols != 1 || !CV_IS_MAT_CONT( sample_idx->type ) )
      CV_ERROR( CV_StsBadArg, "sample index array should be continuous 1-dimensional integer vector" );
    if ( sample_idx->rows + sample_idx->cols - 1 > sample_count )
      CV_ERROR( CV_StsBadSize, "sample index array is too large" );
    map = sample_idx->data.i;
    sample_count = sample_idx->rows + sample_idx->cols - 1;
  }
  
  CV_CALL( out_responses = cvCreateMat( 1, sample_count, CV_32SC1 ) );
  
  if ( !out_response_map )
    CV_ERROR( CV_StsNullPtr, "out_response_map pointer is NULL" );
    
  CV_CALL( response_ptr = ( int** ) cvAlloc( sample_count * sizeof( response_ptr[ 0 ] ) ) );
  
  srci = responses->data.i;
  srcfl = responses->data.fl;
  dst = out_responses->data.i;
  
  for ( i = 0; i < sample_count; i++ ) {
    int idx = map ? map[ i ] : i;
    assert( ( unsigned ) idx < ( unsigned ) sample_all );
    if ( r_type == CV_32SC1 )
      dst[ i ] = srci[ idx * r_step ];
    else {
      real rf = srcfl[ idx * r_step ];
      int ri = cvRound( rf );
      if ( ri != rf ) {
        char buf[ 100 ];
        sprintf( buf, "response #%d is not integral", idx );
        CV_ERROR( CV_StsBadArg, buf );
      }
      dst[ i ] = ri;
    }
    response_ptr[ i ] = dst + i;
  }
  
  qsort( response_ptr, sample_count, sizeof( int* ), icvCmpIntegersPtr );
  
  // count the classes
  for ( i = 1; i < sample_count; i++ )
    cls_count += *response_ptr[ i ] != *response_ptr[ i - 1 ];
    
  if ( cls_count < 2 )
    CV_ERROR( CV_StsBadArg, "There is only a single class" );
    
  CV_CALL( *out_response_map = cvCreateMat( 1, cls_count, CV_32SC1 ) );
  
  if ( class_counts ) {
    CV_CALL( *class_counts = cvCreateMat( 1, cls_count, CV_32SC1 ) );
    cls_counts = ( *class_counts ) ->data.i;
  }
  
  // compact the class indices and build the map
  prev_cls = ~ * response_ptr[ 0 ];
  cls_count = -1;
  cls_map = ( *out_response_map ) ->data.i;
  
  for ( i = 0, prev_i = -1; i < sample_count; i++ ) {
    int cur_cls = *response_ptr[ i ];
    if ( cur_cls != prev_cls ) {
      if ( cls_counts && cls_count >= 0 )
        cls_counts[ cls_count ] = i - prev_i;
      cls_map[ ++cls_count ] = prev_cls = cur_cls;
      prev_i = i;
    }
    *response_ptr[ i ] = cls_count;
  }
  
  if ( cls_counts )
    cls_counts[ cls_count ] = i - prev_i;
    
  __END__;
  
  cvFree( &response_ptr );
  
  return out_responses;
}


const real**
cvGetTrainSamples( const CvMat* train_data, int tflag,
                   const CvMat* var_idx, const CvMat* sample_idx,
                   int* _var_count, int* _sample_count,
                   bool always_copy_data ) {
  real** samples = 0;
  
  CV_FUNCNAME( "cvGetTrainSamples" );
  
  __BEGIN__;
  
  int i, j, var_count, sample_count, s_step, v_step;
  bool copy_data;
  const real* data;
  const int *s_idx, *v_idx;
  
  if ( !CV_IS_MAT( train_data ) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL training data matrix" );
    
  var_count = var_idx ? var_idx->cols + var_idx->rows - 1 :
              tflag == CV_ROW_SAMPLE ? train_data->cols : train_data->rows;
  sample_count = sample_idx ? sample_idx->cols + sample_idx->rows - 1 :
                 tflag == CV_ROW_SAMPLE ? train_data->rows : train_data->cols;
                 
  if ( _var_count )
    * _var_count = var_count;
    
  if ( _sample_count )
    * _sample_count = sample_count;
    
  copy_data = tflag != CV_ROW_SAMPLE || var_idx || always_copy_data;
  
  CV_CALL( samples = ( real** ) cvAlloc( sample_count * sizeof( samples[ 0 ] ) +
                                         ( copy_data ? 1 : 0 ) * var_count * sample_count * sizeof( samples[ 0 ][ 0 ] ) ) );
  data = train_data->data.fl;
  s_step = train_data->step / sizeof( samples[ 0 ][ 0 ] );
  v_step = 1;
  s_idx = sample_idx ? sample_idx->data.i : 0;
  v_idx = var_idx ? var_idx->data.i : 0;
  
  if ( !copy_data ) {
    for ( i = 0; i < sample_count; i++ )
      samples[ i ] = ( real* ) ( data + ( s_idx ? s_idx[ i ] : i ) * s_step );
  } else {
    samples[ 0 ] = ( real* ) ( samples + sample_count );
    if ( tflag != CV_ROW_SAMPLE )
      CV_SWAP( s_step, v_step, i );
      
    for ( i = 0; i < sample_count; i++ ) {
      real* dst = samples[ i ] = samples[ 0 ] + i * var_count;
      const real* src = data + ( s_idx ? s_idx[ i ] : i ) * s_step;
      
      if ( !v_idx )
        for ( j = 0; j < var_count; j++ )
          dst[ j ] = src[ j * v_step ];
      else
        for ( j = 0; j < var_count; j++ )
          dst[ j ] = src[ v_idx[ j ] * v_step ];
    }
  }
  
  __END__;
  
  return ( const real** ) samples;
}


void
cvCheckTrainData( const CvMat* train_data, int tflag,
                  const CvMat* missing_mask,
                  int* var_all, int* sample_all ) {
  CV_FUNCNAME( "cvCheckTrainData" );
  
  if ( var_all )
    * var_all = 0;
    
  if ( sample_all )
    * sample_all = 0;
    
  __BEGIN__;
  
  // check parameter types and sizes
  if ( !CV_IS_MAT( train_data ) || CV_MAT_TYPE( train_data->type ) != CV_32FC1 )
    CV_ERROR( CV_StsBadArg, "train data must be realing-point matrix" );
    
  if ( missing_mask ) {
    if ( !CV_IS_MAT( missing_mask ) || !CV_IS_MASK_ARR( missing_mask ) ||
         !CV_ARE_SIZES_EQ( train_data, missing_mask ) )
      CV_ERROR( CV_StsBadArg,
                "missing value mask must be 8-bit matrix of the same size as training data" );
  }
  
  if ( tflag != CV_ROW_SAMPLE && tflag != CV_COL_SAMPLE )
    CV_ERROR( CV_StsBadArg,
              "Unknown training data layout (must be CV_ROW_SAMPLE or CV_COL_SAMPLE)" );
              
  if ( var_all )
    * var_all = tflag == CV_ROW_SAMPLE ? train_data->cols : train_data->rows;
    
  if ( sample_all )
    * sample_all = tflag == CV_ROW_SAMPLE ? train_data->rows : train_data->cols;
    
  __END__;
}


int
cvPrepareTrainData( const char* /*funcname*/,
                    const CvMat* train_data, int tflag,
                    const CvMat* responses, int response_type,
                    const CvMat* var_idx,
                    const CvMat* sample_idx,
                    bool always_copy_data,
                    const real*** out_train_samples,
                    int* _sample_count,
                    int* _var_count,
                    int* _var_all,
                    CvMat** out_responses,
                    CvMat** out_response_map,
                    CvMat** out_var_idx,
                    CvMat** out_sample_idx ) {
  int ok = 0;
  CvMat* _var_idx = 0;
  CvMat* _sample_idx = 0;
  CvMat* _responses = 0;
  int sample_all = 0, sample_count = 0, var_all = 0, var_count = 0;
  
  CV_FUNCNAME( "cvPrepareTrainData" );
  
  // step 0. clear all the output pointers to ensure we do not try
  // to call free() with uninitialized pointers
  if ( out_responses )
    * out_responses = 0;
    
  if ( out_response_map )
    * out_response_map = 0;
    
  if ( out_var_idx )
    * out_var_idx = 0;
    
  if ( out_sample_idx )
    * out_sample_idx = 0;
    
  if ( out_train_samples )
    * out_train_samples = 0;
    
  if ( _sample_count )
    * _sample_count = 0;
    
  if ( _var_count )
    * _var_count = 0;
    
  if ( _var_all )
    * _var_all = 0;
    
  __BEGIN__;
  
  if ( !out_train_samples )
    CV_ERROR( CV_StsBadArg, "output pointer to train samples is NULL" );
    
  CV_CALL( cvCheckTrainData( train_data, tflag, 0, &var_all, &sample_all ) );
  
  if ( sample_idx )
    CV_CALL( _sample_idx = cvPreprocessIndexArray( sample_idx, sample_all ) );
  if ( var_idx )
    CV_CALL( _var_idx = cvPreprocessIndexArray( var_idx, var_all ) );
    
  if ( responses ) {
    if ( !out_responses )
      CV_ERROR( CV_StsNullPtr, "output response pointer is NULL" );
      
    if ( response_type == CV_VAR_NUMERICAL ) {
      CV_CALL( _responses = cvPreprocessOrderedResponses( responses,
                            _sample_idx, sample_all ) );
    } else {
      CV_CALL( _responses = cvPreprocessCategoricalResponses( responses,
                            _sample_idx, sample_all, out_response_map, 0 ) );
    }
  }
  
  CV_CALL( *out_train_samples =
             cvGetTrainSamples( train_data, tflag, _var_idx, _sample_idx,
                                &var_count, &sample_count, always_copy_data ) );
                                
  ok = 1;
  
  __END__;
  
  if ( ok ) {
    if ( out_responses )
      * out_responses = _responses, _responses = 0;
      
    if ( out_var_idx )
      * out_var_idx = _var_idx, _var_idx = 0;
      
    if ( out_sample_idx )
      * out_sample_idx = _sample_idx, _sample_idx = 0;
      
    if ( _sample_count )
      * _sample_count = sample_count;
      
    if ( _var_count )
      * _var_count = var_count;
      
    if ( _var_all )
      * _var_all = var_all;
  } else {
    if ( out_response_map )
      cvReleaseMat( out_response_map );
    cvFree( out_train_samples );
  }
  
  if ( _responses != responses )
    cvReleaseMat( &_responses );
  cvReleaseMat( &_var_idx );
  cvReleaseMat( &_sample_idx );
  
  return ok;
}


typedef struct CvSampleResponsePair {
  const real* sample;
  const unsigned char* mask;
  int response;
  int index;
}
CvSampleResponsePair;


static int
CV_CDECL icvCmpSampleResponsePairs( const void* a, const void* b ) {
  int ra = ( ( const CvSampleResponsePair* ) a ) ->response;
  int rb = ( ( const CvSampleResponsePair* ) b ) ->response;
  int ia = ( ( const CvSampleResponsePair* ) a ) ->index;
  int ib = ( ( const CvSampleResponsePair* ) b ) ->index;
  
  return ra < rb ? -1 : ra > rb ? 1 : ia - ib;
  //return (ra > rb ? -1 : 0)|(ra < rb);
}


void
cvSortSamplesByClasses( const real** samples, const CvMat* classes,
                        int* class_ranges, const unsigned char** mask ) {
  CvSampleResponsePair * pairs = 0;
  CV_FUNCNAME( "cvSortSamplesByClasses" );
  
  __BEGIN__;
  
  int i, k = 0, sample_count;
  
  if ( !samples || !classes || !class_ranges )
    CV_ERROR( CV_StsNullPtr, "INTERNAL ERROR: some of the args are NULL pointers" );
    
  if ( classes->rows != 1 || CV_MAT_TYPE( classes->type ) != CV_32SC1 )
    CV_ERROR( CV_StsBadArg, "classes array must be a single row of integers" );
    
  sample_count = classes->cols;
  CV_CALL( pairs = ( CvSampleResponsePair* ) cvAlloc( ( sample_count + 1 ) * sizeof( pairs[ 0 ] ) ) );
  
  for ( i = 0; i < sample_count; i++ ) {
    pairs[ i ].sample = samples[ i ];
    pairs[ i ].mask = ( mask ) ? ( mask[ i ] ) : 0;
    pairs[ i ].response = classes->data.i[ i ];
    pairs[ i ].index = i;
    assert( classes->data.i[ i ] >= 0 );
  }
  
  qsort( pairs, sample_count, sizeof( pairs[ 0 ] ), icvCmpSampleResponsePairs );
  pairs[ sample_count ].response = -1;
  class_ranges[ 0 ] = 0;
  
  for ( i = 0; i < sample_count; i++ ) {
    samples[ i ] = pairs[ i ].sample;
    if ( mask )
      mask[ i ] = pairs[ i ].mask;
    classes->data.i[ i ] = pairs[ i ].response;
    
    if ( pairs[ i ].response != pairs[ i + 1 ].response )
      class_ranges[ ++k ] = i + 1;
  }
  
  __END__;
  
  cvFree( &pairs );
}


void
cvPreparePredictData( const CvArr* _sample, int dims_all,
                      const CvMat* comp_idx, int class_count,
                      const CvMat* prob, real** _row_sample,
                      int as_sparse ) {
  real * row_sample = 0;
  int* inverse_comp_idx = 0;
  
  CV_FUNCNAME( "cvPreparePredictData" );
  
  __BEGIN__;
  
  const CvMat* sample = ( const CvMat* ) _sample;
  real* sample_data;
  int sample_step;
  int is_sparse = CV_IS_SPARSE_MAT( sample );
  int d, sizes[ CV_MAX_DIM ];
  int i, dims_selected;
  int vec_size;
  
  if ( !is_sparse && !CV_IS_MAT( sample ) )
    CV_ERROR( !sample ? CV_StsNullPtr : CV_StsBadArg, "The sample is not a valid vector" );
    
  if ( cvGetElemType( sample ) != CV_32FC1 )
    CV_ERROR( CV_StsUnsupportedFormat, "Input sample must have 32fC1 type" );
    
  CV_CALL( d = cvGetDims( sample, sizes ) );
  
  if ( !( is_sparse && d == 1 || !is_sparse && d == 2 && ( sample->rows == 1 || sample->cols == 1 ) ) )
    CV_ERROR( CV_StsBadSize, "Input sample must be 1-dimensional vector" );
    
  if ( d == 1 )
    sizes[ 1 ] = 1;
    
  if ( sizes[ 0 ] + sizes[ 1 ] - 1 != dims_all )
    CV_ERROR( CV_StsUnmatchedSizes,
              "The sample size is different from what has been used for training" );
              
  if ( !_row_sample )
    CV_ERROR( CV_StsNullPtr, "INTERNAL ERROR: The row_sample pointer is NULL" );
    
  if ( comp_idx && ( !CV_IS_MAT( comp_idx ) || comp_idx->rows != 1 ||
                     CV_MAT_TYPE( comp_idx->type ) != CV_32SC1 ) )
    CV_ERROR( CV_StsBadArg, "INTERNAL ERROR: invalid comp_idx" );
    
  dims_selected = comp_idx ? comp_idx->cols : dims_all;
  
  if ( prob ) {
    if ( !CV_IS_MAT( prob ) )
      CV_ERROR( CV_StsBadArg, "The output matrix of probabilities is invalid" );
      
    if ( ( prob->rows != 1 && prob->cols != 1 ) ||
         CV_MAT_TYPE( prob->type ) != CV_32FC1 &&
         CV_MAT_TYPE( prob->type ) != CV_64FC1 )
      CV_ERROR( CV_StsBadSize,
                "The matrix of probabilities must be 1-dimensional vector of 32fC1 type" );
                
    if ( prob->rows + prob->cols - 1 != class_count )
      CV_ERROR( CV_StsUnmatchedSizes,
                "The vector of probabilities must contain as many elements as "
                "the number of classes in the training set" );
  }
  
  vec_size = !as_sparse ? dims_selected * sizeof( row_sample[ 0 ] ) :
             ( dims_selected + 1 ) * sizeof( CvSparseVecElem32f );
             
  if ( CV_IS_MAT( sample ) ) {
    sample_data = sample->data.fl;
    sample_step = sample->step / sizeof( row_sample[ 0 ] );
    
    if ( !comp_idx && sample_step <= 1 && !as_sparse )
      * _row_sample = sample_data;
    else {
      CV_CALL( row_sample = ( real* ) cvAlloc( vec_size ) );
      
      if ( !comp_idx )
        for ( i = 0; i < dims_selected; i++ )
          row_sample[ i ] = sample_data[ sample_step * i ];
      else {
        int* comp = comp_idx->data.i;
        if ( !sample_step )
          for ( i = 0; i < dims_selected; i++ )
            row_sample[ i ] = sample_data[ comp[ i ] ];
        else
          for ( i = 0; i < dims_selected; i++ )
            row_sample[ i ] = sample_data[ sample_step * comp[ i ] ];
      }
      
      *_row_sample = row_sample;
    }
    
    if ( as_sparse ) {
      const real * src = ( const real* ) row_sample;
      CvSparseVecElem32f* dst = ( CvSparseVecElem32f* ) row_sample;
      
      dst[ dims_selected ].idx = -1;
      for ( i = dims_selected - 1; i >= 0; i-- ) {
        dst[ i ].idx = i;
        dst[ i ].val = src[ i ];
      }
    }
  } else {
    CvSparseNode* node;
    CvSparseMatIterator mat_iterator;
    const CvSparseMat* sparse = ( const CvSparseMat* ) sample;
    assert( is_sparse );
    
    node = cvInitSparseMatIterator( sparse, &mat_iterator );
    CV_CALL( row_sample = ( real* ) cvAlloc( vec_size ) );
    
    if ( comp_idx ) {
      CV_CALL( inverse_comp_idx = ( int* ) cvAlloc( dims_all * sizeof( int ) ) );
      memset( inverse_comp_idx, -1, dims_all * sizeof( int ) );
      for ( i = 0; i < dims_selected; i++ )
        inverse_comp_idx[ comp_idx->data.i[ i ] ] = i;
    }
    
    if ( !as_sparse ) {
      memset( row_sample, 0, vec_size );
      
      for ( ; node != 0; node = cvGetNextSparseNode( &mat_iterator ) ) {
        int idx = *CV_NODE_IDX( sparse, node );
        if ( inverse_comp_idx ) {
          idx = inverse_comp_idx[ idx ];
          if ( idx < 0 )
            continue;
        }
        row_sample[ idx ] = *( real* ) CV_NODE_VAL( sparse, node );
      }
    } else {
      CvSparseVecElem32f* ptr = ( CvSparseVecElem32f* ) row_sample;
      
      for ( ; node != 0; node = cvGetNextSparseNode( &mat_iterator ) ) {
        int idx = *CV_NODE_IDX( sparse, node );
        if ( inverse_comp_idx ) {
          idx = inverse_comp_idx[ idx ];
          if ( idx < 0 )
            continue;
        }
        ptr->idx = idx;
        ptr->val = *( real* ) CV_NODE_VAL( sparse, node );
        ptr++;
      }
      
      qsort( row_sample, ptr - ( CvSparseVecElem32f* ) row_sample,
             sizeof( ptr[ 0 ] ), icvCmpSparseVecElems );
      ptr->idx = -1;
    }
    
    *_row_sample = row_sample;
  }
  
  __END__;
  
  if ( inverse_comp_idx )
    cvFree( &inverse_comp_idx );
    
  if ( cvGetErrStatus() < 0 && _row_sample ) {
    cvFree( &row_sample );
    *_row_sample = 0;
  }
}


static void
icvConvertDataToSparse( const unsigned char* src, int src_step, int src_type,
                        unsigned char* dst, int dst_step, int dst_type,
                        CvSize size, int* idx ) {
  CV_FUNCNAME( "icvConvertDataToSparse" );
  
  __BEGIN__;
  
  int i, j;
  src_type = CV_MAT_TYPE( src_type );
  dst_type = CV_MAT_TYPE( dst_type );
  
  if ( CV_MAT_CN( src_type ) != 1 || CV_MAT_CN( dst_type ) != 1 )
    CV_ERROR( CV_StsUnsupportedFormat, "The function supports only single-channel arrays" );
    
  if ( src_step == 0 )
    src_step = CV_ELEM_SIZE( src_type );
    
  if ( dst_step == 0 )
    dst_step = CV_ELEM_SIZE( dst_type );
    
  // if there is no "idx" and if both arrays are continuous,
  // do the whole processing (copying or conversion) in a single loop
  if ( !idx && CV_ELEM_SIZE( src_type ) * size.width == src_step &&
       CV_ELEM_SIZE( dst_type ) * size.width == dst_step ) {
    size.width *= size.height;
    size.height = 1;
  }
  
  if ( src_type == dst_type ) {
    int full_width = CV_ELEM_SIZE( dst_type ) * size.width;
    
    if ( full_width == sizeof( int ) )  // another common case: copy int's or real's
      for ( i = 0; i < size.height; i++, src += src_step )
        * ( int* ) ( dst + dst_step * ( idx ? idx[ i ] : i ) ) = *( int* ) src;
    else
      for ( i = 0; i < size.height; i++, src += src_step )
        memcpy( dst + dst_step * ( idx ? idx[ i ] : i ), src, full_width );
  } else if ( src_type == CV_32SC1 && ( dst_type == CV_32FC1 || dst_type == CV_64FC1 ) )
    for ( i = 0; i < size.height; i++, src += src_step ) {
      unsigned char * _dst = dst + dst_step * ( idx ? idx[ i ] : i );
      if ( dst_type == CV_32FC1 )
        for ( j = 0; j < size.width; j++ )
          ( ( real* ) _dst ) [ j ] = ( real ) ( ( int* ) src ) [ j ];
      else
        for ( j = 0; j < size.width; j++ )
          ( ( double* ) _dst ) [ j ] = ( ( int* ) src ) [ j ];
    }
  else if ( ( src_type == CV_32FC1 || src_type == CV_64FC1 ) && dst_type == CV_32SC1 )
    for ( i = 0; i < size.height; i++, src += src_step ) {
      unsigned char * _dst = dst + dst_step * ( idx ? idx[ i ] : i );
      if ( src_type == CV_32FC1 )
        for ( j = 0; j < size.width; j++ )
          ( ( int* ) _dst ) [ j ] = cvRound( ( ( real* ) src ) [ j ] );
      else
        for ( j = 0; j < size.width; j++ )
          ( ( int* ) _dst ) [ j ] = cvRound( ( ( double* ) src ) [ j ] );
    }
  else if ( src_type == CV_32FC1 && dst_type == CV_64FC1 ||
            src_type == CV_64FC1 && dst_type == CV_32FC1 )
    for ( i = 0; i < size.height; i++, src += src_step ) {
      unsigned char * _dst = dst + dst_step * ( idx ? idx[ i ] : i );
      if ( src_type == CV_32FC1 )
        for ( j = 0; j < size.width; j++ )
          ( ( double* ) _dst ) [ j ] = ( ( real* ) src ) [ j ];
      else
        for ( j = 0; j < size.width; j++ )
          ( ( real* ) _dst ) [ j ] = ( real ) ( ( double* ) src ) [ j ];
    }
  else
    CV_ERROR( CV_StsUnsupportedFormat, "Unsupported combination of input and output vectors" );
    
  __END__;
}


void
cvWritebackLabels( const CvMat* labels, CvMat* dst_labels,
                   const CvMat* centers, CvMat* dst_centers,
                   const CvMat* probs, CvMat* dst_probs,
                   const CvMat* sample_idx, int samples_all,
                   const CvMat* comp_idx, int dims_all ) {
  CV_FUNCNAME( "cvWritebackLabels" );
  
  __BEGIN__;
  
  int samples_selected = samples_all, dims_selected = dims_all;
  
  if ( dst_labels && !CV_IS_MAT( dst_labels ) )
    CV_ERROR( CV_StsBadArg, "Array of output labels is not a valid matrix" );
    
  if ( dst_centers )
    if ( !ICV_IS_MAT_OF_TYPE( dst_centers, CV_32FC1 ) &&
         !ICV_IS_MAT_OF_TYPE( dst_centers, CV_64FC1 ) )
      CV_ERROR( CV_StsBadArg, "Array of cluster centers is not a valid matrix" );
      
  if ( dst_probs && !CV_IS_MAT( dst_probs ) )
    CV_ERROR( CV_StsBadArg, "Probability matrix is not valid" );
    
  if ( sample_idx ) {
    CV_ASSERT( sample_idx->rows == 1 && CV_MAT_TYPE( sample_idx->type ) == CV_32SC1 );
    samples_selected = sample_idx->cols;
  }
  
  if ( comp_idx ) {
    CV_ASSERT( comp_idx->rows == 1 && CV_MAT_TYPE( comp_idx->type ) == CV_32SC1 );
    dims_selected = comp_idx->cols;
  }
  
  if ( dst_labels && ( !labels || labels->data.ptr != dst_labels->data.ptr ) ) {
    if ( !labels )
      CV_ERROR( CV_StsNullPtr, "NULL labels" );
      
    CV_ASSERT( labels->rows == 1 );
    
    if ( dst_labels->rows != 1 && dst_labels->cols != 1 )
      CV_ERROR( CV_StsBadSize, "Array of output labels should be 1d vector" );
      
    if ( dst_labels->rows + dst_labels->cols - 1 != samples_all )
      CV_ERROR( CV_StsUnmatchedSizes,
                "Size of vector of output labels is not equal to the total number of input samples" );
                
    CV_ASSERT( labels->cols == samples_selected );
    
    CV_CALL( icvConvertDataToSparse( labels->data.ptr, labels->step, labels->type,
                                     dst_labels->data.ptr, dst_labels->step, dst_labels->type,
                                     cvSize( 1, samples_selected ), sample_idx ? sample_idx->data.i : 0 ) );
  }
  
  if ( dst_centers && ( !centers || centers->data.ptr != dst_centers->data.ptr ) ) {
    int i;
    
    if ( !centers )
      CV_ERROR( CV_StsNullPtr, "NULL centers" );
      
    if ( centers->rows != dst_centers->rows )
      CV_ERROR( CV_StsUnmatchedSizes, "Invalid number of rows in matrix of output centers" );
      
    if ( dst_centers->cols != dims_all )
      CV_ERROR( CV_StsUnmatchedSizes,
                "Number of columns in matrix of output centers is "
                "not equal to the total number of components in the input samples" );
                
    CV_ASSERT( centers->cols == dims_selected );
    
    for ( i = 0; i < centers->rows; i++ )
      CV_CALL( icvConvertDataToSparse( centers->data.ptr + i * centers->step, 0, centers->type,
                                       dst_centers->data.ptr + i * dst_centers->step, 0, dst_centers->type,
                                       cvSize( 1, dims_selected ), comp_idx ? comp_idx->data.i : 0 ) );
  }
  
  if ( dst_probs && ( !probs || probs->data.ptr != dst_probs->data.ptr ) ) {
    if ( !probs )
      CV_ERROR( CV_StsNullPtr, "NULL probs" );
      
    if ( probs->cols != dst_probs->cols )
      CV_ERROR( CV_StsUnmatchedSizes, "Invalid number of columns in output probability matrix" );
      
    if ( dst_probs->rows != samples_all )
      CV_ERROR( CV_StsUnmatchedSizes,
                "Number of rows in output probability matrix is "
                "not equal to the total number of input samples" );
                
    CV_ASSERT( probs->rows == samples_selected );
    
    CV_CALL( icvConvertDataToSparse( probs->data.ptr, probs->step, probs->type,
                                     dst_probs->data.ptr, dst_probs->step, dst_probs->type,
                                     cvSize( probs->cols, samples_selected ),
                                     sample_idx ? sample_idx->data.i : 0 ) );
  }
  
  __END__;
}

#if 0
void
cvStatModelMultiPredict( const CvStatModel* stat_model,
                         const CvArr* predict_input,
                         int flags, CvMat* predict_output,
                         CvMat* probs, const CvMat* sample_idx ) {
  CvMemStorage * storage = 0;
  CvMat* sample_idx_buffer = 0;
  CvSparseMat** sparse_rows = 0;
  int samples_selected = 0;
  
  CV_FUNCNAME( "cvStatModelMultiPredict" );
  
  __BEGIN__;
  
  int i;
  int predict_output_step = 1, sample_idx_step = 1;
  int type;
  int d, sizes[ CV_MAX_DIM ];
  int tflag = flags == CV_COL_SAMPLE;
  int samples_all, dims_all;
  int is_sparse = CV_IS_SPARSE_MAT( predict_input );
  CvMat predict_input_part;
  CvArr* sample = &predict_input_part;
  CvMat probs_part;
  CvMat* probs1 = probs ? &probs_part : 0;
  
  if ( !CV_IS_STAT_MODEL( stat_model ) )
    CV_ERROR( !stat_model ? CV_StsNullPtr : CV_StsBadArg, "Invalid statistical model" );
    
  if ( !stat_model->predict )
    CV_ERROR( CV_StsNotImplemented, "There is no \"predict\" method" );
    
  if ( !predict_input || !predict_output )
    CV_ERROR( CV_StsNullPtr, "NULL input or output matrices" );
    
  if ( !is_sparse && !CV_IS_MAT( predict_input ) )
    CV_ERROR( CV_StsBadArg, "predict_input should be a matrix or a sparse matrix" );
    
  if ( !CV_IS_MAT( predict_output ) )
    CV_ERROR( CV_StsBadArg, "predict_output should be a matrix" );
    
  type = cvGetElemType( predict_input );
  if ( type != CV_32FC1 ||
       ( CV_MAT_TYPE( predict_output->type ) != CV_32FC1 &&
         CV_MAT_TYPE( predict_output->type ) != CV_32SC1 ) )
    CV_ERROR( CV_StsUnsupportedFormat, "The input or output matrix has unsupported format" );
    
  CV_CALL( d = cvGetDims( predict_input, sizes ) );
  if ( d > 2 )
    CV_ERROR( CV_StsBadSize, "The input matrix should be 1- or 2-dimensional" );
    
  if ( !tflag ) {
    samples_all = samples_selected = sizes[ 0 ];
    dims_all = sizes[ 1 ];
  } else {
    samples_all = samples_selected = sizes[ 1 ];
    dims_all = sizes[ 0 ];
  }
  
  if ( sample_idx ) {
    if ( !CV_IS_MAT( sample_idx ) )
      CV_ERROR( CV_StsBadArg, "Invalid sample_idx matrix" );
      
    if ( sample_idx->cols != 1 && sample_idx->rows != 1 )
      CV_ERROR( CV_StsBadSize, "sample_idx must be 1-dimensional matrix" );
      
    samples_selected = sample_idx->rows + sample_idx->cols - 1;
    
    if ( CV_MAT_TYPE( sample_idx->type ) == CV_32SC1 ) {
      if ( samples_selected > samples_all )
        CV_ERROR( CV_StsBadSize, "sample_idx is too large vector" );
    } else if ( samples_selected != samples_all )
      CV_ERROR( CV_StsUnmatchedSizes, "sample_idx has incorrect size" );
      
    sample_idx_step = sample_idx->step ?
                      sample_idx->step / CV_ELEM_SIZE( sample_idx->type ) : 1;
  }
  
  if ( predict_output->rows != 1 && predict_output->cols != 1 )
    CV_ERROR( CV_StsBadSize, "predict_output should be a 1-dimensional matrix" );
    
  if ( predict_output->rows + predict_output->cols - 1 != samples_all )
    CV_ERROR( CV_StsUnmatchedSizes, "predict_output and predict_input have uncoordinated sizes" );
    
  predict_output_step = predict_output->step ?
                        predict_output->step / CV_ELEM_SIZE( predict_output->type ) : 1;
                        
  if ( probs ) {
    if ( !CV_IS_MAT( probs ) )
      CV_ERROR( CV_StsBadArg, "Invalid matrix of probabilities" );
      
    if ( probs->rows != samples_all )
      CV_ERROR( CV_StsUnmatchedSizes,
                "matrix of probabilities must have as many rows as the total number of samples" );
                
    if ( CV_MAT_TYPE( probs->type ) != CV_32FC1 )
      CV_ERROR( CV_StsUnsupportedFormat, "matrix of probabilities must have 32fC1 type" );
  }
  
  if ( is_sparse ) {
    CvSparseNode * node;
    CvSparseMatIterator mat_iterator;
    CvSparseMat* sparse = ( CvSparseMat* ) predict_input;
    
    if ( sample_idx && CV_MAT_TYPE( sample_idx->type ) == CV_32SC1 ) {
      CV_CALL( sample_idx_buffer = cvCreateMat( 1, samples_all, CV_8UC1 ) );
      cvZero( sample_idx_buffer );
      for ( i = 0; i < samples_selected; i++ )
        sample_idx_buffer->data.ptr[ sample_idx->data.i[ i * sample_idx_step ] ] = 1;
      samples_selected = samples_all;
      sample_idx = sample_idx_buffer;
      sample_idx_step = 1;
    }
    
    CV_CALL( sparse_rows = ( CvSparseMat** ) cvAlloc( samples_selected * sizeof( sparse_rows[ 0 ] ) ) );
    for ( i = 0; i < samples_selected; i++ ) {
      if ( sample_idx && sample_idx->data.ptr[ i * sample_idx_step ] == 0 )
        continue;
      CV_CALL( sparse_rows[ i ] = cvCreateSparseMat( 1, &dims_all, type ) );
      if ( !storage )
        storage = sparse_rows[ i ] ->heap->storage;
      else {
        // hack: to decrease memory footprint, make all the sparse matrices
        // reside in the same storage
        int elem_size = sparse_rows[ i ] ->heap->elem_size;
        cvReleaseMemStorage( &sparse_rows[ i ] ->heap->storage );
        sparse_rows[ i ] ->heap = cvCreateSet( 0, sizeof( CvSet ), elem_size, storage );
      }
    }
    
    // put each row (or column) of predict_input into separate sparse matrix.
    node = cvInitSparseMatIterator( sparse, &mat_iterator );
    for ( ; node != 0; node = cvGetNextSparseNode( &mat_iterator ) ) {
      int * idx = CV_NODE_IDX( sparse, node );
      int idx0 = idx[ tflag ^ 1 ];
      int idx1 = idx[ tflag ];
      
      if ( sample_idx && sample_idx->data.ptr[ idx0 * sample_idx_step ] == 0 )
        continue;
        
      assert( sparse_rows[ idx0 ] != 0 );
      *( real* ) cvPtrND( sparse, &idx1, 0, 1, 0 ) = *( real* ) CV_NODE_VAL( sparse, node );
    }
  }
  
  for ( i = 0; i < samples_selected; i++ ) {
    int idx = i;
    real response;
    
    if ( sample_idx ) {
      if ( CV_MAT_TYPE( sample_idx->type ) == CV_32SC1 ) {
        idx = sample_idx->data.i[ i * sample_idx_step ];
        if ( ( unsigned ) idx >= ( unsigned ) samples_all )
          CV_ERROR( CV_StsOutOfRange, "Some of sample_idx elements are out of range" );
      } else if ( CV_MAT_TYPE( sample_idx->type ) == CV_8UC1 &&
                  sample_idx->data.ptr[ i * sample_idx_step ] == 0 )
        continue;
    }
    
    if ( !is_sparse ) {
      if ( !tflag )
        cvGetRow( predict_input, &predict_input_part, idx );
      else {
        cvGetCol( predict_input, &predict_input_part, idx );
      }
    } else
      sample = sparse_rows[ idx ];
      
    if ( probs )
      cvGetRow( probs, probs1, idx );
      
    CV_CALL( response = stat_model->predict( stat_model, ( CvMat* ) sample, probs1 ) );
    
    if ( CV_MAT_TYPE( predict_output->type ) == CV_32FC1 )
      predict_output->data.fl[ idx * predict_output_step ] = response;
    else {
      CV_ASSERT( cvRound( response ) == response );
      predict_output->data.i[ idx * predict_output_step ] = cvRound( response );
    }
  }
  
  __END__;
  
  if ( sparse_rows ) {
    int i;
    for ( i = 0; i < samples_selected; i++ )
      if ( sparse_rows[ i ] ) {
        sparse_rows[ i ] ->heap->storage = 0;
        cvReleaseSparseMat( &sparse_rows[ i ] );
      }
    cvFree( &sparse_rows );
  }
  
  cvReleaseMat( &sample_idx_buffer );
  cvReleaseMemStorage( &storage );
}
#endif

// By P. Yarykin - begin -

void cvCombineResponseMaps ( CvMat* _responses,
                             const CvMat* old_response_map,
                             CvMat* new_response_map,
                             CvMat** out_response_map ) {
  int** old_data = NULL;
  int** new_data = NULL;
  
  CV_FUNCNAME ( "cvCombineResponseMaps" );
  __BEGIN__
  
  int i, j;
  int old_n, new_n, out_n;
  int samples, free_response;
  int* first;
  int* responses;
  int* out_data;
  
  if ( out_response_map )
    * out_response_map = 0;
    
  // Check input data.
  if ( ( !ICV_IS_MAT_OF_TYPE ( _responses, CV_32SC1 ) ) ||
       ( !ICV_IS_MAT_OF_TYPE ( old_response_map, CV_32SC1 ) ) ||
       ( !ICV_IS_MAT_OF_TYPE ( new_response_map, CV_32SC1 ) ) ) {
    CV_ERROR ( CV_StsBadArg, "Some of input arguments is not the CvMat" )
  }
  
  // Prepare sorted responses.
  first = new_response_map->data.i;
  new_n = new_response_map->cols;
  CV_CALL ( new_data = ( int** ) cvAlloc ( new_n * sizeof ( new_data[ 0 ] ) ) );
  for ( i = 0; i < new_n; i++ )
    new_data[ i ] = first + i;
  qsort ( new_data, new_n, sizeof( int* ), icvCmpIntegersPtr );
  
  first = old_response_map->data.i;
  old_n = old_response_map->cols;
  CV_CALL ( old_data = ( int** ) cvAlloc ( old_n * sizeof ( old_data[ 0 ] ) ) );
  for ( i = 0; i < old_n; i++ )
    old_data[ i ] = first + i;
  qsort ( old_data, old_n, sizeof( int* ), icvCmpIntegersPtr );
  
  // Count the number of different responses.
  for ( i = 0, j = 0, out_n = 0; i < old_n && j < new_n; out_n++ ) {
    if ( *old_data[ i ] == *new_data[ j ] ) {
      i++;
      j++;
    } else if ( *old_data[ i ] < *new_data[ j ] )
      i++;
    else
      j++;
  }
  out_n += old_n - i + new_n - j;
  
  // Create and fill the result response maps.
  CV_CALL ( *out_response_map = cvCreateMat ( 1, out_n, CV_32SC1 ) );
  out_data = ( *out_response_map ) ->data.i;
  memcpy ( out_data, first, old_n * sizeof ( int ) );
  
  free_response = old_n;
  for ( i = 0, j = 0; i < old_n && j < new_n; ) {
    if ( *old_data[ i ] == *new_data[ j ] ) {
      *new_data[ j ] = ( int ) ( old_data[ i ] - first );
      i++;
      j++;
    } else if ( *old_data[ i ] < *new_data[ j ] )
      i++;
    else {
      out_data[ free_response ] = *new_data[ j ];
      *new_data[ j ] = free_response++;
      j++;
    }
  }
  for ( ; j < new_n; j++ ) {
    out_data[ free_response ] = *new_data[ j ];
    *new_data[ j ] = free_response++;
  }
  CV_ASSERT ( free_response == out_n );
  
  // Change <responses> according to out response map.
  samples = _responses->cols + _responses->rows - 1;
  responses = _responses->data.i;
  first = new_response_map->data.i;
  for ( i = 0; i < samples; i++ ) {
    responses[ i ] = first[ responses[ i ] ];
  }
  
  __END__
  
  cvFree( &old_data );
  cvFree( &new_data );
  
}


int icvGetNumberOfCluster( double* prob_vector, int num_of_clusters, real r,
                           real outlier_thresh, int normalize_probs ) {
  int max_prob_loc = 0;
  
  CV_FUNCNAME( "icvGetNumberOfCluster" );
  __BEGIN__;
  
  double prob, maxprob, sum;
  int i;
  
  CV_ASSERT( prob_vector );
  CV_ASSERT( num_of_clusters >= 0 );
  
  maxprob = prob_vector[ 0 ];
  max_prob_loc = 0;
  sum = maxprob;
  for ( i = 1; i < num_of_clusters; i++ ) {
    prob = prob_vector[ i ];
    sum += prob;
    if ( prob > maxprob ) {
      max_prob_loc = i;
      maxprob = prob;
    }
  }
  if ( normalize_probs && fabs( sum - 1. ) > FLT_EPSILON ) {
    for ( i = 0; i < num_of_clusters; i++ )
      prob_vector[ i ] /= sum;
  }
  if ( fabs( r - 1. ) > FLT_EPSILON && fabs( sum - 1. ) < outlier_thresh )
    max_prob_loc = -1;
    
  __END__;
  
  return max_prob_loc;
  
} // End of icvGetNumberOfCluster


void icvFindClusterLabels( const CvMat* probs, real outlier_thresh, real r,
                           const CvMat* labels ) {
  CvMat * counts = 0;
  
  CV_FUNCNAME( "icvFindClusterLabels" );
  __BEGIN__;
  
  int nclusters, nsamples;
  int i, j;
  double* probs_data;
  
  CV_ASSERT( ICV_IS_MAT_OF_TYPE( probs, CV_64FC1 ) );
  CV_ASSERT( ICV_IS_MAT_OF_TYPE( labels, CV_32SC1 ) );
  
  nclusters = probs->cols;
  nsamples = probs->rows;
  CV_ASSERT( nsamples == labels->cols );
  
  CV_CALL( counts = cvCreateMat( 1, nclusters + 1, CV_32SC1 ) );
  CV_CALL( cvSetZero( counts ) );
  for ( i = 0; i < nsamples; i++ ) {
    labels->data.i[ i ] = icvGetNumberOfCluster( probs->data.db + i * probs->cols,
                          nclusters, r, outlier_thresh, 1 );
    counts->data.i[ labels->data.i[ i ] + 1 ] ++;
  }
  CV_ASSERT( ( int ) cvSum( counts ).val[ 0 ] == nsamples );
  // Filling empty clusters with the vector, that has the maximal probability
  for ( j = 0; j < nclusters; j++ )  // outliers are ignored
  {
    int maxprob_loc = -1;
    double maxprob = 0;
    
    if ( counts->data.i[ j + 1 ] )  // j-th class is not empty
      continue;
    // look for the presentative, which is not lonely in it's cluster
    // and that has a maximal probability among all these vectors
    probs_data = probs->data.db;
    for ( i = 0; i < nsamples; i++, probs_data++ ) {
      int label = labels->data.i[ i ];
      double prob;
      if ( counts->data.i[ label + 1 ] == 0 ||
           ( counts->data.i[ label + 1 ] <= 1 && label != -1 ) )
        continue;
      prob = *probs_data;
      if ( prob >= maxprob ) {
        maxprob = prob;
        maxprob_loc = i;
      }
    }
    // maxprob_loc == 0 <=> number of vectors less then number of clusters
    CV_ASSERT( maxprob_loc >= 0 );
    counts->data.i[ labels->data.i[ maxprob_loc ] + 1 ] --;
    labels->data.i[ maxprob_loc ] = j;
    counts->data.i[ j + 1 ] ++;
  }
  
  __END__;
  
  cvReleaseMat( &counts );
} // End of icvFindClusterLabels

#endif
