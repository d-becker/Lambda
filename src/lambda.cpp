#include "lambda.hpp"

#include <sstream>

#include "Parser.hpp"

lambda::lambda(std::string source)
	: m_node(nullptr)
{
	std::stringstream s(source);
	Parser parser(&s);
	m_node = parser.parse();
}

lambda::lambda(const lambda& other)
	: m_node(other.m_node)
{
}

lambda::lambda(lambda&& other)
	: m_node(other.m_node)
{
}

lambda::~lambda() {
}

void lambda::reduce() {
	// TODO
}

std::string lambda::to_string() const {
	return m_node ? m_node->to_string() : "";
}
