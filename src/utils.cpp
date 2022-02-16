#include "utils.hpp"

// Returns the 2's complement of a string
std::string findTwoscomplement(std::string str)
{
	int n = str.length();
	int i;
	for (i = n - 1; i >= 0; i--)
		if (str[i] == '1')
			break;
	if (i == -1)
		return '1' + str;

	for (int k = i - 1; k >= 0; k--)
	{
		if (str[k] == '1')
			str[k] = '0';
		else
			str[k] = '1';
	}
	return str;
	;
}

/* Returns binary string of an interger value */
std::string int_to_bin(int bits, int value)
{
	std::bitset<CPU_ARCH> bin(value);
	return bin.to_string().substr(CPU_ARCH - bits);
}

/* Returns integer from a binary string */
int bin_to_int(std::string binary, bool isImmediate)
{
	if (!isImmediate || binary[0] == '0')
		return stoi(binary, 0, 2);
	return -1 * stoi(findTwoscomplement(binary), 0, 2);
}
