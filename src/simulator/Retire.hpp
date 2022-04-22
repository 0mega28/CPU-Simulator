#pragma once

#include <iostream>

#include "DataMemory.hpp"
#include "Register.hpp"
#include "fu_status.hpp"
#include "../utils.hpp"

class Retire
{
	DataMemory *dataMemory;
	void write_back(fu_enum fu);

	/*
	 * wait until no other instruction is going to read register destination
	 * returns true if war dependency exist else false
	 *
	 * Suppose two instr in order
	 * MUL R1, R2, R3
	 * ADD R2, R3, R4
	 * ADD instr should not write back before MUL instr has fetched the src reg
	 */
	bool check_war(fu_enum fu);

	/*
	 * free all other_fu which are waiting for the current_fu to finish
	 * set all r* of other_fu to true if q* of other_fu == current_fu
	 */
	void free_fu(fu_enum fu);

public:
	Retire(DataMemory *dataMemory);

	void cycle();
};

Retire::Retire(DataMemory *dataMemory)
{
	this->dataMemory = dataMemory;
}

bool Retire::check_war(fu_enum fu)
{
	/* Get the current fu status entry */
	auto &fue = fu_status[fu];
	int fi = fue.fi;

	/* loop over all fu */
	for (int f = 0; f < fu_enum::NUM_FU; f++)
	{
		if (f == fu) /* Obviously, we don't need to check for curr fu */
			continue;

		auto &fue_f = fu_status[f];

		int fj_f = fue_f.fj;
		int fk_f = fue_f.fk;
		int fl_f = fue_f.fl;
		bool fetched_f = fue_f.fetched;
		bool rj_f = fue_f.rj;
		bool rk_f = fue_f.rk;
		bool rl_f = fue_f.rl;

		/* current fu has to write to some reg */
		bool is_write = fi != -1;
		/* dest reg of current fu matches to src reg of some fu */
		bool war = ((fj_f != fi) || (!rj_f)) && ((fk_f != fi) || (!rk_f)) && ((fl_f != fi) || (!rl_f));

		if (is_write && !war && !fetched_f)
			return true;
	}
	return false;
}

void Retire::free_fu(fu_enum fu)
{
	for (int f = 0; f < fu_enum::NUM_FU; f++)
	{
		if (f == fu) /* Obviously, we don't need to anything for curr fu */
			continue;

		auto &fue_f = fu_status[f];
		if (fue_f.qj == fu)
			fue_f.rj = true;
		if (fue_f.qk == fu)
			fue_f.rk = true;
		if (fue_f.ql == fu)
			fue_f.rl = true;
	}
}

void Retire::write_back(fu_enum fu)
{
	auto &fue = fu_status[fu];

	/*
	 * Don't retire the instruction which is fetched after the branch instruction
	 * and if there is unresolved branch
	 */
	if (fu_status[fu_enum::BRCH_FU].busy && fu_status[fu].time > fu_status[fu_enum::BRCH_FU].time)
		return;

	/* check if fu has been assigned (check busy) and has executed */
	if (!fue.busy || !fue.executed)
		return;

	/* if there is a war dependency no need to do anything */
	if (this->check_war(fu))
		return;

	switch (fu)
	{
	case fu_enum::ALU_FU:
	case fu_enum::MUL_FU:
		this->free_fu(fu);
		reg_status[fue.fi] = fu_enum::DMY_FU;
		RegSet::gpr[fue.fi] = fue.aluout;
		break;

	case fu_enum::LDST_FU:
		switch (fue.op)
		{
		case op_enum::LD:
			this->free_fu(fu);
			reg_status[fue.fi] = fu_enum::DMY_FU;
			RegSet::gpr[fue.fi] = this->dataMemory->getData(fue.aluout);
			break;
		case op_enum::ST:
			/* ST doesn't stop any fu */
			this->dataMemory->setData(fue.aluout, fue.fl);
			break;

		default:
			std::cerr << "Invalid op_enum in LDST: " << fue.op << std::endl;
			exit(EXIT_FAILURE);
			break;
		}
		break;

	case fu_enum::BRCH_FU:

		/* if branch prediction was incorrect */
		if (RegSet::branch_taken != branch_predictor[fue.idx].get_last_branch_prediction())
		{
			RegSet::pc = fue.aluout;
			RegSet::flush_pipeline();
			flush_fu_after_branch_taken(fue.time);
			std::cout << "Index= " << fue.idx << std::endl;
		}
		else
		{
			std::cout << "Branch prediction correct, moving on. Index= " << fue.idx << "Branch taken= " << RegSet::branch_taken << std::endl;
		}
#ifdef RETIRE_LOG
		std::cout << "Retire: " << std::endl;
		std::cout << "Branch taken: " << RegSet::branch_taken << " alu output= " << fue.aluout << std::endl;
#endif
		RegSet::decode_stall = false;
		branch_predictor[fue.idx].update_state(RegSet::branch_taken);

		break;

	case fu_enum::UTIL_FU:
		/* Check if all other fu have finished their execution */
		for (int f = 0; f < fu_enum::NUM_FU; f++)
			if (f != fu && fu_status[f].busy)
				return;

		longjmp(halt_cpu, 1); /* HLT instr */

		break;

	default:
		std::cerr << "Error: unknown fu: " << fu << std::endl;
		exit(EXIT_FAILURE);
	}

	/* fu has finished */
	fue.busy = false;
#ifdef RETIRE_LOG
	std::cout << "Retire instruction idx: " << fue.idx << std::endl;
#endif
}

void Retire::cycle()
{

	/*
	 * this will simulate parallel write back of all fu
	 * as it is done in a single CPU cycle
	 */
	for (int f = 0; f < fu_enum::NUM_FU; f++)
		this->write_back((fu_enum)f);
}
