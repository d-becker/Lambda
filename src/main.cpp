#include <iostream>
#include <fstream>

#include "Lexer.hpp"
#include "Token.hpp"

int main() {
	std::ifstream file("proba.txt");
	Lexer l(&file);
	LinkedList<Token> tokens = l.tokenize();

	for (const Token& token : tokens) {
		std::cout << "Type: " << token.type << ", line: " << token.pos.line << ", column: " << token.pos.column << ", match: " << token.match << "." << std::endl;
	}
	
	return 0;
}
