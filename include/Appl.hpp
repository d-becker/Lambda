#ifndef APPL_HPP
#define APPL_HPP

#include <memory>
#include <string>

#include "Vector.hpp"

#include "Node.hpp"

class Appl : public Node {
public:
	Appl(Node *left_child,
	     Node *right_child);
	Appl(const Appl& other);
	Appl(Appl&& other);

	~Appl();
	
	const Node *get_left_child() const;
	const Node *get_right_child() const;

	virtual void reduce() override;
	virtual Vector<std::string> free_variables() const override;
	virtual std::string to_string() const override;
	virtual Node *copy() const override;
private:
	std::unique_ptr<Node> m_left_child;
	std::unique_ptr<Node> m_right_child;
};

#endif // APPL_HPP
