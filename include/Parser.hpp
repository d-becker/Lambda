#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <list>

#include "Lexer.hpp"
#include "Node.hpp"
#include "Token.hpp"

class Parser {
public:
	Parser(std::istream *input);

        std::shared_ptr<const Node> parse();

private:
	const Token& get_look_ahead() const;
	void advance();
	void assert_next_token_type(Token::Type type,
				    std::string expectation) const;
	
	std::shared_ptr<const Node> parse_expression();
        std::shared_ptr<const Node> parse_application();
        std::shared_ptr<const Node> parse_function();
	void parse_possible_ws();
	
	std::list<Token> m_tokens;
};

#endif // PARSER_HPP
