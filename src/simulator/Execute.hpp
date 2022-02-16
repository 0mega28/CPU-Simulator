#pragma once

#include <iostream>

#include "Register.hpp"

class Execute
{
public:
	void cycle();
};

void Execute::cycle()
{
	if (!RegSet::cr.valid)
		return;

	switch (RegSet::cr.value)
	{
	case op_enum::ADD:
	case op_enum::ADDI:
		RegSet::dr.value = RegSet::ir1;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir2 + RegSet::ir3;
		break;

	case op_enum::SUB:
	case op_enum::SUBI:
		RegSet::dr.value = RegSet::ir1;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir2 - RegSet::ir3;
		break;

	case op_enum::MUL:
	case op_enum::MULI:
		RegSet::dr.value = RegSet::ir1;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir2 * RegSet::ir3;
		break;

	case op_enum::LD:
		RegSet::dr.value = RegSet::ir1;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir2 + RegSet::ir3;
		break;

	case op_enum::ST:
		RegSet::dr.value = RegSet::ir1 + RegSet::ir2;
		RegSet::dr.is_memory = true;
		RegSet::aluout.value = RegSet::ir3;
		break;

	case op_enum::LAX:
		/* 17th register is ax register */
		RegSet::dr.value = 16;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir1;
		break;

	case op_enum::STX:
		RegSet::dr.value = RegSet::ir1;
		RegSet::dr.is_memory = false;
		RegSet::aluout.value = RegSet::ir2;
		break;

	case op_enum::JMP:
		RegSet::bt = true;
		RegSet::aluout.value = RegSet::ir1;
		break;

	case op_enum::BEQZ:
		RegSet::bt = RegSet::ir1 == 0;
		RegSet::aluout.value = RegSet::ir2;
		break;

	case op_enum::HLT:
	 	longjmp(halt_cpu, 1);
		break;

	default:
		std::cout << "Invalid opcode: " << RegSet::cr.value << std::endl;
		exit(EXIT_FAILURE);
		break;
	}
	RegSet::aluout.valid = true;

#ifdef EXECUTE_LOG
	std::cout << "Execute: " << std::endl;
	std::cout << "aluout: " << RegSet::aluout.value << "\tbt: " << RegSet::bt << "\tdr: " << RegSet::dr.value << (RegSet::dr.is_memory ? "M" : "R") << std::endl;
#endif
}
