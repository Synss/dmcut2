#ifndef ML_CONTAINER_SORTED_VECTOR_HPP
#define ML_CONTAINER_SORTED_VECTOR_HPP

#include <functional> // provide std::less
#include <algorithm> // provide std::lower_bound, std::lexicographical_compare
#include <vector>

namespace ml_container
{
	/// \brief drop-in replacement to std::set, implemented on a std::vector
	///
	/// STL container adaptor (c) 2008 Mathias Laurin, distributed under the X11/MIT License
	///
	/// \par References:
	///
	/// - Matt Austern, http://lafstern.org/matt/col1.pdf
	/// - Scott Meyers, Effective STL, Item 23
	///	- Andrei Alexandrescu, http://loki-lib.sourceforge.net/html/a00645.html
	///	- http://cboard.cprogramming.com
	///
	/// \author Mathias Laurin
	/// \date 2008-07-24	
	template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T> >
	class sorted_vector
	{
		typedef typename std::vector<T, Alloc> container_type;

		std::vector<T, Alloc> container;
		Compare cmp;

		public:

		typedef typename container_type::value_type value_type;
		typedef value_type key_type;
		typedef Compare key_compare;
		typedef key_compare value_compare;
		typedef typename container_type::pointer pointer;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::difference_type difference_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::const_iterator const_iterator;
		typedef typename container_type::reverse_iterator reverse_iterator;
		typedef typename container_type::const_reverse_iterator const_reverse_iterator;

		iterator begin()
		{ return container.begin(); }

		const_iterator begin() const
		{ return container.begin(); }

		iterator end()
		{ return container.end(); }

		const_iterator end() const
		{ return container.end(); } 

		reverse_iterator rbegin()
		{ return container.rbegin(); }

		const_reverse_iterator rbegin() const
		{ return container.rbegin(); }

		reverse_iterator rend()
		{ return container.rend(); }

		const_reverse_iterator rend() const
		{ return container.rend(); }

		size_type size() const 
		{ return container.size(); }

		size_type max_size() const 
		{ return container.max_size(); }

		bool empty() const 
		{ return container.empty(); }

		key_compare key_comp() const
		{ return cmp; }

		value_compare value_comp() const
		{ return key_comp(); }

		explicit
		sorted_vector(const Compare& c = Compare())
		: container(), cmp(c)
		{ }

		template <class InputIterator>
		sorted_vector(InputIterator first, InputIterator last, const Compare& c = Compare())
		: container(first, last), cmp(c)
		{ std::sort(begin(), end(), cmp); }

		sorted_vector(const sorted_vector& other)
		: container(other.container), cmp(other.cmp)
		{ }

		sorted_vector& operator=(const sorted_vector& other)
		{
			if (this == &other) return *this;
			container = other.container;
			cmp = other.cmp;
			return *this;
		}

		void swap(sorted_vector& other)
		{ container.swap(other); }

		std::pair<iterator, bool> insert(const value_type& x)
		{
			iterator i = lower_bound(x);
			return (i == end() || cmp(x, *i)) ?
			std::make_pair(container.insert(i, x), true) : // x not found, insert
			std::make_pair(i, false); // found
		}

		iterator insert(iterator pos, const value_type& x)
		{ return insert(x)->first; }

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			container.insert(first, last);
			std::sort(container.begin(), container.end(), cmp);
		}

		void erase(iterator first, iterator last)
		{ container.erase(first, last); }

		void clear()
		{ container.clear(); }

		iterator find(const key_type& k)
		{
			iterator i = lower_bound(k);
			return (i == end() || cmp(k, *i)) ? end() : i;
		}

		const_iterator find(const key_type& k) const
		{
			const_iterator i = lower_bound(k);
			return (i == end() || cmp(k, *i)) ? end() : i;
		}

		size_type count(const key_type& k) const
		{ 
			std::pair<const_iterator, const_iterator> res = equal_range(k);
			return res.second - res.first;
		}

		iterator lower_bound(const key_type& k)
		{ return std::lower_bound(container.begin(), container.end(), k, cmp); }

		const_iterator lower_bound(const key_type& k) const
		{ return std::lower_bound(container.begin(), container.end(), k, cmp); }

		iterator upper_bound(const key_type& k)
		{ return std::upper_bound(container.begin(), container.end(), k, cmp); }

		const_iterator upper_bound(const key_type& k) const
		{ return std::upper_bound(container.begin(), container.end(), k, cmp); }

		std::pair<iterator, iterator> equal_range(const key_type& k)
		{ return std::equal_range(container.begin(), container.end(), k, cmp); }

		std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const
		{ return std::equal_range(container.begin(), container.end(), k, cmp); }
	};

	template <typename T>
	inline bool operator==(const sorted_vector<T>& lhs, const sorted_vector<T>& rhs)
	{ return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

	template <typename T>
	inline bool operator<(const sorted_vector<T>& lhs, const sorted_vector<T>& rhs)
	{ return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

} // namespace

#endif

