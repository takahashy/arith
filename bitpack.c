/*******************************************************************************
 *                               bitpack.c
 *
 *     Assignment: arith
 *     Authors:  Etha Hua(thua03), Yuma Takahashi (ytakah01)
 *     Date:     October 27, 2021
 *
 *     Implementation of the bitpack interface. Provide an instrument to store 
 *     and get signed and unsigned integeral values into a uint64_t word. 
 *
 ******************************************************************************/
#include "bitpack.h"
#include <stdio.h>
#include <inttypes.h>


Except_T Bitpack_Overflow = { "Overflow packing bits" };


/*
 Bitpack_fitsu
 Purpose: Determines whether an unsigned n can be represented in width bits.
 Parameters: 
                n  -  an uint64_t that is to be decided whether or not it 
                      fits width bits
            width  -  an unsigned integer representing the number of bits
 Returns: true if n fits width bits, and false otherwise.
*/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    uint64_t upperBound = ((uint64_t)1 << width) - 1;
    
    if (width == 64) {
        upperBound = (uint64_t)~0;
    }
    
    if (n <= upperBound) {
        return true;
    } else {
        return false;
    }
}

/*
 Bitpack_fitss
 Purpose: Determines whether a signed n can be represented in width bits.
 Parameters: 
                n  -  an int64_t that is to be decided whether or not it 
                      fits width bits
            width  -  an unsigned integer representing the number of bits 
 Returns: true if n fits width bits, and false otherwise.
*/
bool Bitpack_fitss(int64_t n, unsigned width)
{
    if (width == 0) {
        return false;
    }
    
    return Bitpack_fitsu((uint64_t)(n + ((int64_t)1 << (width - 1))), width);
}

/*
 Bitpack_getu
 Purpose: Extract an unsigned integer from a uint64_t word
 Parameters:  
                word - an uint64_t word that contains information of bits
               width - an unsigned integer representing the width of the value
                       to be extracted from the word
                 lsb - an unsigned integer representing the least significant 
                       bit of the value to be extracted in the word  
 Returns: an uint64_t that represents the extracted value
 Note: It is a checked run-time error to call Bitpack_getu with a width that 
       does not satisfy 0 <= w <= 64. It is a run-time error to call 
       Bitpack_getu with a width w and lsb that do not satisfy w + lsb <= 64.
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    if (width > 64) {
        RAISE(Bitpack_Overflow);
    }
    if (width + lsb > 64) {
        RAISE(Bitpack_Overflow);
    }
    
    if (width == 64) {
        return word;
    }
    
    if (width == 0) {
        return 0;
    }
    
    uint64_t mask = ((uint64_t)~0) >> (64 - width) << lsb;

    mask &= word;

    return (mask >> lsb);
}

/*
 Bitpack_gets
 Purpose: Extract an signed integer from a uint64_t word
 Parameters:  
                word - an uint64_t word that contains information of bits
               width - an unsigned integer representing the width of the value
                       to be extracted from the word
                 lsb - an unsigned integer representing the least significant 
                       bit of the value to be extracted in the word  
 Returns:  an int64_t that represents the extracted value 
 Note: It is a checked run-time error to call Bitpack_gets with a width that 
       does not satisfy 0 <= w <= 64. It is a run-time error to call 
       Bitpack_gets with a width w and lsb that do not satisfy w + lsb <= 64.
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    uint64_t raw = Bitpack_getu(word, width, lsb);

    int64_t result = Bitpack_getu(raw, width - 1, 0) - 
                    (Bitpack_getu(raw, 1, width - 1) << (width - 1));
    return result;
}

/*
 Bitpack_newu
 Purpose: return a new word which is identical to the original word, except 
          that the field of width-width with least significant bit at lsb is 
          replaced by a width-bit representation of an unsigned value.
 Parameters: 
            word - an uint64_t word that contains information of bits
            width - an unsigned integer representing the width of the value
                    to be put into the word
            lsb - an unsigned integer representing the least significant 
                  bit of location in the word at which the value will be put in
            value - an uint64_t unsigned integer representing the value that is 
                    to be put into the word
 Returns: an uint64_t word which is identical to the original word, except 
          that the field of width-width with least significant bit at lsb is 
          replaced by a width-bit representation of value.
 Note: It is a checked run-time error to call Bitpack_newu with a width that 
 does not satisfy 0 <= w <= 64. It is a run-time error to call 
 Bitpack_newu with a width w and lsb that do not satisfy w + lsb <= 64.
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width,
                      unsigned lsb, uint64_t value)
{
    if (width > 64) {
        RAISE(Bitpack_Overflow);
    }
    if (width + lsb > 64) {
        RAISE(Bitpack_Overflow);
    }
    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    
    if (width == 64) {
        return value;
    }
    
    if (width == 0) {
        return word;
    }
    
    uint64_t mask = (uint64_t)~0 >> (64 - width) << lsb;
    mask = ~mask;
    mask &= word;

    return (mask | (value << lsb));
}

/*
Bitpack_news
Purpose: return a new word which is identical to the original word, except 
         that the field of width-width with least significant bit at lsb is 
         replaced by a width-bit representation of an signed value.
Parameters: 
           word - an uint64_t word that contains information of bits
           width - an unsigned integer representing the width of the value
                   to be put into the word
           lsb - an unsigned integer representing the least significant 
                 bit of location in the word at which the value will be put in
           value - an int64_t signed integer representing the value that is 
                   to be put into the word
Returns: an uint64_t word which is identical to the original word, except 
         that the field of width-width with least significant bit at lsb is 
         replaced by a width-bit representation of value.
Note: It is a checked run-time error to call Bitpack_news with a width that 
does not satisfy 0 <= w <= 64. It is a run-time error to call 
Bitpack_news with a width w and lsb that do not satisfy w + lsb <= 64.
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value)
{
    uint64_t mask = (uint64_t)~0 >> (64 - width);

    mask &= value;

    return Bitpack_newu(word, width, lsb, mask);
}
