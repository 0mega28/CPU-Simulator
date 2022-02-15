#pragma once

#include "InstructionMemory.hpp"
#include "Register.hpp"

class Fetch
{
private:
	/* Reference to instruction memory */
	InstructionMemory *instructionMemory;

public:
	Fetch(InstructionMemory *instructionMemory);
	void cycle();
};

Fetch::Fetch(InstructionMemory *instructionMemory)
{
	this->instructionMemory = instructionMemory;
}

void Fetch::cycle()
{
	std::cout << "Fetch: " << std::endl;

	/* Fetch instruction from instruction memory */
	Instruction *i = instructionMemory->getInstruction(RegSet::pc);

	/* Update instruction pointer */
	RegSet::ip.i = i;
	RegSet::ip.valid = true;

	/* Increment program counter */
	RegSet::pc++;

	i->dumpInstruction();
}
