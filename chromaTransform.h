/*******************************************************************************
 *                               chromaTranform.h
 *
 *     arith
 *     Author:   Yuma Takahashi 
 *     Date:     October 27, 2021
 *     
 *     chromaTranform interface
 *     Determine the average chroma elements of the pixels for a 2x2 block by 
 *     taking the average value of the four pixels in the block. ADDITIONALLY, 
 *     set the chroma value for each 2x2 block in an UArray2 of struct XYZ by 
 *     using their average.
 *
 ******************************************************************************/

#ifndef CHROMATRANSFORM_H
#define CHROMATRANSFORM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "a2methods.h"
#include "uarray2.h"
#include "a2plain.h"
#include "assert.h"
#include "rgbxyz.h"

#define currM uarray2_methods_plain

typedef struct Bundle1 {
    float y[4];
    float apb, apr;
} *Bundle1;

A2Methods_UArray2 averageChroma(A2Methods_UArray2 xyz);
A2Methods_UArray2 deAverageChroma(A2Methods_UArray2 avg);

#endif