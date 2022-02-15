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

class RegSet
{
public:
	static int pc;
	static instructionPointer ip;

	/* General Purpose Registers */
	static int gpr[16];

	/*
	 * Control and intermediate registers
	 * Values fetched by decode unit from
	 * registers will be stored in intermediate
	 * register
	 */
	static intermReg cr;
	static int ir1;
	static int ir2;
	static int ir3;

	static void dumpRegisters();
};

int RegSet::pc = 0;
instructionPointer RegSet::ip = {0};

int RegSet::gpr[16] = {0};

intermReg RegSet::cr = {0};
int RegSet::ir1 = 0;
int RegSet::ir2 = 0;
int RegSet::ir3 = 0;

void RegSet::dumpRegisters()
{
	using namespace std;
	cout << "PC: " << RegSet::pc << endl;
	cout << "IP: " << RegSet::ip.i << endl;

	for (int i = 0; i < 16; i++)
	{
		cout << "R" << i << ": " << RegSet::gpr[i] << endl;
	}

	cout << "CR: " << RegSet::cr.value << endl;
	cout << "IR1: " << RegSet::ir1 << endl;
	cout << "IR2: " << RegSet::ir2 << endl;
	cout << "IR3: " << RegSet::ir3 << endl;
}
