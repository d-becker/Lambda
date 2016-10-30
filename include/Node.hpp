#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_set>

class Node {
public:
	virtual ~Node() {}

	virtual std::shared_ptr<const Node>
	raw_substitute(const std::string& orig_var_name,
		       std::shared_ptr<const Node> expr) const = 0;

	virtual std::shared_ptr<const Node>
	substitute(const std::string& var_name,
		   std::shared_ptr<const Node> expr) const = 0;
	
	virtual std::shared_ptr<const Node> reduce() const = 0;

	virtual std::unordered_set<std::string> abstractions() const = 0;
	virtual std::unordered_set<std::string> variables() const = 0;
	virtual std::unordered_set<std::string> free_variables() const = 0;

	virtual bool is_function() const {
		return false;
	}
	
	virtual std::string to_string() const = 0;
};

#endif // NODE_HPP
