#include "Subs.hpp"

Subs::Subs(std::string label)
	: m_label(label)
{
}

Subs::Subs(const Subs& other)
	: m_label(other.m_label)
{
}

Subs::Subs(Subs&& other)
	: m_label(other.m_label)
{
}

Subs::~Subs() {
	
}

const std::string& Subs::get_variable_name() const {
	return m_label;
}

void Subs::reduce() {
}

Vector<std::string> Subs::free_variables() const {
	return {m_label};
}

std::string Subs::to_string() const {
	return m_label;
}

Node *Subs::copy() const {
	return new Subs(*this);	
}
