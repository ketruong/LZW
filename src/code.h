// code.h                                         Stan Eisenstat (10/11/17)
//
// Interface to putBits() / getBits() / flushBits()

#include <limits.h>

// Maximum allowed value of NBITS
#define MAX_NBITS ((sizeof(long)-1) * CHAR_BIT)

// Write the low-order NBITS bits of CODE to a bit stream on the standard
// output.  If CODE < 0 or 1<<NBITS <= CODE, write an error message to
// the standard error instead.

// The bit stream is padded with up to CHAR_BIT-1 zero bits so that its
// length is a multiple of CHAR_BIT, and then is broken into a sequence of
// CHAR_BIT-bit characters that are written to the standard output.  To do so
// some low-order bits in CODE may have to be saved until the next call to
// putBits(), and the final call must be followed by a call to flushBits().

// When the environment variable STAGE exists and is equal to 1, CODE is
// written in ASCII using the format "%d\n".  When STAGE is equal to 2, NBITS
// and CODE are written using "%d:%d\n".

void putBits (int nBits, int code);


// If there are any bits in the bit stream being saved for the next call to
// putBits(), write to the standard output a character containing those bits
// padded with up to CHAR_BIT-1 zero bits.

// When the environment variable STAGE exists and is equal to 1 or 2, return
// instead.

void flushBits (void);


// Return an integer whose low-order bits are the next NBITS bits from a
// bit stream on the standard input and whose high-order bits are zero.
// Return EOF if there are fewer than NBITS remaining.

// The bit stream is assumed to be padded with up to CHAR_BIT-1 zero bits
// so that its length is a multiple of CHAR_BIT and to be broken into
// CHAR_BIT-bit characters.

// When the environment variable STAGE exists and is equal to 1, the bits are
// read as an ASCII integer CODE using the format "%d\n".  When STAGE is equal
// to 2, the number of bits NRCVD and CODE are read using "%d:%d\n" and an
// error message is written to the standard error if NBITS != NRCVD.  In both
// cases an error message is written to the standard error if CODE < 0 or
// 1<<NBITS <= CODE.

int getBits (int nBits);
