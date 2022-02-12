#pragma once

#include <string>
#include <bitset>

#define NUM_REGS 16
#define CPU_ARCH 16

/* Returns binary string of an interger value */
std::string int_to_bin(int bits, int value);

/* Returns integer from a binary string */
int bin_to_int(std::string binary, bool isImmediate);

enum op_enum
{
	ADD,
	ADDI,
	SUB,
	SUBI,
	MUL,
	MULI,
	LD,
	ST,
	JMP,
	BEQZ,
	HLT,
	LAX,
	STX
};