#pragma once

#include <string>
#include <bitset>
#include <setjmp.h>
#include <unordered_map>

#define NUM_REGS 16
#define CPU_ARCH 16

/* Log level */
#define FETCH_LOG
#define DECODE_LOG
#define ISSUE_LOG
#define OP_FETCH_LOG
#define EXECUTE_LOG
#define RETIRE_LOG

#define ALU_DELAY 2
#define MUL_DELAY 5
#define BRCH_DELAY 3
#define LDST_DELAY 10
#define UTIL_DELAY 0

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
	BRCH_FU,     /* JMP / BEQZ */
	UTIL_FU,     /* HLT */
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

/* map from op_enum to fu_enum */
inline std::unordered_map<op_enum, fu_enum> op_to_fu = {
    {ADD, ALU_FU},
    {ADDI, ALU_FU},
    {SUB, ALU_FU},
    {SUBI, ALU_FU},
    {MUL, MUL_FU},
    {MULI, MUL_FU},
    {LD, LDST_FU},
    {ST, LDST_FU},
    {JMP, BRCH_FU},
    {BEQZ, BRCH_FU},
    {HLT, UTIL_FU},
    {LAX, ALU_FU},
    {STX, ALU_FU}};

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
