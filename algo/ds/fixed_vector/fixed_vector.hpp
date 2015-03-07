#ifndef __FIXED_VECTOR_HPP__
#define __FIXED_VECTOR_HPP__

#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T, typename Alloc>
struct fixed_vector_holder : public Alloc
{
	using AllocTraits = std::allocator_traits<Alloc>;

	using allocator_type = Alloc;
	using size_type = typename AllocTraits::size_type;
	using pointer = typename AllocTraits::pointer;


	fixed_vector_holder(size_type size)
		: allocator_type()
		, begin_(AllocTraits::allocate(alloc(), size))
		, end_(begin_ + size)
		, free_(begin_)
	{ }

	explicit fixed_vector_holder(size_type size,
					const allocator_type& a)
		: allocator_type(a)
		, begin_(AllocTraits::allocate(alloc(), size))
		, end_(begin_ + size)
		, free_(begin_)
	{ }

	fixed_vector_holder(fixed_vector_holder&& other) noexcept
		: allocator_type(std::move(static_cast<allocator_type&>(other)))
		, begin_(other.begin_)
		, end_(other.end_)
		, free_(other.free_)
	{
		other.begin_ = nullptr;
		other.end_ = nullptr;
		other.free_ = nullptr;
	}

	~fixed_vector_holder()
	{
		clear();
		AllocTraits::deallocate(alloc(), begin_, end_ - begin_);
	}

	fixed_vector_holder(const fixed_vector_holder&) = delete;
	fixed_vector_holder& operator=(const fixed_vector_holder&) = delete;

	allocator_type& alloc() noexcept
	{ return *static_cast<allocator_type*>(this); }

	const allocator_type& alloc() const noexcept
	{ return *static_cast<const allocator_type *>(this); }

	size_type capacity() const noexcept
	{ return static_cast<size_type>(end_ - begin_); }

	size_type size() const noexcept
	{ return static_cast<size_type>(free_ - begin_); }

	void clear() noexcept
	{
		for (pointer it = begin_; it != free_; ++it)
			AllocTraits::destroy(alloc(), it);
		free_ = begin_;
	}

	void swap(fixed_vector_holder& other) noexcept
	{
		std::swap(begin_, other.begin_);
		std::swap(end_, other.end_);
		std::swap(free_, other.free_);
	}

	pointer begin_;
	pointer end_;
	pointer free_;
};


template <typename T, typename Alloc = std::allocator<T>>
class fixed_vector
{
	using AllocTraits = std::allocator_traits<Alloc>;
	using AllocHolder = fixed_vector_holder<T, Alloc>;
public:
	using allocator_type = Alloc;
	using size_type = typename AllocTraits::size_type;
	using difference_type = typename AllocTraits::difference_type;

	using value_type = T;
	using pointer = typename AllocTraits::pointer;
	using const_pointer = typename AllocTraits::const_pointer;
	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;


	explicit fixed_vector(size_type size,
				const allocator_type& alloc = allocator_type())
		: holder_(size, alloc)
	{ }

	fixed_vector(const fixed_vector& other)
		: holder_(other.size(), AllocTraits::select_on_container_copy_construction(other.get_allocator()))
	{ std::copy(other.begin(), other.end(), std::back_inserter(*this)); }

	fixed_vector(const fixed_vector& other,
				const allocator_type& alloc = allocator_type())
		: holder_(other.size(), alloc)
	{ std::copy(other.begin(), other.end(), std::back_inserter(*this)); }

	template <typename OtherAlloc>
	fixed_vector(const fixed_vector<value_type, OtherAlloc>& other,
				const allocator_type& alloc = allocator_type())
		: holder_(other.size(), alloc)
	{ std::copy(other.begin(), other.end(), std::back_inserter(*this)); }

	fixed_vector(fixed_vector&& other) noexcept
		: holder_(std::move(other.holder_))
	{ }

