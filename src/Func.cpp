#include "Func.hpp"

#include <sstream>

Func::Func(std::string param_name,
	   Node *body)
	: m_param_name(param_name),
	  m_function_body(body)
{
}

Func::Func(const Func& other)
	: m_param_name(other.m_param_name),
	  m_function_body(other.m_function_body
			  ? other.m_function_body->copy()
			  : nullptr)
{
}

Func::~Func()
{
}

const std::string& Func::get_param_name() const {
	return m_param_name;
}

const Node *Func::get_function_body() const {
	return m_function_body.get();
}

void Func::reduce() {
	// TODO
}

std::string Func::to_string() const {
	std::stringstream s;
	s << '\\' << m_param_name << '.' << m_function_body->to_string();
	return s.str();
}

Node *Func::copy() const {
	return new Func(*this);
}
