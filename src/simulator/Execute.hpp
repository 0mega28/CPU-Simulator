#pragma once

#include <iostream>

#include "Register.hpp"
#include "../utils.hpp"

class Execute
{
private:
	/* Functional units that execute in this cycle */
	std::array<bool, fu_enum::NUM_FU> fu_exec;

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

	void reset_exec_status();
	void dump_exec();

public:
	void cycle();
};

void Execute::alu_fu()
{
	static int alu_delay = ALU_DELAY;
	auto &fue = fu_status[fu_enum::ALU_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		if (alu_delay != 0)
		{
			alu_delay--;
			return;
		}
		else
			alu_delay = ALU_DELAY;

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
			fue.aluout = fue.imm;
			break;
		case op_enum::STX:
			fue.aluout = fue.fj;
			break;
		default:
			std::cerr << "Error: unknown ALU operation: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
		}

		fue.executed = true;
		fu_exec[fu_enum::ALU_FU] = true;
	}
}

void Execute::mul_fu()
{
	static int mul_delay = MUL_DELAY;
	auto &fue = fu_status[fu_enum::MUL_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		if (mul_delay != 0)
		{
			mul_delay--;
			return;
		}
		else
			mul_delay = MUL_DELAY;

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
		fu_exec[fu_enum::MUL_FU] = true;
	}
}

void Execute::ldst_fu()
{
	static int ldst_delay = LDST_DELAY;
	auto &fue = fu_status[fu_enum::LDST_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		if (ldst_delay != 0)
		{
			ldst_delay--;
			return;
		}
		else
			ldst_delay = LDST_DELAY;

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
		fu_exec[fu_enum::LDST_FU] = true;
	}
}

void Execute::brch_fu()
{
	static int brch_delay = BRCH_DELAY;
	auto &fue = fu_status[fu_enum::BRCH_FU];

	if (fue.busy && fue.fetched && !fue.executed)
	{
		if (brch_delay != 0)
		{
			brch_delay--;
			return;
		}
		else
			brch_delay = BRCH_DELAY;

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
		fu_exec[fu_enum::BRCH_FU] = true;
	}
}

void Execute::util_fu()
{
	static int util_delay = UTIL_DELAY;
	auto &fue = fu_status[fu_enum::UTIL_FU];
	if (fue.busy && fue.fetched && !fue.executed)
	{
		if (util_delay != 0)
		{
			util_delay--;
			return;
		}
		else
			util_delay = UTIL_DELAY;
		/* No need to do anything */
		if (fue.op != op_enum::HLT)
		{
			std::cerr << "Error: HLT operation expected: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
		}
		fue.executed = true;
		fu_exec[fu_enum::UTIL_FU] = true;
	}
}

void Execute::reset_exec_status()
{
	for (int i = 0; i < fu_enum::NUM_FU; i++)
		fu_exec[i] = false;
}

void Execute::dump_exec()
{
	using namespace std;
	for (int i = 0; i < fu_enum::NUM_FU; i++)
	{
		if (fu_exec[i])
		{
			auto &fue = fu_status[i];
			cout << "Functional unit: " << i << endl;
			cout << "Alu output = " << fue.aluout << endl;
		}
	}
	cout << endl;
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
	std::cout << "Execute:" << std::endl;
	dump_exec();
#endif
	reset_exec_status();
}
