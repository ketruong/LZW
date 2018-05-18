C = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
PRGM = decode encode 
all: ${PRGM}

${PRGM}: lzw 
	ln -f $< $@

lzw: lzw.o code.o trie.o Decode.o Encode.o
	 ${CC} ${CFLAGS} -o $@ $^

trie.o: trie.h
Decode.o: Decode.h code.o trie.o
Encode.o: Encode.h code.o trie.o

clean: 
	rm -if *.o *~
