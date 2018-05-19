C = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
PRGM = decode encode 
Hwk = /c/cs323/Hwk4
all: ${PRGM}
${PRGM}: lzw 
	ln -f $< $@

lzw: lzw.o ${Hwk}/code.o trie.o Decode.o Encode.o
	 ${CC} ${CFLAGS} -o $@ $^

trie.o: trie.h ${Hwk}/code.o
Decode.o: Decode.h ${Hwk}/code.o trie.o
Encode.o: Encode.h ${Hwk}/code.o trie.o

clean: 
	rm -if *.o *~
