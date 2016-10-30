#include "Subs.hpp"
Subs::Subs(std::string label)
	: m_label(label)
{	
}

Subs::~Subs() {
	
}

const std::string& Subs::get_variable_name() const {
	return m_label;
}

std::shared_ptr<const Node> Subs::reduce() const {
	return m_weak_this.lock();
}

Vector<std::string> Subs::free_variables() const {
	return {m_label};
}

std::string Subs::to_string() const {
	return m_label;
}
