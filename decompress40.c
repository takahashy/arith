/*******************************************************************************
 *                               decompress40.c
 *
 *     arith
 *     Author:  Yuma Takahashi
 *     Date:    October 27, 2021
 *
 *     Implementation of the decompress interface
 *     Printout the decompressed ppm file using Pnm_ppmwrite(). The valid input 
 *     compressed ppm file is a sequence of the 32 bit words with a specified 
 *     header.
 *
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "assert.h"
#include "rgbxyz.h"
#include "chromaTransform.h"
#include "DCT_Quantization.h"
#include "wordTransform.h"
#include "compress40.h"
#include "mem.h"

#define currM uarray2_methods_plain

typedef struct Float_rgb {
    float red, green, blue;
} *Float_rgb;

A2Methods_UArray2 readWords(FILE *input, unsigned width, unsigned height);
uint64_t readaWord(FILE *input);
void applyReads(A2Methods_Object *myObj, void *cl);
void rgbToUnsigned(void *elem, void *cl);

/*
 decompress40
 Purpose: a meta function that handles the decompression operation
 Parameters: 
            input - a file pointer that represents a compressed ppm that is to 
                    be decompressed
 Returns: N/A
*/
void decompress40(FILE *input)
{
    unsigned height, width;
    int read = 
    fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    A2Methods_UArray2 wordAry = readWords(input, width/2, height/2);
    A2Methods_UArray2 qdt = decodeWord(wordAry);
    A2Methods_UArray2 averagedAry = inverseDCTQ(qdt);
    A2Methods_UArray2 xyz = deAverageChroma(averagedAry);
    A2Methods_UArray2 rgb = toRGB(xyz);

    unsigned denom = 255;
    currM->small_map_row_major(rgb, rgbToUnsigned, &denom);

    Pnm_ppm deImg;
    NEW(deImg);
    assert(deImg != NULL);

    deImg->pixels = rgb;
    deImg->width = currM->width(rgb);
    deImg->height = currM->height(rgb);
    deImg->denominator = denom;
    deImg->methods = uarray2_methods_plain;

    Pnm_ppmwrite(stdout, deImg);

    currM->free(&wordAry);
    currM->free(&qdt);
    currM->free(&averagedAry);
    currM->free(&xyz);
    Pnm_ppmfree(&deImg);
}

/*
 readWords
 Purpose: read 32-bit words from a given input, store the words in uint64_t
          containers and put them into an array of width width and height height
 Parameters: 
          input - the file pointer that represents the input to be read from 
          width - an unsigned integer representing the width of the wordAry
          container, which is half the width of the original ppm
          height - an unsigned integer representing the height of the wordAry
          container, which is half the height of the original ppm
 Returns: an A2Methods_UArray2 array of 32-bit words in uint64_t containers 
*/
A2Methods_UArray2 readWords(FILE *input, unsigned width, unsigned height)
{
    A2Methods_UArray2 wordAry = currM->new(width, height, sizeof(uint64_t));

    currM->small_map_row_major(wordAry, applyReads, input);
    
    return wordAry;
}

/*
 readaWord
 Purpose: reads a single 32-bit word from the input
 Parameters:  
           input - the file pointer that represents the input to be read from
 Returns: an uint64_t that contains the 32-bit word read from the input in its 
          less significant half
*/
uint64_t readaWord(FILE *input)
{
    uint64_t currWord = 0;
    for (int i = 3; i >= 0; i--)
    {
        uint64_t currChar = getc(input);
        
        currWord = Bitpack_newu(currWord, 8, i * 8, currChar);
    }
    
    return currWord;
}

/*
 applyReads
 Purpose: as a small apply function, it reads a 32-bit word for an element in 
          a wordAry 
 Parameters: myObj - a void pointer pointing to an uint64_t that is to contain 
                     a 32-bit word 
             cl    - a void pointer pointing to the intended input
 Returns: N/A

*/
void applyReads(A2Methods_Object *myObj, void *cl)
{
    *((uint64_t *)myObj) = readaWord((FILE *)cl);
}

/*
 rgbToFloat
 Purpose: as a small apply function, it maps through an array of Float_rgb and 
          turn an element in type Float_rgb to an Pnm_rgb type. It multiplies
          the flaot rgb values by a denominator of 255 and put back 
          the unsigned value of the same rgb values.
 Parameters: 
          elem - a void pointer pointing to the current Float_rgb element
          cl   - a void pointer pointing to an unsigned integer denominator
 Returns: N/A
 Note: automatically fixed the range of Float_rgb's rgb values to be 0 to 1, 
       inclusive.
*/
void rgbToUnsigned(void *elem, void *cl)
{
        unsigned int denominator = *((unsigned int *)cl);

        float currRed = ((Float_rgb) elem)->red;
        float currGreen = ((Float_rgb) elem)->green;
        float currBlue = ((Float_rgb) elem)->blue;

        if (currRed < 0.0) {
            currRed = 0.0;
        }
        
        if (currGreen < 0.0) {
            currGreen = 0.0;
        }
        
        if (currBlue < 0.0) {
            currBlue = 0.0;
        }
        
        if (currRed > 1.0) {
            currRed = 1.0;
        }
        
        if (currGreen > 1.0) {
            currGreen = 1.0;
        }
        
        if (currBlue > 1.0) {
            currBlue = 1.0;
        }


        ((Pnm_rgb) elem)->red = currRed * denominator;
        ((Pnm_rgb) elem)->green = currGreen * denominator;
        ((Pnm_rgb) elem)->blue = currBlue * denominator;
}