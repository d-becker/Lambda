#include "Parser.hpp"

#include <sstream>

#include "Appl.hpp"
#include "Func.hpp"
#include "Subs.hpp"
#include "ParserException.hpp"

Parser::Parser(std::istream *input)
	: m_tokens(Lexer(input).tokenize())
{
	
}

std::shared_ptr<const Node> Parser::parse() {
        std::shared_ptr<const Node> res = parse_expression();

	assert_next_token_type(Token::END_OF_INPUT, "expected end of input");

	return res;
}

const Token& Parser::get_look_ahead() const {
	return m_tokens.front();
}

void Parser::advance() {
	if (!m_tokens.empty() && get_look_ahead().type != Token::END_OF_INPUT) {
		m_tokens.pop_front();	
	}
}

void Parser::assert_next_token_type(Token::Type type,
				    std::string expectation) const {
	if (m_tokens.empty()) {
		throw ParserException("Unexpected end of input: "
				      + expectation + ".");
	} else {
		const Token& token = get_look_ahead();
		if (token.type != type) {
			std::stringstream s;
			s << "Error at line " << token.pos.line
			  << " column " << token.pos.column
			  << ": " << expectation << ".";
			throw ParserException(s.str());;
		}
	}
}

std::shared_ptr<const Node> Parser::parse_expression() {
	const Token& look_ahead = get_look_ahead();

	// expression -> IDENTIFIER
	if (look_ahead.type == Token::IDENTIFIER) {
		std::shared_ptr<const Node> res
			= Subs::create(look_ahead.match);
		advance();
		return res;
	}

	// expression -> function
	if (look_ahead.type == Token::LAMBDA) {
		return parse_function();
	}

	// expression -> application
	if (look_ahead.type == Token::OPEN_PAREN) {
		return parse_application();
	}

	// Throwing an exception
	std::stringstream s;
	s << "Unexpected token at line " << look_ahead.pos.line
	  << " column " << look_ahead.pos.column << ": "
	  << look_ahead.match << ".";
	throw ParserException(s.str());
}

std::shared_ptr<const Node> Parser::parse_application() {
	//application -> OPEN_PAREN possible_ws expression WS expression
	//possible_ws CLOSE_PAREN
	assert_next_token_type(Token::OPEN_PAREN,
			       "expected an opening parenthesis");

        advance(); // consuming the OPEN_PAREN
	parse_possible_ws();

	std::shared_ptr<const Node> left_term = parse_expression();

	assert_next_token_type(Token::WS, "expected whitespace");

	advance(); // consuming the WS
	
	std::shared_ptr<const Node> right_term = parse_expression();
	parse_possible_ws();

	assert_next_token_type(Token::CLOSE_PAREN,
			       "expected a closing parenthesis");
	
	advance(); // consuming the CLOSE_PAREN

	return Appl::create(left_term, right_term);
}

std::shared_ptr<const Node> Parser::parse_function() {
	// LAMBDA possible_ws IDENTIFIER possible_ws DOT possible_ws expression
	assert_next_token_type(Token::LAMBDA, "expected a lambda");
	advance();

	parse_possible_ws();

	assert_next_token_type(Token::IDENTIFIER, "expected an identifier");
	const Token& param = get_look_ahead();
	std::string param_string = param.match;
	advance();

	parse_possible_ws();

	assert_next_token_type(Token::DOT, "expected a dot");
	advance();

        parse_possible_ws();

        std::shared_ptr<const Node> function_body = parse_expression();

	return Func::create(param_string, function_body);
}

void Parser::parse_possible_ws() {
	if (get_look_ahead().type == Token::WS) {
		advance();
	}
}
