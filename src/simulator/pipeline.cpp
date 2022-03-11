#include "Core.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " < binary file>" << std::endl;
		exit(1);
	}

	Core core(argv[1]);
	core.run();

	return 0;
}
