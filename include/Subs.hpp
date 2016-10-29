#ifndef SUBS_HPP
#define SUBS_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class Subs : public Node {
public:
	Subs(std::string label);
	Subs(const Subs& other);
	Subs(Subs&& other);
	virtual ~Subs();
	
	const std::string& get_variable_name() const;

	virtual std::shared_ptr<const Node> reduce() const override;
	virtual Vector<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	const std::string m_label;
};

#endif // SUBS_HPP
