#pragma once

#include <iostream>

#include "Register.hpp"
#include "fu_status.hpp"
#include "../utils.hpp"

class Issue
{
public:
	void cycle();
};

void Issue::cycle()
{
	/* If instruction queue is empty do nothing */
	if (RegSet::iq.size() == 0)
		return;

	iq_entry instr = RegSet::iq.front();
	op_enum op = instr.op;

	/* Get the refrence of fu which will execute theh current op */
	fu_status_entry &fu = fu_status[op_to_fu[op]];

	/* if functional unit is busy do nothing */
	if (fu.busy)
		return;

	/*
	 * If the current destination register is produced by some other
	 * function unit do nothing (Removes WAW data hazards)
	 */
	if (reg_status[instr.dest_reg] != fu_enum::DMY_FU)
		return;

	/* Reset the current fu entry */
	fu.reset_entry();

	/* Set fu state to busy (Removes structural hazards) and op */
	fu.busy = true;
	fu.op = op;
	/* Set index entry in fue */
	fu.idx = instr.idx;

	/* Assign fu according to op */
	switch (op)
	{
	case op_enum::ADD:		/* ADD R1 R2 R3 */
	case op_enum::SUB:		/* SUB R1 R2 R3 */
	case op_enum::MUL:		/* MUL R1 R2 R3 */
	case op_enum::LD:		/* LD R1 R2[R3] */
		fu.fi = instr.dest_reg; /* R1 */
		fu.fj = instr.src_reg1; /* R2 */
		fu.fk = instr.src_reg2; /* R3 */

		fu.qj = reg_status[instr.src_reg1];
		fu.qk = reg_status[instr.src_reg2];

		fu.rj = fu.qj == fu_enum::DMY_FU;
		fu.rk = fu.qk == fu_enum::DMY_FU;

		reg_status[instr.dest_reg] = op_to_fu[op];
		// TODO: handle memory validity for load instruction
		// is it necessary?
		break;

	case op_enum::ADDI:		/* ADDI R1 R2 #5 */
	case op_enum::SUBI:		/* SUBI R1 R2 #5 */
	case op_enum::MULI:		/* MULI R1 R2 #5 */
		fu.fi = instr.dest_reg; /* R1 */
		fu.fj = instr.src_reg1; /* R2 */
		fu.imm = instr.imm_val; /* #5 */

		fu.qj = reg_status[instr.src_reg1];

		fu.rj = fu.qj == fu_enum::DMY_FU;

		reg_status[instr.dest_reg] = op_to_fu[op];
		break;

	case op_enum::ST:		/* ST R1[R2] R3 */
		fu.fj = instr.src_reg1; /* R1 */
		fu.fk = instr.src_reg2; /* R2 */
		fu.fl = instr.src_reg3; /* R3 */

		fu.qj = reg_status[instr.src_reg1];
		fu.qk = reg_status[instr.src_reg2];
		fu.ql = reg_status[instr.src_reg3];

		fu.rj = fu.qj == fu_enum::DMY_FU;
		fu.rk = fu.qk == fu_enum::DMY_FU;
		fu.rl = fu.ql == fu_enum::DMY_FU;
		// TODO: handle memory validity
		// is it necessary?
		break;

	case op_enum::LAX:		/* LAX #23 */
		fu.fi = instr.dest_reg; /* ax reg */
		fu.imm = instr.imm_val; /* #23 */

		reg_status[instr.dest_reg] = op_to_fu[op];
		break;

	case op_enum::JMP:		/* JMP L1 */
		fu.imm = instr.imm_val; /* L1 */
		break;

	case op_enum::STX:		/* STX R1 */
		fu.fi = instr.dest_reg; /* R1 */
		fu.fj = instr.src_reg1; /* ax reg */

		fu.qj = reg_status[instr.src_reg1];

		fu.rj = fu.qj == fu_enum::DMY_FU;

		reg_status[instr.dest_reg] = op_to_fu[op];
		break;

	case op_enum::BEQZ:		/* BEQZ R1 L1 */
		fu.fj = instr.src_reg1; /* R1 */
		fu.imm = instr.imm_val; /* L1 */

		fu.qj = reg_status[instr.src_reg1];

		fu.rj = fu.qj == fu_enum::DMY_FU;

		reg_status[instr.dest_reg] = op_to_fu[op];
		break;

	case op_enum::HLT: /* HLT */
		break;

	default:
		std::cerr << "Invalid operation code: " << op << std::endl;
		exit(EXIT_FAILURE);
		break;
	}

#ifdef ISSUE_LOG
	std::cout << "Issue: " << std::endl;
	dump_fu_entry(op_to_fu[op]);
#endif

	/* Instruction is assigned to fu so pop the instruction from queue */
	RegSet::iq.pop();
}
