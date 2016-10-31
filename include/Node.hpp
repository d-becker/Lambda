#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_set>

class Func;

/**
 * An interface that represents a lambda expression. Concrete classes that
 * implement this interface are \c Subs, \c Func and \c Appl. Objects of these
 * types are immutable and reference-counted - they should only be accessed
 * through \c std::shared_ptr's. This way the expressions can be manipulated
 * in a way similar to functional programming languages.
 *
 * To make this possible, the constructors of the classes are private and new
 * objects can be created using the static \c create method of the approproate
 * class, which returns an std::shared_ptr. Every object has a std::weak_ptr that
 * points to it so that it can always generate a valid std::shared_ptr to itself.
 */
class Node {
public:
	virtual ~Node() {}

	virtual std::shared_ptr<const Node>
	raw_substitute(const std::string& orig_var_name,
		       std::shared_ptr<const Node> expr) const = 0;

	/**
	 * Substitutes an expression for a variable in this expression in a
	 * capture-avoiding manner, possibly using alpha conversions.
	 *
	 * \param var_name The name of the variable to substitute \a expr for.
	 * \param expr The lambda expression to substitute for \a var_name.
	 *
	 * \return A new lambda expression node that is the result of
	 *         substituting \a expr for \a var_name in this expression.
	 */
	virtual std::shared_ptr<const Node>
	substitute(const std::string& var_name,
		   std::shared_ptr<const Node> expr) const = 0;

	/**
	 * Reduces an expression using the 'normal order' reduction strategy.
	 * For more information, see
	 * https://en.wikipedia.org/wiki/Lambda_calculus#Reduction_strategies.
	 *
	 * The maximal number of steps can be controlled with the parameter
	 * \a count. Every call to this function (not only effective beta
	 * reductions) decrements \a count by one. When \a count becomes zero or
	 * negative, no further reductions are performed and the expression is
	 * returned as computed so far. Checking whether a reduction was complete
	 * can be done by checking the variable passed to this function after the
	 * call: a negative value means the reduction was not complete.
	 *
	 * \param count The maximal number of steps that will be performed
	 *        during the reduction.
	 *
	 * \return A new lambda expression node the result of reducing
	 *         this expression.
	 */
	virtual std::shared_ptr<const Node> reduce(long long& count) const = 0;

	/**
	 * Returns an \c std::unordered_set with the parameter names of the
	 * lambda abstractions (functions) contained within this expression.
	 *
	 * \return An \c std::unordered_set with the parameter names of the
	 *         lambda abstractions (functions) contained within
	 *         this expression.
	 */
	virtual std::unordered_set<std::string> abstractions() const = 0;

	/**
	 * Returns an \c std::unordered_set with the names of the variables
	 * contained within this expression.
	 *
	 * \return An \c std::unordered_set with the names of the variables
	 *         contained within this expression.
	 */
	virtual std::unordered_set<std::string> variables() const = 0;

	
	/**
	 * Returns an \c std::unordered_set with the names of the free variables
	 * contained within this expression.
	 *
	 * \return An \c std::unordered_set with the names of the free variables
	 *         contained within this expression.
	 */
	virtual std::unordered_set<std::string> free_variables() const = 0;

	/**
	 * If this is a lambda abstraction (function), returns a smart pointer
	 * to this lambda expression node as a \c Func;
	 * otherwise \c nullptr is returned.
	 *
	 * \return A smart pointer to this lambda expression node as a \c Func
	 *         if this is a function or \c nullptr otherwise.
	 */
	virtual std::shared_ptr<const Func> as_function() const {
		return nullptr;
	}

	/**
	 * Returns the string representation of this lambda expression node.
	 *
	 * \return The string representation of this lambda expression node.
	 */
	virtual std::string to_string() const = 0;
};

#endif // NODE_HPP
