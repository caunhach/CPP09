#include "RPN.hpp"

RPN::RPN() { }

RPN::RPN(const RPN &src)
{
	*this = src;
}

RPN::~RPN() { }

RPN & RPN::operator=(RPN const &src)
{
	(void)src;
	return (*this);
}

void	RPN::fillStack(const std::string &src)
{
	int len = src.size();
	double	value1;
	double	value2;

	for (int i = 0; i < len; i++)
	{
		if (convertToEnum(src[i]) == None && src[i] != ' ')
			throw InvalidArgumentsException();
		if (convertToEnum(src[i]) == Number)
			_number.push(static_cast<double>(src[i] - '0'));
		else if (convertToEnum(src[i]) != Number && convertToEnum(src[i]) != None)
		{
			if (_number.size() < 2)
				throw InvalidArgumentsException();
			value2 = _number.top();
			_number.pop();
			value1 = _number.top();
			_number.pop();
			_number.push(doOperation(value1, value2, convertToEnum(src[i])));
		}
	}
	if (_number.size() == 1)
		std::cout << _number.top() << std::endl;
	else
		throw InvalidArgumentsException();
}

double	RPN::doOperation(double a, double b, t_operator op)
{
	switch(op) {
        case Addition: return a + b;
        case Deletion: return a - b;
        case Multiply: return a * b;
    }
	return a / b;
}

void	RPN::printStackNumber()
{
	std::stack<double>	_tmp = _number;

	std::cout << "stack size: " << _number.size() << std::endl;
	while (_tmp.size())
	{
		std::cout << _tmp.top() << std::endl;
		_tmp.pop();
	}
}

std::string	RPN::operatorToString(t_operator op)
{
	switch(op) {
        case Addition: return "ADD";
        case Deletion: return "SUBTRACT";
        case Multiply: return "MULTIPLY";
        case Division: return "DIVIDE";
		case Number: return "Number";
    }
	return "UNKNOWN";
}

t_operator	RPN::convertToEnum(char c)
{
	switch(c) {
		case Addition: return Addition;
        case Deletion: return Deletion;
        case Multiply: return Multiply;
        case Division: return Division;
	}
	if (c - '0' >= 0 && c - '0' <= 9)
		return Number;
	return None;
}

const char	*RPN::InvalidArgumentsException::what() const throw()
{
	return ("Error");
}