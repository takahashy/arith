/*******************************************************************************
 *                               DCT_Quantization.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *   Implementation of DCT_Quantization interface.
 *   Transform the 4 luminance values into integer coefficients. Convert the 
 *   average chroma elements into integers. ADDITIONALLY, Convert the indexed 
 *   chroma elements back to float numbers, and convert the integer coefficient 
 *   that represents the luminance values back to four luminance values in float 
 *   numbers.
 *
 ******************************************************************************/
#include "DCT_Quantization.h"

#define currM uarray2_methods_plain

typedef struct InterBundle {
    float a, b, c, d, apb, apr;  
} *InterBundle;

void DCT(Bundle1 currBd1, InterBundle currIB);
void inverseDCT(InterBundle currIB, Bundle1 currBd1);

uint64_t quantizea(float val);
float dequantizea(uint64_t val);
int64_t quantizebcd(float val);
float dequantizebcd(int64_t val);

void quantize(InterBundle currIB, Bundle2 currBd2);
void dequantize(Bundle2 currBd2, InterBundle currIB);

void applyDCTQ(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
void applyinDCTQ(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);

/*
 DCTQ
 Purpose: perform the discrete cosine transorm, quantize the values of 
          the average component video color space and return a uarray2 that 
          that holds those informations in a struct. 
 Parameters: 
            A2Methods_UArray2 avg - uarray2 that holds a struct of bundle1s
                                    which has the average component video
                                    color spaces
 Returns: uarray2 that holds a struct of quantized coefficents and quantized 
          average pb pr values.
*/
A2Methods_UArray2 DCTQ(A2Methods_UArray2 avg)
{
    A2Methods_UArray2 bd2Ary = currM->new(currM->width(avg), currM->height(avg), 
                                       sizeof(struct Bundle2));

    currM->map_row_major(avg, applyDCTQ, bd2Ary);
    
    return bd2Ary;
}

/*
 inverseDCTQ
 Purpose: perform the inverse discrete cosine transorm, dequantize the values of
          the quantized coefficients and quantized average pb pr values and 
          return a uarray2 that holds the average component video color space
 Parameters: 
            A2Methods_UArray2 bd2Ary - uarray2 that holds a struct of 
                                       quantized coefficents and quantized 
                                       average pb pr values.
 Returns: uarray2 that holds a struct of the average component video color space
*/
A2Methods_UArray2 inverseDCTQ(A2Methods_UArray2 bd2Ary)
{
    A2Methods_UArray2 avg = currM->new(currM->width(bd2Ary), 
    currM->height(bd2Ary), sizeof(struct Bundle1));

    currM->map_row_major(bd2Ary, applyinDCTQ, avg);

    return avg;
}

/*
 DCT
 Purpose: perform the discrete cosine transorm
 Parameters: 
            Bundle1 currBd1 - struct that holds the 4 y values and the average
                              pb, pr values
            InterBundle currIB - struct that holds the coefficients and the 
                                 average pb, pr values
 Returns: n/a
*/
void DCT(Bundle1 currBd1, InterBundle currIB)
{
    currIB->a = 
        (currBd1->y[3] + currBd1->y[2] + currBd1->y[1] + currBd1->y[0]) / 4.0;

    currIB->b = 
        (currBd1->y[3] + currBd1->y[2] - currBd1->y[1] - currBd1->y[0]) / 4.0;

    currIB->c = 
        (currBd1->y[3] - currBd1->y[2] + currBd1->y[1] - currBd1->y[0]) / 4.0;

    currIB->d = 
        (currBd1->y[3] - currBd1->y[2] - currBd1->y[1] + currBd1->y[0]) / 4.0;

    currIB->apb = currBd1->apb;
    currIB->apr = currBd1->apr;
}

/*
 inverseDCT
 Purpose: perform the inverse discrete cosine transorm
 Parameters: 
            InterBundle currIB - struct that holds the coefficients and the 
                                 average pb, pr values
            Bundle1 currBd1 - struct that holds the 4 y values and the average
                              pb, pr values    
 Returns: n/a
*/
void inverseDCT(InterBundle currIB, Bundle1 currBd1)
{
    currBd1->y[0] = 
        currIB->a - currIB->b - currIB->c + currIB->d;
    
    currBd1->y[1] = 
        currIB->a - currIB->b + currIB->c - currIB->d;
    
    currBd1->y[2] = 
        currIB->a + currIB->b - currIB->c - currIB->d;
    
    currBd1->y[3] = 
        currIB->a + currIB->b + currIB->c + currIB->d;

    currBd1->apb = currIB->apb;
    currBd1->apr = currIB->apr;
}

/*
 quantize
 Purpose: Quantizes the coefficients and the average pb pr values.
 Parameters: 
            InterBundle currIB - struct that holds the coefficients and the 
                                 average pb, pr values
            Bundle2 currBd2 - struct that holds the quantized coefficients and 
                              the quantized average pb, pr values.
 Returns: n/a
*/
void quantize(InterBundle currIB, Bundle2 currBd2)
{
    currBd2->qa = quantizea(currIB->a);
    currBd2->qb = quantizebcd(currIB->b);
    currBd2->qc = quantizebcd(currIB->c);
    currBd2->qd = quantizebcd(currIB->d);
    currBd2->iapb = Arith40_index_of_chroma(currIB->apb);
    currBd2->iapr = Arith40_index_of_chroma(currIB->apr);
} 

/*
 dequantize
 Purpose: Dequantizes the coefficients and the average pb pr values.
 Parameters: 
            Bundle2 currBd2 - struct that holds the quantized coefficients and 
                              the quantized average pb, pr values.
           InterBundle currIB - struct that holds the coefficients and the 
                                 average pb, pr values
 Returns: n/a
*/
void dequantize(Bundle2 currBd2, InterBundle currIB)
{
    currIB->a = dequantizea(currBd2->qa);
    currIB->b = dequantizebcd(currBd2->qb);
    currIB->c = dequantizebcd(currBd2->qc);
    currIB->d = dequantizebcd(currBd2->qd);
    currIB->apb = Arith40_chroma_of_index(currBd2->iapb);
    currIB->apr = Arith40_chroma_of_index(currBd2->iapr);
} 

/*
 quantizea
 Purpose: quantizes the a value 
 Parameters: 
            float val - the coefficient a value 
 Returns: unsigned scale integer
*/
uint64_t quantizea(float val)
{
    uint64_t result = 63 * val;
    return result;
}

/*
 dequantizea
 Purpose: dequantizes the a value 
 Parameters: 
            uint64_t val - quantized a value
 Returns: float value of a which is the cosine coefficient
*/
float dequantizea(uint64_t val)
{
    float result = val / 63.;
    return result;
}

/*
 quantizebcd
 Purpose: quantizes the b, c, and d values
 Parameters: 
            float val - the coefficient b, c, or d values
 Returns: signed scale integer
*/
int64_t quantizebcd(float val)
{
    int64_t result = 0;

    if (val > 0.3) {
        result = 31;
    } else if (val < -0.3) {
        result = -31; 
    } else {
        result = (int64_t)(val / 0.009677);
    }

    return result;
}

/*
 dequantizebcd
 Purpose: dequantizes the b,c, and d values 
 Parameters: 
            int64_t val - quantized b, c, or d values
 Returns: float value of b, c, d which are the cosine coefficients
*/
float dequantizebcd(int64_t val)
{
    float result = val * 0.009677;

    return result;
}

/*
applyDCTQ
 Purpose: perform the discrete cosine transorm, quantize the values of 
          the average component video color space 
 Parameters: 
            int i - ith column of array2 
            int j - jth row of array2
            A2Methods_UArray2 array2 - uarray2 that holds a struct of bundle1s
                                       which has the average component video
                                       color spaces
            void *elem - points to the element at(i,j) in array2 
            void *cl - a void pointer that points to a user defined 
                       variable that can be used for multiple 
                       purposes
 Returns: n/a
*/
void applyDCTQ(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    InterBundle currIB = malloc(sizeof(*currIB));

    DCT((Bundle1)elem, currIB);

    Bundle2 currBd2 = currM->at((A2Methods_UArray2) cl, i, j);

    quantize(currIB, currBd2);

    free(currIB);
}

/*
 applyinDCTQ
 Purpose: perform the inverse discrete cosine transorm, dequantize the values of
          the quantized coefficients and quantized average pb pr values.
 Parameters: 
            int i - ith column of array2 
            int j - jth row of array2
            A2Methods_UArray2 array2 - uarray2 that holds a struct of bundle1s
                                       which has the average component video
                                       color spaces
            void *elem - points to the element at(i,j) in array2 
            void *cl - a void pointer that points to a user defined 
                       variable that can be used for multiple 
                       purposes
 Returns: n/a
*/
void applyinDCTQ(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;

    Bundle1 currBd1 = currM->at((A2Methods_UArray2) cl, i, j);

    InterBundle currIB = malloc(sizeof(*currIB));

    dequantize((Bundle2)elem, currIB);

    inverseDCT(currIB, currBd1);

    free(currIB);
    
}
