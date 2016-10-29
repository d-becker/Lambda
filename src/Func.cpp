#include "Func.hpp"

#include <algorithm>
#include <sstream>

Func::Func(std::string param_name,
	   std::shared_ptr<const Node> body)
	: m_param_name(param_name),
	  m_function_body(body)
{
}

Func::Func(const Func& other)
	: m_param_name(other.m_param_name),
	  m_function_body(other.m_function_body)
{
}

Func::~Func()
{
}

const std::string& Func::get_param_name() const {
	return m_param_name;
}

std::shared_ptr<const Node> Func::get_function_body() const {
	return m_function_body;
}

std::shared_ptr<const Node> Func::reduce() const {
	// TODO
}

Vector<std::string> Func::free_variables() const {
	Vector<std::string> vars = m_function_body->free_variables();
	auto it = std::find(vars.begin(), vars.end(), m_param_name);
	if (it != vars.end()) {
		vars.erase(it);
	}

	return vars;
}

std::string Func::to_string() const {
	std::stringstream s;
	s << '\\' << m_param_name << '.' << m_function_body->to_string();
	return s.str();
}
