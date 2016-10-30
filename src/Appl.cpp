#include "Appl.hpp"

#include <sstream>

#include "Func.hpp"

Appl::Appl(std::shared_ptr<const Node> left_child,
	   std::shared_ptr<const Node> right_child)
	: m_left_child(left_child),
	  m_right_child(right_child)
{
}

Appl::~Appl()
{
}

std::shared_ptr<const Node> Appl::get_left_child() const {
	return m_left_child;
}

std::shared_ptr<const Node> Appl::get_right_child() const {
	return m_right_child;
}

std::shared_ptr<const Node>
Appl::substitute(const std::string& var_name,
		 std::shared_ptr<const Node> expr) const {
	std::shared_ptr<const Node> new_left
		= m_left_child->substitute(var_name, expr);
	std::shared_ptr<const Node> new_right
		= m_right_child->substitute(var_name, expr);

	if (new_left == m_left_child && new_right == m_right_child) {
		return m_weak_this.lock();
	} else {
		return Appl::create(new_left, new_right);
	}
}

std::shared_ptr<const Node>
Appl::raw_substitute(const std::string& orig_var_name,
		     std::shared_ptr<const Node> expr) const {
	std::shared_ptr<const Node> new_left
		= m_left_child->raw_substitute(orig_var_name, expr);
	std::shared_ptr<const Node> new_right
		= m_right_child->raw_substitute(orig_var_name, expr);

	if (new_left == m_left_child && new_right == m_right_child) {
		return m_weak_this.lock();
	} else {
		return Appl::create(new_left, new_right);
	}
}
		
std::shared_ptr<const Node> Appl::reduce() const {
	// If we are a redex
	if (m_left_child->is_function()) {
		// TODO - refactor to avoid casting
		const Func* func = static_cast<const Func*>(m_left_child.get());
		std::string param_name = func->get_param_name();
		std::shared_ptr<const Node> sub = func->get_function_body()->substitute(param_name, m_right_child);
		return sub->reduce();
	}

	// If we are not a redex, first reduce the left child.
	std::shared_ptr<const Node> left_reduct = m_left_child->reduce();
	if (m_left_child == left_reduct) {
		// It is not possible to reduce this \a Appl,
		// reduce the right side.
		std::shared_ptr<const Node> right_reduct = m_right_child->reduce();
		if (m_right_child == right_reduct) {
			return m_weak_this.lock();
		} else {
			return Appl::create(left_reduct, right_reduct);
		}
	} else {
		// The left child has been reduced, start from the beginning.
		std::shared_ptr<const Node> new_application = Appl::create(left_reduct, m_right_child);
		return new_application->reduce();
	}
}

std::unordered_set<std::string> Appl::abstractions() const {
	std::unordered_set<std::string> left = m_left_child->abstractions();
	std::unordered_set<std::string> right = m_right_child->abstractions();

	left.insert(right.begin(), right.end());

	return left;
}

std::unordered_set<std::string> Appl::variables() const {
	std::unordered_set<std::string> vars_left = m_left_child->variables();
	std::unordered_set<std::string> vars_right = m_right_child->variables();

	vars_left.insert(vars_right.begin(), vars_right.end());

	return vars_left;
}

std::unordered_set<std::string> Appl::free_variables() const {
	std::unordered_set<std::string> vars_left
		= m_left_child->free_variables();
	std::unordered_set<std::string> vars_right
		= m_right_child->free_variables();

	vars_left.insert(vars_right.begin(), vars_right.end());

	return vars_left;
}

std::string Appl::to_string() const {
	std::stringstream s;
	s << '(' << m_left_child->to_string()
	  << ' ' << m_right_child->to_string() << ')';

	return s.str();
}
