#pragma once

#include "InstructionMemory.hpp"
#include "Register.hpp"
#include "../utils.hpp"

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
	/* Fetch instruction from instruction memory */
	Instruction *i = instructionMemory->getInstruction(RegSet::pc);

	/* Update instruction pointer */
	RegSet::ip.i = i;
	RegSet::ip.valid = true;

	/* Increment program counter */
	RegSet::pc++;

#ifdef FETCH_LOG
	std::cout << "Fetch: " << std::endl;
	i->dumpInstruction();
#endif
}
