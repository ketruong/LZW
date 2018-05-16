C = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
PRGM = decode encode 

all: ${PRGM}

${PRGM}: lzw 
	ln -f $< $@

lzw: lzw.o code.o trie.o Decode.o Encode.o
	 ${CC} ${CFLAGS} -o $@ $^

Decode.o: Decode.h code.o trie.o
Encode.o: Encode.h code.o trie.o
code.o: code.h
trie.o: trie.h

clean: 
	rm -if *.o *~
