#pragma once

#include "fu_status.hpp"

class Op_fetch
{
private:
	/*
	 * read operands for fu pointed by fu_index
	 * it is not necessary that operand read is required
	 * returns true if it actually performs any operation else false
	 */
	bool read_operands(int fu_index);

public:
	void cycle();
};

bool Op_fetch::read_operands(int fu_index)
{
	/* Get the current fu status entry */
	auto &fue = fu_status[fu_index];

	/* Wait until all source operands are available (Removes RAW) */
	if (!fue.rj || !fue.rk || !fue.rl)
		return false;

	if (fue.fetched)
		return false; /* Operand already fetched */

	/* Fetch the register is f* is not -1 */
	if (fue.fj != -1)
		fue.fj = RegSet::gpr[fue.fj];
	if (fue.fk != -1)
		fue.fk = RegSet::gpr[fue.fk];
	if (fue.fl != -1)
		fue.fl = RegSet::gpr[fue.fl];

	/* set fetched state to true */
	fue.fetched = true;
	return true;
}

void Op_fetch::cycle()
{
	/* loop over all fu */
	for (int i = 0; i < (int)fu_status.size(); i++)
	{
		/* Get the reference to current fu */
		auto &fu_entry = fu_status[i];

		/* If the fu is busy we may need to read the operand */
		if (fu_entry.busy)
		{
			bool status = read_operands(i);
#ifdef OP_FETCH_LOG
			if (status)
			{
				std::cout<<"Operand Fetch: ("<<i<<")"<<std::endl;
				dump_fu_entry((fu_enum)i);
			}
				
#endif
		}
	}
}
