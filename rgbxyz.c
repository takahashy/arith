/*******************************************************************************
 *                               rgbxyz.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *     Implementation of rgbxyz interface
 *     Transforms the RGB information of all pixels in a UArray2 of Float_rgb’s 
 *     into the corresponding XYZ format of the same pixels. ADDITIONALLY, 
 *     Transforms the XYZ format into Float_rgb format which is stored in a 
 *     UArray2.
 *      
 ******************************************************************************/
#include "rgbxyz.h"

#define currM uarray2_methods_plain

typedef struct Float_rgb {
    float red, green, blue;
} *Float_rgb;

void xyztrans(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
void rgbtrans(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);

/*
 toXYZ
 Purpose: transforms an array of Float_rgb to a corresponding array of XYZs 
 Parameters: 
          rgb - an A2Methods_UArray2 that represents an array of Float_rgb and 
                is to be transformed
 Returns: an A2Methods_UArray2 of XYZs that contains the same information as in 
          rgb, except stores the information in a XYZ format.
*/
A2Methods_UArray2 toXYZ(A2Methods_UArray2 rgb)
{
    A2Methods_UArray2 xyz = currM->new(currM->width(rgb), currM->height(rgb), 
                                       sizeof(struct XYZ));
    
    currM->map_row_major(xyz, xyztrans, rgb);
    
    return xyz;
}

/*
 toRGB
 Purpose: transforms an array of XYZ to a corresponding array of Float_rgb 
 Parameters: 
          xyz - an A2Methods_UArray2 that represents an array of XYZ and 
                is to be transformed
 Returns: an A2Methods_UArray2 of Float_rgb that contains the same information 
          as in xyz, except stores the information in a Float_rgb format.
*/
A2Methods_UArray2 toRGB(A2Methods_UArray2 xyz)
{
    A2Methods_UArray2 rgb = currM->new(currM->width(xyz), currM->height(xyz), 
                                       sizeof(struct Float_rgb));
    
    currM->map_row_major(rgb, rgbtrans, xyz);
    
    return rgb;
}

/*
 xyztrans
 Purpose: as an apply function, transforms an element of Float_rgb to an 
          element of XYZ that contains the same brightness and chroma 
          information
 Parameters:
          i - an integer representing the column location of the current 
              element
          j - an integer representing the row location of the current 
              element
          array2 - an A2Methods_UArray2 that represents the array of 
                   Float_rgb
          elem - a void pointer pointing to the current Float_rgb element
          cl   - a void pointer pointing to the new XYZ array to be stored into
 Returns: N/A
*/
void xyztrans(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    Float_rgb currObj = currM->at((A2Methods_UArray2) cl, i, j);

    ((XYZ) elem)->y = 0.299 * currObj->red + 
                      0.587 * currObj->green + 
                      0.114 * currObj->blue;
    ((XYZ) elem)->pb = -0.168736 * currObj->red - 
                       0.331264 * currObj->green + 
                       0.5 * currObj->blue;
    ((XYZ) elem)->pr = 0.5 * currObj->red - 
                       0.418688 * currObj->green - 
                       0.081312 * currObj->blue;
}

/*
 rgbtrans
 Purpose: as an apply function, transforms an element of XYZ to an 
          element of Float_rgb that contains the same brightness and chroma 
          information
 Parameters:
          i - an integer representing the column location of the current 
              element
          j - an integer representing the row location of the current 
              element
          array2 - an A2Methods_UArray2 that represents the array of 
                   XYZ
          elem - a void pointer pointing to the current XYZ element
          cl   - a void pointer pointing to the new Float_rgb array to be 
                   stored into
 Returns: N/A
*/
void rgbtrans(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    XYZ currObj = currM->at((A2Methods_UArray2) cl, i, j);

    ((Float_rgb) elem)->red = 1.0 * currObj->y + 
                            1.402 * currObj->pr;
    ((Float_rgb) elem)->green = 1.0 * currObj->y - 
                              0.344136 * currObj->pb - 
                              0.714136 * currObj->pr;
    ((Float_rgb) elem)->blue = 1.0 * currObj->y + 
                             1.772 * currObj->pb;

}