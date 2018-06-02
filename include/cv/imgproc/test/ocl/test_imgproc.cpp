/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2010-2012, Institute Of Software Chinese Academy Of Science, all rights reserved.
// Copyright (C) 2010-2012, Advanced Micro Devices, Inc., all rights reserved.
// Copyright (C) 2010-2012, Multicoreware, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// @Authors
//    Niko Li, newlife20080214@gmail.com
//    Jia Haipeng, jiahaipeng95@gmail.com
//    Shengen Yan, yanshengen@gmail.com
//    Jiang Liyuan, lyuan001.good@163.com
//    Rock Li, Rock.Li@amd.com
//    Wu Zailong, bullet@yeah.net
//    Xu Pang, pangxu010@163.com
//    Sen Liu, swjtuls1987@126.com
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "../test_precomp.hpp"
#include "cvconfig.h"
#include "opencv2/ts/ocl_test.hpp"

#ifdef HAVE_OPENCL

namespace cvtest {
namespace ocl {

///////////////////////////////////////////////////////////////////////////////

PARAM_TEST_CASE(ImgprocTestBase, MatType,
                int, // blockSize
                int, // border type
                bool) // roi or not
{
    int type, borderType, blockSize;
    bool useRoi;

    TEST_DECLARE_INPUT_PARAMETER(src);
    TEST_DECLARE_OUTPUT_PARAMETER(dst);

    virtual void SetUp()
    {
        type = GET_PARAM(0);
        blockSize = GET_PARAM(1);
        borderType = GET_PARAM(2);
        useRoi = GET_PARAM(3);
    }

    void random_roi()
    {
        CvSize roiSize = randomSize(1, MAX_VALUE);
        Border srcBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(src, src_roi, roiSize, srcBorder, type, 5, 256);

        Border dstBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(dst, dst_roi, roiSize, dstBorder, type, 5, 16);

        UMAT_UPLOAD_INPUT_PARAMETER(src);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst);
    }

    void Near(double threshold = 0.0, bool relative = false)
    {
        if (relative)
            OCL_EXPECT_MATS_NEAR_RELATIVE(dst, threshold);
        else
            OCL_EXPECT_MATS_NEAR(dst, threshold);
    }
};

//////////////////////////////// copyMakeBorder ////////////////////////////////////////////

PARAM_TEST_CASE(CopyMakeBorder, MatDepth, // depth
                Channels, // channels
                bool, // isolated or not
                BorderType, // border type
                bool) // roi or not
{
    int type, borderType;
    bool useRoi;

    TestUtils::Border border;
    Scalar val;

    TEST_DECLARE_INPUT_PARAMETER(src);
    TEST_DECLARE_OUTPUT_PARAMETER(dst);

    virtual void SetUp()
    {
        type = CC_MAKE_TYPE(GET_PARAM(0), GET_PARAM(1));
        borderType = GET_PARAM(3);

        if (GET_PARAM(2))
            borderType |= BORDER_ISOLATED;

        useRoi = GET_PARAM(4);
    }

    void random_roi()
    {
        border = randomBorder(0, MAX_VALUE << 2);
        val = randomScalar(-MAX_VALUE, MAX_VALUE);

        CvSize roiSize = randomSize(1, MAX_VALUE);
        Border srcBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(src, src_roi, roiSize, srcBorder, type, -MAX_VALUE, MAX_VALUE);

        Border dstBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        dstBorder.top += border.top;
        dstBorder.lef += border.lef;
        dstBorder.rig += border.rig;
        dstBorder.bot += border.bot;

        randomSubMat(dst, dst_roi, roiSize, dstBorder, type, -MAX_VALUE, MAX_VALUE);

        UMAT_UPLOAD_INPUT_PARAMETER(src);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst);
    }

    void Near()
    {
        OCL_EXPECT_MATS_NEAR(dst, 0);
    }
};

OCL_TEST_P(CopyMakeBorder, CvMat)
{
    for (int i = 0; i < test_loop_times; ++i)
    {
        random_roi();

        OCL_OFF(copyMakeBorder(src_roi, dst_roi, border.top, border.bot, border.lef, border.rig, borderType, val));
        OCL_ON(copyMakeBorder(usrc_roi, udst_roi, border.top, border.bot, border.lef, border.rig, borderType, val));

        Near();
    }
}

//////////////////////////////// equalizeHist //////////////////////////////////////////////

typedef ImgprocTestBase EqualizeHist;

OCL_TEST_P(EqualizeHist, CvMat)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        OCL_OFF(equalizeHist(src_roi, dst_roi));
        OCL_ON(equalizeHist(usrc_roi, udst_roi));

        Near(1);
    }
}

//////////////////////////////// Corners test //////////////////////////////////////////

