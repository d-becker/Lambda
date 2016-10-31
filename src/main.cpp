#include <iostream>
#include <fstream>
#include <unordered_set>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

#include "Subs.hpp"
#include "Func.hpp"

using namespace std;

int main() {
	std::ifstream file("proba.txt");
	Parser p(&file);

        std::shared_ptr<const Node> tree = p.parse();

         

	cout << "The tree: " << endl;
	cout << tree->to_string() << "." << endl;

	long long count = 100;
	std::shared_ptr<const Node> reduced_tree = tree->reduce(count);
	cout << "The reduced tree: " << reduced_tree->to_string() << "." << endl;

	/*cout << "Free variables:" << endl;
	std::unordered_set<std::string> free_vars = tree->free_variables();

	for (std::string var : free_vars) {
		cout << var << ", ";
	}

	cout << endl;

	std::shared_ptr<const Node> subst = Subs::create("x");
	std::shared_ptr<const Node> subst_y = Subs::create("y");
	std::shared_ptr<const Func> func = Func::create("x", subst_y);
	//func = Func::create("x", func);
	cout << "New formula: " << func->to_string() << "." << endl;

	std::shared_ptr<const Node> alpha = func->substitute("y", subst);
	cout << "After substitution: " << alpha->to_string() << "." << endl;*/
	
	return 0;
}
