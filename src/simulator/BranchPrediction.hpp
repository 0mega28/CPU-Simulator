#pragma once

#include <iostream>
#include <unordered_map>

class BranchPrediction
{
private:
	enum b_states
	{
		SNT,
		NT,
		T,
		ST
	};

	b_states current_state = b_states::SNT;
	/* target index if branch taken */
	int target_index;
	int current_instruction_index;

	enum b_actions
	{
		BNT, /* Branch Not Taken */
		BT
	};

	struct transaction
	{
		b_states current_state;
		b_actions action;
		b_states next_state;
	};

	inline static transaction transactions[] = {
	    {SNT, BNT, SNT},
	    {SNT, BT, NT},
	    {NT, BNT, SNT},
	    {NT, BT, T},
	    {T, BT, ST},
	    {T, BNT, NT},
	    {ST, BT, ST},
	    {ST, BNT, T}};

	b_states get_next_state(b_actions action)
	{
		int size = sizeof(transactions) / sizeof(transactions[0]);
		for (int i = 0; i < size; i++)
		{
			if (transactions[i].current_state == this->current_state && transactions[i].action == action)
				return transactions[i].next_state;
		}

		std::cerr << "Incorrect input state or action" << std::endl;
		exit(EXIT_FAILURE);
	}

public:
	BranchPrediction() {}

	BranchPrediction(int current_instruction_index, int target_index)
	{
		this->current_instruction_index = current_instruction_index;
		this->target_index = target_index;
	}

	void update_state(bool branch_taken)
	{
		this->current_state = get_next_state((b_actions)branch_taken);
	}

	int get_target_address_index()
	{
		if (current_state <= NT)
			return current_instruction_index + 1;
		else
			return target_index;
	}

	bool get_last_branch_prediction()
	{
		return current_state >= 2;
	}
};

inline std::unordered_map<int, BranchPrediction> branch_predictor;