struct CornerTestBase :
        public ImgprocTestBase
{
    void random_roi()
    {
        CvMat image = readImageType("../gpu/stereobm/aloe-L.png", type);
        ASSERT_FALSE(image.empty());

        bool isFP = CC_MAT_DEPTH(type) >= CC_32F;
        float val = 255.0f;
        if (isFP)
        {
            image.convertTo(image, -1, 1.0 / 255);
            val /= 255.0f;
        }

        CvSize roiSize = image.size();
        Border srcBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);

        CvSize wholeSize = CvSize(roiSize.width + srcBorder.lef + srcBorder.rig, roiSize.height + srcBorder.top + srcBorder.bot);
        src = randomMat(wholeSize, type, -val, val, false);
        src_roi = src(Rect(srcBorder.lef, srcBorder.top, roiSize.width, roiSize.height));
        image.copyTo(src_roi);

        Border dstBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(dst, dst_roi, roiSize, dstBorder, CC_32FC1, 5, 16);

        UMAT_UPLOAD_INPUT_PARAMETER(src);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst);
    }
};

typedef CornerTestBase CornerMinEigenVal;

OCL_TEST_P(CornerMinEigenVal, CvMat)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        int apertureSize = 3;

        OCL_OFF(cornerMinEigenVal(src_roi, dst_roi, blockSize, apertureSize, borderType));
        OCL_ON(cornerMinEigenVal(usrc_roi, udst_roi, blockSize, apertureSize, borderType));

        Near(1e-5, true);
    }
}

//////////////////////////////// cornerHarris //////////////////////////////////////////

typedef CornerTestBase CornerHarris;

OCL_TEST_P(CornerHarris, CvMat)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        int apertureSize = 3;
        double k = randomDouble(0.01, 0.9);

        OCL_OFF(cornerHarris(src_roi, dst_roi, blockSize, apertureSize, k, borderType));
        OCL_ON(cornerHarris(usrc_roi, udst_roi, blockSize, apertureSize, k, borderType));

        Near(1e-6, true);
    }
}

//////////////////////////////// preCornerDetect //////////////////////////////////////////

typedef ImgprocTestBase PreCornerDetect;

OCL_TEST_P(PreCornerDetect, CvMat)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        const int apertureSize = blockSize;

        OCL_OFF(preCornerDetect(src_roi, dst_roi, apertureSize, borderType));
        OCL_ON(preCornerDetect(usrc_roi, udst_roi, apertureSize, borderType));

        Near(1e-6, true);
    }
}


////////////////////////////////// integral /////////////////////////////////////////////////

struct Integral :
        public ImgprocTestBase
{
    int sdepth, sqdepth;

    TEST_DECLARE_OUTPUT_PARAMETER(dst2);

    virtual void SetUp()
    {
        type = GET_PARAM(0);
        sdepth = GET_PARAM(1);
        sqdepth = GET_PARAM(2);
        useRoi = GET_PARAM(3);
    }

    void random_roi()
    {
        ASSERT_EQ(CC_MAT_CN(type), 1);

        CvSize roiSize = randomSize(1, MAX_VALUE), isize = CvSize(roiSize.width + 1, roiSize.height + 1);
        Border srcBorder = randomBorder(0, useRoi ? 2 : 0);
        randomSubMat(src, src_roi, roiSize, srcBorder, type, 5, 256);

        Border dstBorder = randomBorder(0, useRoi ? 2 : 0);
        randomSubMat(dst, dst_roi, isize, dstBorder, sdepth, 5, 16);

        Border dst2Border = randomBorder(0, useRoi ? 2 : 0);
        randomSubMat(dst2, dst2_roi, isize, dst2Border, sqdepth, 5, 16);

        UMAT_UPLOAD_INPUT_PARAMETER(src);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst2);
    }

    void Near2(double threshold = 0.0, bool relative = false)
    {
        if (relative)
            OCL_EXPECT_MATS_NEAR_RELATIVE(dst2, threshold);
        else
            OCL_EXPECT_MATS_NEAR(dst2, threshold);
    }
};

OCL_TEST_P(Integral, Mat1)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        OCL_OFF(integral(src_roi, dst_roi, sdepth));
        OCL_ON(integral(usrc_roi, udst_roi, sdepth));

        Near();
    }
}

OCL_TEST_P(Integral, Mat2)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        OCL_OFF(integral(src_roi, dst_roi, dst2_roi, sdepth, sqdepth));
        OCL_ON(integral(usrc_roi, udst_roi, udst2_roi, sdepth, sqdepth));

        Near();
        sqdepth == CC_32F ? Near2(1e-6, true) : Near2();
    }
}

////////////////////////////////////////  threshold //////////////////////////////////////////////

struct Threshold :
        public ImgprocTestBase
{
    int thresholdType;

    virtual void SetUp()
    {
        type = GET_PARAM(0);
        thresholdType = GET_PARAM(2);
        useRoi = GET_PARAM(3);
    }
};

