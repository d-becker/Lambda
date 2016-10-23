#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>

#include <string>

struct Token {
	enum Type {
		NONE = 0,
		LAMBDA,
		DOT,
		OPEN_PAREN,
		CLOSE_PAREN,
		IDENTIFIER,
		APPLICATION,
		WS,
		END_OF_INPUT
	};

	struct Position {
		Position(std::size_t p_line, std::size_t p_column)
			: line(p_line),
			  column(p_column)
		{
		}

		std::size_t line;
		std::size_t column;
	};

	Token(Type p_type,
	      const Position& p_pos,
	      std::string p_match)
		: type(p_type),
		  pos(p_pos),
		  match(p_match)
	{
	}

	const Type type;
	const Position pos;
	const std::string match;
};

#endif // TOKEN_HPP
