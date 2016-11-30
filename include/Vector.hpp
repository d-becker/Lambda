#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

#include <algorithm>
#include <iterator>
#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class Vector {
public:
public:
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<Allocator>::pointer pointer;
	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	Vector(const Allocator& alloc = Allocator())
		: m_allocator(alloc),
		  m_growth_rate(2),
		  m_size(0),
		  m_capacity(0),
		  m_data(nullptr)
	{
		
	}

	template< class InputIt >
	Vector(InputIt first, InputIt last,
	       const Allocator& alloc = Allocator())
		: Vector(alloc)
		  
	{
		for (InputIt it = first; it != last; ++it) {
			push_back(*it);
		}
	}

	Vector(std::initializer_list<T> ilist,
	       const Allocator& alloc = Allocator())
		: Vector(ilist.begin(), ilist.end(), alloc)
	{
		
	}

	Vector(const Vector& other)
		: m_allocator(other.m_allocator),
		  m_growth_rate(other.m_growth_rate),
		  m_size(other.m_size),
		  m_capacity(other.m_size),
		  m_data(allocate(m_size))
	{
		for (size_type i = 0; i < size(); ++i) {
			construct_at(i, T(other[i]));
		}
	}

	Vector(Vector&& other)
		: m_allocator(other.m_allocator),
		  m_growth_rate(other.m_growth_rate),
		  m_size(other.m_size),
		  m_capacity(other.m_capacity),
		  m_data(other.m_data)
	{
		// Leave the other in a valid, destructable state.
		other.m_size = 0;
		other.m_capacity = 0;
		other.m_data = nullptr;
	}

	virtual ~Vector() {
	        destroy_storage();
		if (m_data) // If m_data == nullptr, we have been moved from.
			deallocate_storage();
	}

	Vector& operator=(Vector other) {
		this->swap(other);
		return *this;
	}
	
	Vector& operator=(std::initializer_list<T> ilist) {
		Vector<T> temp(ilist);
		this->swap(temp);

		return *this;
	}

	allocator_type get_allocator() const {
		return m_allocator;
	}

	// Element access
	reference at(size_type pos) {
		if (pos >= size())
			throw std::out_of_range("Array index out of range.");
		return m_data[pos];
	}
	
	const_reference at(size_type pos) const {
		if (pos >= size())
			throw std::out_of_range("Array index out of range.");
		return m_data[pos];
	}
	
	reference operator[](size_type pos) {
		return m_data[pos];
	}
	
	const_reference operator[](size_type pos) const {
		return m_data[pos];
	}

	reference front() {
		return m_data[0];
	}
	
	const_reference front() const {
		return m_data[0];
	}

	reference back() {
		return m_data[size() - 1];
	}
	
	const_reference back() const {
		return m_data[size() -1];
	}

	// Iterators
        iterator begin() {
		return m_data;
	}
		
	const_iterator begin() const {
		return m_data;
	}
		
	const_iterator cbegin() const {
		return begin();
	}

	iterator end() {
		return m_data + size();
	}
		
	const_iterator end() const {
		return m_data + size();
	}
		
	const_iterator cend() const {
		return end();
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
		
	const_reverse_iterator rbegin() const {
	        return reverse_iterator(end());
	}
		
	const_reverse_iterator crbegin() const {
		return rbegin();
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
		
	const_reverse_iterator rend() const {
	        return reverse_iterator(begin());
	}
		
	const_reverse_iterator crend() const {
		return rend();
	}

	// Capacity
	bool empty() const {
		return m_size == 0;
	}
	
	size_type size() const {
		return m_size;
	}
	
	size_type capacity() const {
		return m_capacity;
	}

	bool contains(const value_type& elem) const {
		return std::find(cbegin(), cend(), elem) != cend();
	}
	
	// Modifiers
	void clear() {
		destroy_storage();
		m_size = 0;
	}

	iterator insert(iterator pos, const T& value) {
		return insert(pos, 1, value);
	}

	iterator insert(iterator pos, size_type count, const T& value) {
		if (size() + count > capacity()) {
			return insert_with_reallocation(pos, count, value);
		} else {
			return insert_without_reallocation(pos, count, value);	
		}
		
	}
	
	iterator erase(const_iterator pos) {
		size_type index = pos - begin();
		for (size_type i = index; i < size() - 1; ++i) {
			m_data[i] = std::move(m_data[i + 1]);
		}
		
		destroy_at(--m_size);
		return begin() + index;
	}

	iterator erase(iterator first, iterator last) {
		size_type num_to_erase = last - first;
		while(last < end()) {
			*first = std::move(*last);
			
			++first;
			++last;
		}

		for (size_type i = size() - num_to_erase; i < size(); ++i) {
			destroy_at(i);
		}

		m_size -= num_to_erase;
		return first;
	}
	
	void push_back(const T& elem) {
		if (size() >= capacity()) {
			size_t new_capacity = std::max(size() + 1,
						       capacity() * m_growth_rate);
			T *new_data = allocate(new_capacity);
			for (size_type i = 0; i < size(); ++i) {
				construct_at_address(new_data + i,
						     std::move(m_data[i]));
			}

			destroy_storage();
			deallocate_storage();
			
			m_capacity = new_capacity;
			m_data = new_data;
		}

		construct_at(size(), T(elem));
		++m_size;
	}

	void pop_back() {
		destroy_at(--m_size);
	}

	void swap(Vector& other) {
		using std::swap;
		
		if (std::allocator_traits<allocator_type>::
		    propagate_on_container_swap::value) {
			swap(m_allocator, other.m_allocator);
		}

		swap(m_growth_rate, other.m_growth_rate);
		swap(m_size, other.m_size);
		swap(m_capacity, other.m_capacity);
		swap(m_data, other.m_data);
	}

private:
	pointer allocate(size_type n) {
		return std::allocator_traits<allocator_type>
			::allocate(m_allocator, n);
	}

	void deallocate(pointer p, size_type n) {
	        std::allocator_traits<allocator_type>::deallocate(
			m_allocator, p, n);
	}

	void deallocate_storage() {
		deallocate(m_data, capacity());
	}

	void destroy_storage() {
		for (size_type i = 0; i < size(); ++i) {
			destroy_at(i);
		}
	}

	void destroy_at(size_type index) {
		std::allocator_traits<allocator_type>::destroy(
				m_allocator, m_data + index);
	}

	void construct_at(size_type index, T&& elem) {
		std::allocator_traits<allocator_type>
			::construct(m_allocator,
				    m_data + index,
				    std::forward<T>(elem));
	}

	void construct_at_address(pointer address, T&& elem) {
		std::allocator_traits<allocator_type>
			::construct(m_allocator,
				    address,
				    std::forward<T>(elem));
	}

	iterator insert_with_reallocation(iterator pos, size_type count, const T& value) {
		size_type index = pos - cbegin();
		size_t new_capacity = std::max(size() + count,
					       capacity() * m_growth_rate);
		m_capacity = new_capacity;
		T *new_data = allocate(new_capacity);
		// Moving the elements before pos.
		for (size_type i = 0; i < index; ++i) {
			construct_at_address(new_data + i, std::move(m_data[i]));
		}

		// Inserting the new elements.
		for (size_type i = index; i < index + count; ++i) {
			construct_at_address(new_data  + i, T(value));
		}

		// Moving the elements originally after pos.
		for (size_type i = index; i < size(); ++i) {
			construct_at_address(new_data + i + count, std::move(m_data[i]));
		}

		// Destroying the old storage.
		destroy_storage();
		deallocate_storage();
		m_data = new_data;

		m_size += count;
		return pos;
	}

	iterator insert_without_reallocation(iterator pos, size_type count, const T& value) {
		// Move-construct elements at the end
		// where the memory is not initialized.
		for (size_type i = 0; i < count; ++i) {
			construct_at(size() + i,
				     std::move(m_data[size()
						      - count + i]));
		}

		// Move the elements from pos to size() - count
		std::move_backward(pos, end() - count, end());

		// Moving in the new elements
		for (iterator it = pos; it < pos + count; ++it) {
			*it = T(value);
		}

		m_size += count;
		return pos;
	}

	allocator_type m_allocator;
        std::size_t m_growth_rate;
	std::size_t m_size;
	std::size_t m_capacity;

	T *m_data;
};

#endif // VECTOR_HPP
