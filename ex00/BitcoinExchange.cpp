#include "BitcoinExchange.hpp"

const char* WHITESPACE = " \t\n\r";

static std::string& ltrim(std::string& s)
{
	s.erase(0, s.find_first_not_of(WHITESPACE));
	return s;
}

static std::string& rtrim(std::string& s)
{
	s.erase(s.find_last_not_of(WHITESPACE) + 1);
	return s;
}

static std::string& trim(std::string& s)
{
	return ltrim(rtrim(s));
}

BitcoinExchange::BitcoinExchange() { }

BitcoinExchange::BitcoinExchange(std::string inputFile) : _inputFile(inputFile)
{

}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &src)
{
	*this = src;
}

BitcoinExchange::~BitcoinExchange() { }

BitcoinExchange	&BitcoinExchange::operator=(BitcoinExchange const &src)
{
	(void) src;
	return *this;
}

void	BitcoinExchange::readDatabase()
{
	std::ifstream	db("./data.csv");
	std::string		line;
	std::string		date;
	std::string		value;
	size_t			pos;
	float			rate;

	if (!db.is_open())
	{
		std::cerr << "Error: Could not open Database file" << std::endl;
		return;
	}
	std::getline(db, line);
	date = line.substr(0, line.find_first_of(","));
	value = line.substr(line.find_first_of(",") + 1);
	if (trim(date) != "date")
	{
		std::cerr << "Error: Invalid Database header format" << line << std::endl;
		return;
	}
	if (trim(value) != "exchange_rate")
	{
		std::cerr << "Error: Invalid Database header format" << line << std::endl;
		return;
	}
	while (std::getline(db, line))
	{
		pos = line.find(",");
		date = line.substr(0, pos);
		if (!checkDate(trim(date)))
		{
			std::cerr << "Error : Invalid Database's Date: " << line << std::endl;
			continue;
		}
		value = line.substr(pos + 1);
		for (std::string::const_iterator it = trim(value).begin(); it != trim(value).end(); ++it) {
			if (!std::isdigit(*it) && *it != '.' && *it != '-') {
				std::cerr << "Error : Database's rate not a number: " << line << std::endl;
				continue;
			}
		}
		if (std::count(trim(value).begin(), trim(value).end(), '.') > 1)
		{
			std::cerr << "Error : Database's rate: '.' has more than 1: " << line << std::endl;
			continue;
		}
		// std::cout << "date: " << date << " value: " << value << std::endl;
		try {
			rate = std::stof(trim(value));
			// std::cout << "date: " << date << " rate: " << rate << std::endl;
		} catch (std::exception &e) {
			(void)e;
			std::cerr << "Error: Could not convert string to double: " << line << std::endl;
			continue;
		}
		_PriceRates.push_back(std::make_pair(date, rate));
	}
	// for (int i = 0; i < _PriceRates.size(); i++)
	// 	std::cout  << _PriceRates[i].first << ": " << _PriceRates[i].second << std::endl;
	// std::cout  << _PriceRates[0].first << std::endl;
	db.close();
}

void	BitcoinExchange::parseInput()
{
	std::ifstream	input(_inputFile);
	std::string		line;
	std::string		date;
	std::string		value;
	size_t			pos;
	float			btc;
	float			rate;

	if (!input.is_open())
	{
		std::cerr << "Error: Could not open Input file" << std::endl;
		return;
	}
	std::getline(input, line);
	date = line.substr(0, line.find_first_of("|"));
	value = line.substr(line.find_first_of("|") + 1);
	if (trim(date) != "date")
	{
		std::cerr << "Error: Invalid Input header format" << std::endl;
		return;
	}
	if (trim(value) != "value")
	{
		std::cerr << "Error: Invalid Input header format" << std::endl;
		return;
	}
	while (std::getline(input, line))
	{
		pos = line.find("|");
		date = line.substr(0, pos);
		value = line.substr(pos + 1);
		if (!checkDate(trim(date)))
		{
			std::cerr << "Error: bad Input => " << trim(date) << std::endl;
			continue;
		}
		// std::cout << date << " => " << value << std::endl;
		for (std::string::const_iterator it = trim(value).begin(); it != trim(value).end(); ++it) {
			if (!std::isdigit(*it) && *it != '.' && *it != '-') {
				std::cerr << "Error : Not a number." << std::endl;
				continue;
			}
		}
		if (std::count(trim(value).begin(), trim(value).end(), '.') > 1)
		{
			std::cerr << "Error : Input's value: '.' has more than 1 => " << line << std::endl;
			continue;
		}
		if (!checkValue(value))
			continue;
		if (binarySearch(date) < 0)
		{
			std::cerr << "Error : Input Date is lower than Database Date => " << line << std::endl;
			continue;
		}
		rate = _PriceRates[binarySearch(date)].second;
		btc = std::stof(trim(value));
		std::cout << date << " => " << btc << " = " << rate * btc << std::endl;
		// std::cout << date << " => " << value << " = " << std::endl;
	}
}

