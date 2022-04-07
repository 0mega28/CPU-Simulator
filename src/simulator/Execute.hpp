#pragma once

#include <iostream>

#include "Register.hpp"
#include "utils.hpp"

class Execute
{
private:
	/* ADD ADDI SUB SUBI LAX STX */
	void alu_fu();
	/* MUL MULI */
	void mul_fu();
	/* LD ST */
	void ldst_fu();
	/* JMP BEQZ */
	void brch_fu();
	/* HLT */
	void util_fu();

public:
	void cycle();
};

void Execute::alu_fu()
{
	auto &fue = fu_status[fu_enum::ALU_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		switch (fue.op)
		{
		/* Store result of ALU OP in aluout */
		case op_enum::ADD:
			fue.aluout = fue.fj + fue.fk;
			break;
		case op_enum::ADDI:
			fue.aluout = fue.fj + fue.imm;
			break;
		case op_enum::SUB:
			fue.aluout = fue.fj - fue.fk;
			break;
		case op_enum::SUBI:
			fue.aluout = fue.fj - fue.imm;
			break;
		case op_enum::LAX:
			fue.aluout = fue.fj;
			break;
		case op_enum::STX:
			fue.aluout = fue.fj;
			break;
		default:
			std::cerr << "Error: unknown ALU operation: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
		}

		fue.executed = true;
	}
}

void Execute::mul_fu()
{
	auto &fue = fu_status[fu_enum::MUL_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		switch (fue.op)
		{
		/* Store result of ALU OP in aluout */
		case op_enum::MUL:
			fue.aluout = fue.fj * fue.fk;
			break;
		case op_enum::MULI:
			fue.aluout = fue.fj * fue.imm;
			break;
		default:
			std::cerr << "Error: unknown MUL operation: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
		}
		fue.executed = true;
	}
}

void Execute::ldst_fu()
{
	auto &fue = fu_status[fu_enum::LDST_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		switch (fue.op)
		{
		case op_enum::LD: /* LD R1 R2[R3] */
			/* Store R2[R3] in aluout, retire unit has to put [aluout] to fi */
			fue.aluout = fue.fj + fue.fk;
			break;

		case op_enum::ST: /* ST R1[R2] R3 */
				  /* Store R1[R2] in aluout, retire unit has to put fl to [aluout] */
			fue.aluout = fue.fj + fue.fk;
			break;

		default:
			std::cerr << "Error: unknown LDST operation: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
			break;
		}
		fue.executed = true;
	}
}

void Execute::brch_fu()
{
	auto &fue = fu_status[fu_enum::BRCH_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		switch (fue.op)
		{
		/*
		 * why fue.imm / 2?
		 * For jmp or branch instruction we consider an
		 * address difference of 2 bytes (in our assembler implementation)
		 * between instruction
		 * but, we are storing instruction in an array so we don't need the 2 byte offset
		 */
		case op_enum::JMP:
			fue.aluout = fue.idx + fue.imm / 2;
			break;
		/*
		 * Why RegSet::pc + 1?
		 * aluout value will be used in next cycle in retire stage, in that cycle pc will
		 * be incremented by 1 so, wrong instruction will be fetched hence save incremented
		 * pc value in alu output
		 */
		case op_enum::BEQZ:
			fue.aluout = (fue.fj == 0) ? (fue.idx + fue.imm / 2) : (RegSet::pc + 1);
			break;

		default:
			std::cerr << "Error: unknown BRCH operation: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
			break;
		}
		fue.executed = true;
	}
}

void Execute::util_fu()
{
	auto &fue = fu_status[fu_enum::UTIL_FU];
	if (fue.busy && fue.fetched && !fue.executed)
	{
		/* No need to do anything */
		if (fue.op != op_enum::HLT)
		{
			std::cerr << "Error: HLT operation expected: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
		}
		fue.executed = true;
	}
}

void Execute::cycle()
{
	/* Simulating all fu as they are running in parallel */
	this->alu_fu();
	this->mul_fu();
	this->ldst_fu();
	this->brch_fu();
	this->util_fu();

#ifdef EXECUTE_LOG
	// TODO: Execute log
#endif
}
