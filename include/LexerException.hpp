#ifndef LEXER_EXCEPTION_HPP
#define LEXER_EXCEPTION_HPP

#include <stdexcept>

class LexerException : public std::runtime_error {
public:
	LexerException(std::string message)
		: std::runtime_error(message)
	{
	}
};

#endif // LEXER_EXCEPTION_HPP
