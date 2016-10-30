#ifndef FUNC_HPP
#define FUNC_HPP

#include <memory>
#include <string>

#include "Node.hpp"

class Func : public Node {
public:
	template <class... U>
	static std::shared_ptr<const Func> create(U&&... u) {
		Func *raw_res = new Func(std::forward<U>(u)...);
		std::shared_ptr<Func> res(raw_res);
		raw_res->m_weak_this = res;

		return res;
	}
	
	Func(const Func& other) = delete;
	virtual ~Func();
	
	const std::string& get_param_name() const;
	std::shared_ptr<const Node> get_function_body() const;

	virtual std::shared_ptr<const Node> reduce() const override;
	virtual Vector<std::string> free_variables() const override;
	virtual std::string to_string() const override;
private:
	Func(std::string param_name,
	     std::shared_ptr<const Node> body);
	
	const std::string m_param_name;
	const std::shared_ptr<const Node> m_function_body;

	std::weak_ptr<const Func> m_weak_this;
};

#endif // FUNC_HPP
