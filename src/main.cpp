#include <iostream>
#include <fstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

using namespace std;

int main() {
	std::ifstream file("proba.txt");
	Parser p(&file);

        Node *tree = p.parse();

	cout << "The tree: " << endl;
	cout << tree->to_string() << "." << endl;

	cout << "Free variables:" << endl;
	Vector<string> free_vars = tree->free_variables();

	for (std::string var : free_vars) {
		cout << var << ", ";
	}

	cout << endl;
	
	return 0;
}
