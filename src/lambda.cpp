#include "lambda.hpp"

#include <sstream>

#include "Parser.hpp"

lambda::lambda(std::string source)
	: m_node(nullptr)
{
	std::stringstream s(source);
	Parser parser(&s);
	m_node = std::unique_ptr<Node>(parser.parse());
}

lambda::lambda(const lambda& other)
	: m_node(other.m_node ? other.m_node->copy() : nullptr)
{
}

lambda::lambda(lambda&& other)
	: m_node(std::move(other.m_node))
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
