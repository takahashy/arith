/*******************************************************************************
 *                               chromaTranform.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *     
 *     Implementation of the chromaTranform interface
 *     Determine the average chroma elements of the pixels for a 2x2 block by 
 *     taking the average value of the four pixels in the block. ADDITIONALLY, 
 *     set the chroma value for each 2x2 block in an UArray2 of struct XYZ by 
 *     using their average.
 *
 ******************************************************************************/

#include "chromaTransform.h"

/*
 averageChroma
 Purpose: Reads in cells of 2x2 blocks of the uarray2 to determine the y values,
          average pb value, and average pr value.
 Parameters: 
            A2Methods_UArray2 xyz - uarray2 that holds the xyz format of the 
                                    original image.
 Returns: a uarray2 that holds a struct of bundle1s. In each bundle1 it holds
          an array for the y values of length 4, average pb value, and average
          pr value
*/
A2Methods_UArray2 averageChroma(A2Methods_UArray2 xyz)
{
    int newHeight = currM->height(xyz) / 2;
    int newWidth = currM->width(xyz) / 2;

    A2Methods_UArray2 averagedAry 
    = currM->new(newWidth, newHeight, sizeof(struct Bundle1));

    for (int i = 0; i < (newHeight * newWidth); i++) {
        int m = (i / newWidth) * 2; 
        int o = (i % newWidth) * 2;
        int index = 0;
        float accuPb = 0.0;
        float accuPr = 0.0;

        Bundle1 currBundle1 = currM->at(averagedAry, (i % newWidth), (i / newWidth));

        for (int j = m; j < m + 2; j++) {
            for (int k = o; k < o + 2; k++) {
                (currBundle1->y)[index] = ((XYZ)(currM->at(xyz, k, j)))->y;
                accuPb += ((XYZ)(currM->at(xyz, k, j)))->pb;
                accuPr += ((XYZ)(currM->at(xyz, k, j)))->pr;
                index++;
            }
        }
        currBundle1->apb = accuPb / 4.0;
        currBundle1->apr = accuPr / 4.0;
    }

    return averagedAry;
}

/*
deaverageChroma
 Purpose: Reads in a uarray2 that holds a struct of bundle1s and determines the 
          corresponding xyz values.
 Parameters: 
            A2Methods_UArray2 avg - uarray2 where each element holds a bundle1 
                                    struct
 Returns: uarray2 that holds xyz format of the partially decompressed image.
*/
A2Methods_UArray2 deAverageChroma(A2Methods_UArray2 avg)
{
    int newHeight = currM->height(avg) * 2;
    int newWidth = currM->width(avg) * 2;

    A2Methods_UArray2 deaveragedAry 
    = currM->new(newWidth, newHeight, sizeof(struct XYZ));

    for (int i = 0; i < (currM->height(avg) * currM->width(avg)); i++) {
        int m = (i / currM->width(avg)) * 2; 
        int o = (i % currM->width(avg)) * 2;
        int index = 0;

        Bundle1 currBundle1 = 
            currM->at(avg, (i % currM->width(avg)), (i / currM->width(avg)));

        for (int j = m; j < m + 2; j++) {
            for (int k = o; k < o + 2; k++) {

                ((XYZ)(currM->at(deaveragedAry, k, j)))->y = 
                        (currBundle1->y)[index];
                ((XYZ)(currM->at(deaveragedAry, k, j)))->pb = 
                        (currBundle1->apb);
                ((XYZ)(currM->at(deaveragedAry, k, j)))->pr = 
                        (currBundle1->apr);
                index++;
            }
        }
        
    }

    return deaveragedAry;
}