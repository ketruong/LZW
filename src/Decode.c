#include "Decode.h"

// starting number of dbits 
unsigned int dbits = START;

// escape and add to the table
void escape(codeTable * table, int * oldC){
    int c;
    if ((c = getBits(CHAR_BIT)) != EOF) {
        putchar(c);
        insertTable(table, EMP, c);
        oldC = EMP;
    }
}

// safely increase the number of dbits to read
void increment(int maxbits){
    if((dbits+1) <= maxbits) dbits++;
}

// prune table 
void prune(codeTable * table, int * oldC){
    *oldC = EMP; 
    pruneTable(table);
}

void decode() {

    // get maxbits and the escape flag
    int maxbits = getBits(5);
    unsigned int e = getBits(1); 
    if (maxbits <= 8 || maxbits > 20) DIE("Decode: %s","corrupted file!"); 
    
    // create the table
    codeTable * table = createTable(maxbits, e);
    
    // KwKwK problem flag;
    int kwkwk = 0;
    

    int * oldCode, *newCode, *Code;
    oldCode = calloc(1,sizeof(int)); 
    newCode = calloc(1,sizeof(int)); 
    Code = calloc(1,sizeof(int)); 
    
    *oldCode = EMP;
    // for getting characters from the stream
    unsigned int c, K; 
    
    while((c = getBits(dbits)) != EOF) {
        *newCode =  *Code = c;
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
        if (kwkwk) putchar(K);
        
        // add to the table if possible
        if (*oldCode != EMP) insertTable(table,*oldCode, K);
        
        *oldCode = *newCode;
    }

    // received END code
    exit_loop: ;
    // if (*Code != END) DIE("Decode: %s", "did not end with termination sequence");

    // free everything 
    destroyTable(table);
    free(oldCode);
    free(newCode);
    free(Code);

    // did not receive END code
}
