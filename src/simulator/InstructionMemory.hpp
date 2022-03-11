#pragma once

#include "Instruction.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>

#include "../utils.hpp"

class InstructionMemory
{
private:
	Instruction **instructionCache;
	const int instructionCacheSize = 1024;
	int totalInstructions;

public:
	InstructionMemory(std::string filename)
	{
		instructionCache = new Instruction *[instructionCacheSize];
		totalInstructions = 0;

		std::fstream newfile;
		newfile.open(filename, std::fstream::in);

		/* Check if file opened properly */
		if (!newfile.is_open())
		{
			std::cerr << "Error opening file: " << filename << std::endl;
			exit(1);
		}

		std::string binaryInstruction;
		while (getline(newfile, binaryInstruction))
		{
			Instruction *i = new Instruction(binaryInstruction);
			instructionCache[totalInstructions++] = i;
		}
	}

	~InstructionMemory()
	{
		for (int i = totalInstructions - 1; i >= 0; i--)
		{
			delete (instructionCache[i]);
		}
		delete[] instructionCache;
	}

	Instruction *getInstruction(int programCounter)
	{
		if (programCounter < instructionCacheSize)
			return instructionCache[programCounter];

		else
			return NULL;
	}

	void dumpMemory()
	{
		std::cout << "Instruction Memory: " << std::endl;
		for (int i = 0; i < totalInstructions; i++)
		{
			Instruction *instr = instructionCache[i];
			std::cout << "OPCODE " << op_string[instr->getOpcode()] << "\tO1 " << instr->getop1() << "\tO2 "
				  << instr->getop2() << "\tO3 " << instr->getop3() << std::endl;
		}
	}
};