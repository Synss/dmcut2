#ifndef LESS_PTR_HPP
#define LESS_PTR_HPP

//#include <tr1/functional>

namespace ml_functional
{
	/// Compare pointees
	/// Scott Meyers, Effective STL, Item 20
	struct less_ptr //: std::tr1::function<bool(const T&, const T&)>
	{
		template< typename PtrType >
		bool operator()(const PtrType rhs, const PtrType lhs) const
		{ return *rhs < *lhs; }
	};

} // namespace

#endif

