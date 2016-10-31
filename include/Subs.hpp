#ifndef SUBS_HPP
#define SUBS_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class Subs : public Node {
public:	
	template <class... U>
	static std::shared_ptr<const Subs> create(U&&... u) {
		Subs *raw_res = new Subs(std::forward<U>(u)...);
		std::shared_ptr<Subs> res(raw_res);
		raw_res->m_weak_this = res;

		return res;
	}
	
	Subs(const Subs& other) = delete;
	virtual ~Subs();
	
	const std::string& get_variable_name() const;

	virtual std::shared_ptr<const Node>
	substitute(const std::string& var_name,
		   std::shared_ptr<const Node> expr) const override;
	
	virtual std::shared_ptr<const Node>
	raw_substitute(const std::string& orig_var_name,
		       std::shared_ptr<const Node> expr) const override;
	virtual std::shared_ptr<const Node> reduce(long long& count) const override;

	virtual std::unordered_set<std::string> abstractions() const override;
	virtual std::unordered_set<std::string> variables() const override;
	virtual std::unordered_set<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	Subs(std::string label);
	
	const std::string m_label;
	std::weak_ptr<const Subs> m_weak_this;
};

#endif // SUBS_HPP
