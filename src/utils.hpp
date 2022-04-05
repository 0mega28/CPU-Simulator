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

/* Returns binary string of an interger value */
std::string int_to_bin(int bits, int value);

/* Returns integer from a binary string */
int bin_to_int(std::string binary, bool isImmediate);

/* Functional unit enum */
enum fu_enum
{
	DMY_FU = -1, /* DMY */
	ALU_FU,	     /* ADD / SUB / LAX / STX */
	MUL_FU,	     /* MUL */
	LDST_FU,     /* LD / ST */
	NUM_FU,
};

/* Operand enum */
enum op_enum
{
	DMY = -1, /* Dummy */
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
	STX,
	NUM_OP
};

/* Operand enum to string */
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
