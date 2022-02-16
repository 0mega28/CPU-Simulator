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
	static int pc;
	static instructionPointer ip;

	/* 16 General Purpose Registers and 17th ax register for (LAX and STX) */
	static int gpr[17];

	/*
	 * Control and intermediate registers values
	 * fetched by decode unit from registers will
	 * be stored in intermediate register
	 * ir1 will store the destination register (if any used)
	 * ir1 and ir2 will store the source registers (values)
	 */
	static intermReg cr;
	static int ir1;
	static int ir2;
	static int ir3;

	/*
	 * aluout stores result calculated by ALU
	 * bt stores if actually we will take the
	 * branch or not in case of BEQZ instr and
	 * true if the instruction is JMP
	 * dr stores the destination (register or address)
	 */
	static intermReg aluout;
	static bool bt;
	static destinationReg dr;

	static void dumpRegisters();
};

int RegSet::pc = 0;
instructionPointer RegSet::ip = {0};

int RegSet::gpr[17] = {0};

intermReg RegSet::cr = {0};
int RegSet::ir1 = 0;
int RegSet::ir2 = 0;
int RegSet::ir3 = 0;

intermReg RegSet::aluout = {0};
bool RegSet::bt = false;
destinationReg RegSet::dr = {0};

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
