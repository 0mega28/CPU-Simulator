#pragma once

#include <iostream>
#include <iomanip>
#include <queue>
#include <cassert>

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
	int dest_reg = -1;
	int imm_val;
	int idx;
};

class RegSet
{
public:
	inline static int pc = 0;
	inline static instructionPointer ip = {0};
	inline static bool decode_halt = false;

	/* 16 General Purpose Registers and 17th ax register for (LAX and STX) */
	inline static int gpr[NUM_REGS + 1] = {0};

	/*
	 * Instruction queue
	 * Decode unit will pick up the instruction decodes the instruction and put into instruction queue
	 * Issue stage will use instruction queue to allocate function unit
	 */
	inline static std::queue<iq_entry> iq = {};

	static void dumpRegisters();

	/* Reset fetch decode intermediate register */
	static void reset_fetch_decode_im();

	/* Reset everything */
	static void flush_pipeline();

	inline static bool branch_taken = false;
};

void RegSet::reset_fetch_decode_im()
{
	ip = {0};
}

// TODO: change reset function call
void RegSet::flush_pipeline()
{
	reset_fetch_decode_im();
	/* Empty the instr queue */
	iq = {};
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
}
