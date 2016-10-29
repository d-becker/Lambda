#ifndef NODE_HPP
#define NODE_HPP

#include <string>

#include "Vector.hpp"

class Node {
public:
	virtual ~Node() {}
	
	virtual void reduce() = 0;

	virtual Vector<std::string> free_variables() const = 0;
	virtual std::string to_string() const = 0;
	virtual Node *copy() const = 0;
};

#endif // NODE_HPP
