#pragma once

#include <iostream>
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
	/* Ready state of src reg, true means the output will be produced by some fu */
	bool rj, rk, rl;
};

/*
 * Tracks the status of the functional units
 * Core class constructor initialises the array with all fu busy state to false
 */
inline std::array<fu_status_entry, fu_enum::NUM_FU> fu_status;

inline void dump_fu_entry(fu_enum fu)
{
	using namespace std;
	auto &fu_entry = fu_status[fu];

	cout << "FU: " << fu << " busy: " << fu_entry.busy << endl;
	cout << "Op: " << fu_entry.op << endl;
	cout << "Src reg fj: " << fu_entry.fj << " fk: " << fu_entry.fk << " fl: " << fu_entry.fl << endl;
	cout << "imm: " << fu_entry.imm << endl;
	cout << "Dest reg fi: " << fu_entry.fi << endl;
	cout << "FU prod qj: " << fu_entry.qj << " qk: " << fu_entry.qk << " ql: " << fu_entry.ql << endl;
	cout << "Src reg ready rj: " << fu_entry.rj << " rk: " << fu_entry.rk << " rl: " << fu_entry.rl << endl;
}

/*
 * Why NUM_REGS + 1?
 * NUM_REGS = 16 general purpose register + 1 ax register
 * Core class constructor initialises the array with value fu_enum::DMY_FU
 * It stores which reservation statiion will write result into the register
 */
inline std::array<fu_enum, NUM_REGS + 1> reg_status;
