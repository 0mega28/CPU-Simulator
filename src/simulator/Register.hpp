#pragma once

#include <iostream>

#include "Instruction.hpp"

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
};

class RegSet
{
public:
	inline static int pc = 0;
	inline static instructionPointer ip = {0};

	/* 16 General Purpose Registers and 17th ax register for (LAX and STX) */
	inline static int gpr[17] = {0};

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

	static void dumpRegisters();
};

void RegSet::dumpRegisters()
{
	using namespace std;
	cout << "Registers: " << endl;
	cout << "\nPC: " << RegSet::pc << endl;
	cout << "\nIP: ";
	ip.i->dumpInstruction();

	cout << "\nGeneral Purpose Registers" << endl;
	for (int i = 0; i < 16; i++)
	{
		cout << "R" << i << ": " << RegSet::gpr[i] << '\t';

		if ((i + 1) % 4 == 0)
			cout << endl;
	}

	cout << "\nIntermediate Registers" << endl;
	cout << "CR: " << RegSet::cr.value << '\t';
	cout << "IR1: " << RegSet::ir1 << '\t';
	cout << "IR2: " << RegSet::ir2 << '\t';
	cout << "IR3: " << RegSet::ir3 << endl;
}
