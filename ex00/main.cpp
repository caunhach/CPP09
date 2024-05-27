#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error: could not open file" << std::endl;
		exit(1);
	}
	BitcoinExchange btc(argv[1]);
	btc.readDatabase();
	btc.parseInput();
	return (0);
}