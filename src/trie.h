#ifndef TRIE_H 
#define TRIE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "/c/cs323/Hwk4/code.h"
// Error Printing 
#define WARN(format,...) fprintf (stderr, "" format "\n", __VA_ARGS__)
#define DIE(format,...)  WARN(format,__VA_ARGS__), exit (EXIT_FAILURE)

// Empty, Escape, Prune, Increase, and End
#define EMP 0
#define ESC 1
#define PRU 2
#define INC 3
#define END 4

// Starting number of bits 
#define START 9

// how many entries are in the table 
extern int entries; 

// size of the table
extern int size; 

// entry in the table 
typedef struct tab {
    unsigned int pref: 20;
    int c;
    unsigned int code: 20;
} tab;

// code Table is represented as a hashTable and as an array
typedef struct codeTable {
    struct tab * hashTable;
    struct tab * array;
} codeTable;

// check if the table is full 
bool isFull();

// creates the table with 2^maxbits entries 
// allocates space for an array and hash table 
codeTable * createTable(int maxbits, int escape, int d);

// searches the hash table to see if the pref and char is already inserted 
int searchCode(codeTable * table, unsigned int pref, int c);

// inserts the pref and char into the hash table and array
void insertTable(codeTable * table, unsigned int pref, int c, int d);

// check for KwKwK event
int kwkwkTable(codeTable * table, int c); 

// for decode: recursively prints out the characters instead of using stack 
unsigned int printTableDecode(codeTable * table, int c); 

// frees all the memory
void destroyTable(codeTable * table);

// prunes table when the table is full 
void pruneTable(codeTable * table);
#endif
