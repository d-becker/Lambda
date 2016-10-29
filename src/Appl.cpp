#include "Appl.hpp"

#include <sstream>

Appl::Appl(Node *left_child,
	   Node *right_child)
	: m_left_child(left_child),
	  m_right_child(right_child)
{
}

Appl::Appl(const Appl& other)
	: m_left_child(other.m_left_child
		       ? other.m_left_child->copy()
		       : nullptr),
	  m_right_child(other.m_right_child
			? other.m_right_child->copy()
			: nullptr)
{
}

Appl::Appl(Appl&& other)
	: m_left_child(std::move(other.m_left_child)),
	  m_right_child(std::move(other.m_right_child))
{
}

Appl::~Appl()
{
}

const Node *Appl::get_left_child() const {
	return m_left_child.get();
}

const Node *Appl::get_right_child() const {
	return m_right_child.get();
}

void Appl::reduce() {
	// TODO
}

Vector<std::string> Appl::free_variables() const {
	Vector<std::string> vars_left = m_left_child->free_variables();
	Vector<std::string> vars_right = m_right_child->free_variables();

	for (const std::string var : vars_right) {
		if (!vars_left.contains(var)) {
			vars_left.push_back(var);
		}
	}

	return vars_left;
}

std::string Appl::to_string() const {
	std::stringstream s;
	s << '(' << m_left_child->to_string()
	  << ' ' << m_right_child->to_string() << ')';

	return s.str();
}

Node *Appl::copy() const {
	return new Appl(*this);
}
