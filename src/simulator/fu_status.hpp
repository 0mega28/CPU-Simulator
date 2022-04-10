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
	/* flags that indicates when f* is ready for and are not yet read */
	bool rj, rk, rl;
	/*
	 * is the operand fetched
	 * it also signal the execution unit that it can perform operation
	 */
	bool fetched;
	/*
	 * has the fu executed
	 * it also signals the retire unit that it can perform operation
	 */
	bool executed;
	/* aluout value */
	int aluout;
	/* index of instruction (Stored for calculating pc for JMP and BEQZ) */
	int idx;

	void reset_entry();
};

void fu_status_entry::reset_entry()
{
	busy = false;
	op = op_enum::DMY;
	fj = fk = fl = imm = fi = -1;
	qj = qk = ql = DMY_FU;
	rj = rk = rl = true;
	fetched = executed = false;
	aluout = idx = 0;
}

/*
 * Tracks the status of the functional units
 * Core class constructor initialises the array with all fu busy state to false
 */
inline std::array<fu_status_entry, fu_enum::NUM_FU> fu_status;

inline void dump_fu_entry(fu_enum fu)
{
	using namespace std;
	auto &fue = fu_status[fu];

	cout << "FU: " << fu << " busy: " << fue.busy << endl;
	cout << "Op: " << fue.op << " isfetched: " << fue.fetched << " isExecuted: " << fue.executed << endl;
	cout << "Src reg fj: " << fue.fj << " fk: " << fue.fk << " fl: " << fue.fl << endl;
	cout << "imm: " << fue.imm << endl;
	cout << "Dest reg fi: " << fue.fi << endl;
	cout << "FU prod qj: " << fue.qj << " qk: " << fue.qk << " ql: " << fue.ql << endl;
	cout << "Src reg ready rj: " << fue.rj << " rk: " << fue.rk << " rl: " << fue.rl << endl;
	cout << "aluout: " << fue.aluout << " idx: " << fue.idx << endl;
	cout << endl;
}

/*
 * Why NUM_REGS + 1?
 * NUM_REGS = 16 general purpose register + 1 ax register
 * Core class constructor initialises the array with value fu_enum::DMY_FU
 * It stores which reservation statiion will write result into the register
 */
inline std::array<fu_enum, NUM_REGS + 1> reg_status;
