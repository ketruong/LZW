/* chop.c

     chop N

   copies every other group of N characters from STDIN to STDOUT,
   separated by the string "MISSING\n".                           */

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
	    getchar();
	printf ("MISSING\n");
    }
}
