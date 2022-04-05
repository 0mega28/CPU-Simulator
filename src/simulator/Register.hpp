#pragma once

#include <iostream>
#include <iomanip>
#include <queue>

#include "Instruction.hpp"
#include "../utils.hpp"

struct instructionPointer
{
	Instruction *i;
	bool valid;
};

struct intermReg
{
	int value;
	bool valid;
};

struct destinationReg
{
	int value;
	bool is_memory;
	bool is_store;
};

/* Instruction queue entry */
// TODO add dump iq_entry function
struct iq_entry
{
	op_enum op;
	int src_reg1;
	int src_reg2;
	int src_reg3;
	int dest_reg;
	int imm_val;
};

class RegSet
{
public:
	inline static int pc = 0;
	inline static instructionPointer ip = {0};

	/* 16 General Purpose Registers and 17th ax register for (LAX and STX) */
	inline static int gpr[NUM_REGS + 1] = {0};

	// TODO: remove the reg_valid as the status is stored in fu_status -> reg_status
	/* Holds the status of the pipeline if the register is written back or not */
	inline static bool reg_valid[17] = {true, true, true, true, true,
					    true, true, true, true, true,
					    true, true, true, true, true,
					    true, true};

	/*
	 * Instruction queue
	 * Decode unit will pick up the instruction decodes the instruction and put into instruction queue
	 * Issue stage will use instruction queue to allocate function unit
	 */
	// TODO: add reset function for decode issue stage
	inline static std::queue<iq_entry> iq = {};

	/*
	 * aluout stores result calculated by ALU
	 * bt stores if actually we will take the
	 * branch or not in case of BEQZ instr and
	 * true if the instruction is JMP
	 * dr stores the destination (register or address)
	 */
	inline static intermReg aluout = {0};
	inline static bool bt = false;
	inline static destinationReg dr = {0};

	/* Flag to store if the current instruction is halt instruction */
	inline static bool is_halt_instr = false;

	static void dumpRegisters();

	/* Reset fetch decode intermediate register */
	static void reset_fetch_decode_im();

	// TODO: add why and when will this be used
	// In case of pipeline flush? yes.
	/* Reset instruction queue */
	static void reset_decode_issue_im();

	/* Reset execute retire intermediate register */
	static void reset_execute_retire_im();

	/* Reset everything */
	static void flush_pipeline();
};

void RegSet::reset_fetch_decode_im()
{
	ip = {0};
}

void RegSet::reset_decode_issue_im()
{
	// TODO: implement
	assert(false);
}

void RegSet::reset_execute_retire_im()
{
	aluout = {0};
	bt = false;
	dr = {0};
}

// TODO: change reset function call
void RegSet::flush_pipeline()
{
	int reg_valid_size = sizeof(reg_valid) / sizeof(reg_valid[0]);

	/* Set all the registers as valid */
	for (int i = 0; i < reg_valid_size; i++)
		reg_valid[i] = true;

	is_halt_instr = false;

	reset_fetch_decode_im();
	// TODO handle decode issue reset
	reset_execute_retire_im();
}

// TODO: check this function for correct dump of register as there will be
// many changes due to implementation of scoreboarding
void RegSet::dumpRegisters()
{
	using namespace std;
	cout << "\nRegisters: " << endl;
	cout << "PC: " << RegSet::pc << endl;

	cout << "General Purpose Registers" << endl;
	for (int i = 0; i < 17; i++)
	{
		cout << "R" << setfill('0') << setw(2) << i << ": " << setfill('0') << setw(4) << RegSet::gpr[i] << '\t';

		if ((i + 1) % 4 == 0)
			cout << endl;
	}

	cout << "\nRegister Validity" << endl;
	for (int i = 0; i < 17; i++)
	{
		cout << "R" << setfill('0') << setw(2) << i << ": " << RegSet::reg_valid[i] << '\t';

		if ((i + 1) % 4 == 0)
			cout << endl;
	}
}
