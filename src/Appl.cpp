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
		
std::shared_ptr<const Node> Appl::reduce(long long& count) const {
	--count;
	
        if (count < 0) {
		return m_weak_this.lock();
	}
	
	// If we are a redex
	std::shared_ptr<const Func> func = m_left_child->as_function();
	if (func != nullptr) {
		std::string param_name = func->get_param_name();
		std::shared_ptr<const Node> sub = func->get_function_body()
			->substitute(param_name, m_right_child);
		return sub->reduce(count);
	}

	// If we are not a redex, first reduce the left child.
	std::shared_ptr<const Node> left_reduct = m_left_child->reduce(count);
	if (m_left_child == left_reduct) {
		// It is not possible to reduce this \a Appl,
		// reduce the right side.
		std::shared_ptr<const Node> right_reduct
			= m_right_child->reduce(count);
		if (m_right_child == right_reduct) {
			return m_weak_this.lock();
		} else {
			return Appl::create(left_reduct, right_reduct);
		}
	} else {
		// The left child has been reduced, start from the beginning.
		std::shared_ptr<const Node> new_application
			= Appl::create(left_reduct, m_right_child);
		return new_application->reduce(count);
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

std::vector<std::size_t> Appl::get_tree_level_widths() const {
	std::vector<std::size_t> left_vec(
		get_left_child()->get_tree_level_widths());
	std::vector<std::size_t> right_vec(
		get_right_child()->get_tree_level_widths());

	std::vector<std::size_t>& longer = left_vec.size() >= right_vec.size() ?
		left_vec : right_vec;
	std::vector<std::size_t>& shorter = left_vec.size() < right_vec.size() ?
		left_vec : right_vec;

	for (auto longer_it = longer.rbegin(),
		     shorter_it = shorter.rbegin();
	     shorter_it != shorter.rend();
	     ++longer_it, ++shorter_it)
	{
		*longer_it += *shorter_it;
	}

	longer.emplace_back(1);
	
	return longer;
}

std::string Appl::to_string() const {
	std::stringstream s;
	s << '(' << m_left_child->to_string()
	  << ' ' << m_right_child->to_string() << ')';

	return s.str();
}
