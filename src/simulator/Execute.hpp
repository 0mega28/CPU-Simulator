#pragma once

#include <iostream>

#include "Register.hpp"

class Execute
{
private:
	void stall_execute_unit_action();

public:
	void cycle();
};

void Execute::stall_execute_unit_action()
{
#ifdef EXECUTE_LOG
	std::cout << "Stall execute unit" << std::endl;
#endif
	RegSet::fu_ready[fu::EXECUTE] = false;
}

void Execute::cycle()
{
	if (!RegSet::fu_ready[fu::RETIRE])
	{
		// Retire unit is not ready stall the pipline
		this->stall_execute_unit_action();
		return;
	}

	/* Check if control register is valid (value isn't garbage) */
	if (!RegSet::cr.valid)
		return;

	RegSet::bt = false;
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
		RegSet::dr.is_memory = true;
		RegSet::dr.is_store = false;
		RegSet::aluout.value = RegSet::ir2 + RegSet::ir3;
		break;

	case op_enum::ST:
		RegSet::dr.value = RegSet::ir1 + RegSet::ir2;
		RegSet::dr.is_memory = true;
		RegSet::dr.is_store = true;
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

	/*
	 * Why (RegSet::pc - 2)?
	 * Because the instruction was fetched two cycle ago hence the pc value
	 * must have been increased by 2 when the instruction was fetched and now
	 * it is in execute stage hence we must have to decrease it by 2 to get
	 * the correct pc value
	 *
	 * For jmp or branch instruction
	 * we consider an address difference of 2 bytes (in our assembler implementation)
	 * between instruction
	 * but, we are storing instruction in an array so we don't need the 2 byte offset
	 */
	case op_enum::JMP:
		/* branch instr */
		RegSet::bt = true;
		RegSet::aluout.value = RegSet::pc - 2 + RegSet::ir1 / 2;
		break;

	case op_enum::BEQZ:
		/* branch instr */
		RegSet::bt = true;
		/*
		 * Why RegSet::pc + 1?
		 * aluout value will be used in next cycle in retire stage, in that cycle pc will
		 * be incremented by 1 so, wrong instruction will be fetched hence save incremented
		 * pc value in alu output
		 */
		RegSet::aluout.value = (RegSet::ir1 == 0) ? (RegSet::pc - 2 + RegSet::ir2 / 2) : (RegSet::pc + 1);
		break;

	case op_enum::HLT:
		RegSet::is_halt_instr = true;
		break;

	default:
		std::cerr << "Invalid opcode: " << RegSet::cr.value << std::endl;
		exit(EXIT_FAILURE);
		break;
	}
	RegSet::aluout.valid = true;

#ifdef EXECUTE_LOG
	std::cout << "Execute: " << std::endl;
	std::cout << "aluout: " << RegSet::aluout.value << "\tbt: " << RegSet::bt << "\tdr: " << RegSet::dr.value << (RegSet::dr.is_memory ? "M" : "R") << std::endl;
#endif

	RegSet::fu_ready[fu::EXECUTE] = true;

	/*
	 * Execute unit consumed current instruction so set cr validity to false
	 * in the next decode cycle it will be set to true by decode unit
	 */
	RegSet::reset_decode_execute_im();
}
