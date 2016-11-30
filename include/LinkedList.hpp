#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>

#include <iterator>
#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class LinkedList {
private:
	struct Node_base {
		Node_base(Node_base *p_previous,
			  Node_base *p_next)
			: previous(p_previous),
			  next(p_next)
		{
		}

		virtual ~Node_base() {}
		
		Node_base *previous;
		Node_base *next;
	};
	
	template <typename U>
	struct Node : public Node_base {
		Node(const U& p_elem,
		     Node_base *p_previous,
		     Node_base *p_next)
			: Node_base(p_previous, p_next),
			  elem(p_elem)
			{
			}
		
		Node(U&& p_elem,
		     Node_base *p_previous,
		     Node_base *p_next)
			: Node_base(p_previous, p_next),
			  elem(p_elem)
			{
			}
		
	        U elem;
	};
	
public:
	template <typename U>
	class Iterator : public std::iterator<std::bidirectional_iterator_tag, U> {
	public:
		Iterator(Node_base *node)
			: m_node(node)
			{
			}

		Iterator(const Iterator& other)
			: m_node(other.m_node)
			{
			}
		
		Iterator& operator++() {
			m_node = m_node->next;
			return *this;
		}

		Iterator& operator++(int) {
			Iterator res(*this);
			m_node = m_node->next;
			
			return res;
		}

		Iterator& operator--() {
			m_node = m_node->previous;			
			return *this;
		}

		Iterator& operator--(int) {
			Iterator res(*this);
		        m_node = m_node->previous;

			return res;
		}

		typename LinkedList<U>::reference operator*() {
			return static_cast<Node<U>*>(m_node)->elem;
		}

		typename LinkedList<U>::pointer operator->() {
			return &static_cast<Node<U>*>(m_node)->elem;
		}

		bool operator==(const Iterator& other) {
			return this->m_node == other.m_node;
		}

		bool operator!=(const Iterator& other) {
			return !(this->operator==(other));
		}

		//private:
		Node_base *m_node;
	};

	template <typename U>
	class ConstIterator : public std::iterator<std::bidirectional_iterator_tag, U> {
	public:
		ConstIterator(Node_base *node)
			: m_node(node)
			{
			}

		ConstIterator(const ConstIterator& other)
			: m_node(other.m_node)
			{
			}

		ConstIterator(const Iterator<U>& other)
			: m_node(other.m_node)
			{
			}
		
		ConstIterator& operator++() {
			m_node = m_node->next;
			return *this;
		}

		ConstIterator& operator++(int) {
			ConstIterator res(*this);
			m_node = m_node->next;
			
			return res;
		}

		ConstIterator& operator--() {
			m_node = m_node->previous;			
			return *this;
		}

		ConstIterator& operator--(int) {
			ConstIterator res(*this);
		        m_node = m_node->previous;

			return res;
		}

		typename LinkedList<U>::const_reference operator*() {
			return static_cast<Node<U>*>(m_node)->elem;
		}

		typename LinkedList<U>::const_pointer operator->() {
			return &static_cast<Node<U>*>(m_node)->elem;
		}

		bool operator==(const ConstIterator& other) {
			return this->m_node == other.m_node;
		}

		bool operator!=(const ConstIterator& other) {
			return !(this->operator==(other));
		}

		//private:
		Node_base *m_node;
	};	

