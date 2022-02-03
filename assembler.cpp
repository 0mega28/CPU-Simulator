/*
 _____                 ____
|_   _|_      _____   |  _ \ __ _ ___ ___ 
  | | \ \ /\ / / _ \  | |_) / _` / __/ __|
  | |  \ V  V / (_) | |  __/ (_| \__ \__ \
  |_|   \_/\_/ \___/  |_|   \__,_|___/___/

    _                           _     _
   / \   ___ ___  ___ _ __ ___ | |__ | | ___ _ __
  / _ \ / __/ __|/ _ \ '_ ` _ \| '_ \| |/ _ \ '__|
 / ___ \\__ \__ \  __/ | | | | | |_) | |  __/ |
/_/   \_\___/___/\___|_| |_| |_|_.__/|_|\___|_|

* Copyright (C) 2022
* Abhay Kumar, Deepanshu Matia,
* Saksham Gupta, Shourya Shukla
*
* All rights reserved
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#define NUM_REGS 16
#define CPU_ARCH 16

/* mapping of label and address */
static std::unordered_map<std::string, int> label_map;

/* mapping of operator and opcode */
static std::unordered_map<std::string, std::string> opcode_map;

/* mapping of register and index */
static std::unordered_map<std::string, std::string> register_map;

/* list of assembly program's line-by-line instructions */
static std::vector<std::string> instr_list;

/* Returns binary string of an interger value */
std::string int_to_bin(int bits, int value)
{
	std::bitset<CPU_ARCH> bin(value);
	return bin.to_string().substr(CPU_ARCH - bits);
}

void parse_asm_file(std::string asm_f) {
	std::fstream asm_str;
	asm_str.open(asm_f, std::fstream::in);

	std::string line;
	while (std::getline(asm_str, line)) {
		/* trim leading and trailing whitespaces */
		boost::algorithm::trim(line);

		/* skip empty lines and comments */
		if (line.length() == 0 || line[0] == ';')
			continue;

		instr_list.push_back(line);
	}
}

void initialise()
{
	/* map register to index */
	for (int i = 0; i < NUM_REGS; i++) {
		std::string reg_name = "R" + std::to_string(i);
		register_map[reg_name] = int_to_bin(4, i);
	}

	/* map operator to opcode */
	opcode_map["ADD"] = "0000";
	opcode_map["ADDI"] = "0001";
	opcode_map["SUB"] = "0010";
	opcode_map["SUBI"] = "0011";
	opcode_map["MUL"] = "0100";
	opcode_map["MULI"] = "0101";
	opcode_map["LD"] = "0110";
	opcode_map["ST"] = "0111";
	opcode_map["JMP"] = "1000";
	opcode_map["BEQZ"] = "1001";
	opcode_map["HLT"] = "1010";
	opcode_map["LAX"] = "1011";
}

/*
 * Find the byte of the label and purge the label from the
 * instruction list.
 */
void label_parser() {
	for (int ip = 0; ip < instr_list.size(); ip++) {
		std::string instr = instr_list[ip];
		std::string label;

		/* parse label */
		int label_index = instr.find(":");
		if (label_index != std::string::npos) {
			label = instr.substr(0, label_index);

			/*
			 * Since the system is 16-bit, an instruction takes
			 * two bytes of space. We divide by the size of one
			 * byte.
			 */
			label_map[label] = ip * (CPU_ARCH / 8);

			/* purge label name(s) from the instruction list */
			instr_list.erase(instr_list.begin() + ip);
			ip--;
		}
	}
}

/* Parses string of type R0[R1] and assigns value rs = R0 & rt = R1 */
void ld_st_operand_parser(std::string operand, std::string &rs, std::string &rt) {
	std::vector<std::string> operand_list;
	boost::split(operand_list, operand, boost::is_any_of("["));

	rs = operand_list[0];
	rt = operand_list[1].substr(0, operand_list[1].length() - 1);
}

std::string gen_bin_from_instr(std::string operand, std::string rd,
			       std::string rs, std::string rt)
{
	std::string opcode = opcode_map[operand];
	std::string rd_bin = register_map[rd];
	std::string rs_bin = register_map[rs];
	std::string rt_bin = register_map[rt];

	return opcode + rd_bin + rs_bin + rt_bin;
}

