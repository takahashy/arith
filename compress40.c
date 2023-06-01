/*******************************************************************************
 *                               compress40.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *     Implementation of compress40 interface. 
 *     Read in a valid ppm file to be processed using Pnm_ppmread()
 *     Trim, if needed, the last column and/or the last row of the ppm.
 *
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "uarray2.h"
#include "a2plain.h"
#include "assert.h"
#include "rgbxyz.h"
#include "chromaTransform.h"
#include "DCT_Quantization.h"
#include "wordTransform.h"

#define currM uarray2_methods_plain

typedef struct Float_rgb {
    float red, green, blue;
} *Float_rgb;


A2Methods_UArray2 trim(A2Methods_UArray2 untrimmed);
void copy(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
void rgbToFloat(void *elem, void *cl);
void printWord(uint64_t currWord);
void printElemWords(A2Methods_Object *myObj, void *cl);

/*
 compress40
 Purpose: a meta function that handles the compression operation
 Parameters: 
            input - a file pointer that represents a ppm file to be compressed
 Returns: N/A
*/
void compress40(FILE *input)
{
        Pnm_ppm origPpm = Pnm_ppmread(input, currM);

        bool trimmed = false;
        
        A2Methods_UArray2 rgb = NULL;
        if ((origPpm->width % 2 != 0) || (origPpm->height % 2 != 0)) {
            rgb = trim(origPpm->pixels);
            trimmed = true;
        } else {
            rgb = origPpm->pixels;
            trimmed = false;
        }

        currM->small_map_row_major(rgb, rgbToFloat, &origPpm->denominator);

        A2Methods_UArray2 xyz = toXYZ(rgb);
        A2Methods_UArray2 averagedAry = averageChroma(xyz);
        A2Methods_UArray2 qdt = DCTQ(averagedAry);
        A2Methods_UArray2 wordAry = codeWord(qdt);
        
        printf("COMP40 Compressed image format 2\n%u %u\n", 
                            currM->width(rgb), currM->height(rgb));
        
        currM->small_map_row_major(wordAry, printElemWords, NULL);
        
        if (trimmed) {
            currM->free(&rgb);
            Pnm_ppmfree(&origPpm);
        } else {
            Pnm_ppmfree(&origPpm);
        }
        currM->free(&xyz);
        currM->free(&averagedAry);
        currM->free(&qdt);
        currM->free(&wordAry);        
}


/*
 printWord
 Purpose: print a single 32-bit word in a big endian order, given a uint64_t
          container of that word, to stdout
 Parameters: 
          currWord - an uint64_t that contains a 32-bit word that is to be 
                     printed out
 Returns: N/A
*/
void printWord(uint64_t currWord)
{
    unsigned char *currPtr = (unsigned char*)&currWord;
    
    putchar(currPtr[3]);
    putchar(currPtr[2]);
    putchar(currPtr[1]);
    putchar(currPtr[0]);
}

/*
 printElemWords
 Purpose: as an small apply function, it prints out a 32-bit word in a uint64_t
          container to stdout
 Parameters: 
            myObj - pointer to an uint64_t container which contains the 32-bit 
            word to be printed out
            cl - a closure variable which will not be used.
 Returns: N/A
*/
void printElemWords(A2Methods_Object *myObj, void *cl)
{
    (void) cl;
    printWord(*((uint64_t *)myObj));
}

/*
 copy
 Purpose: as an apply function, it copies an element in a 2-d array of 
          Pnm_rgb to another 2-d array of Pnm_rgb
 Parameters:
          i - an integer representing the column location of the current 
              element
          j - an integer representing the row location of the current 
              element
          array2 - an A2Methods_UArray2 that represents the array to be copied 
                   from 
          elem - a void pointer pointing to the current Pnm_rgb element
          cl   - a void pointer pointing to the new array to be copied into
 Returns: N/A
*/
void copy(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
        (void) array2;
        Pnm_rgb currObj = currM->at((A2Methods_UArray2) cl, i, j);

        ((Pnm_rgb) elem)->red = currObj->red;
        ((Pnm_rgb) elem)->green = currObj->green;
        ((Pnm_rgb) elem)->blue = currObj->blue;
}

/*
 rgbToFloat
 Purpose: as a small apply function, it maps through an array of Pnm_rgb and 
          turn an element in type Pnm_rgb to an Float_rgb type. It devides 
          the unsigned rgb values by its corresponding denominator and put back 
          the float value of the same rgb values.
 Parameters: 
          elem - a void pointer pointing to the current Pnm_rgb element
          cl   - a void pointer pointing to an unsigned integer denominator
 Returns: N/A
*/
void rgbToFloat(void *elem, void *cl)
{
        unsigned int denominator = *((unsigned int *)cl);

        float currRed = ((Pnm_rgb) elem)->red;
        float currGreen = ((Pnm_rgb) elem)->green;
        float currBlue = ((Pnm_rgb) elem)->blue;

        ((Float_rgb) elem)->red = currRed / denominator;
        ((Float_rgb) elem)->green = currGreen / denominator;
        ((Float_rgb) elem)->blue = currBlue / denominator;
}

/*
 trim
 Purpose: to trim off the last column and/or row of an array that contains 
          odd numebrs of columns and/or rows
 Parameters: 
          untrimmed - an A2Methods_UArray2 representing the array to be trimmed
 Returns:
          an A2Methods_UArray2 that is the trimmed version of untrimmed

*/
A2Methods_UArray2 trim(A2Methods_UArray2 untrimmed)
{
    int trimmedWidth = currM->width(untrimmed) / 2 * 2;
    int trimmedHeight = currM->height(untrimmed) / 2 * 2;
    A2Methods_UArray2 trimmed = UArray2_new(trimmedWidth, trimmedHeight, 
                                    currM->size(untrimmed));
                                    
    currM->map_row_major(trimmed, copy, untrimmed);
                            

    return trimmed;
}