int	BitcoinExchange::binarySearch(const std::string &date)
{
	int	start = 0;
	int	end = _PriceRates.size() - 1;
	int	mid;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (date >= _PriceRates[mid].first)
		{
			if (date == _PriceRates[mid].first)
				return mid;
			start = mid + 1;
		}
		else
			end = mid - 1;
		// std::cout << "mid: " << mid << std::endl;
		// std::cout << "mid_value: " << _PriceRates[mid].first << std::endl;
	}
	// std::cout << "end: " << end << std::endl;
	// std::cout << "end_value: " << _PriceRates[end].first << std::endl;
	if (end < 0)
		return -1;
	else
		return end;
}

bool	BitcoinExchange::checkDate(const std::string &date)
{
	std::string	year;
	std::string	month;
	std::string	day;

	year = date.substr(0, 4);
	month = date.substr(5, 2);
	day = date.substr(8, 2);
	// std::cout << "date: " << date << std::endl;
	// std::cout << "year: " << year << std::endl;
	// std::cout << "month: " << month << std::endl;
	// std::cout << "day: " << day << std::endl;
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	if (!checkYear(year))
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	if (!checkMonth(month))
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	if (!checkDay(day, month, year))
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	return true;
}

bool	BitcoinExchange::checkYear(const std::string &year)
{
	for (std::string::const_iterator it = year.begin(); it != year.end(); ++it) {
		// std::cout << *it << std::endl;
		if (!std::isdigit(*it)) {
			// std::cerr << "Error: bad Input" << std::endl;
			return false;
		}
	}
	return true;
}

bool	BitcoinExchange::checkMonth(const std::string &month)
{
	int	m;

	// std::cout << "month: " << month << std::endl;
	for (std::string::const_iterator it = month.begin(); it != month.end(); ++it) {
		// std::cout << *it << std::endl;
		if (!std::isdigit(*it)) {
			// std::cerr << "Error: bad Input" << std::endl;
			return false;
		}
	}
	m = atoi(month.c_str());
	// std::cout << "m: " << m << std::endl;
	if (m < 1 || m > 12)
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	return true; 
}

bool	BitcoinExchange::checkDay(const std::string &day, const std::string &month, const std::string &year)
{
	int	d;
	int m;
	int y;

	for (std::string::const_iterator it = day.begin(); it != day.end(); ++it) {
		if (!std::isdigit(*it)) {
			// std::cerr << "Error: bad Input" << std::endl;
			return false;
		}
	}
	d = atoi(day.c_str());
	m = atoi(month.c_str());
	y = atoi(year.c_str());
	if (d < 1 || d > 31)
	{
		// std::cerr << "Error: bad Input" << std::endl;
		return false;
	}
	if (d == 29 && m == 2)
	{
		if (y % 4 != 0)
		{
			// std::cerr << "Error: bad Input" << std::endl;
			return false;
		}
		else if (y % 4 == 0 && y % 100 == 0 && y % 400 != 0)
		{
			// std::cerr << "Error: bad Input" << std::endl;
			return false;
		}
	}
	return true; 
}

bool	BitcoinExchange::checkValue(const std::string &value)
{
	std::string	value2 = value;

	if (!checkPositive(trim(value2)))
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	else if (!checkLargeNumber(trim(value2)))
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

bool	BitcoinExchange::checkPositive(const std::string &value)
{
	float	btc;

	btc = std::stof(value);
	if (btc < 0)
		return false;
	return true;
}

bool	BitcoinExchange::checkLargeNumber(const std::string &value)
{
	float	btc;

	btc = std::stof(value);
	if (btc > 1000)
		return false;
	return true;
}

const char	*BitcoinExchange::ErrorOpeningFileException::what() const throw()
{
	return ("Error: could not open file");
}