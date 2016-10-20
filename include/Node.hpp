#ifndef NODE_HPP
#define NODE_HPP

class Node {
public:
	virtual ~Node() {}
	
	virtual void reduce() = 0;
	virtual std::string to_string() const = 0;
	virtual Node *copy() const = 0;
};

#endif // NODE_HPP
