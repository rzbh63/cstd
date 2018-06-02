/*
 * drv_tvp5158.h
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _DRV_TVP5158_H_
#define _DRV_TVP5158_H_

#include <linux/videodev.h>
#include <linux/videodev2.h>

#define TVP5158_STD_NTSC_00  ((v4l2_std_id)0x0000000100000000ULL)
#define TVP5158_STD_NTSC_01  ((v4l2_std_id)0x0000000200000000ULL)
#define TVP5158_STD_NTSC_02  ((v4l2_std_id)0x0000000400000000ULL)
#define TVP5158_STD_NTSC_03  ((v4l2_std_id)0x0000000800000000ULL)
#define TVP5158_STD_NTSC_04  ((v4l2_std_id)0x0000001000000000ULL)
#define TVP5158_STD_NTSC_05  ((v4l2_std_id)0x0000002000000000ULL)
#define TVP5158_STD_NTSC_06  ((v4l2_std_id)0x0000004000000000ULL)
#define TVP5158_STD_NTSC_07  ((v4l2_std_id)0x0000008000000000ULL)
#define TVP5158_STD_NTSC_08  ((v4l2_std_id)0x0000010000000000ULL)
#define TVP5158_STD_NTSC_09  ((v4l2_std_id)0x0000020000000000ULL)
#define TVP5158_STD_NTSC_10  ((v4l2_std_id)0x0000040000000000ULL)
#define TVP5158_STD_NTSC_11  ((v4l2_std_id)0x0000080000000000ULL)
#define TVP5158_STD_NTSC_12  ((v4l2_std_id)0x0000100000000000ULL)
#define TVP5158_STD_NTSC_13  ((v4l2_std_id)0x0000200000000000ULL)
#define TVP5158_STD_NTSC_14  ((v4l2_std_id)0x0000400000000000ULL)
#define TVP5158_STD_NTSC_15  ((v4l2_std_id)0x0000800000000000ULL)
#define TVP5158_STD_NTSC_16  ((v4l2_std_id)0x0001000000000000ULL)
#define TVP5158_STD_NTSC_17  ((v4l2_std_id)0x0002000000000000ULL)
#define TVP5158_STD_NTSC_18  ((v4l2_std_id)0x0004000000000000ULL)
#define TVP5158_STD_NTSC_19  ((v4l2_std_id)0x0008000000000000ULL)
#define TVP5158_STD_NTSC_20  ((v4l2_std_id)0x0010000000000000ULL)
#define TVP5158_STD_NTSC_21  ((v4l2_std_id)0x0020000000000000ULL)
#define TVP5158_STD_NTSC_22  ((v4l2_std_id)0x0040000000000000ULL)
#define TVP5158_STD_NTSC_23  ((v4l2_std_id)0x0080000000000000ULL)
#define TVP5158_STD_NTSC_24  ((v4l2_std_id)0x0100000000000000ULL)

#define TVP5158_STD_ALL      ((v4l2_std_id)0x01FFFFFF00000000ULL)

#endif
