#ifndef FUNC_HPP
#define FUNC_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class Func : public Node {
public:
	Func(std::string param_name,
	     std::shared_ptr<const Node> body);
	Func(const Func& other);
	virtual ~Func();
	
	const std::string& get_param_name() const;
	std::shared_ptr<const Node> get_function_body() const;

	virtual std::shared_ptr<const Node> reduce() const override;
	virtual Vector<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	const std::string m_param_name;
	const std::shared_ptr<const Node> m_function_body;
};

#endif // FUNC_HPP
