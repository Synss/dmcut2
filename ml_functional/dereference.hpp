#ifndef ML_FUNCTIONAL_DEREFERENCE_HPP
#define ML_FUNCTIONAL_DEREFERENCE_HPP

namespace ml_functional {
struct dereference {
	template <typename T>
	const T& operator()(const T* const ptr) const
	{ return *ptr; }

	template <typename T>
	T& operator()(T* const ptr) const
	{ return *ptr; }
};
}

#endif

