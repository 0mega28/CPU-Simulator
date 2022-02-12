#include "Instruction.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

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
		return instructionCache[programCounter];
	}

	void dumpMemory()
	{
		for (int i = 0; i < totalInstructions; i++)
		{
			Instruction *instr = instructionCache[i];
			std::cout << "opcode " << instr->getOpcode() << "\too " << instr->getOutputOperand() << "\tio1 " << instr->getInputOperand1() << "\tio2 " << instr->getInputOperand2() << std::endl;
		}
	}
};