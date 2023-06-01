/*******************************************************************************
 *                               rgbxyz.h
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *     rgbxyz interface.
 *     Transforms the RGB information of all pixels in a UArray2 of Float_rgb’s 
 *     into the corresponding XYZ format of the same pixels. ADDITIONALLY, 
 *     Transforms the XYZ format into Float_rgb format which is stored in a 
 *     UArray2.
 *      
 ******************************************************************************/

#ifndef RGBXYZ_H
#define RGBXYZ_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "a2methods.h"
#include "uarray2.h"
#include "a2plain.h"
#include "assert.h"
#include "math.h"

typedef struct XYZ {
    float y, pb, pr;
} *XYZ;

A2Methods_UArray2 toXYZ(A2Methods_UArray2 rgb);
A2Methods_UArray2 toRGB(A2Methods_UArray2 xyz);

#endif