#include "trie.h"

#define INIT(p,c) (p)                               // Initial slot
#define STEP(p,c) (2 * (((p)<<CHAR_BIT)|(c)) + 1)   // Step to next slot
#define REDUCE(h,size) ((h) & ((size)-1))           // Reduce to [0,SIZE-1)

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
    return (entries) == size; 
}

// for encode: hash function 
int hash(unsigned int p, unsigned int c, int stepper) {
    return REDUCE(INIT(p,c) + stepper * STEP(p,c), size);
}

// creates table of size of 2^ MAXBITS with an array and hashTable 
codeTable * createTable(int maxbits, int escape) {
    size = 1 << (maxbits);
    
    codeTable * table = calloc(1, sizeof(codeTable));
    table->hashTable = calloc(size, sizeof(tab)); 
    table->array= calloc(size, sizeof(tab ));
    unsigned char i;

    // reserve table space for EMP, ESC, PRU, and INC  
    for (i = 0; i<=5; i++) {
        table->array[entries] = calloc(1, sizeof(tab));
        table->array[entries]->pref = size + i;
        table->array[entries]->c = i;
        table->array[entries]->code = entries;
        entries++; 
    }
    
    // if the escape flag is specified 
    // do not add all 256 ASCII characters in the table
    // there is no prefix if the characters are added in
    if(!escape) for(i = 0; i < 255; i++) {
        table->array[entries] = calloc(1, sizeof(tab));
        table->array[entries]->pref = 0;
        table->array[entries]->c = i;
        table->array[entries]->code = entries;
        entries++; 
    }
    
    return table;
}

 // for decode: find PREF and CHAR given the CODE for a nonempty string
 // uses the array in codeTable
tab * searchPC(codeTable * table, int code) {
    return table->array[code];
}

// for encode: insert into hashTable using double hashing 
void insertTable(struct codeTable * table, unsigned int pref, unsigned int c) {

    //Print(("Attempting to insert new entry...\n"));
    
    // if is full
    if(isFull()) {
        //Print(("Table is full!\n"));
        return;
    }

    // code already inserted
    /*if(searchCode(table,pref,c) >= 0) {
        // Print(("Code already inserted in\n"));
        return;
    }*/
    
    //get index from first hash
    int index = hash(pref, c, 0); 
    
    //if collision occurs
    if (table->hashTable[index] != NULL) {
        
        int i = 1;
        // loop until there is an empty spot;
        while (1) {
            // get newIndex
            index = hash(pref,c,i); 

            // if no collision occurs, store the key
            if (!table->hashTable[index]) {
                break;
            }
            i++;
        }
    } 
    
    // add entry into hashTable 
    table->hashTable[index] = calloc(1,sizeof(tab));
    table->hashTable[index]->pref = pref;
    table->hashTable[index]->c = c;
    table->hashTable[index]->code  = entries;

    // add entry to array
    //table->array[entries] = calloc(1,sizeof(tab));
    table->array[entries] = table->hashTable[index];
    
    // update number of entries in the table
    entries++;
    //Print(("entries = %i\n", entries));
}

// searches the hash table to see if the pref and char is already inserted 
int searchCode(codeTable * table, unsigned int pref, unsigned int c) {
    int index = hash(pref, c, 0);
    int start = index;
    int i = 0;
    
    // loop until the right triple is found else return -1 to indicate not found
    while(table->hashTable[index] != NULL) {
        i++;
        if(table->hashTable[index]->pref == pref && table->hashTable[index]->c == c) 
            return table->hashTable[index]->code;
        index = hash(pref, c, i);
        if(index == start) return -1;
    }
    return -1;
}

// check for KwKwK event
int kwkwkTable(codeTable * table, unsigned int c) {
    // no KwKwK event
    if (c < entries) return 1;
    // KwKwK event
    else if (c == entries) return 0;
    // something bad happen
    DIE("decode: %s", "corrupted file");
}

// for decode: recursively prints out the characters instead of using stack 
unsigned int printTableDecode(codeTable * table, unsigned int code) {
    int K;
    
    if (table->array[code]->pref == EMP) {
        K = table->array[code]->c;
        putchar(K);
    } else {
        K = printTableDecode(table, table->array[code]->pref);
        putchar(table->array[code]->c);
    }

    return K;
}
// printing the array 
void printTable(codeTable * table) {
    for (int i = 0; i < entries; i++) {
        if(table->array[i]) 
            Print(("pref = %u, c = %u, code = %i\n", 
                    table->array[i]->pref, table->array[i]->c, i));
    }
}

// destroy the table 
void destroyTable(struct codeTable * table) {
    for (int i = 0; i < entries; i++) if(table->array[i]) free(table->array[i]);
    free(table->hashTable);
    free(table->array);
    free(table);
}

// prunes table when the table is full 
void pruneTable(codeTable * table) {

}

