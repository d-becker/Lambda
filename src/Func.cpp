#include "Func.hpp"

#include <algorithm>
#include <sstream>

#include "Subs.hpp"

Func::Func(std::string param_name,
	   std::shared_ptr<const Node> body)
	: m_param_name(param_name),
	  m_function_body(body)
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

std::shared_ptr<const Node>
Func::alpha_convert(std::string new_var_name_hint,
		    std::shared_ptr<const Node> other) const {
	std::string new_name = get_new_alpha_name(new_var_name_hint, other);
	std::shared_ptr<const Node> function_body
		= m_function_body->substitute(m_param_name,
						  Subs::create(new_name));

	return Func::create(new_name, function_body);
}

std::shared_ptr<const Node>
Func::substitute(const std::string& var_name,
		 std::shared_ptr<const Node> expr) const {
	if (var_name == m_param_name) {
		// No substitution: (\x.M)[x := N]  ≡ \λx.M
		return m_weak_this.lock();
	}

	std::unordered_set<std::string> free_in_expr = expr->free_variables();
	if (free_in_expr.find(m_param_name) == free_in_expr.end()) {
                // No alpha conversion needed
		std::shared_ptr<const Node> new_body
			= m_function_body->substitute(var_name, expr);

		return new_body == m_function_body ? m_weak_this.lock()
			: Func::create(m_param_name, new_body);
	} else {
		std::shared_ptr<const Node> new_abstraction
			= alpha_convert(m_param_name, expr);
		std::shared_ptr<const Node> res
			= new_abstraction->substitute(var_name, expr);
		return res;
	}
}

std::shared_ptr<const Node>
Func::raw_substitute(const std::string& orig_var_name,
		     std::shared_ptr<const Node> expr) const {
	if (orig_var_name == m_param_name) {
		return m_weak_this.lock();
	} else {
		std::shared_ptr<const Node> new_body
			= m_function_body->raw_substitute(orig_var_name, expr);

		if (new_body == m_function_body) {
			return m_weak_this.lock();
		} else {
			return Func::create(m_param_name, new_body);
		}
	}
}

std::shared_ptr<const Node> Func::reduce(long long& count) const {
	--count;
	
        if (count < 0) {
		return m_weak_this.lock();
	}
	
	std::shared_ptr<const Node> f_body_reduct
		= m_function_body->reduce(count);

	if (m_function_body == f_body_reduct) {
		return m_weak_this.lock();
	} else {
		return Func::create(m_param_name, f_body_reduct);
	}
}

std::unordered_set<std::string> Func::abstractions() const {
	std::unordered_set<std::string> a = m_function_body->abstractions();
	a.insert(m_param_name);

	return a;
}

std::unordered_set<std::string> Func::variables() const {
	return m_function_body->variables();
}

std::unordered_set<std::string> Func::free_variables() const {
	std::unordered_set<std::string> vars
		= m_function_body->free_variables();

        vars.erase(m_param_name);

	return vars;
}

std::vector<std::size_t> Func::get_tree_level_widths() const {
	std::vector<std::size_t> sub_vec(
		get_function_body()->get_tree_level_widths());
	sub_vec.emplace_back(1);
	return sub_vec;
}

std::shared_ptr<const Func> Func::as_function() const {
	return m_weak_this.lock();
}

std::string Func::to_string() const {
	std::stringstream s;
	s << '\\' << m_param_name << '.' << m_function_body->to_string();
	return s.str();
}

std::string Func::get_new_alpha_name(const std::string& hint,
				     std::shared_ptr<const Node> other) const {
	std::unordered_set<std::string> a = m_function_body->abstractions();

	if (other != nullptr) {
		std::unordered_set<std::string> free_vars
			= other->free_variables();
		a.insert(free_vars.begin(), free_vars.end());
	}

	unsigned int i = 0;
	std::string new_hint = hint;
	while (a.find(new_hint) != a.end()) {
		std::stringstream s;
		s << hint << i;
		new_hint = s.str();
		++i;
	}

	return new_hint;
}
