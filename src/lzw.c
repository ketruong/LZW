#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Decode.h"
#include "Encode.h"

#define MAX_BITS 12

int main(int argc, char** argv) {
   
    // Turn off buffering (and thereby the allocation of buffers)
    setvbuf (stdin,  NULL, _IONBF, 0);    // Avoid malloc()ing buffers
    setvbuf (stdout, NULL, _IONBF, 0);    // for stdin/stdout
    
    // flags for maxbits, prune, and escape 
    bool p = 0, mb = 0, e = 0;
    int maxbits = MAX_BITS;
    
    // decode
    if(!(strcmp(argv[0], "./decode"))) {
         
        // too many command line arguments for decode
        if(argc > 1) DIE("usage: %s", "decode");
        
        decode();
    
    // encode 
    } else if(!strcmp(argv[0], "./encode")) {
        
        // parsing for -m and -p flag
        for (int i = 1; i < argc; i++) {
            
            if(!strcmp("-m", argv[i])) {

                // more than 1 command line argument for maxBit 
                if(mb) DIE("encode: %s", "invalid maxBit input");
                
                mb = 1;

            //maxbit flag is set
            } else if(mb) {
                int inputMaxBit = atoi(argv[i]);
                
                // -m MAXBITS has 0 or some non-numerical input 
                if (!inputMaxBit) DIE("encode: %s", "invalid maxBit input");
                
                // if valid input, change the maxbits
                if (inputMaxBit > 8 && inputMaxBit <= 20) maxbits = inputMaxBit;
                
                mb = 0;
            
            // prune flag is set
            } else if(!strcmp("-p", argv[i])) {
                p = 1;
            
            // escape flag is set 
            } else if(!strcmp("-e", argv[i])) {
                e = 1;
            
            // incorrect flag
            } else DIE("encode: %s", "invalid input");
            
        }
        encode(maxbits, p, e);
    // error 
    } else DIE("encode/decode: %s", "illegal command name");
    
    return 0;
}
