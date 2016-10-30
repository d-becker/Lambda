#ifndef APPL_HPP
#define APPL_HPP

#include <memory>
#include <string>

#include "Vector.hpp"

#include "Node.hpp"

class Appl : public Node {
public:
	template <class... U>
	static std::shared_ptr<const Appl> create(U&&... u) {
		Appl *raw_res = new Appl(std::forward<U>(u)...);
		std::shared_ptr<Appl> res(raw_res);
		raw_res->m_weak_this = res;

		return res;
	}
	

	Appl(const Appl& other) = delete;
	~Appl();
	
	std::shared_ptr<const Node> get_left_child() const;
	std::shared_ptr<const Node> get_right_child() const;

	virtual std::shared_ptr<const Node>
	substitute(const std::string& var_name,
		   std::shared_ptr<const Node> expr) const override;
	
	virtual std::shared_ptr<const Node>
	raw_substitute(const std::string& orig_var_name,
		       std::shared_ptr<const Node> expr) const override;
	virtual std::shared_ptr<const Node> reduce() const override;

	virtual std::unordered_set<std::string> abstractions() const override;
	virtual std::unordered_set<std::string> variables() const override;
	virtual std::unordered_set<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	Appl(std::shared_ptr<const Node> left_child,
	     std::shared_ptr<const Node> right_child);
	
	std::shared_ptr<const Node> m_left_child;
	std::shared_ptr<const Node> m_right_child;

	std::weak_ptr<const Appl> m_weak_this;
};

#endif // APPL_HPP