public:
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<Allocator>::pointer pointer;
	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
	typedef Iterator<T> iterator;
	typedef ConstIterator<T> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	explicit LinkedList(const Allocator& alloc = Allocator())
		: m_allocator(alloc),
		  m_size_node_allocator(),
		  m_node(create_m_node())
	{
	}

	LinkedList(const LinkedList& other)
		: LinkedList(other.m_allocator)
	{
		insert(begin(), other.begin(), other.end());
	}

	LinkedList(LinkedList&& other)
		: m_allocator(other.m_allocator),
		  m_size_node_allocator(other.m_size_node_allocator),
		  m_node(other.m_node)
	{
		other.m_node = nullptr;
	}

	LinkedList(std::initializer_list<T> ilist)
		: LinkedList()
	{
			insert(begin(), ilist);
	}

	virtual ~LinkedList() {
		clear();

		if (m_node != nullptr) { // Otherwise the list has been moved.
			dispose_of_m_node();
		}
	}

	LinkedList& operator=(LinkedList other) {
	        this->swap(other);
		return *this;
	}

	LinkedList& operator=(std::initializer_list<T> ilist) {
		LinkedList temp(ilist);
		this->swap(temp);

		return *this;
	}

	allocator_type get_allocator() const {
		return allocator_type(m_allocator);
	}

	// Element access
	reference front() {
		return *begin();
	}
		
	const_reference front() const {
		return *cbegin();
	}

	reference back() {
	        iterator tmp = end();
		--tmp;
		return *tmp;
	}
		
	const_reference back() const {
		const_iterator tmp = end();
		--tmp;
		return *tmp;
	}

	// Iterators
	iterator begin() {
		return iterator(m_node->next);
	}
		
        const_iterator begin() const {
		return const_iterator(m_node->next);
	}
		
	const_iterator cbegin() const {
		return begin();
	}

	iterator end() {
		return iterator(m_node);
	}
		
	const_iterator end() const {
		return const_iterator(m_node);
	}
		
	const_iterator cend() const {
		return end();
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
		
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
		
	const_reverse_iterator crbegin() const {
		return rbegin();
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
		
	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}
		
	const_reverse_iterator crend() const {
		return rend();
	}

	// Capacity
	bool empty() const {
		return size() == 0;
	}
	
	size_type size() const {
		return m_node->elem;
	}

	// Modifiers
	void clear() {
	        Node_base *current = m_node->next;

		while (current != m_node) {
			Node_base *next = current->next;
			
			Node<T> *current_cast = static_cast<Node<T>*>(current);
			destroy(current_cast);
			deallocate(current_cast, 1);

			current = next;
		}


		m_node->previous = m_node;
		m_node->next = m_node;
		set_size(0);
	}

	iterator insert(const_iterator pos, const T& value) {
	        Node_base *node_at_pos = pos.m_node;
		Node_base *node_before_pos = node_at_pos->previous;

		Node<T> *new_node = allocate(1);
		construct(new_node, value, node_before_pos, node_at_pos);
		
		node_before_pos->next = new_node;
		node_at_pos->previous = new_node;

		increase_size();
		
		return iterator(new_node);
	}

	iterator insert(const_iterator pos, T&& value) {
		Node_base *node_at_pos = pos.m_node;
		Node_base *node_before_pos = node_at_pos->previous;

		Node<T> *new_node = allocate(1);
		construct(new_node, std::move(value),
			  node_before_pos, node_at_pos);
		
		node_before_pos->next = new_node;
		node_at_pos->previous = new_node;

		increase_size();
		
		return iterator(new_node);
	}

	iterator insert(const_iterator pos, size_type count, const T& value) {
		Node_base *node_at_pos = pos.m_node;
		Node_base *node_before_pos = node_at_pos->previous;

		Node_base *before_current = node_before_pos;
		for (size_type i = 0; i < count; ++i) {
			Node<T> *new_node = allocate(1);
			construct(new_node, value, before_current, nullptr);

			before_current->next = new_node;
			before_current = new_node;
		}

		before_current->next = node_at_pos;
		node_at_pos->previous = before_current;

		increase_size(count);

		return node_before_pos->next;
	}

	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		Node_base *node_at_pos = pos.m_node;
		Node_base *node_before_pos = node_at_pos->previous;

		Node_base *before_current = node_before_pos;
		for (auto it = first; it != last; ++it) {
			Node<T> *new_node = allocate(1);
			construct(new_node, *it, before_current, nullptr);

			before_current->next = new_node;
			before_current = new_node;

			increase_size();
		}

		before_current->next = node_at_pos;
		node_at_pos->previous = before_current;


		return node_before_pos->next;
	}
	
	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		return insert(pos, ilist.begin(), ilist.end());
	}

	iterator erase(const_iterator pos) {
		Node_base *node_at_pos = pos.m_node;
		Node_base *node_before_pos = node_at_pos->previous;
		Node_base *node_after_pos = node_at_pos->next;

		node_before_pos->next = node_after_pos;
		node_after_pos->previous = node_before_pos;

		Node<T> *node_at_pos_cast = static_cast<Node<T>*>(node_at_pos);
		destroy(node_at_pos_cast);
		deallocate(node_at_pos_cast, 1);

		decrease_size();

		return node_after_pos;
	}

	iterator erase(const_iterator first, const_iterator last) {
		Node_base *node_at_first = first.m_node;
		Node_base *node_before_first = node_at_first->previous;
		Node_base *node_at_last = last.m_node;

		node_before_first->next = node_at_last;
		node_at_last->previous = node_before_first;

		for (auto it = first; it != last; ++it) {
			Node<T> *current_node = static_cast<Node<T>*>(it.m_node);
			destroy(current_node);
			deallocate(current_node, 1);

			decrease_size();
		}

		return iterator(last.m_node);
	}

	void push_back( const T& value ) {
		insert(end(), value);
	}
	
	void push_back( T&& value ) {
		insert(end(), std::move(value));
	}

	void pop_back() {
		iterator tmp = end();
		--tmp;
		erase(tmp);
	}

	void push_front(const T& value) {
		insert(begin(), value);
	}

	void push_front(T&& value) {
		insert(begin(), std::move(value));
	}

	void pop_front() {
		erase(begin());
	}

	void swap(LinkedList& other) {
		using std::swap;
	        swap(this->m_node, other.m_node);

		if (std::allocator_traits<node_allocator_type>::
		    propagate_on_container_swap::value) {
			swap(m_allocator, other.m_allocator);
		}

		if (std::allocator_traits<size_node_allocator_type>::
		    propagate_on_container_swap::value) {
			swap(m_size_node_allocator,
			     other.m_size_node_allocator);
		}
	}

	/**
	 * Swaps the internal nodes pointed to by the iterators.
	 * The two iterators may or may not be of the same \c LinkedList.
	 */
	void swap_elements(const_iterator it1, const_iterator it2) {
		Node_base *it1_node = it1.m_node;
		Node_base *it2_node = it2.m_node;

		if (it1_node->next == it2_node) {
			swap_adjacent_elements(it1_node, it2_node);
		} else if (it2_node->next == it1_node) {
			swap_adjacent_elements(it2_node, it1_node);
		} else {
			swap_non_adjacent_elements(it1_node, it2_node);
		}
	}

