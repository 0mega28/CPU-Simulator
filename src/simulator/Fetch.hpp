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
	if (RegSet::decode_stall)
		return;
	/* Fetch instruction from instruction memory */
	Instruction *i = instructionMemory->getInstruction(RegSet::pc);

	if (i)
	{
		/* Update instruction pointer */
		RegSet::ip.i = i;
		RegSet::ip.valid = true;
	}
	else
	{
		/* Invalid instruction */
		RegSet::ip.valid = false;
		return;
	}

	/* Increment program counter */
	RegSet::pc++;

#ifdef FETCH_LOG
	std::cout << "Fetch: " << std::endl;
	i->dumpInstruction();
#endif
}
