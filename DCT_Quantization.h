/*******************************************************************************
 *                               DCT_Quantization.h
 *
 *     arith
 *     Author:  Yuma Takahashi
 *     Date:    October 27, 2021
 *
 *   DCT_Quantization interface.
 *   Transform the 4 luminance values into integer coefficients. Convert the 
 *   average chroma elements into integers. ADDITIONALLY, Convert the indexed 
 *   chroma elements back to float numbers, and convert the integer coefficient 
 *   that represents the luminance values back to four luminance values in float 
 *   numbers.
 *
 ******************************************************************************/
#ifndef DCT_QUANTIZATION_H
#define DCT_QUANTIZATION_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "a2methods.h"
#include "uarray2.h"
#include "a2plain.h"
#include "assert.h"
#include "rgbxyz.h"
#include "chromaTransform.h"
#include "arith40.h"
#include "math.h"

#define currM uarray2_methods_plain

/* quantized values of a, b, c, d, pb, pr*/
typedef struct Bundle2 {
    uint64_t qa, iapb, iapr;
    int64_t qb, qc, qd;  
} *Bundle2;

A2Methods_UArray2 DCTQ(A2Methods_UArray2 avg);
A2Methods_UArray2 inverseDCTQ(A2Methods_UArray2 bd2Ary);


#endif