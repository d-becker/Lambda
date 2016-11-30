#ifndef LAMBDA_HPP
#define LAMBDA_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class lambda {
public:
	lambda(std::string source, std::size_t max_calc_steps = 1000);
	lambda(const lambda& other);
	lambda(lambda&& other);

	virtual ~lambda();

	lambda& operator=(lambda other);
	void swap(lambda& other);
	
        long long get_max_calc_steps() const;
	void set_max_calc_steps(long long max_calc_steps);
	
	void reduce();
	std::string to_string() const;
private:	
	std::shared_ptr<const Node> m_node;
        long long m_max_calc_steps;
};

inline
void swap(lambda& left, lambda& right) {
	left.swap(right);
}

#endif // LAMBDA_HPP
