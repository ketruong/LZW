#include "Decode.h"

// starting number of dbits 
unsigned int dbits = START;

// escape and add to the table
void escape(codeTable * table, int * oldC){
    int c;
    // Get the next 8 bits 
    if ((c = getBits(CHAR_BIT)) != EOF) {
        
        // output the letter 
        putBits(8,c);
        
        // insert into the table 
        insertTable(table, EMP, c, 1);
        
        *oldC = EMP;
    }
}

// safely increase the number of dbits to read
void increment(int maxbits){
    if((dbits+1) <= maxbits) dbits++;
}

// prune table 
void prune(codeTable * table, int * oldC){
    pruneTable(table);
    *oldC = EMP; 
}

codeTable * decode() {

    // get maxbits and the escape flag
    int maxbits = getBits(5);
    int e = getBits(1); 
    
    // Something went wrong with encode
    if (maxbits <= 8 || maxbits > 20) DIE("Decode: %s","corrupted file!"); 
    
    // create the table
    codeTable * table = createTable(maxbits, e, 1);
    
    // KwKwK problem flag;
    int kwkwk = 0;
    

    int * oldCode, *newCode, *Code;
    oldCode = calloc(1,sizeof(int)); 
    newCode = calloc(1,sizeof(int)); 
    Code = calloc(1,sizeof(int)); 
    
    // initially empty 
    *oldCode = EMP;
    
    // for getting characters from the stream
    int c, K; 
    
    while((c = getBits(dbits)) != EOF) {
        
        *newCode =  *Code = c;

        // identify if flags have occurred
        switch(*Code) {
            case ESC:
                escape(table, oldCode);
                continue;
            case INC:
                increment(maxbits);
                continue;
            case PRU:
                prune(table, oldCode);
                continue;
            case END:
                goto exit_loop;
            default:
                break;
        }
        
        // KwKwK found - delay
        if (!kwkwkTable(table, *Code)) {             
            *Code = *oldCode;
            kwkwk = 1;
        }
        
        // it's ok to print 
        else kwkwk = 0;
        
        // recursively print using the Code 
        K = printTableDecode(table, *Code);         
        
        // print last letter because of KwKwK
        if (kwkwk) putBits(8,K);
        
        // add to the table if possible
        if (*oldCode != EMP) insertTable(table,*oldCode, K, 1);
        
        *oldCode = *newCode;
    }

    // received END code
    exit_loop: ;

    // free everything 
    free(oldCode);
    free(newCode);
    free(Code);
    return table; 
}
