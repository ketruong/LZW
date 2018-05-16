// code.c                                           Stan Eisenstat (10/11/17)
//
// Implementation of putBits() / flushBits() / getBits() described in code.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "/c/cs323/Hwk4/code.h"
#include "code.h"

// Return the stage number from the environment variable STAGE (or 3 if STAGE
// does not exist or is not equal to 1 or 2).
int stage (void)
{
    char *s = getenv("STAGE");
    if (s && !strcmp (s, "1"))
	return 1;
    else if (s && !strcmp (s, "2"))
	return 2;
    else
	return 3;
}


// == PUTBITS MODULE =======================================================

// Information shared by putBits() and flushBits()
static int nExtra = 0;                  // #bits from previous byte(s)
static unsigned long extraBits = 0;     // Extra bits from previous byte(s)


// Write CODE (NBITS bits) to standard output
void putBits (int nBits, int code)
{
    if (code < 0 || (1 << nBits) <= code) {             // CODE representable?
	fprintf (stderr,
		 "nBits = %d, maxCode = %d, code = %d\n",
		  nBits,      (1<<nBits)-1, code        );
	return;

    } else if (stage() == 1) {
	printf ("%d\n", code);
	return;

    } else if (stage() == 2) {
	printf ("%d:%d\n", nBits, code);
	return;
    }

    if (nBits > MAX_NBITS)
	exit (fprintf (stderr, "putBits: nBits = %d too large\n", nBits));
     
    nExtra += nBits;                                    // Add bits to extraBits
    extraBits = (extraBits << nBits) | code;
    while (nExtra >= CHAR_BIT) {                        // Output whole chars &
	nExtra -= CHAR_BIT;                             //  save remaining bits
	unsigned int c = extraBits >> nExtra;
	putchar (c);
	extraBits ^= c << nExtra;
    }
}


// Flush remaining bits to standard output
void flushBits (void)
{
    if (stage() == 3) {
	if (nExtra != 0)
	    putchar (extraBits << (CHAR_BIT - nExtra));
    }
}


// == GETBITS MODULE =======================================================

// Return next code (#bits = NBITS) from input stream or EOF on end-of-file
int getBits (int nBits)
{
    if (stage() == 1) {
	int code;
	if (scanf ("%d\n", &code) < 1){                 // No code?
	    return EOF;

	} else if (code < 0 || (1 << nBits) <= code) {  // Not within bounds?
	    fprintf (stderr, "nBits = %d, code = %d\n",
			      nBits,      code        );
	    return EOF;

	} else {
	    return code;
	}

    } else if (stage() == 2) {
	int code, nRcvd;
	if (scanf ("%d:%d\n", &nRcvd, &code) < 2        // No NRCVD:CODE or
	      || nRcvd != nBits) {                      //   wrong #bits?
	    return EOF;

	} else if (nRcvd == nBits                       // Wrong #bits or
		   && (code < 0                         //   not within bounds?
		       || (1 << nBits) <= code)) {
	    fprintf (stderr, "nBits = %d, nRcvd = %d, code = %d\n",
			      nBits,      nRcvd,      code        );
	    return EOF;

	} else {
	    return code;
	}
    }

    int c;
    static int nExtra = 0;              // #bits from previous byte(s)
    static long unsigned extra = 0;     // Extra bits from previous byte(s)
									      
    if (nBits > MAX_NBITS)
	exit (fprintf (stderr, "getBits: nBits = %d too large\n", nBits));

    // Read enough new bytes to have at least NBITS bits to extract code
    while (nExtra < nBits) {
	if ((c = getchar()) == EOF)                     // Too few bits?
	    return EOF;
	nExtra += CHAR_BIT;
	extra = (extra << CHAR_BIT) | c;
    }
    nExtra -= nBits;                                    // Return nBits bits
    c = extra >> nExtra;
    extra ^= c << nExtra;                               // Save remainder
    return c;
}
