CC=g++
CFLAGS=-g -Wall -Werror
assembler: assembler.cpp
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f assembler a.out 2>/dev/null
