
/* 
   Finds L1 norm between two blocks.
*/
static int
icvCmpBlocksL1_8u_C1(const uchar * vec1, const uchar * vec2, int len)
{
    int i, sum = 0;

    for(i = 0; i <= len - 4; i += 4)
    {
        int t0 = abs(vec1[i] - vec2[i]);
        int t1 = abs(vec1[i + 1] - vec2[i + 1]);
        int t2 = abs(vec1[i + 2] - vec2[i + 2]);
        int t3 = abs(vec1[i + 3] - vec2[i + 3]);

        sum += t0 + t1 + t2 + t3;
    }

    for(; i < len; i++)
    {
        int t0 = abs(vec1[i] - vec2[i]);
        sum += t0;
    }

    return sum;
}


static void
icvCopyBM_8u_C1R(const uchar* src, int src_step,
                  uchar* dst, int dst_step, CSize size)
{
    for(; size.height--; src += src_step, dst += dst_step)
        memcpy(dst, src, size.width);
}


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name: icvCalcOpticalFlowBM_8u32fR
//    Purpose: calculate Optical flow for 2 images using block matching algorithm
//    Context:
//    Parameters:
//            imgA,         // pointer to first frame ROI
//            imgB,         // pointer to second frame ROI
//            imgStep,      // full width of input images in bytes
//            imgSize,      // size of the image
//            blockSize,    // size of basic blocks which are compared
//            shiftSize,    // coordinates increments.
//            maxRange,     // size of the scanned neighborhood.
//            usePrevious,  // flag of using previous velocity field
//            velocityX,    //  pointer to ROI of horizontal and
//            velocityY,    //  vertical components of optical flow
//            velStep);     //  full width of velocity frames in bytes
//    Returns: CC_OK or error code
//    Notes:
//F*/
#define SMALL_DIFF 2
#define BIG_DIFF 128

