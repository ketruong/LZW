#include "trie.h"

#define INIT(p,c) (p)                               // Initial slot
#define STEP(p,c) (2 * (((p)<<CHAR_BIT)|(c)) + 1)   // Step to next slot
#define REDUCE(h,s) ((h) & ((s)-1))                 // Reduce to [0,SIZE-1)

// printing debugger 
#if 1 
  #define Print(a) printf a
#else
  #define Print(a) (void)0
#endif

// counts how many entries are in the table
int entries = 0;
int size = 0;

// check if the codeTable is full 
bool isFull() {
    return entries >= ((0.99) * size); 
}
void updateArr(codeTable * table, unsigned int pref, int c){
    table->array[entries].pref = pref;
    table->array[entries].c = c;
    table->array[entries].code = entries;
    entries++;
}
// creates table of size of 2^ MAXBITS with an array and hashTable 
codeTable * createTable(int maxbits, int escape, int d) {
    size = 1 << (maxbits);
    
    int i;
    codeTable * table = malloc(sizeof(codeTable));
    if(!d) {
        table->hashTable = malloc(size * sizeof(tab)); 
        for(int i = 0; i < size; i++) {
            table->hashTable[i].pref = size + 7;
        }
    }
    else table->array= malloc(size*sizeof(tab));

    // reserve table space for EMP, ESC, PRU, and INC  
    for (i = 0; i<=5; i++) {
        if(d) updateArr(table, size+i, i); 
        else insertTable(table, size+i, i, d);
    }
    
    // if the escape flag is specified 
    // do not add all 256 ASCII characters in the table
    // there is no prefix if the characters are added in
    if(!escape)
        for(i = 0; i < 256; i++) {
            if(d) updateArr(table,0,i);
            else insertTable(table, 0, i, d);
    }
    return table;
}

// for encode: insert into hashTable using double hashing 
void insertTable(struct codeTable * table, unsigned int pref, int c, int d) {
    
    // if is full
    if(isFull()) return;
    
    // if decode, add to the array
    if (d) updateArr(table, pref, c); 
    
    // if encode, add to the hashTable 
    else { 
        // code already inserted
        /*if(searchCode(table,pref,c) >= 0) {
            // Print(("Code already inserted in\n"));
            return;
        }*/
        
        //get index from first hash
        unsigned in = INIT(pref,c);
        unsigned st = STEP(pref,c);
        unsigned index = REDUCE(in, size); 
        
        //if collision occurs
        if (table->hashTable[index].pref != (size + 7)) {
            
            int i = 1;
            // loop until there is an empty spot;
            while (1) {
                // get newIndex
                in += st; 
                index = REDUCE(in, size); 

                // if no collision occurs, store the key
                if (table->hashTable[index].pref == (size + 7)) {
                    break;
                }
                i++;
            }
        } 
        
        // add entry into hashTable 
        table->hashTable[index].pref = pref;
        table->hashTable[index].c = c;
        table->hashTable[index].code  = entries;
        entries++;
    } 
}
// searches the hash table to see if the pref and char is already inserted 
int searchCode(codeTable * table, unsigned int pref, int c) {
    unsigned int in = INIT(pref,c);
    unsigned int st = STEP(pref,c);
    unsigned int index = REDUCE(in, size); 
    unsigned int start = index;
    
    // loop until the right triple is found else return -1 to indicate not found
    while(table->hashTable[index].pref != (size + 7)) {
        if(table->hashTable[index].pref == pref && table->hashTable[index].c == c) 
            return table->hashTable[index].code;
        in += st;
        index = REDUCE(in, size); 
        if(index == start) return -1;
    }
    return -1;
}

// check for KwKwK event
int kwkwkTable(codeTable * table, int c) {
    // no KwKwK event
    if (c < entries) return 1;
    // KwKwK event
    else if (c == entries) return 0;
    // something bad happen
    DIE("decode: %s", "corrupted file");
}

// for decode: recursively prints out the characters instead of using stack 
unsigned int printTableDecode(codeTable * table, int code) {
    int K;
    
    if (table->array[code].pref == EMP) {
        K = table->array[code].c;
        putBits(8,K);
    } else {
        K = printTableDecode(table, table->array[code].pref);
        putBits(8,table->array[code].c);
    }

    return K;
}

// destroy the table 
void destroyTable(struct codeTable * table) {
    // for (int i = 0; i < entries; i++) if(table->array[i]) free(table->array[i]);
    // free(table->hashTable);
    // free(table->array);
    // free(table);
}

// prunes table when the table is full 
void pruneTable(codeTable * table) {

}

