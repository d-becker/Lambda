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

	/**
	 * Rename every variable in the expression that are bound by by this
	 * abstraction. The new name is \a new_var_name_hint if that is not
	 * present in this expression as another (embedded) abstraction and is
	 * not free in \a other; otherwise a new name is generated - the new
	 * name may be based on hint.
	 *
	 * \param new_var_name_hint The proposed new name of the parameter of
	 *        this abstraction.
	 * \param other An expression which whose free variables should not be
	 *        chosen as the new name of the parameter of this abstraction.
	 *
	 * \return A new lambda abstraction that is the result of applying
	 *         alpha conversion to this abstraction.
	 */
	virtual std::shared_ptr<const Node> alpha_convert(std::string new_var_name_hint,
							  std::shared_ptr<const Node> other
							  = nullptr) const;

	virtual std::shared_ptr<const Node>
	substitute(const std::string& var_name,
		   std::shared_ptr<const Node> expr) const override;
	
	virtual std::shared_ptr<const Node>
	raw_substitute(const std::string& orig_var_name,
		       std::shared_ptr<const Node> expr) const override;
	virtual std::shared_ptr<const Node> reduce() const override;

	virtual std::unordered_set<std::string> abstractions() const override;
	virtual std::unordered_set<std::string> variables() const override;
	virtual std::unordered_set<std::string> free_variables() const override;

	virtual bool is_function() const override;

	virtual std::string to_string() const override;
private:
	Func(std::string param_name,
	     std::shared_ptr<const Node> body);

	std::string get_new_alpha_name(const std::string& hint,
				       std::shared_ptr<const Node> other
				       = nullptr) const;
	
	const std::string m_param_name;
	const std::shared_ptr<const Node> m_function_body;

	std::weak_ptr<const Func> m_weak_this;
};

#endif // FUNC_HPP
