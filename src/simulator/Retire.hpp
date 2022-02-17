#pragma once

#include <iostream>

#include "DataMemory.hpp"
#include "Register.hpp"
#include "../utils.hpp"

class Retire
{
	DataMemory *dataMemory;

public:
	Retire(DataMemory *dataMemory);

	void cycle();
};

Retire::Retire(DataMemory *dataMemory)
{
	this->dataMemory = dataMemory;
}

void Retire::cycle()
{
	if (!RegSet::aluout.valid)
		return;

	/* Branch taken */
	if (RegSet::bt)
	{
		/* 
		 * Reverse the effect of fetch unit 
		 * since, fetch unit always increases pc by 1 
		 * we have to substract one in case of branch instr*/
		RegSet::pc -= 1;

		/*
		 * For jmp or branch instruction we consider an
		 * address difference of 2 bytes between instruction
		 * but, we are storing instruction in an array so
		 * we don't need the 2 byte offset
		 */
		RegSet::pc = RegSet::pc + RegSet::aluout.value / 2;
	}

	/* Branch not taken */
	else
	{
		if (RegSet::dr.is_memory)
		{
			/* Memory access required */

			/* Store aluout in memory location dr.value */
			if (RegSet::dr.is_store)
			{
				dataMemory->setData(RegSet::dr.value, RegSet::aluout.value);
			}
			/* Load value from memory location aluout in register dr.value */
			else
			{
				RegSet::gpr[RegSet::dr.value] = dataMemory->getData(RegSet::aluout.value);
			}
		}
		else
		{
			/* Memory acceess not required */

			/* Directly store aluout in register dr.value */
			RegSet::gpr[RegSet::dr.value] = RegSet::aluout.value;
		}
	}

	std::cout << "Retire: " << std::endl;
	std::cout << "bt: " << RegSet::bt << "\tpc: " << RegSet::pc << std::endl;
	std::cout << "dr: " << RegSet::dr.value << "\t" << RegSet::dr.is_memory << "\t" << RegSet::dr.is_store << std::endl;
	std::cout << "aluout: " << RegSet::aluout.value << "\t" << std::endl;
}