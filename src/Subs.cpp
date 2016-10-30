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

std::shared_ptr<const Node>
Subs::substitute(const std::string& var_name,
		 std::shared_ptr<const Node> expr) const {
	return raw_substitute(var_name, expr);
}

std::shared_ptr<const Node>
Subs::raw_substitute(const std::string& orig_var_name,
		     std::shared_ptr<const Node> expr) const {
	if (orig_var_name == m_label) {
		return expr;
	} else {
		return m_weak_this.lock();
	}
}

std::shared_ptr<const Node> Subs::reduce() const {
	return m_weak_this.lock();
}

std::unordered_set<std::string> Subs::abstractions() const {
	return {};
}

std::unordered_set<std::string> Subs::variables() const {
	return {m_label};
}

std::unordered_set<std::string> Subs::free_variables() const {
	return {m_label};
}

std::string Subs::to_string() const {
	return m_label;
}
