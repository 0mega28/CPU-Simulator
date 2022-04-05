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
	/* Source 1,2,3 register number, respecively */
	int fj, fk, fl;
	/* Immediate value */
	int imm;
	/* Output register number */
	int fi;
	/* Functional unit that will produce the source registers fj, fk, fl*/
	fu_enum qj, qk, ql;
};

/*
 * Tracks the status of the functional units
 * Core class constructor initialises the array with all fu busy state to false
 */
inline std::array<fu_status_entry, fu_enum::NUM_FU> fu_status;

/*
 * Why NUM_REGS + 1?
 * NUM_REGS = 16 general purpose register + 1 ax register
 * Core class constructor initialises the array with value fu_enum::DMY
 * It stores which reservation statiion will write result into the register
 */
inline std::array<fu_enum, NUM_REGS + 1> reg_status;
