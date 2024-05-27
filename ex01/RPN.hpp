#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <stack>

typedef enum
{
	Addition = '+',
	Deletion = '-',
	Multiply = '*',
	Division = '/',
	Number,
	None
} t_operator;

class RPN
{
	private:
		std::stack<double>	_number;
	public:
		RPN();
		RPN(const RPN &src);
		RPN &operator=(RPN const &src);
		~RPN();

		void	fillStack(const std::string &src);
		double	doOperation(double a, double b, t_operator op);
		void	printStackNumber();
		std::string	operatorToString(t_operator op);
		t_operator	convertToEnum(char c);

		class InvalidArgumentsException : public std::exception {
			public:
				virtual const char *what() const throw() override ;
		};
};

#endif