private:
	Node<size_type>* create_m_node() {
		Node<size_type> *res
			= std::allocator_traits<size_node_allocator_type>
			::allocate(m_size_node_allocator, 1);

		std::allocator_traits<size_node_allocator_type>
			::construct(m_size_node_allocator, res, 0, res, res);

		return res;
	}

	void dispose_of_m_node() {
		std::allocator_traits<size_node_allocator_type>
			::destroy(m_size_node_allocator, m_node);

		std::allocator_traits<size_node_allocator_type>
			::deallocate(m_size_node_allocator, m_node, 1);
	}
	
	Node<T>* allocate(size_type n) {
		return std::allocator_traits<node_allocator_type>
			::allocate(m_allocator, n);
	}

	void deallocate(Node<T>* address, size_type n) {
		std::allocator_traits<node_allocator_type>
			::deallocate(m_allocator, address, n);
	}

	template <typename... U>
	void construct(Node<T>* address, U&&... args) {
		std::allocator_traits<node_allocator_type>
		    ::construct(m_allocator, address, std::forward<U>(args)...);
	}

	void destroy(Node<T>* address) {
		std::allocator_traits<node_allocator_type>
			::destroy(m_allocator, address);
	}

	void set_size(size_type size) {
		m_node->elem = size;
	}

	void increase_size(size_type n = 1) {
		m_node->elem += n;
	}

	void decrease_size(size_type n = 1) {
		m_node->elem -= n;
	}

	void swap_adjacent_elements(Node_base *first, Node_base *second) {
		Node_base *before = first->previous;
		Node_base *after = second->next;

		before->next = second;
		second->previous = before;

		second->next = first;
		first->previous = second;

		first->next = after;
		after->previous = first;
	}

	void swap_non_adjacent_elements(Node_base *it1_node,
					Node_base *it2_node) {
		Node_base *it1_before = it1_node->previous;
		Node_base *it1_after = it1_node->next;
		
		Node_base *it2_before = it2_node->previous;
		Node_base *it2_after = it2_node->next;

		it1_node->previous = it2_before;
		it1_node->next = it2_after;
		it2_before->next = it1_node;
		it2_after->previous = it1_node;

		it2_node->previous = it1_before;
		it2_node->next = it1_after;
		it1_before->next = it2_node;
		it1_after->previous = it2_node;
	}


	typedef typename std::allocator_traits<Allocator>
	::template rebind_alloc<Node<T>> node_allocator_type;

	typedef typename std::allocator_traits<Allocator>
	::template rebind_alloc<Node<size_type>> size_node_allocator_type;
	
	node_allocator_type m_allocator;
	size_node_allocator_type m_size_node_allocator;
	
        Node<size_type> *m_node;
};

template<class T, class Alloc>
void swap(LinkedList<T,Alloc>& lhs, LinkedList<T,Alloc>& rhs) {
	lhs.swap(rhs);
}

#endif // LINKED_LIST_HPP
