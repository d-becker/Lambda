#ifndef LAMBDA_HPP
#define LAMBDA_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class lambda {
public:
	lambda(std::string source);
	lambda(const lambda& other);
	lambda(lambda&& other);

	virtual ~lambda();

	void reduce();
	std::string to_string() const;
private:
	std::unique_ptr<Node> m_node;
};

#endif // LAMBDA_HPP
