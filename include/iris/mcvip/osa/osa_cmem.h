/*
 * osa_cmem.h
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed �as is� WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef _OSA_CMEM_H_
#define _OSA_CMEM_H_

#include <osa.h>
#include <cmem.h>

Uint8 *OSA_cmemAllocCached(Uint32 size, Uint32 alignment);
Uint8 *OSA_cmemAlloc(Uint32 size, Uint32 alignment);
int    OSA_cmemFree(Uint8 *ptr);
Uint8 *OSA_cmemGetPhysAddr(Uint8 *virtAddr);

#define OSA_cmemCacheWb(virtPtr, size)    CMEM_cacheWb((void*)virtPtr, size)
#define OSA_cmemCacheInv(virtPtr, size)   CMEM_cacheInv((void*)virtPtr, size)
#define OSA_cmemCacheWbInv(virtPtr, size) CMEM_cacheWbInv((void*)virtPtr, size)

#endif  /*  _OSA_CMEM_H_  */
