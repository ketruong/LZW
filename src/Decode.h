#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "code.h"
extern unsigned int bits;

/*
    1. Read in maxbits and the escape flag from input 
    2. Create the table 
    3. Initialize KwKwK flag to be false
    4. Initialize oldCode = empty 
    5. Initialize bits = 9 
    6. While c = getBits(bits) != EOF
        1. newCode = Code = c 
        2. switch(Code)
            1. ESC: oldC = EMP, output next char, and insert into table
            2. INC: increase bits by 1
            3. PRU: oldC = EMP and prune table
            4. END: exit loop
        3. If KwKwK is found 
            1. Code = oldC
            2. Set the KwKwK flag
        4. Else Set the KwKwK flag to false
        5. Recursively print the string 
        6. If the KwKwK flag is set
            1. Output the last character
        7. If oldC is not EMP
            1. Insert into the table 
        8. oldC = newC
    7. Destroy Table  
*/

codeTable * decode();


#endif
