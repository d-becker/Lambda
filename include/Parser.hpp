#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>

#include <LinkedList.hpp>

#include "Lexer.hpp"
#include "Node.hpp"
#include "Token.hpp"

class Parser {
public:
	Parser(std::istream *input);

	Node *parse();

private:
	const Token& get_look_ahead() const;
	void advance();
	void assert_next_token_type(Token::Type type,
				    std::string expectation) const;
	
	Node *parse_expression();
	Node *parse_application();
	Node *parse_function();
	void parse_possible_ws();
	
	LinkedList<Token> m_tokens;
};

#endif // PARSER_HPP
