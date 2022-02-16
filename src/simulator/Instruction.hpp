#pragma once

#include <string>
#include <iostream>
#include "../utils.hpp"

class Instruction
{
private:
	int opcode;
	int op1;
	int op2;
	int op3;

public:
	Instruction(std::string binaryInstruction);

	int getOpcode() { return opcode; }

	int getop1() { return op1; }

	int getop2() { return op2; }

	int getop3() { return op3; }

	void dumpInstruction();
};

Instruction::Instruction(std::string binaryInstruction)
{
	std::string opcode = binaryInstruction.substr(0, 4);
	int operationCode = bin_to_int(opcode, false);
	this->opcode = operationCode;

	/* Creates instruction object for ADD, SUB, MUL, LD, ST operations */
	if (operationCode == op_enum::ADD || operationCode == op_enum::SUB ||
	    operationCode == op_enum::MUL || operationCode == op_enum::LD || operationCode == op_enum::ST)
	{
		op1 = bin_to_int(binaryInstruction.substr(4, 4), false);
		this->op1 = op1;

		op2 = bin_to_int(binaryInstruction.substr(8, 4), false);
		this->op2 = op2;

		op3 = bin_to_int(binaryInstruction.substr(12, 4), false);
		this->op3 = op3;
	}

	/* Instruction object for ADDI, SUBI, MULI */
	else if (operationCode == op_enum::ADDI || operationCode == op_enum::SUBI || operationCode == op_enum::MULI)
	{
		op1 = bin_to_int(binaryInstruction.substr(4, 4), false);
		this->op1 = op1;

		op2 = bin_to_int(binaryInstruction.substr(8, 4), false);
		this->op2 = op2;

		op3 = bin_to_int(binaryInstruction.substr(12, 4), true);
		this->op3 = op3;
	}

	else if (operationCode == op_enum::JMP)
	{
		op1 = bin_to_int(binaryInstruction.substr(4, 12), true);
		this->op1 = op1;
	}

	else if (operationCode == op_enum::STX)
	{
		op1 = bin_to_int(binaryInstruction.substr(12, 4), false);
		this->op1 = op1;
	}

	else if (operationCode == op_enum::LAX)
	{
		op1 = bin_to_int(binaryInstruction.substr(4, 12), true);
		this->op1 = op1;
	}

	else if (operationCode == op_enum::BEQZ)
	{
		op1 = bin_to_int(binaryInstruction.substr(4, 4), false);
		this->op1 = op1;

		op2 = bin_to_int(binaryInstruction.substr(8, 8), true);
		this->op2 = op2;
	}

	else if (operationCode == op_enum::HLT)
	{
		/* Do nothing */
	}

	else
	{
		std::cout << "Invalid operation code: " << operationCode << std::endl;
		exit(1);
	}
}

void Instruction::dumpInstruction()
{
	std::cout << "OPCODE: " << op_string[opcode] << "\tOP1: " << op1 << "\tOP2: " << op2 << "\tOP3: " << op3 << std::endl;
}
