#pragma once

#include <array>

#include "../utils.hpp"
#include "Instruction.hpp"

struct fu_status_entry
{
	/* Indicates whether the station is used or not */
	bool busy = false;
	/* Operation to perform in the unit */
	op_enum op;
	/* Source 1 and Source 2 register number, respecively */
	int fj, fk;
	/* Output register number */
	int fi;
	/* Functional unit that will produce the source registers fj, fk */
	fu_enum qj, qk;
};

class Fu_status
{
private:
	/* Entries for different functional unit */
	static inline std::array<fu_status_entry, fu_enum::NUM_FU> entry;

public:
	/*
	 * Init is called from Core class constructor
	 * Set all fu status busy state to false
	 */
	static void init();

	/* Add the instr to the entry for the functional unit fu */
	static void set_entry(Instruction instr, fu_enum fu);

	static fu_status_entry get_entry(fu_enum fu);
};

void Fu_status::init()
{
	entry.fill({.busy = false});
}

#include <cassert>
void Fu_status::set_entry(Instruction instr, fu_enum fu)
{
	// use something else instead of instr arg
	// TODO: implement
	assert(false);
}

fu_status_entry Fu_status::get_entry(fu_enum fu)
{
	return entry[fu];
}

/*
 * Why NUM_REGS + 1?
 * NUM_REGS = 16 general purpose register + 1 ax register
 * Core class constructor initialises the array with value fu_enum::DMY
 * It stores which reservation statiion will write result into the register
 */
inline std::array<fu_enum, NUM_REGS + 1> reg_status;
