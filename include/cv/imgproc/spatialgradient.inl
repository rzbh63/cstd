


#include "opencv2/core/hal/intrin.hpp"

#include <iostream>
namespace cv
{

/* NOTE:
 *
 * Sobel-x: -1  0  1
 *          -2  0  2
 *          -1  0  1
 *
 * Sobel-y: -1 -2 -1
 *           0  0  0
 *           1  2  1
 */
template <typename T>
static inline void spatialGradientKernel( T& vx, T& vy,
                                          const T& v00, const T& v01, const T& v02,
                                          const T& v10,               const T& v12,
                                          const T& v20, const T& v21, const T& v22 )
{
    // vx = (v22 - v00) + (v02 - v20) + 2 * (v12 - v10)
    // vy = (v22 - v00) + (v20 - v02) + 2 * (v21 - v01)

    T tmp_add = v22 - v00,
      tmp_sub = v02 - v20,
      tmp_x   = v12 - v10,
      tmp_y   = v21 - v01;

    vx = tmp_add + tmp_sub + tmp_x + tmp_x;
    vy = tmp_add - tmp_sub + tmp_y + tmp_y;
}

void spatialGradient( const img_t* _src, img_t* dx, img_t* dy,
                      int ksize, int borderType )
{
    CC_INSTRUMENT_REGION()

    // Prepare const img_t* src
    img_t *src = _src;
    CC_Assert( !src.empty() );
    CC_Assert( src->tid == CC_8UC1 );
    CC_Assert( borderType == CC_BORDER_DEFAULT || borderType == CC_BORDER_REPLICATE );

    // Prepare OutputArrays dx, dy
    _dx cvSetMat( cvGetSize(src), CC_16SC1 );
    _dy cvSetMat( cvGetSize(src), CC_16SC1 );
    img_t dx = _dx,
        dy = _dy;

    // TODO: Allow for other kernel sizes
    CC_Assert(ksize == 3);

    // Get dimensions
    const int H = src->rows,
              W = src->cols;

    // Row, column indices
    int i = 0,
        j = 0;

    // Handle border types
    int i_top    = 0,     // Case for H == 1 && W == 1 && CC_BORDER_REPLICATE
        i_bottom = H - 1,
        j_offl   = 0,     // j offset from 0th   pixel to reach -1st pixel
        j_offr   = 0;     // j offset from W-1th pixel to reach Wth  pixel

    if ( borderType == CC_BORDER_DEFAULT ) // Equiv. to CC_BORDER_REFLECT_101
    {
        if ( H > 1 )
        {
            i_top    = 1;
            i_bottom = H - 2;
        }
        if ( W > 1 )
        {
            j_offl = 1;
            j_offr = -1;
        }
    }

    // Pointer to row vectors
    uchar *p_src, *c_src, *n_src; // previous, current, next row
    short *c_dx,  *c_dy;

    int i_start = 0;
    int j_start = 0;
#if CC_SIMD128 && CC_SSE2
    if(hasSIMD128())
    {
        uchar *m_src;
        short *n_dx, *n_dy;

        // Characters in variable names have the following meanings:
        // u: unsigned char
        // s: signed int
        //
        // [row][column]
        // m: offset -1
        // n: offset  0
        // p: offset  1
        // Example: umn is offset -1 in row and offset 0 in column
        for ( i = 0; i < H - 1; i += 2 )
        {
            if   ( i == 0 ) p_src = src.ptr<uchar>(i_top);
            else            p_src = src.ptr<uchar>(i-1);

            c_src = src.ptr<uchar>(i);
            n_src = src.ptr<uchar>(i+1);

            if ( i == H - 2 ) m_src = src.ptr<uchar>(i_bottom);
            else              m_src = src.ptr<uchar>(i+2);

            c_dx = dx.ptr<short>(i);
            c_dy = dy.ptr<short>(i);
            n_dx = dx.ptr<short>(i+1);
            n_dy = dy.ptr<short>(i+1);

            v_uint8x16 v_select_m = v_uint8x16(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                               0, 0, 0, 0xFF);

            // Process rest of columns 16-column chunks at a time
            for ( j = 1; j < W - 16; j += 16 )
            {
                // Load top row for 3x3 Sobel filter
                v_uint8x16 v_um = v_load(&p_src[j-1]);
                v_uint8x16 v_up = v_load(&p_src[j+1]);
                // TODO: Replace _mm_slli_si128 with hal method
                v_uint8x16 v_un = v_select(v_select_m, v_uint8x16(_mm_slli_si128(v_up.val, 1)),
                                                       v_uint8x16(_mm_srli_si128(v_um.val, 1)));
                v_uint16x8 v_um1, v_um2, v_un1, v_un2, v_up1, v_up2;
                v_expand(v_um, v_um1, v_um2);
                v_expand(v_un, v_un1, v_un2);
                v_expand(v_up, v_up1, v_up2);
                v_int16x8 v_s1m1 = v_reinterpret_as_s16(v_um1);
                v_int16x8 v_s1m2 = v_reinterpret_as_s16(v_um2);
                v_int16x8 v_s1n1 = v_reinterpret_as_s16(v_un1);
                v_int16x8 v_s1n2 = v_reinterpret_as_s16(v_un2);
                v_int16x8 v_s1p1 = v_reinterpret_as_s16(v_up1);
                v_int16x8 v_s1p2 = v_reinterpret_as_s16(v_up2);

                // Load second row for 3x3 Sobel filter
                v_um = v_load(&c_src[j-1]);
                v_up = v_load(&c_src[j+1]);
                // TODO: Replace _mm_slli_si128 with hal method
                v_un = v_select(v_select_m, v_uint8x16(_mm_slli_si128(v_up.val, 1)),
                                            v_uint8x16(_mm_srli_si128(v_um.val, 1)));
                v_expand(v_um, v_um1, v_um2);
                v_expand(v_un, v_un1, v_un2);
                v_expand(v_up, v_up1, v_up2);
                v_int16x8 v_s2m1 = v_reinterpret_as_s16(v_um1);
                v_int16x8 v_s2m2 = v_reinterpret_as_s16(v_um2);
                v_int16x8 v_s2n1 = v_reinterpret_as_s16(v_un1);
                v_int16x8 v_s2n2 = v_reinterpret_as_s16(v_un2);
                v_int16x8 v_s2p1 = v_reinterpret_as_s16(v_up1);
                v_int16x8 v_s2p2 = v_reinterpret_as_s16(v_up2);

                // Load third row for 3x3 Sobel filter
                v_um = v_load(&n_src[j-1]);
                v_up = v_load(&n_src[j+1]);
                // TODO: Replace _mm_slli_si128 with hal method
                v_un = v_select(v_select_m, v_uint8x16(_mm_slli_si128(v_up.val, 1)),
                                            v_uint8x16(_mm_srli_si128(v_um.val, 1)));
                v_expand(v_um, v_um1, v_um2);
                v_expand(v_un, v_un1, v_un2);
                v_expand(v_up, v_up1, v_up2);
                v_int16x8 v_s3m1 = v_reinterpret_as_s16(v_um1);
                v_int16x8 v_s3m2 = v_reinterpret_as_s16(v_um2);
                v_int16x8 v_s3n1 = v_reinterpret_as_s16(v_un1);
                v_int16x8 v_s3n2 = v_reinterpret_as_s16(v_un2);
                v_int16x8 v_s3p1 = v_reinterpret_as_s16(v_up1);
                v_int16x8 v_s3p2 = v_reinterpret_as_s16(v_up2);

                // dx & dy for rows 1, 2, 3
                v_int16x8 v_sdx1, v_sdy1;
                spatialGradientKernel<v_int16x8>( v_sdx1, v_sdy1,
                                                  v_s1m1, v_s1n1, v_s1p1,
                                                  v_s2m1,         v_s2p1,
                                                  v_s3m1, v_s3n1, v_s3p1 );

                v_int16x8 v_sdx2, v_sdy2;
                spatialGradientKernel<v_int16x8>( v_sdx2, v_sdy2,
                                                  v_s1m2, v_s1n2, v_s1p2,
                                                  v_s2m2,         v_s2p2,
                                                  v_s3m2, v_s3n2, v_s3p2 );

                // Store
                v_store(&c_dx[j],   v_sdx1);
                v_store(&c_dx[j+8], v_sdx2);
                v_store(&c_dy[j],   v_sdy1);
                v_store(&c_dy[j+8], v_sdy2);

                // Load fourth row for 3x3 Sobel filter
                v_um = v_load(&m_src[j-1]);
                v_up = v_load(&m_src[j+1]);
                // TODO: Replace _mm_slli_si128 with hal method
                v_un = v_select(v_select_m, v_uint8x16(_mm_slli_si128(v_up.val, 1)),
                                            v_uint8x16(_mm_srli_si128(v_um.val, 1)));
                v_expand(v_um, v_um1, v_um2);
                v_expand(v_un, v_un1, v_un2);
                v_expand(v_up, v_up1, v_up2);
                v_int16x8 v_s4m1 = v_reinterpret_as_s16(v_um1);
                v_int16x8 v_s4m2 = v_reinterpret_as_s16(v_um2);
                v_int16x8 v_s4n1 = v_reinterpret_as_s16(v_un1);
                v_int16x8 v_s4n2 = v_reinterpret_as_s16(v_un2);
                v_int16x8 v_s4p1 = v_reinterpret_as_s16(v_up1);
                v_int16x8 v_s4p2 = v_reinterpret_as_s16(v_up2);

                // dx & dy for rows 2, 3, 4
                spatialGradientKernel<v_int16x8>( v_sdx1, v_sdy1,
                                                  v_s2m1, v_s2n1, v_s2p1,
                                                  v_s3m1,         v_s3p1,
                                                  v_s4m1, v_s4n1, v_s4p1 );

                spatialGradientKernel<v_int16x8>( v_sdx2, v_sdy2,
                                                  v_s2m2, v_s2n2, v_s2p2,
                                                  v_s3m2,         v_s3p2,
                                                  v_s4m2, v_s4n2, v_s4p2 );

                // Store
                v_store(&n_dx[j],   v_sdx1);
                v_store(&n_dx[j+8], v_sdx2);
                v_store(&n_dy[j],   v_sdy1);
                v_store(&n_dy[j+8], v_sdy2);
            }
        }
    }
    i_start = i;
    j_start = j;
#endif
    int j_p, j_n;
    uchar v00, v01, v02, v10, v11, v12, v20, v21, v22;
    for ( i = 0; i < H; i++ )
    {
        if   ( i == 0 ) p_src = src.ptr<uchar>(i_top);
        else            p_src = src.ptr<uchar>(i-1);

        c_src = src.ptr<uchar>(i);

        if ( i == H - 1 ) n_src = src.ptr<uchar>(i_bottom);
        else              n_src = src.ptr<uchar>(i+1);

        c_dx = dx.ptr<short>(i);
        c_dy = dy.ptr<short>(i);

        // Process left-most column
        j = 0;
        j_p = j + j_offl;
        j_n = 1;
        if ( j_n >= W ) j_n = j + j_offr;
        v00 = p_src[j_p]; v01 = p_src[j]; v02 = p_src[j_n];
        v10 = c_src[j_p]; v11 = c_src[j]; v12 = c_src[j_n];
        v20 = n_src[j_p]; v21 = n_src[j]; v22 = n_src[j_n];
        spatialGradientKernel<short>( c_dx[0], c_dy[0], v00, v01, v02, v10,
                                      v12, v20, v21, v22 );
        v00 = v01; v10 = v11; v20 = v21;
        v01 = v02; v11 = v12; v21 = v22;

        // Process middle columns
        j = i >= i_start ? 1 : j_start;
        j_p = j - 1;
        v00 = p_src[j_p]; v01 = p_src[j];
        v10 = c_src[j_p]; v11 = c_src[j];
        v20 = n_src[j_p]; v21 = n_src[j];

        for ( ; j < W - 1; j++ )
        {
            // Get values for next column
            j_n = j + 1; v02 = p_src[j_n]; v12 = c_src[j_n]; v22 = n_src[j_n];
            spatialGradientKernel<short>( c_dx[j], c_dy[j], v00, v01, v02, v10,
                                          v12, v20, v21, v22 );

            // Move values back one column for next iteration
            v00 = v01; v10 = v11; v20 = v21;
            v01 = v02; v11 = v12; v21 = v22;
        }

        // Process right-most column
        if ( j < W )
        {
            j_n = j + j_offr; v02 = p_src[j_n]; v12 = c_src[j_n]; v22 = n_src[j_n];
            spatialGradientKernel<short>( c_dx[j], c_dy[j], v00, v01, v02, v10,
                                          v12, v20, v21, v22 );
        }
    }

}

}
