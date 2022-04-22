CC=g++
CFLAGS=-g -Wall -Werror

C_SRC=assembler.cpp ../utils.cpp

assembler: ${C_SRC}
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f assembler a.out 2>/dev/null
