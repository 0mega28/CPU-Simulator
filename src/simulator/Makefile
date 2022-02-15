CC=g++
CFLAGS=-g -Wall -Werror

simulator: pipeline.cpp ../utils.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f simulator