static CStatus C_STDCALL
icvCalcOpticalFlowBM_8u32fR(uchar * imgA, uchar * imgB,
                             int imgStep, CSize imgSize,
                             CSize blockSize, CSize shiftSize,
                             CSize maxRange, int usePrev,
                             float *velocityX, float *velocityY,
                             int velStep)
{
    const float back = 1.f / (float) (1 << 16);

    /* scanning scheme coordinates */

    CPoint *ss = 0;
    int ss_count = 0;

    int stand_accept_level = blockSize.height * blockSize.width * SMALL_DIFF;
    int stand_escape_level = blockSize.height * blockSize.width * BIG_DIFF;

    int i, j;

    int *int_velocityX = (int *) velocityX;
    int *int_velocityY = (int *) velocityY;

    /* if image sizes can't be divided by block sizes then right blocks will  */
    /* have not full width  - BorderWidth                                     */
    /* and bottom blocks will                                                 */
    /* have not full height - BorderHeight                                    */
    int BorderWidth;
    int BorderHeight;

    int CurrentWidth;
    int CurrentHeight;

    int NumberBlocksX;
    int NumberBlocksY;

    int Y1 = 0;
    int X1 = 0;

    int DownStep = blockSize.height * imgStep;

    uchar *blockA = 0;
    uchar *blockB = 0;
    uchar *blockZ = 0;
    int blSize = blockSize.width * blockSize.height;
    int bufferSize = cvAlign(blSize + 9,16);
    int cmpSize = cvAlign(blSize,4);
    int patch_ofs = blSize & -8;
    int64 patch_mask = (((int64) 1) << (blSize - patch_ofs * 8)) - 1;

    velStep /= sizeof(velocityX[0]);

    if(patch_ofs == blSize)
        patch_mask = (int64) - 1;

/****************************************************************************************\
*   Checking bad arguments                                                               *
\****************************************************************************************/
    if(imgA == NULL)
        return CC_NULLPTR_ERR;
    if(imgB == NULL)
        return CC_NULLPTR_ERR;

/****************************************************************************************\
*   Allocate buffers                                                                     *
\****************************************************************************************/
    blockA = (uchar *) cAlloc(bufferSize * 3);
    if(!blockA)
        return CC_OUTOFMEM_ERR;

    blockB = blockA + bufferSize;
    blockZ = blockB + bufferSize;

    memset(blockZ, 0, bufferSize);

    ss = (CPoint *) cAlloc((2 * maxRange.width + 1) * (2 * maxRange.height + 1) *
                               sizeof(CPoint));
    if(!ss)
    {
        cFree(&blockA);
        return CC_OUTOFMEM_ERR;
    }

/****************************************************************************************\
*   Calculate scanning scheme                                                            *
\****************************************************************************************/
    {
        int X_shift_count = maxRange.width / shiftSize.width;
        int Y_shift_count = maxRange.height / shiftSize.height;
        int min_count = MIN(X_shift_count, Y_shift_count);

        /* cycle by neighborhood rings */
        /* scanning scheme is 

           . 9  10 11 12
           . 8  1  2  13
           . 7  *  3  14
           . 6  5  4  15      
           20 19 18 17 16
         */

        for(i = 0; i < min_count; i++)
        {
            /* four cycles along sides */
            int y = -(i + 1) * shiftSize.height;
            int x = -(i + 1) * shiftSize.width;

            /* upper side */
            for(j = -i; j <= i + 1; j++, ss_count++)
            {
                x += shiftSize.width;
                ss[ss_count].x = x;
                ss[ss_count].y = y;
            }

            /* right side */
            for(j = -i; j <= i + 1; j++, ss_count++)
            {
                y += shiftSize.height;
                ss[ss_count].x = x;
                ss[ss_count].y = y;
            }

            /* bottom side */
            for(j = -i; j <= i + 1; j++, ss_count++)
            {
                x -= shiftSize.width;
                ss[ss_count].x = x;
                ss[ss_count].y = y;
            }

            /* left side */
            for(j = -i; j <= i + 1; j++, ss_count++)
            {
                y -= shiftSize.height;
                ss[ss_count].x = x;
                ss[ss_count].y = y;
            }
        }

        /* the rest part */
        if(X_shift_count < Y_shift_count)
        {
            int xleft = -min_count * shiftSize.width;

            /* cycle by neighbor rings */
            for(i = min_count; i < Y_shift_count; i++)
            {
                /* two cycles by x */
                int y = -(i + 1) * shiftSize.height;
                int x = xleft;

                /* upper side */
                for(j = -X_shift_count; j <= X_shift_count; j++, ss_count++)
                {
                    ss[ss_count].x = x;
                    ss[ss_count].y = y;
                    x += shiftSize.width;
                }

                x = xleft;
                y = -y;
                /* bottom side */
                for(j = -X_shift_count; j <= X_shift_count; j++, ss_count++)
                {
                    ss[ss_count].x = x;
                    ss[ss_count].y = y;
                    x += shiftSize.width;
                }
            }
        }
        else if(X_shift_count > Y_shift_count)
        {
            int yupper = -min_count * shiftSize.height;

            /* cycle by neighbor rings */
            for(i = min_count; i < X_shift_count; i++)
            {
                /* two cycles by y */
                int x = -(i + 1) * shiftSize.width;
                int y = yupper;

                /* left side */
                for(j = -Y_shift_count; j <= Y_shift_count; j++, ss_count++)
                {
                    ss[ss_count].x = x;
                    ss[ss_count].y = y;
                    y += shiftSize.height;
                }

                y = yupper;
                x = -x;
                /* right side */
                for(j = -Y_shift_count; j <= Y_shift_count; j++, ss_count++)
                {
                    ss[ss_count].x = x;
                    ss[ss_count].y = y;
                    y += shiftSize.height;
                }
            }
        }

    }

/****************************************************************************************\
*   Calculate some neeeded variables                                                     *
\****************************************************************************************/
    /* Calculate number of full blocks */
    NumberBlocksX = (int) imgSize.width / blockSize.width;
    NumberBlocksY = (int) imgSize.height / blockSize.height;

    /* add 1 if not full border blocks exist */
    BorderWidth = imgSize.width % blockSize.width;
    if(BorderWidth)
        NumberBlocksX++;
    else
        BorderWidth = blockSize.width;

    BorderHeight = imgSize.height % blockSize.height;
    if(BorderHeight)
        NumberBlocksY++;
    else
        BorderHeight = blockSize.height;

/****************************************************************************************\
* Round input velocities integer searching area center position                          *
\****************************************************************************************/
    if(usePrev)
    {
        float *velxf = velocityX, *velyf = velocityY;
        int* velx = (int*)velocityX, *vely = (int*)velocityY;

        for(i = 0; i < NumberBlocksY; i++, velxf += velStep, velyf += velStep,
                                            velx += velStep, vely += velStep)
        {
            for(j = 0; j < NumberBlocksX; j++)
            {
                int vx = cRound(velxf[j]), vy = cRound(velyf[j]);
                velx[j] = vx; vely[j] = vy;
            }
        }
    }
/****************************************************************************************\
* Main loop                                                                              *
\****************************************************************************************/
    Y1 = 0;
    for(i = 0; i < NumberBlocksY; i++)
    {
        /* calculate height of current block */
        CurrentHeight = (i == NumberBlocksY - 1) ? BorderHeight : blockSize.height;
        X1 = 0;

        for(j = 0; j < NumberBlocksX; j++)
        {
            int accept_level;
            int escape_level;

            int blDist;

            int VelocityX = 0;
            int VelocityY = 0;

            int offX = 0, offY = 0;

            int CountDirection = 1;

            int main_flag = i < NumberBlocksY - 1 && j < NumberBlocksX - 1;
            CSize CurSize;

            /* calculate width of current block */
            CurrentWidth = (j == NumberBlocksX - 1) ? BorderWidth : blockSize.width;

            /* compute initial offset */
            if(usePrev)
            {
                offX = int_velocityX[j];
                offY = int_velocityY[j];
            }

            CurSize.width = CurrentWidth;
            CurSize.height = CurrentHeight;

            if(main_flag)
            {
                icvCopyBM_8u_C1R(imgA + X1, imgStep, blockA,
                                  CurSize.width, CurSize);
                icvCopyBM_8u_C1R(imgB + (Y1 + offY)*imgStep + (X1 + offX),
                                  imgStep, blockB, CurSize.width, CurSize);

                *((int64 *) (blockA + patch_ofs)) &= patch_mask;
                *((int64 *) (blockB + patch_ofs)) &= patch_mask;
            }
            else
            {
                memset(blockA, 0, bufferSize);
                memset(blockB, 0, bufferSize);

                icvCopyBM_8u_C1R(imgA + X1, imgStep, blockA, blockSize.width, CurSize);
                icvCopyBM_8u_C1R(imgB + (Y1 + offY) * imgStep + (X1 + offX), imgStep,
                                  blockB, blockSize.width, CurSize);
            }

            if(!main_flag)
            {
                int tmp = CurSize.width * CurSize.height;

                accept_level = tmp * SMALL_DIFF;
                escape_level = tmp * BIG_DIFF;
            }
            else
            {
                accept_level = stand_accept_level;
                escape_level = stand_escape_level;
            }

            blDist = icvCmpBlocksL1_8u_C1(blockA, blockB, cmpSize);

            if(blDist > accept_level)
            {
                int k;
                int VelX = 0;
                int VelY = 0;

                /* walk around basic block */

                /* cycle for neighborhood */
                for(k = 0; k < ss_count; k++)
                {
                    int tmpDist;

                    int Y2 = Y1 + offY + ss[k].y;
                    int X2 = X1 + offX + ss[k].x;

                    /* if we break upper border */
                    if(Y2 < 0)
                    {
                        continue;
                    }
                    /* if we break bottom border */
                    if(Y2 + CurrentHeight >= imgSize.height)
                    {
                        continue;
                    }
                    /* if we break left border */
                    if(X2 < 0)
                    {
                        continue;
                    }
                    /* if we break right border */
                    if(X2 + CurrentWidth >= imgSize.width)
                    {
                        continue;
                    }

                    if(main_flag)
                    {
                        icvCopyBM_8u_C1R(imgB + Y2 * imgStep + X2,
                                          imgStep, blockB, CurSize.width, CurSize);

                        *((int64 *) (blockB + patch_ofs)) &= patch_mask;
                    }
                    else
                    {
                        memset(blockB, 0, bufferSize);
                        icvCopyBM_8u_C1R(imgB + Y1 * imgStep + X1, imgStep,
                                          blockB, blockSize.width, CurSize);
                    }

                    tmpDist = icvCmpBlocksL1_8u_C1(blockA, blockB, cmpSize);

                    if(tmpDist < accept_level)
                    {
                        VelX = ss[k].x;
                        VelY = ss[k].y;
                        break;  /*for */
                    }
                    else if(tmpDist < blDist)
                    {
                        blDist = tmpDist;
                        VelX = ss[k].x;
                        VelY = ss[k].y;
                        CountDirection = 1;
                    }
                    else if(tmpDist == blDist)
                    {
                        VelX += ss[k].x;
                        VelY += ss[k].y;
                        CountDirection++;
                    }
                }
                if(blDist > escape_level)
                {
                    VelX = VelY = 0;
                    CountDirection = 1;
                }
                if(CountDirection > 1)
                {
                    int temp = CountDirection == 2 ? 1 << 15 : ((1 << 16) / CountDirection);

                    VelocityX = VelX * temp;
                    VelocityY = VelY * temp;
                }
                else
                {
                    VelocityX = VelX << 16;
                    VelocityY = VelY << 16;
                }
            }                   /*if */

            int_velocityX[j] = VelocityX + (offX << 16);
            int_velocityY[j] = VelocityY + (offY << 16);

            X1 += blockSize.width;

        }                       /*for */
        int_velocityX += velStep;
        int_velocityY += velStep;

        imgA += DownStep;
        Y1 += blockSize.height;
    }                           /*for */

/****************************************************************************************\
* Converting fixed point velocities to floating point                                    *
\****************************************************************************************/
    {
        float *velxf = velocityX, *velyf = velocityY;
        int* velx = (int*)velocityX, *vely = (int*)velocityY;

        for(i = 0; i < NumberBlocksY; i++, velxf += velStep, velyf += velStep,
                                            velx += velStep, vely += velStep)
        {
            for(j = 0; j < NumberBlocksX; j++)
            {
                float vx = (float)velx[j]*back, vy = (float)vely[j]*back;
                velxf[j] = vx; velyf[j] = vy;
            }
        }
    }

    cFree(&ss);
    cFree(&blockA);
    
    return CC_OK;
}                               /*cvCalcOpticalFlowBM_8u */


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvCalcOpticalFlowBM
//    Purpose: Optical flow implementation
//    Context: 
//    Parameters:
//             srcA, srcB - source image
//             velx, vely - destination image
//    Returns:
//
//    Notes:
//F*/
CC_IMPL void
cvCalcOpticalFlowBM(const void* srcarrA, const void* srcarrB,
                     CSize blockSize, CSize shiftSize,
                     CSize maxRange, int usePrevious,
                     void* velarrx, void* velarry)
{
    CC_FUNCNAME("cvCalcOpticalFlowBM");

    __BEGIN__;

    img_t stubA, *srcA = srcarrA;
    img_t stubB, *srcB = srcarrB;
    img_t stubx, *velx = velarrx;
    img_t stuby, *vely = velarry;

    CC_CALL(srcA = cvGetMat(srcA, &stubA));
    CC_CALL(srcB = cvGetMat(srcB, &stubB));

    CC_CALL(velx = cvGetMat(velx, &stubx));
    CC_CALL(vely = cvGetMat(vely, &stuby));

    if(!CC_ARE_TYPES_EQ(srcA, srcB))
        CC_ERROR(CC_StsUnmatchedFormats, "Source images have different formats");

    if(!CC_ARE_TYPES_EQ(velx, vely))
        CC_ERROR(CC_StsUnmatchedFormats, "Destination images have different formats");

    if(!CC_ARE_SIZES_EQ(srcA, srcB) ||
        !CC_ARE_SIZES_EQ(velx, vely) ||
        (unsigned)(velx->width*blockSize.width - srcA->width) >= (unsigned)blockSize.width ||
        (unsigned)(velx->height*blockSize.height - srcA->height) >= (unsigned)blockSize.height)
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(CC_MAT_TYPE(srcA) != CC_8UC1 ||
        CC_MAT_TYPE(velx->tid) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "Source images must have 8uC1 type and "
                                           "destination images must have 32fC1 type");

    if(srcA->step != srcB->step || velx->step != vely->step)
        CC_ERROR(CC_BadStep, "two source or two destination images have different steps");

    IPPI_CALL(icvCalcOpticalFlowBM_8u32fR((uchar*)srcA->tt.data, (uchar*)srcB->tt.data,
                                            srcA->step, cvGetMatSize(srcA), blockSize,
                                            shiftSize, maxRange, usePrevious,
                                            velx->tt.fl, vely->tt.fl, velx->step));
    __END__;
}



