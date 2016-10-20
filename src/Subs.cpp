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

const std::string& Subs::get_label() const {
	return m_label;
}

void Subs::reduce() {
}

std::string Subs::to_string() const {
	return m_label;
}

Node *Subs::copy() const {
	return new Subs(*this);	
}
