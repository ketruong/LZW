#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "code.h"

/* LZW Algorithm - Encode

   1. Create the table 
   2. Code = Empty, nbits = 9
   3. Send the number of bits to represent the maxbits (8) and escape (1)
   *** Looping ***
   4. Get Char until EOF 
    a. If the number of codes is greater thatn 2^nbits and less than 2^maxbits
        1. nbits = nbits + 1
    b. If the (CODE, Char) pair is in the Array
        1. set the code to be the index in the Array
    c. Else 
        *** For the Escape Flag only ***
        1. If the code is empty, this means that that the escape flag was specified
            a. Send Escape code and Char
            b. If there is space in the CodeTable, add the pair in 
            c. Else prune the CodeTable
        2. Else output the code
        *** For the Escape Flag only ***
        3. If the (Char, Empty) pair is not in the table
            a. go back a character and set code to be Empty since the escaped 
            character needs to be added in the table first
        4. Else 
            a. If there is space in the table, add the (Char,Code) pair in and 
            set the code to be the (Code, Empty) pair 
   5. If the code is not empty
    a. send the code out 
   6. Output any lower bits left over

*/
void encode();
#endif
