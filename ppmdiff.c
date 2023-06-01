/*******************************************************************************
 *                               ppmdiff.c
 *
 *     arith
 *     Author: Yuma Takahashi
 *     Date:   October 27, 2021
 *
 *     Compares two ppm files and printout their differences.
 *      
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"
#include "math.h"

float compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods);
float square(float x);

int main(int argc, char** argv) {

    assert(argc == 3);

    Pnm_ppm imageAry[2];
    A2Methods_T methods = uarray2_methods_plain;

    FILE *inputfp = NULL;
    int numStdinput = 0;

    for (int i = 1; i <= 2; i++) {
        if ((strcmp(argv[i], "-") == 0) && (numStdinput == 0)) {
            inputfp = stdin;
            imageAry[i - 1] =  Pnm_ppmread(inputfp, methods);
            numStdinput++;
        } else {
            inputfp = fopen(argv[i], "r");
            assert(inputfp);
            imageAry[i - 1] = Pnm_ppmread(inputfp, methods);
            fclose(inputfp);
        }
    }
    
    float result = compare(imageAry[0], imageAry[1], methods);
    printf("%.4f\n", result);

    Pnm_ppmfree(&(imageAry[0]));
    Pnm_ppmfree(&(imageAry[1]));
    return 0;
}


float compare(Pnm_ppm image1, Pnm_ppm image2, A2Methods_T methods)
{
    int sWidth, sHeight;
    
    if ( ((int)image1->width - (int)image2->width > 1) 
        || ((int)image1->width - (int)image2->width < -1) 
        || ((int)image1->height - (int)image2->height > 1)
        || ((int)image1->height - (int)image2->height < -1))  {
        perror("The width and height of I and I' should differ by at most 1\n");
        return (float) 1.0000;
    }
    
    if ((image1->width) <= (image2->width)) {
        sWidth = image1->width;
    } else {
        sWidth = image2->width;
    }

    if ((image1->height) <= (image2->height)) {
        sHeight = image1->height;
    } else {
        sHeight = image2->height;
    }

    A2Methods_UArray2 ary1 = image1->pixels;
    A2Methods_UArray2 ary2 = image2->pixels;

    float accDiff = 0;
    float denom = image1->denominator;

    for (int j = 0; j < sHeight; j++) {
        for (int i = 0; i < sWidth; i++) {
            Pnm_rgb pix1 = methods->at(ary1, i, j);
            Pnm_rgb pix2 = methods->at(ary2, i, j);
            accDiff += pow((((float)(pix1->red)/denom) - 
                            ((float)(pix2->red)/denom)), 2) + 
                       pow((((float)(pix1->green)/denom) - 
                            ((float)(pix2->green)/denom)), 2) +
                       pow((((float)(pix1->blue)/denom) - 
                            ((float)(pix2->blue)/denom)), 2);

        }
    }
    return (float) sqrt(accDiff / (3 * sWidth * sHeight));
}
