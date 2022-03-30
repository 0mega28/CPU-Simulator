#pragma once

#include <string>
#include <bitset>
#include <setjmp.h>

#define NUM_REGS 16
#define CPU_ARCH 16

/* Log level */
#define FETCH_LOG
#define DECODE_LOG
#define EXECUTE_LOG
#define RETIRE_LOG

enum fu
{
	FETCH,
	DECODE,
	EXECUTE,
	RETIRE,
	NUM_FU
};

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

inline std::string op_string[] = {
    "ADD",
    "ADDI",
    "SUB",
    "SUBI",
    "MUL",
    "MULI",
    "LD",
    "ST",
    "JMP",
    "BEQZ",
    "HLT",
    "LAX",
    "STX"};

/* Reference for halt instruction to exit out of while loop */
inline jmp_buf halt_cpu;
