#pragma once

#include <iostream>

#include "Instruction.hpp"
#include "Register.hpp"
#include "../utils.hpp"

class Decode
{
public:
	void cycle();
};

void Decode::cycle()
{
	/* Read instruction from instruction pointer */
	Instruction *i = RegSet::ip.i;
	bool valid = RegSet::ip.valid;

	if (!valid)
		return;

	RegSet::cr.value = i->getOpcode();
	RegSet::cr.valid = true;
	
	/* Decode instruction and store in intermediate registers */
	switch (RegSet::cr.value)
	{
	case op_enum::ADD:
	case op_enum::SUB:
	case op_enum::MUL:
	case op_enum::LD:
		RegSet::ir1 = i->getop1();
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = RegSet::gpr[i->getop3()];
		break;

	case op_enum::ADDI:
	case op_enum::SUBI:
	case op_enum::MULI:
		RegSet::ir1 = i->getop1();
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = i->getop3();
		break;

	case op_enum::ST:
		RegSet::ir1 = RegSet::gpr[i->getop1()];
		RegSet::ir2 = RegSet::gpr[i->getop2()];
		RegSet::ir3 = RegSet::gpr[i->getop3()];
		break;

	case op_enum::LAX:
	case op_enum::STX:
	case op_enum::JMP:
		RegSet::ir1 = i->getop1();
		break;

	case op_enum::BEQZ:
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
}
