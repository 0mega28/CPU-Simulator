#include <iostream>

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
	std::cout << "\nCPU Cycle: " << CPUclock << std::endl;
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
		std::cout << "Usage: " << argv[0] << " < binary file>" << std::endl;
		exit(1);
	}

	std::cout << "Starting Simulator" << std::endl;

	initialize(argv[1]);

	while (1)
		cycle();

	std::cout << std::endl;

	instructionMemory->dumpMemory();
	RegSet::dumpRegisters();

	std::cout << "Simulation Complete" << std::endl;

	return 0;
}