std::string gen_bin_from_instr(std::string operand, std::string rd,
			       std::string rs, int imme)
{
	std::string opcode = opcode_map[operand];
	std::string rd_bin = register_map[rd];
	std::string rs_bin = register_map[rs];
	std::string imme_bin = int_to_bin(4, imme);

	return opcode + rd_bin + rs_bin + imme_bin;
}

void translate(std::string bin_file) {
	std::ofstream bin_strm;
	bin_strm.open(bin_file);

	for (int ip = 0; ip < instr_list.size(); ip++) {
		std::string instr = instr_list[ip];
		std::string opcode;
		std::string bin_out = "";

		std::stringstream ss(instr);

		ss >> opcode;

		if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL") {
			/* This if case handles operant: 
			 * ADD, ADDI
			 * SUB, SUBI
			 * MUL, MULI
			 */
			if (instr.find("#") != std::string::npos) {
				/* Instruction contains immediate value */
				/* ADDI R1 R2 #4 */
				std::string rd, rs, imm;
				ss >> rd;
				ss >> rs;
				ss >> imm;
				/* Since it's an immediate value instruction */
				opcode += "I";

				int imm_val = std::stoi(imm.substr(1));

				bin_out = gen_bin_from_instr(opcode, rd,
							     rs, imm_val);
			} else {
				/* Instruction doesn't contain immediate value */
				/* ADD R1 R2 R3 */
				std::string rd, rs, rt;
				ss >> rd;
				ss >> rs;
				ss >> rt;

				bin_out = gen_bin_from_instr(opcode, rd, rs, rt);
			}
		} else if (opcode == "LD") {
			/* LD R1 R0[R2] */
			std::string rd, rs, rt;
			ss >> rd;

			std::string ld_operand;
			ss >> ld_operand;

			ld_st_operand_parser(ld_operand, rs, rt);

			bin_out = gen_bin_from_instr(opcode, rd, rs, rt);
		} else if (opcode == "ST") {
			/* ST R0[R1] R2 */
			std::string rd, rs, rt;

			std::string st_operand;
			ss >> st_operand;

			ss >> rt;

			ld_st_operand_parser(st_operand, rd, rs);

			bin_out = gen_bin_from_instr(opcode, rd, rs, rt);
		}
		/*
		 * To load a location as an immediate value, utilise the
		 * instruction 'LAX' i.e., 'Load into Accumulator'. This
		 * is an implicit computation using a special register
		 * called the Accumulator (AX) otherwise inaccessible to
		 * the user. Now, locations as big as 12-bits in length
		 * can be loaded.
		 *
		 * For instance,
		 *      LAX #512
		 */
		else if (opcode == "LAX") {
			if (instr.find("#") != std::string::npos) {
				std::string imm;
				ss >> imm;

				int imm_val = std::stoi(imm.substr(1));
				bin_out = opcode_map[opcode] +
					  int_to_bin(12, imm_val);
			}
		} else if (opcode == "JMP") {
			/* JMP label */
			std::string label;
			ss >> label;

			int label_pos = label_map[label];
			int curr_pos = ip * (CPU_ARCH / 8);

			int offset = label_pos - curr_pos;

			bin_out = opcode_map[opcode] + int_to_bin(12, offset);
		} else if (opcode == "BEQZ") {
			/* BEQZ R1 label */
			std::string rd, label;
			ss >> rd;
			ss >> label;

			int label_pos = label_map[label];
			int curr_pos = ip * (CPU_ARCH / 8);

			int offset = label_pos - curr_pos;

			bin_out = opcode_map[opcode] + register_map[rd] +
				  int_to_bin(8, offset);
		} else if (opcode == "HLT") {
			/* HLT */
			bin_out = opcode_map[opcode];
			/* padding */
			bin_out += "000000000000";
		}

		bin_strm << bin_out << std::endl;
	}

	bin_strm.close();
}

void dump_instr_list() {
	for (int ip = 0; ip < instr_list.size(); ip++)
		std::cout << (ip * 2) << "\t" << instr_list[ip] << std::endl;
}

void dump_labels() {
	for (auto labels : label_map)
		std::cout << labels.first << "\t" << labels.second << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] <<
			     " <input file>" << std::endl;
		return 1;
	}

	std::string asm_file = argv[1];

	int ext_index = asm_file.find_last_of(".");
	std::string bin_file = asm_file.substr(0, ext_index) + ".bin";

	initialise();
	parse_asm_file(asm_file);
	label_parser();
	translate(bin_file);

	dump_instr_list();
	dump_labels();

	return 0;
}
