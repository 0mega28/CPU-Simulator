#pragma once

#include <iostream>
#include <iomanip>

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

class RegSet
{
public:
	inline static int pc = 0;
	inline static instructionPointer ip = {0};

	/* 16 General Purpose Registers and 17th ax register for (LAX and STX) */
	inline static int gpr[17] = {0};

	/* Holds the status of the pipeline if the register is written back or not */
	inline static bool reg_valid[17] = {true, true, true, true, true,
					    true, true, true, true, true,
					    true, true, true, true, true,
					    true, true};

	/*
	 * tracks the status if decode unit is able to fetch all the operand values
	 * due to data hazards
	 */
	inline static bool is_operand_ready = true;

	/*
	 * Control and intermediate registers values
	 * fetched by decode unit from registers will
	 * be stored in intermediate register
	 * ir1 will store the destination register (if any used)
	 * ir1 and ir2 will store the source registers (values)
	 */
	inline static intermReg cr = {0};
	inline static int ir1 = 0;
	inline static int ir2 = 0;
	inline static int ir3 = 0;

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

	/* Reset decode execute intermediate register */
	static void reset_decode_execute_im();

	/* Reset execute retire intermediate register */
	static void reset_execute_retire_im();

	/* Reset everything */
	static void flush_pipeline();
};

void RegSet::reset_fetch_decode_im()
{
	ip = {0};
}

void RegSet::reset_decode_execute_im()
{
	cr = {0};
	ir1 = 0;
	ir2 = 0;
	ir3 = 0;
}

void RegSet::reset_execute_retire_im()
{
	aluout = {0};
	bt = false;
	dr = {0};
}

void RegSet::flush_pipeline()
{
	int reg_valid_size = sizeof(reg_valid) / sizeof(reg_valid[0]);

	/* Set all the registers as valid */
	for (int i = 0; i < reg_valid_size; i++)
		reg_valid[i] = true;

	is_operand_ready = true;

	is_halt_instr = false;

	reset_fetch_decode_im();
	reset_decode_execute_im();
	reset_execute_retire_im();
}

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

	cout << "Is operand ready: " << RegSet::is_operand_ready << endl;
}
