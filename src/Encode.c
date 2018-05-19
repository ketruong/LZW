#include "Encode.h"

// starting number of bits is 9
unsigned int bits = START;

void pruneHelper(codeTable * table) {
    pruneTable(table);
    putBits(bits, PRU); 
    bits = 0;
    while((1 << ++bits) < entries);
}

// if the table has space, add to the table 
// if there is no space and the prune flag is enabled, prune
void addorPrune(codeTable * table, unsigned int pref, unsigned int c, int p) {
    if (isFull()) {if(p) pruneHelper(table);}
    else insertTable(table, pref, c, 0);
}

codeTable * encode(int maxbits, int p, int e) {
    // create table
    codeTable * table = createTable(maxbits, e, 0 );
    
    // code is empty at the beginning 
    unsigned int code = EMP;

    // input character
    unsigned int c; 
    
    // send number of bits and escape bit 
    putBits(5, maxbits);
    putBits(1, e);

    // loop through input 
    while((c = getchar()) != EOF){

        // if the number of entries is greater than 2^bits + 1, add another bit 
        // signal that the number of bits has increased

        if((entries >= ((1 << bits))) && ((bits + 1) <= maxbits)) {
            putBits(bits, INC);
            bits++;
        }
        
        int searched = searchCode(table, code, c); 
        
        // (CODE, Char) is in the hashTable 
        if(searched >= 0) code = searched;
        
        // not in the hashTable 
        else {
            // ESC is enabled  
            if (code == EMP) {

                // output ESC signal and the character 
                putBits(bits, ESC);
                putBits(CHAR_BIT, c);
                
                // add the letter to the table or prune
                addorPrune(table, EMP, c, p);
            
                continue;    
            // output the code
            } else {
                putBits(bits, code);
            }
            // check to see if (EMP, CHAR) is in the table     
            /*searched = 2;
            
            // enable for ESC
            
            // Must send ESC code first 
            if (searched < 0) {
                ungetc(c, stdin);
                code = EMP;
            } else {*/
                //searched = searchCode(table, EMP, c);
                
                // add (CODE, CHAR) to the table 
            addorPrune(table, code, c, p);
                
                // code is associated with (EMP, CHAR) pair
            if(p) code = searchCode(table, EMP, c);
            else code = (c+6);
            //}
        }
    }
    
    if(code != EMP) putBits(bits, code);
        
    putBits(bits, END);
    //printTable(table);
    flushBits();
    destroyTable(table);
    return table;
}
