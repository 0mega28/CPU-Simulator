#include <stdio.h>

#include "Fetch.hpp"
#include "Decode.hpp"
#include "Execute.hpp"
#include "Retire.hpp"
#include "Register.hpp"

#include "InstructionMemory.hpp"

Fetch *fetch;
Decode *decode;
Execute *execute;
Retire *retire;

InstructionMemory *instructionMemory;

unsigned int CPUclock = 0;

void initialize(std::string binFile)
{
	instructionMemory = new InstructionMemory(binFile);

	fetch = new Fetch(instructionMemory);
	decode = new Decode();
	execute = new Execute();
	retire = new Retire();
}

void cycle()
{
	// retire->cycle();
	// execute->cycle();
	decode->cycle();
	fetch->cycle();

	CPUclock++;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("%s %s %s\n", "Usage", argv[0], "filename");
		exit(1);
	}

	initialize(argv[1]);
	instructionMemory->dumpMemory();
	cycle();

	return 0;
}
