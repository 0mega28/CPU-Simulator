#pragma once

#include <iostream>

#include "DataMemory.hpp"
#include "Register.hpp"
#include "../utils.hpp"

class Retire
{
	DataMemory *dataMemory;

	void stall_retire_unit_action();

public:
	Retire(DataMemory *dataMemory);

	void cycle();
};

void Retire::stall_retire_unit_action()
{
	// do nothing
}

Retire::Retire(DataMemory *dataMemory)
{
	this->dataMemory = dataMemory;
}

void Retire::cycle()
{
	/* Check if execute unit output is valid (value isn't garbage) */
	if (!RegSet::aluout.valid)
		return;

	/* Halt instruction stop pipeline */
	if (RegSet::is_halt_instr)
		longjmp(halt_cpu, 1);

	if (RegSet::bt)
	{
		/* Branch instruction*/

		int old_pc = RegSet::pc;
		RegSet::pc = RegSet::aluout.value;

		/* Flush the pipeline if branch is taken */
		if (old_pc != RegSet::aluout.value)
		{
			RegSet::flush_pipeline();
		}
	}
	else
	{
		/* Not a branch instruction */
		if (RegSet::dr.is_memory)
		{
			/* Memory access required */

			/* Store aluout in memory location dr.value */
			if (RegSet::dr.is_store)
			{ /* Store instruction */
				dataMemory->setData(RegSet::dr.value, RegSet::aluout.value);
			}
			/* Load value from memory location aluout in register dr.value */
			else
			{ /* Load instruction */
				RegSet::gpr[RegSet::dr.value] = dataMemory->getData(RegSet::aluout.value);

				/* Set register as valid */
				RegSet::reg_valid[RegSet::dr.value] = true;
			}
		}
		else
		{
			/* Memory acceess not required */

			/* Directly store aluout in register dr.value */
			RegSet::gpr[RegSet::dr.value] = RegSet::aluout.value;

			/* Set register as valid */
			RegSet::reg_valid[RegSet::dr.value] = true;
		}
	}

	std::cout << "Retire: " << std::endl;
	std::cout << "bt: " << RegSet::bt << "\tpc: " << RegSet::pc << std::endl;
	std::cout << "dr: " << RegSet::dr.value << "\t" << RegSet::dr.is_memory << "\t" << RegSet::dr.is_store << std::endl;
	std::cout << "aluout: " << RegSet::aluout.value << "\t" << std::endl;

	RegSet::fu_ready[fu::RETIRE] = true;

	/*
	 * Retire unit consumed current instruction so set aluout validity to false
	 * in the next execute cycle it will be set to true by execute unit
	 */
	RegSet::reset_execute_retire_im();
}
