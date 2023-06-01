/*******************************************************************************
 *                               wordTransform.h
 *
 *     arith
 *     Author:  Yuma Takahashi 
 *     Date:    October 27, 2021
 *
 *     wordTranform interface
 *     Transform information contained in Bundle2 structs to a 32-bit word that 
 *     scontains the identical information (in a uint64_t variable). This step
 *     is implemented with guide from bitpack.c
 *
 ******************************************************************************/
#ifndef WORDTRANFORM_H
#define WORDTRANFORM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "a2methods.h"
#include "uarray2.h"
#include "a2plain.h"
#include "assert.h"
#include "DCT_Quantization.h"
#include "bitpack.h"

A2Methods_UArray2 codeWord(A2Methods_UArray2 bd2Ary);
A2Methods_UArray2 decodeWord(A2Methods_UArray2 words);

#endif