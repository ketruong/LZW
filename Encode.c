#include "Encode.h"

// starting number of bits is 9
unsigned int bits = START;

void prune(codeTable * table) {
    pruneTable(table);
    putBits(bits, PRU); 
    bits = 0;
    while((1 << ++bits) < entries);
}

// if the table has space, add to the table 
// if there is no space and the prune flag is enabled, prune
void addorPrune(codeTable * table, unsigned int pref, unsigned int c, int p) {
    if (isFull()) {if(p) prune(table);}
    else insertTable(table, pref, c);
}

void encode(int maxbits, int p, int e) {
    // create table
    codeTable * table = createTable(maxbits, e);
    /*  Uncomment to test table 
        
        codeTable * table = createTable(12,  e);
        insertTable(table, 12, 12);
        insertTable(table, 12, 12);
        insertTable(table, 12, 12);
        insertTable(table, 12, 12);
        insertTable(table, 1, 12);
        insertTable(table, 12, 11);
        insertTable(table, 12, 10);
        printf("index in table: %i\n", searchCode(table, 12,12));
        printTable(table);
    
    */
     
    // code is empty at the beginning 
    unsigned int code = EMP;


    // input character
    unsigned int c; 
    
    // used if number of bits needs to be increased

    // send number of bits and escape bit 
    putBits(bits, maxbits);
    // putBits(1, e);

    // loop through input 
    while((c = getchar()) != EOF){

        // if the number of entries is greater than 2^bits + 1, add another bit 
        // signal that the number of bits has increased 
        if((entries > ((1 << bits) + 1)) && ((bits + 1) <= maxbits)) {
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
            // output the code
            } else {
                putBits(bits, code);
            }
            // check to see if (EMP, CHAR) is in the table     
            searched = searchCode(table, EMP, c);
            
            // Must send ESC code first 
            if (searched < 0) {
                ungetc(c, stdin);
                code = EMP;
            } else {
                
                // add (CODE, CHAR) to the table 
                addorPrune(table, code, c, p);
                
                // code is associated with (EMP, CHAR) pair
                if(p) code = searchCode(table, EMP, c);
                else code = searched;
            }
        }
    }
    if(code != EMP) {
        putBits(bits, code);
    }

    flushBits();
    destroyTable(table);
}