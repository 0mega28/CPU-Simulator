#pragma once

#include "InstructionMemory.hpp"
#include "Register.hpp"
#include "../utils.hpp"

class Fetch
{
private:
	/* Reference to instruction memory */
	InstructionMemory *instructionMemory;

	/* Actions to perform when fetch unit is stalled */
	void stall_fetch_unit_action();

public:
	Fetch(InstructionMemory *instructionMemory);
	void cycle();
};

void Fetch::stall_fetch_unit_action()
{
#ifdef FETCH_LOG
	std::cout << "Stall fetch unit" << std::endl;
#endif
	RegSet::fu_ready[fu::FETCH] = false;
}

Fetch::Fetch(InstructionMemory *instructionMemory)
{
	this->instructionMemory = instructionMemory;
}

void Fetch::cycle()
{
	if (!RegSet::fu_ready[fu::DECODE])
	{
		// Decode unit is not ready stall the pipline
		this->stall_fetch_unit_action();
		return;
	}

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

	/* Fetch unit is ready to perform other operations */
	RegSet::fu_ready[fu::FETCH] = true;
}
