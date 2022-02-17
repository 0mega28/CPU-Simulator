#include <iostream>
#include <setjmp.h>

#include "Fetch.hpp"
#include "Decode.hpp"
#include "Execute.hpp"
#include "Retire.hpp"
#include "Register.hpp"
#include "InstructionMemory.hpp"
#include "DataMemory.hpp"
#include "../utils.hpp"

Fetch *fetch;
Decode *decode;
Execute *execute;
Retire *retire;

InstructionMemory *instructionMemory;
DataMemory *dataMemory;

unsigned int CPUclock = 1;

void initialize(std::string binFile)
{
	instructionMemory = new InstructionMemory(binFile);
	dataMemory = new DataMemory();

	fetch = new Fetch(instructionMemory);
	decode = new Decode();
	execute = new Execute();
	retire = new Retire(dataMemory);
}

void cycle()
{
	std::cout << "\nCPU Cycle: " << CPUclock << std::endl;

	retire->cycle();
	execute->cycle();
	decode->cycle();
	fetch->cycle();

	CPUclock++;
}

void clean_memory()
{
	/* Gracefully clean all the memory allocated by the program */
	delete instructionMemory;
	delete dataMemory;

	delete fetch;
	delete decode;
	delete execute;
	delete retire;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " < binary file>" << std::endl;
		exit(1);
	}

	std::cout << "Starting Simulator" << std::endl;

	initialize(argv[1]);
	instructionMemory->dumpMemory();

	while (!setjmp(halt_cpu))
		cycle();

	std::cout << std::endl;

	dataMemory->dumpMemory();
	RegSet::dumpRegisters();

	std::cout << "Simulation Complete" << std::endl;

	clean_memory();
	return 0;
}