	fixed_vector(std::initializer_list<value_type> il,
				const allocator_type& alloc = allocator_type())
		: holder_(il.size(), alloc)
	{ std::copy(il.begin(), il.end(), std::back_inserter(*this)); }

	~fixed_vector()
	{ }

	fixed_vector& operator=(fixed_vector other)
	{
		holder_.swap(other.holder_);
		return *this;
	}

	fixed_vector& operator=(fixed_vector&& other) noexcept
	{
		AllocHolder temp(std::move(holder_));
		holder_.swap(other.holder_);
		return *this;
	}

	fixed_vector& operator=(std::initializer_list<value_type> il)
	{
		fixed_vector temp(il, get_allocator());
		holder_.swap(temp.holder_);
		return *this;
	}

	void push_back(const_reference x)
	{
		if (size() == capacity())
			throw std::length_error("fixed_vector is full");
		AllocTraits::construct(get_allocator(), holder_.free_, x);
		++holder_.free_;
	}

	void push_back(value_type &&x)
	{
		if (size() == capacity())
			throw std::length_error("fixed_vector is full");
		AllocTraits::construct(get_allocator(), holder_.free_,
					std::move(x));
		++holder_.free_;
	}

	template <typename ... Args>
	void emplace_back(Args&&... args)
	{
		if (size() == capacity())
			throw std::length_error("fixed_vector is full");
		AllocTraits::construct(get_allocator(), holder_.free_,
					std::forward(args)...);
		++holder_.free_;
	}

	void clear() noexcept
	{ holder_.clear(); }

	void pop_back()
	{
		if (empty())
			throw std::length_error("fixed_vector is empty");
		AllocTraits::destroy(get_allocator(), holder_.free_ - 1);
		--holder_.free_;
	}

	void swap(fixed_vector &other) noexcept
	{ holder_.swap(other.swap_); }

	size_type size() const noexcept
	{ return holder_.size(); }

	bool empty() const noexcept
	{ return !size(); };

	size_type capacity() const noexcept
	{ return holder_.capacity(); }

	allocator_type& get_allocator() noexcept
	{ return holder_.alloc(); }

	const allocator_type& get_allocator() const noexcept
	{ return holder_.alloc(); }

	iterator begin() noexcept
	{ return iterator(holder_.begin_); }

	const_iterator cbegin() const noexcept
	{ return const_iterator(holder_.begin_); }

	iterator end() noexcept
	{ return iterator(holder_.free_); }

	const_iterator cend() noexcept
	{ return const_iterator(holder_.free_); }

	reverse_iterator rbegin() noexcept
	{ return reverse_iterator(end()); }

	const_reverse_iterator rcbegin() const noexcept
	{ return const_reverse_iterator(cend()); }

	reverse_iterator rend() noexcept
	{ return reverse_iterator(begin()); }

	const_reverse_iterator rcend() const noexcept
	{ return const_reverse_iterator(cbegin()); }

	reference at(size_type pos)
	{
		if (pos >= size())
			throw std::out_of_range("item index out of range");
		return operator[](pos);
	}

	const_reference at(size_type pos) const
	{
		if (pos >= size())
			throw std::out_of_range("item index out of range");
		return operator[](pos);
	}

	reference operator[](size_type pos) noexcept
	{ return *(begin() + pos); }

	const_reference operator[](size_type pos) const noexcept
	{ return *(cbegin() + pos); }

	reference front() noexcept
	{ return operator[](0); }

	const_reference front() const noexcept
	{ return operator[](0); }

	reference back() noexcept
	{ return operator[](size() - 1); }

	const_reference back() const noexcept
	{ return operator[](size() - 1); }

	pointer data() noexcept
	{ return holder_.begin_; };

	const_pointer data() const noexcept
	{ return holder_.begin_; };

private:
	AllocHolder holder_;
};

#endif /*__FIXED_VECTOR_HPP__*/
