#pragma once

#include <string>
#include <iostream>
#include "../utils.hpp"

class Instruction
{
private:
	int opcode;
	int inputOperand1;
	int inputOperand2;
	int outputOperand;

public:
	Instruction(std::string binaryInstruction)
	{
		std::string opcode = binaryInstruction.substr(0, 4);
		int operationCode = bin_to_int(opcode, false);
		this->opcode = operationCode;

		/* Creates instruction object for ADD, SUB, MUL, LD, ST operations */
		if (operationCode == op_enum::ADD || operationCode == op_enum::SUB ||
		    operationCode == op_enum::MUL || operationCode == op_enum::LD || operationCode == op_enum::ST)
		{
			outputOperand = bin_to_int(binaryInstruction.substr(4, 4), false);
			this->outputOperand = outputOperand;

			inputOperand1 = bin_to_int(binaryInstruction.substr(8, 4), false);
			this->inputOperand1 = inputOperand1;

			inputOperand2 = bin_to_int(binaryInstruction.substr(12, 4), false);
			this->inputOperand2 = inputOperand2;
		}

		/* Instruction object for ADDI, SUBI, MULI */
		else if (operationCode == op_enum::ADDI || operationCode == op_enum::SUBI || operationCode == op_enum::MULI)
		{
			outputOperand = bin_to_int(binaryInstruction.substr(4, 4), false);
			this->outputOperand = outputOperand;

			inputOperand1 = bin_to_int(binaryInstruction.substr(8, 4), false);
			this->inputOperand1 = inputOperand1;

			inputOperand2 = bin_to_int(binaryInstruction.substr(12, 4), true);
			this->inputOperand2 = inputOperand2;
		}

		else if (operationCode == op_enum::JMP)
		{
			inputOperand1 = bin_to_int(binaryInstruction.substr(4, 12), true);
			this->inputOperand1 = inputOperand1;
		}

		else if (operationCode == op_enum::STX)
		{
			outputOperand = bin_to_int(binaryInstruction.substr(4, 4), false);
			this->outputOperand = outputOperand;
		}

		else if (operationCode == op_enum::LAX)
		{
			inputOperand1 = bin_to_int(binaryInstruction.substr(4, 12), true);
			this->inputOperand1 = inputOperand1;
		}

		else if (operationCode == op_enum::BEQZ)
		{
			inputOperand1 = bin_to_int(binaryInstruction.substr(4, 4), false);
			this->inputOperand1 = inputOperand1;

			inputOperand2 = bin_to_int(binaryInstruction.substr(8, 8), true);
			this->inputOperand2 = inputOperand2;
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

	// getters and setters
	int getOpcode() { return opcode; }

	int getInputOperand1() { return inputOperand1; }

	int getInputOperand2() { return inputOperand2; }

	int getOutputOperand() { return outputOperand; }
};