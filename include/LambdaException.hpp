#ifndef LAMBDA_EXCEPTION_HPP
#define LAMBDA_EXCEPTION_HPP

#include <stdexcept>

class LambdaException : public std::runtime_error {
public:
	LambdaException(std::string message)
		: std::runtime_error(message)
	{
	}
};

#endif // LAMBDA_EXCEPTION_HPP
