/* garble.c

     garble N

   Copy stdin to stdout, XORing every other group of N characters
   with a random bit sequence.  */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int c, i, nChar;

    nChar = atoi(argv[1]);

    while ((c = getchar()) != EOF) {
	ungetc (c, stdin);
	for (i = 0; i < nChar; i++)
	    putchar (getchar());
	for (i = 0; i < nChar; i++)
	    putchar (getchar() ^ rand());
    }
}
