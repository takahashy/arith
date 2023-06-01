/*******************************************************************************
 *                               wordTransform.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *     Implementation of the wordTranform inteface.
 *     Transform information contained in Bundle2 structs to a 32-bit word that 
 *     contains the identical information (in a uint64_t variable). This step
 *     is implemented with guide from bitpack.c
 *      
 ******************************************************************************/
#include "wordTransform.h"

#define currM uarray2_methods_plain

void applyCode(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
void applydecode(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
uint64_t bd2ToWord(Bundle2 currBd2);
void wordToBd2(Bundle2 currBd2, uint64_t currWord);

/*
 codeWord 
 Purpose: transforms an array of Bundle2 structs to an array of uint64_t that 
          contains 32-bit words which corresponds to the data contained in the 
          Bundle2 structs
 Parameters: 
          bd2Ary  -  an A2Methods_UArray2 of Bundle2 to be transformed
 Returns: an A2Methods_UArray2 of uint64_t which contains 32-bit words that 
          bitpacked the information from Bundle2s

*/
A2Methods_UArray2 codeWord(A2Methods_UArray2 bd2Ary)
{
    A2Methods_UArray2 wordAry = 
    currM->new(currM->width(bd2Ary), currM->height(bd2Ary), sizeof(uint64_t));
    
    currM->map_row_major(bd2Ary, applyCode, wordAry);
    
    return wordAry;
}

/*
 decodeWord 
 Purpose: transforms an array of uint64_t that contains 32-bit words to an 
          array of Bundle2 structs which contain identical information as
          the 32-bit words in the original array
 Parameters: 
          words  -  an A2Methods_UArray2 of words to be deworded
 Returns: an A2Methods_UArray2 of Bundle2s in which each Bundle2 of the new 
          array contains identical information by unpacking the 32-bit words 

*/
A2Methods_UArray2 decodeWord(A2Methods_UArray2 words)
{
    A2Methods_UArray2 bd2Ary = 
    currM->new(currM->width(words), currM->height(words), 
                                    sizeof(struct Bundle2));
    
    currM->map_row_major(words, applydecode, bd2Ary);
    
    return bd2Ary;
}

/*
 applyCode
 Purpose: applies the bd2ToWord function
 Parameters: 
             int i - ith column of array2 
             int j - jth row of array2
             A2Methods_UArray2 array2 - 2 dimensional array
             void *elem - points to the element at(i,j) in array2 
             void *cl - a void pointer that points to a user defined 
                        variable that can be used for multiple 
                        purposes
 Returns: n/a
*/
void applyCode(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    *((uint64_t *)currM->at(((A2Methods_UArray2) cl), i, j))
     = bd2ToWord((Bundle2) elem);
}

/*
 applydecode
 Purpose: applies the wordToBd2 function 
 Parameters:
             int i - ith column of array2 
             int j - jth row of array2
             A2Methods_UArray2 array2 - 2 dimensional array
             void *elem - points to the element at(i,j) in array2 
             void *cl - a void pointer that points to a user defined 
                        variable that can be used for multiple 
                        purposes
 Returns: n/a
*/
void applydecode(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    wordToBd2((Bundle2)(currM->at(((A2Methods_UArray2) cl), i, j)), 
                                              *((uint64_t *)elem));
    
}

/*
 bd2ToWord
 Purpose: bitpack all the information into a word
 Parameters: 
            Bundle2 currBd2 - struct that holds quantized coefficents and 
                              quantized average pb pr values.
 Returns: unsigned integer that holds a word
*/
uint64_t bd2ToWord(Bundle2 currBd2)
{
    uint64_t currWord = 0;
    
    currWord = Bitpack_newu(currWord, 4, 0, currBd2->iapr);
    currWord = Bitpack_newu(currWord, 4, 4, currBd2->iapb);
    currWord = Bitpack_news(currWord, 6, 8, currBd2->qd);
    currWord = Bitpack_news(currWord, 6, 14, currBd2->qc);
    currWord = Bitpack_news(currWord, 6, 20, currBd2->qb);
    currWord = Bitpack_newu(currWord, 6, 26, currBd2->qa);
    
    return currWord;
}

/*
 wordToBd2
 Purpose: unpack all the information from a word
 Parameters:
             Bundle2 currBd2 - struct that holds quantized coefficents and 
                               quantized average pb pr values.
             uint64_t currWord - unsigned integer that holds a word
 Returns: n/a

*/
void wordToBd2(Bundle2 currBd2, uint64_t currWord)
{
    currBd2->iapr = Bitpack_getu(currWord, 4, 0);
    currBd2->iapb = Bitpack_getu(currWord, 4, 4);
    currBd2->qd = Bitpack_gets(currWord, 6, 8);
    currBd2->qc = Bitpack_gets(currWord, 6, 14);
    currBd2->qb = Bitpack_gets(currWord, 6, 20);
    currBd2->qa = Bitpack_getu(currWord, 6, 26);
}