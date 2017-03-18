#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <list>

#include "Token.hpp"

class Lexer {
public:
	Lexer(std::istream *input);

	std::list<Token> tokenize();
private:
	struct CheckToken;
	
	void tokenize_line(const std::string& line);

	bool is_lambda_begin(char c) const;
	void tokenize_lambda(std::string::const_iterator start,
			     std::string::const_iterator end);
	
	bool is_dot_begin(char c) const;
	void tokenize_dot(std::string::const_iterator start,
			  std::string::const_iterator end);

	bool is_open_paren_begin(char c) const;
	void tokenize_open_paren(std::string::const_iterator start,
				 std::string::const_iterator end);

	bool is_close_paren_begin(char c) const;
	void tokenize_close_paren(std::string::const_iterator start,
				  std::string::const_iterator end);

	bool is_identifier_begin(char c) const;
	bool is_identifier(char c) const;
	void tokenize_identifier(std::string::const_iterator start,
				 std::string::const_iterator end);

	bool is_whitespace_begin(char c) const;
	void tokenize_whitespace(std::string::const_iterator start,
				 std::string::const_iterator end);
	
	std::istream *m_input;
	std::size_t m_line;
	std::size_t m_column;

	std::list<Token> m_tokens;
};

#endif // LEXER_HPP
