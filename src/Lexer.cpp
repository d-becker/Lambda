#include "Lexer.hpp"

#include <cctype>

#include <functional>
#include <sstream>
#include <vector>

#include "LexerException.hpp"

struct Lexer::CheckToken {
	typedef bool (Lexer::*is_begin_type) (char) const;
	typedef void (Lexer::*tokenize_type) (std::string::const_iterator,
					      std::string::const_iterator);
	
	CheckToken(is_begin_type p_is_begin,
		   tokenize_type p_tokenize)
		: is_begin(p_is_begin),
		  tokenize(p_tokenize)
		{
		}
	
        is_begin_type is_begin;
        tokenize_type tokenize;

	// The actual tokenizing functions collected. The order matters.
	static const std::vector<CheckToken> to_check;
};

/* static */ const std::vector<Lexer::CheckToken> Lexer::CheckToken::to_check {
        {&Lexer::is_lambda_begin, &Lexer::tokenize_lambda},
	{&Lexer::is_dot_begin, &Lexer::tokenize_dot},
	{&Lexer::is_open_paren_begin, &Lexer::tokenize_open_paren},
	{&Lexer::is_close_paren_begin, &Lexer::tokenize_close_paren},
	{&Lexer::is_identifier_begin, &Lexer::tokenize_identifier},
	{&Lexer::is_whitespace_begin, &Lexer::tokenize_whitespace}
};


Lexer::Lexer(std::istream *input)
	: m_input(input),
	  m_line(0),
	  m_column(0),
	  m_tokens{}
{
	if (m_input == nullptr) {
		throw LexerException(
			"The input stream pointer must not be nullptr.");
	}
}

std::list<Token> Lexer::tokenize() {
	std::string line;

	while (getline(*m_input, line)) {		
		++m_line;
		tokenize_line(line);
	}

	m_tokens.push_back(Token(Token::END_OF_INPUT,
				 Token::Position(m_line, m_column), ""));

	return m_tokens;
}

// Private
void Lexer::tokenize_line(const std::string& line) {
	m_column = 0;
	while (m_column < line.length()) {
		bool success = false;
		for (auto check : CheckToken::to_check) {
			if ((this->*check.is_begin)(line[m_column])) {
				(this->*check.tokenize)(line.begin() + m_column,
							line.end());
				
				success = true;
				break;
			}
		}

		if (!success) {
			std::stringstream s;
			s << "Unexpected charecter at line "
			  << m_line << " in column "
			  << m_column << ": "
			  << line[m_column] << ".";
		        throw LexerException(s.str());
		}
	}
}

bool Lexer::is_lambda_begin(char c) const {
	return c == '\\';
}

void Lexer::tokenize_lambda(std::string::const_iterator start,
			    std::string::const_iterator end) {
	if (end - start > 0) {
		m_tokens.push_back(Token(Token::LAMBDA,
					 Token::Position(m_line, m_column),
					 "\\"));
		
		++m_column;
	}
}

bool Lexer::is_dot_begin(char c) const {
	return c == '.';
}

void Lexer::tokenize_dot(std::string::const_iterator start,
			 std::string::const_iterator end) {
	if (end - start > 0) {
		m_tokens.push_back(Token(Token::DOT,
					 Token::Position(m_line, m_column),
					 "."));
		++m_column;
	}
}

bool Lexer::is_open_paren_begin(char c) const {
	return c == '(';
}

void Lexer::tokenize_open_paren(std::string::const_iterator start,
				std::string::const_iterator end) {
	if (end - start > 0) {
		m_tokens.push_back(Token(Token::OPEN_PAREN,
					 Token::Position(m_line, m_column),
					 "("));
		++m_column;
	}
}

bool Lexer::is_close_paren_begin(char c) const {
	return c == ')';
}

void Lexer::tokenize_close_paren(std::string::const_iterator start,
				 std::string::const_iterator end) {
	if (end - start > 0) {
		m_tokens.push_back(Token(Token::CLOSE_PAREN,
					 Token::Position(m_line, m_column),
					 ")"));
		++m_column;
	}
}

bool Lexer::is_identifier_begin(char c) const {
	return std::isalpha(c) || c == '_';
}

bool Lexer::is_identifier(char c) const {
	return is_identifier_begin(c) || std::isdigit(c);
}

void Lexer::tokenize_identifier(std::string::const_iterator start,
				std::string::const_iterator end) {
	std::string::const_iterator current = start;
	while (current != end && is_identifier(*current)) {
		++current;
	}

	m_tokens.push_back(Token(Token::IDENTIFIER,
				 Token::Position(m_line, m_column),
				 std::string(start, current)));
	m_column += current - start;
}

bool Lexer::is_whitespace_begin(char c) const {
	return std::isspace(c);
}

void Lexer::tokenize_whitespace(std::string::const_iterator start,
				std::string::const_iterator end) {
	std::string::const_iterator current = start;

	while (current != end && std::isspace(*current)) {
		++current;
	}

	m_tokens.push_back(Token(Token::WS, Token::Position(m_line, m_column),
				 std::string(start, current)));
	m_column += current - start;
}
