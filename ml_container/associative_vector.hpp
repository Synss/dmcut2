#ifndef ML_CONTAINER_ASSOCIATIVE_VECTOR_HPP
#define ML_CONTAINER_ASSOCIATIVE_VECTOR_HPP

#include <functional> // provide std::less
#include <algorithm> // provide std::lower_bound, std::lexicographical_compare
#include <vector>

namespace ml_container
{
	/// \brief drop-in replacement to std::map, implemented on a std::vector
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
	template < typename K, typename T, typename Compare = std::less<K> >
	class KeyCompare
		{
		public:
			KeyCompare(): cmp() { };
			
			bool operator()(const std::pair<K, T>& lhs, const std::pair<K, T>& rhs) const
			{ return do_compare(lhs.first, rhs.first); }
			
			bool operator()(const std::pair<K, T>& lhs, const K& k) const
			{ return do_compare(lhs.first, k); }
			
			bool operator()(const K& k, const std::pair<K, T>& rhs) const
			{ return do_compare(k, rhs.first); }
			
		private:
			bool do_compare(const K& k1, const K& k2) const
			{ return cmp(k1, k2); }
			
			Compare cmp;
		};
	
	
	template < typename K, typename T, typename Compare = std::less<K>, typename Alloc = std::allocator< std::pair<K, T> > >
	class associative_vector
		{
			// not std::pair<const K, T>, Meyers, Effective STL Item 23
			typedef typename std::vector< std::pair<K, T>, Alloc > container_type;
			
		public:
			// http://www.sgi.com/tech/stl/Map.html
			typedef typename container_type::value_type value_type;
			typedef typename container_type::value_type::first_type key_type;
			typedef typename container_type::value_type::second_type mapped_type;
			typedef KeyCompare<key_type, mapped_type, Compare> key_compare;
			typedef key_compare value_compare;
			typedef mapped_type* pointer;
			typedef mapped_type& reference;
			typedef const mapped_type& const_reference;
			typedef typename container_type::size_type size_type;
			typedef typename container_type::difference_type difference_type;
			typedef typename container_type::iterator iterator;
			typedef typename container_type::const_iterator const_iterator;
			typedef typename container_type::reverse_iterator reverse_iterator;
			typedef typename container_type::const_reverse_iterator const_reverse_iterator;
			
		private:
			container_type container;
			key_compare cmp;
			
		public:
			iterator begin()
			{ return container.begin(); }
			
			iterator end()
			{ return container.end(); }
			
			const_iterator begin() const
			{ return container.begin(); }
			
			const_iterator end() const
			{ return container.end(); }
			
			reverse_iterator rbegin()
			{ return container.rbegin(); }
			
			reverse_iterator rend()
			{ return container.rend(); }
			
			const_reverse_iterator rbegin() const
			{ return container.rbegin(); }
			
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
			
			associative_vector():
			container(), cmp()
			{ }
			
			template <class InputIterator>
			associative_vector(InputIterator first, InputIterator last):
			container(first, last), cmp()
			{ std::sort(begin(), end(), cmp); }
			
			associative_vector(const associative_vector& v):
			container(v.container), cmp(v.cmp)
			{ }
			
			associative_vector& operator=(const associative_vector& v)
			{
				if (this == &v) return *this;
				container = v.container;
				cmp = v.cmp;
				return *this;
			}
			
			void swap(associative_vector& v)
			{ container.swap(v); }
			
			std::pair<iterator, bool>
			insert(const value_type& x)
			{
				iterator i(lower_bound(x.first));
				return (i == end() || cmp(x, *i)) ?
				std::make_pair(container.insert(i, x), true) : // x not found, insert
				std::make_pair(i, false); // x found
			}
			
			iterator insert(iterator pos, const value_type& x)
			{
				if ( (pos == begin() || cmp(*(--pos), x)) &&
					 (pos == end()   || cmp(x, *pos)) )
				{ return container.insert(pos, x); }
				return insert(x).first; 
			}
			
			template< class InputIterator >
			void insert(InputIterator first, InputIterator last)
			{
				const size_type initial_size = size();
				container.insert(container.end(), first, last);
				std::sort(container.begin() + initial_size, container.end(), cmp);
				std::inplace_merge(container.begin(), container.begin() + initial_size, container.end(), cmp);
			}
			
			void erase(iterator pos)
			{ container.erase(pos); }
			
			size_type erase(const key_type& k)
			{
				iterator i(find(k));
				if (i == end()) return 0;
				erase(i); 
				return 1; 
			}
			
			void erase(iterator first, iterator last)
			{ container.erase(first, last); }
			
			void clear()
			{ container.clear(); }
			
			iterator find(const key_type& k)
			{
				iterator i(lower_bound(k));
				return (i == end() || cmp(k, *i)) ? end() : i;
			}
			
			const_iterator find(const key_type& k) const
			{ 
				const_iterator i(lower_bound(k));
				return (i == end() || cmp(k, *i)) ? end() : i;
			}
			
			size_type count(const key_type& k) const
			{ return find(k) != end(); }
			
			iterator lower_bound(const key_type& k)
			{ return std::lower_bound(begin(), end(), k, cmp); }
			
			const_iterator lower_bound(const key_type& k) const
			{ return std::lower_bound(begin(), end(), k, cmp); }
			
			iterator upper_bound(const key_type& k)
			{ return std::upper_bound(begin(), end(), k, cmp); }
			
			const_iterator upper_bound(const key_type& k) const
			{ return std::upper_bound(begin(), end(), k, cmp); }
			
			std::pair<iterator, iterator> equal_range(const key_type& k)
			{ return std::equal_range(begin(), end(), k, cmp); }
			
			std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const
			{ return std::equal_range(begin(), end(), k, cmp); }
			
			reference operator[](const key_type& k)
			{ return (insert(value_type(k, T())).first)->second; }
			
			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator==(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);
			
			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator!=(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);

			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator<(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);

			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator>(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);

			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator<=(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);

			template< typename K1, typename T1, typename C1, typename A1 >
			friend bool operator>=(const associative_vector<K1, T1, C1, A1>&, const associative_vector<K1, T1, C1, A1>&);
		};
	
	template< typename K, typename T, typename C, typename A >
	inline bool operator==(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container == rhs.container; }
	
	template< typename K, typename T, typename C, typename A >
	inline bool operator!=(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container != rhs.container; }
	
	template< typename K, typename T, typename C, typename A >
	inline bool operator<(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container < rhs.container; }

	template< typename K, typename T, typename C, typename A >
	inline bool operator>(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container < rhs.container; }

	template< typename K, typename T, typename C, typename A >
	inline bool operator<=(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container <= rhs.container; }

	template< typename K, typename T, typename C, typename A >
	inline bool operator>=(const associative_vector<K, T, C, A>& lhs, const associative_vector<K, T, C, A>& rhs)
	{ return lhs.container <= rhs.container; }

} // namespace

#endif

