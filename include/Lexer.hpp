#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>

#include <LinkedList.hpp>

#include "Token.hpp"

class Lexer {
public:
	Lexer(std::istream *input);

	LinkedList<Token> tokenize();
private:
	void tokenize_line(const std::string& line,
			   const std::size_t line_number);
	
	std::istream *m_input;

	LinkedList<Token> m_tokens;
};

#endif // LEXER_HPP
