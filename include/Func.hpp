#ifndef FUNC_HPP
#define FUNC_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class Func : public Node {
public:
	Func(std::string param_name,
	     Node *body);
	Func(const Func& other);
	virtual ~Func();
	
	const std::string& get_param_name() const;
	const Node *get_function_body() const;

	virtual void reduce() override;
	virtual std::string to_string() const override;
	virtual Node *copy() const override;
private:
	const std::string m_param_name;
	std::unique_ptr<Node> m_function_body;
};

#endif // FUNC_HPP
