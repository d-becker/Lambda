#include "Lexer.hpp"

#include <cctype>

#include <functional>
#include <sstream>

#include <Vector.hpp>

#include "LexerException.hpp"

namespace /* anonymous */ {
struct CheckToken {
	const std::function<bool(char)> is_begin;
	const std::function<Token(const char*,
				  const Token::Position&)> tokenize;
};

bool is_lambda_begin(char c) {
	return c == '\\';
}

Token tokenize_lambda(const char*, const Token::Position& pos) {
	return Token(Token::LAMBDA, pos, "\\");
		     
}

bool is_dot_begin(char c) {
	return c == '.';
}

Token tokenize_dot(const char*, const Token::Position& pos) {
	return Token(Token::DOT, pos, ".");
}

bool is_open_paren_begin(char c) {
	return c == '(';
}

Token tokenize_open_paren(const char*, const Token::Position& pos) {
	return Token(Token::OPEN_PAREN, pos, "(");
}

bool is_close_paren_begin(char c) {
	return c == ')';
}

Token tokenize_close_paren(const char*, const Token::Position& pos) {
	return Token(Token::CLOSE_PAREN, pos, ")");
}

bool is_identifier_begin(char c) {
	return std::isalpha(c) || c == '_';
}

bool is_identifier(char c) {
	return is_identifier_begin(c) || std::isdigit(c);
}

Token tokenize_identifier(const char *s, const Token::Position& pos) {
	const char *current = s;

	while (is_identifier(*current)) {
		++current;
	}

	return Token(Token::IDENTIFIER, pos, std::string(s, current));
}

bool is_whitespace_begin(char c) {
	return std::isspace(c);
}

Token tokenize_whitespace(const char *s, const Token::Position& pos) {
	const char *current = s;

	while (std::isspace(*current)) {
		++current;
	}

	return Token(Token::WS, pos, std::string(s, current));
}

// The actual tokenizing functions collected. The order matters.
const Vector<CheckToken> to_check {
	{is_lambda_begin, tokenize_lambda},
	{is_dot_begin, tokenize_dot},
	{is_open_paren_begin, tokenize_open_paren},
	{is_close_paren_begin, tokenize_close_paren},
	{is_identifier_begin, tokenize_identifier},
	{is_whitespace_begin, tokenize_whitespace}
};

} // anonymous namespace

Lexer::Lexer(std::istream *input)
	: m_input(input),
	  m_tokens{}
{
}

LinkedList<Token> Lexer::tokenize() {
	std::size_t line_number = 1;
	std::string line;

	while (getline(*m_input, line)) {
		tokenize_line(line, line_number);		
		++line_number;
	}

	return m_tokens;
}

// Private
void Lexer::tokenize_line(const std::string& line,
			  const std::size_t line_number) {
	const char *string_begin = line.c_str();
	std::size_t column_number = 0;

	while (column_number < line.length()) {
		bool success = false;
		for (auto check : to_check) {
			if (check.is_begin(string_begin[column_number])) {
				Token::Position pos(line_number, column_number);

				Token token = check.tokenize(string_begin
							     + column_number, pos);
				m_tokens.push_back(token);
				column_number += token.match.length();
				success = true;
				break;
			}
		}

		if (!success) {
			std::stringstream s;
			s << "Unexpected charecter at line "
			  << line_number << " in column "
			  << column_number << ": "
			  << string_begin[column_number] << ".";
		        throw LexerException(s.str());
		}
	}
}
