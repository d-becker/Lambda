#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>

#include "Vector.hpp"

class Node {
public:
	virtual ~Node() {}
	
	virtual std::shared_ptr<const Node> reduce() const = 0;

	virtual Vector<std::string> free_variables() const = 0;
	virtual std::string to_string() const = 0;
};

#endif // NODE_HPP
