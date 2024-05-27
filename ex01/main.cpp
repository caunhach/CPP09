#include "RPN.hpp"

int main(int argc, char **argv)
{
	RPN	rpn;

	if (argc != 2)
	{
		std::cout << "Error: Invalid number of arguments" << std::endl;
		exit(1);
	}
	try {
		rpn.fillStack(argv[1]);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}