CC=g++
CFLAGS=-g -Wall -Werror

C_SRC=pipeline.cpp ../utils.cpp
H_SRC=$(wildcard *.hpp ../utils.hpp)

simulator: ${C_SRC} ${H_SRC}
	$(CC) $(CFLAGS) -o $@ ${C_SRC}

clean:
	rm -f simulator
