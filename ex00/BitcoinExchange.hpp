#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>

class BitcoinExchange
{
	private:
		const std::string	_inputFile;
		std::vector<std::pair<std::string, float>>	_PriceRates;
	public:
		BitcoinExchange();
		BitcoinExchange(std::string inputFile);
		BitcoinExchange(BitcoinExchange const &src);
		~BitcoinExchange();
		void readDatabase();
		void parseInput();
		int binarySearch(const std::string &date);
		bool checkDate(const std::string &date);
		bool checkYear(const std::string &year);
		bool checkMonth(const std::string &month);
		bool checkDay(const std::string &day, const std::string &month, const std::string &year);
		bool checkValue(const std::string &value);
		bool checkPositive(const std::string & value);
		bool checkLargeNumber(const std::string & value);

		class ErrorOpeningFileException : public std::exception {
			public:
				virtual const char *what() const throw() override ;
		};

		BitcoinExchange &operator=(BitcoinExchange const &src);
};

#endif