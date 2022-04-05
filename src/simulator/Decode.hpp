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

	/* check if instruction pointer valid is valid (value isn't garbage) */
	if (!RegSet::ip.valid)
		return;

	/* Issue queue entry */
	iq_entry iqe;
	/* Add the opcode entry to iq entry */
	iqe.op = (op_enum)i->getOpcode();

	/* Decode instruction and store in intermediate registers */
	switch (i->getOpcode())
	{
	case op_enum::ADD:		    /* ADD R1 R2 R3 */
	case op_enum::SUB:		    /* SUB R1 R2 R3 */
	case op_enum::MUL:		    /* MUL R1 R2 R3 */
	case op_enum::LD:		    /* LD R1 R2[R3] */
		iqe.dest_reg = i->getop1(); /* R1 */
		iqe.src_reg1 = i->getop2(); /* R2 */
		iqe.src_reg2 = i->getop3(); /* R3 */
		// TODO: handle memory validity for load instruction
		// TODO: move the above todo to appropriate location
		break;

	case op_enum::ADDI:		    /* ADDI R1 R2 #5 */
	case op_enum::SUBI:		    /* SUBI R1 R2 #5 */
	case op_enum::MULI:		    /* MULI R1 R2 #5 */
		iqe.dest_reg = i->getop1(); /* R1 */
		iqe.src_reg1 = i->getop2(); /* R2 */
		iqe.imm_val = i->getop3();  /* #5 */
		break;

	case op_enum::ST:		    /* ST R1[R2] R3 */
		iqe.src_reg1 = i->getop1(); /* R1 */
		iqe.src_reg2 = i->getop2(); /* R2 */
		iqe.src_reg3 = i->getop3(); /* R3 */
		// TODO: handle memory validity
		// TODO: move the above todo to appropriate location
		break;

	case op_enum::LAX:		   /* LAX #23 */
		iqe.dest_reg = 16;	   /* ax register */
		iqe.imm_val = i->getop1(); /* #23 */
		break;

	case op_enum::JMP:		   /* JMP L1 */
		iqe.imm_val = i->getop1(); /* L1 */
		break;

	case op_enum::STX:		    /* STX R1 */
		iqe.dest_reg = i->getop1(); /* R1 */
		iqe.src_reg1 = 16;	    /* ax register */
		break;

	case op_enum::BEQZ:		    /* BEQZ R1 L1 */
		iqe.src_reg1 = i->getop1(); /* R1 */
		iqe.imm_val = i->getop2();  /* L1 */
		break;

	case op_enum::HLT: /* HLT */
		break;

	default:
		std::cerr << "Invalid operation code: " << i->getOpcode() << std::endl;
		exit(EXIT_FAILURE);
		break;
	}

#ifdef DECODE_LOG
	std::cout << "Decode: " << std::endl;
	// TODO: handle decode log
#endif

	RegSet::iq.push(iqe);

	/*
	 * Decode unit consumed current instruction so set ip validity to false
	 * in the next fetch cycle it will be set to true by fetch unit
	 */
	RegSet::reset_fetch_decode_im();
}
