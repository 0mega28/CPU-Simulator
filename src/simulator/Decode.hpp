#pragma once

#include <iostream>

#include "Instruction.hpp"
#include "Register.hpp"
#include "../utils.hpp"

class Decode
{
private:
	/* Actions to perform when decode unit is stalled */
	void stall_decode_unit_action();

public:
	void cycle();
};

void Decode::stall_decode_unit_action()
{
#ifdef DECODE_LOG
	std::cout << "Stall decode unit" << std::endl;
#endif
	RegSet::fu_ready[fu::DECODE] = false;
}

void Decode::cycle()
{
	if (!RegSet::fu_ready[fu::EXECUTE])
	{
		// Execute unit is not ready stall the pipline
		this->stall_decode_unit_action();
		return;
	}

	/* Read instruction from instruction pointer */
	Instruction *i = RegSet::ip.i;

	/* check if instruction pointer valid is valid (value isn't garbage) */
	if (!RegSet::ip.valid)
		return;

	/* Decode instruction and store in intermediate registers */
	switch (i->getOpcode())
	{
	case op_enum::ADD:
	case op_enum::SUB:
	case op_enum::MUL:
	case op_enum::LD:
		/* If input registers are not valid, return */
		if (!RegSet::reg_valid[i->getop2()] ||
		    !RegSet::reg_valid[i->getop3()])
		{
			this->stall_decode_unit_action();
			return;
		}

		RegSet::ir1 = i->getop1();
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = RegSet::gpr[i->getop3()];

		/* Set destination register invalid */
		RegSet::reg_valid[RegSet::ir1] = false;

		// TODO: handle memory validity for load instruction
		break;

	case op_enum::ADDI:
	case op_enum::SUBI:
	case op_enum::MULI:
		/* If input register is not valid, return */
		if (!RegSet::reg_valid[i->getop2()])
		{
			this->stall_decode_unit_action();
			return;
		}

		RegSet::ir1 = i->getop1();
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = i->getop3();

		/* Set destination register invalid */
		RegSet::reg_valid[RegSet::ir1] = false;
		break;

	case op_enum::ST:
		/* If input registers are not valid, return */
		if (!RegSet::reg_valid[i->getop1()] ||
		    !RegSet::reg_valid[i->getop2()] ||
		    !RegSet::reg_valid[i->getop3()])
		{
			this->stall_decode_unit_action();
			return;
		}

		RegSet::ir1 = RegSet::gpr[i->getop1()];
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = RegSet::gpr[i->getop3()];

		// TODO: handle memory validity
		break;

	case op_enum::LAX:
		RegSet::ir1 = i->getop1();

		/* Set destination register invalid */
		RegSet::reg_valid[16] = false;
		break;

	case op_enum::JMP:
		RegSet::ir1 = i->getop1();
		break;

	case op_enum::STX:
		/* If input register is not valid, return */
		if (!RegSet::reg_valid[16])
		{
			this->stall_decode_unit_action();
			return;
		}

		RegSet::ir1 = i->getop1();
		RegSet::ir2 = RegSet::gpr[16];

		/* Set destination register invalid */
		RegSet::reg_valid[RegSet::ir1] = false;
		break;

	case op_enum::BEQZ:
		/* If input register is not valid, return */
		if (!RegSet::reg_valid[i->getop1()])
		{
			this->stall_decode_unit_action();
			return;
		}

		RegSet::ir1 = RegSet::gpr[i->getop1()];
		RegSet::ir2 = i->getop2();
		break;

	case op_enum::HLT:
		break;

	default:
		std::cerr << "Invalid operation code: " << RegSet::cr.value << std::endl;
		exit(EXIT_FAILURE);
		break;
	}

	/* Set control register for execute unit */
	RegSet::cr.value = i->getOpcode();
	RegSet::cr.valid = true;

#ifdef DECODE_LOG
	std::cout << "Decode: " << std::endl;
	std::cout << "CR: " << op_string[RegSet::cr.value] << "\tIR1: " << RegSet::ir1 << "\tIR2: " << RegSet::ir2 << "\tIR3: " << RegSet::ir3 << std::endl;
#endif

	RegSet::fu_ready[fu::DECODE] = true;

	/*
	 * Decode unit consumed current instruction so set ip validity to false
	 * in the next fetch cycle it will be set to true by fetch unit
	 */
	RegSet::reset_fetch_decode_im();
}
