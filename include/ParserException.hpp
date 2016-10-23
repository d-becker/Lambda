#ifndef PARSER_EXCEPTION_HPP
#define PARSER_EXCEPTION_HPP

#include <stdexcept>

class ParserException : public std::runtime_error {
public:
	ParserException(std::string message)
		: std::runtime_error(message)
	{
	}
};

#endif // PARSER_EXCEPTION_HPP