OCL_TEST_P(Threshold, CvMat)
{
    for (int j = 0; j < test_loop_times; j++)
    {
        random_roi();

        double maxVal = randomDouble(20.0, 127.0);
        double thresh = randomDouble(0.0, maxVal);

        OCL_OFF(threshold(src_roi, dst_roi, thresh, maxVal, thresholdType));
        OCL_ON(threshold(usrc_roi, udst_roi, thresh, maxVal, thresholdType));

        Near(1);
    }
}

/////////////////////////////////////////// CLAHE //////////////////////////////////////////////////

PARAM_TEST_CASE(CLAHETest, CvSize, double, bool)
{
    CvSize gridSize;
    double clipLimit;
    bool useRoi;

    TEST_DECLARE_INPUT_PARAMETER(src);
    TEST_DECLARE_OUTPUT_PARAMETER(dst);

    virtual void SetUp()
    {
        gridSize = GET_PARAM(0);
        clipLimit = GET_PARAM(1);
        useRoi = GET_PARAM(2);
    }

    void random_roi()
    {
        CvSize roiSize = randomSize(MAX(gridSize.height, gridSize.width), MAX_VALUE);
        Border srcBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(src, src_roi, roiSize, srcBorder, CC_8UC1, 5, 256);

        Border dstBorder = randomBorder(0, useRoi ? MAX_VALUE : 0);
        randomSubMat(dst, dst_roi, roiSize, dstBorder, CC_8UC1, 5, 16);

        UMAT_UPLOAD_INPUT_PARAMETER(src);
        UMAT_UPLOAD_OUTPUT_PARAMETER(dst);
    }

    void Near(double threshold = 0.0)
    {
        OCL_EXPECT_MATS_NEAR(dst, threshold);
    }
};

OCL_TEST_P(CLAHETest, Accuracy)
{
    for (int i = 0; i < test_loop_times; ++i)
    {
        random_roi();

        Ptr<CLAHE> clahe = createCLAHE(clipLimit, gridSize);

        OCL_OFF(clahe->apply(src_roi, dst_roi));
        OCL_ON(clahe->apply(usrc_roi, udst_roi));

        Near(1.0);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, EqualizeHist, Combine(
                            Values((MatType)CC_8UC1),
                            Values(0), // not used
                            Values(0), // not used
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, CornerMinEigenVal, Combine(
                            Values((MatType)CC_8UC1, (MatType)CC_32FC1),
                            Values(3, 5),
                            Values((BorderType)BORDER_CONSTANT, (BorderType)BORDER_REPLICATE,
                                   (BorderType)BORDER_REFLECT, (BorderType)BORDER_REFLECT101),
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, CornerHarris, Combine(
                            Values((MatType)CC_8UC1, CC_32FC1),
                            Values(3, 5),
                            Values( (BorderType)BORDER_CONSTANT, (BorderType)BORDER_REPLICATE,
                                    (BorderType)BORDER_REFLECT, (BorderType)BORDER_REFLECT_101),
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, PreCornerDetect, Combine(
                            Values((MatType)CC_8UC1, CC_32FC1),
                            Values(3, 5),
                            Values( (BorderType)BORDER_CONSTANT, (BorderType)BORDER_REPLICATE,
                                    (BorderType)BORDER_REFLECT, (BorderType)BORDER_REFLECT_101),
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, Integral, Combine(
                            Values((MatType)CC_8UC1), // TODO does not work with CC_32F, CC_64F
                            Values(CC_32SC1, CC_32FC1), // desired sdepth
                            Values(CC_32FC1, CC_64FC1), // desired sqdepth
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, Threshold, Combine(
                            Values(CC_8UC1, CC_8UC2, CC_8UC3, CC_8UC4,
                                   CC_16SC1, CC_16SC2, CC_16SC3, CC_16SC4,
                                   CC_32FC1, CC_32FC2, CC_32FC3, CC_32FC4),
                            Values(0),
                            Values(ThreshOp(THRESH_BINARY),
                                   ThreshOp(THRESH_BINARY_INV), ThreshOp(THRESH_TRUNC),
                                   ThreshOp(THRESH_TOZERO), ThreshOp(THRESH_TOZERO_INV)),
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(Imgproc, CLAHETest, Combine(
                            Values(CvSize(4, 4), CvSize(32, 8), CvSize(8, 64)),
                            Values(0.0, 10.0, 62.0, 300.0),
                            Bool()));

OCL_INSTANTIATE_TEST_CASE_P(ImgprocTestBase, CopyMakeBorder, Combine(
                            testing::Values((MatDepth)CC_8U, (MatDepth)CC_16S, (MatDepth)CC_32S, (MatDepth)CC_32F),
                            testing::Values(Channels(1), Channels(3), (Channels)4),
                            Bool(), // border isolated or not
                            Values((BorderType)BORDER_CONSTANT, (BorderType)BORDER_REPLICATE, (BorderType)BORDER_REFLECT,
                                   (BorderType)BORDER_WRAP, (BorderType)BORDER_REFLECT_101),
                            Bool()));

} } // namespace cvtest::ocl

#endif // HAVE_OPENCL
