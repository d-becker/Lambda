#include "lambda.hpp"

#include <sstream>

#include "Parser.hpp"

lambda::lambda(std::string source, std::size_t max_calc_steps)
	: m_node(nullptr),
	  m_max_calc_steps(max_calc_steps)
{
	std::stringstream s(source);
	Parser parser(&s);
	m_node = parser.parse();
}

lambda::lambda(const lambda& other)
	: m_node(other.m_node),
	  m_max_calc_steps(other.m_max_calc_steps)
{
}

lambda::lambda(lambda&& other)
	: m_node(other.m_node),
	  m_max_calc_steps(other.m_max_calc_steps)
{
}

lambda::~lambda() {
}

long long lambda::get_max_calc_steps() const  {
	return m_max_calc_steps;
}

void lambda::set_max_calc_steps(long long max_calc_steps) {
	m_max_calc_steps = max_calc_steps;
}

void lambda::reduce() {
	long long count = m_max_calc_steps;
	m_node = m_node->reduce(count);
}

std::string lambda::to_string() const {
	return m_node ? m_node->to_string() : "";
}
