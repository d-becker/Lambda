#ifndef APPL_HPP
#define APPL_HPP

#include <memory>
#include <string>

#include "Vector.hpp"

#include "Node.hpp"

class Appl : public Node {
public:
	Appl(std::shared_ptr<const Node> left_child,
	     std::shared_ptr<const Node> right_child);
	Appl(const Appl& other);
	Appl(Appl&& other);

	~Appl();
	
	std::shared_ptr<const Node> get_left_child() const;
	std::shared_ptr<const Node> get_right_child() const;

	virtual std::shared_ptr<const Node> reduce() const override;
	virtual Vector<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	std::shared_ptr<const Node> m_left_child;
	std::shared_ptr<const Node> m_right_child;
};

#endif // APPL_HPP
