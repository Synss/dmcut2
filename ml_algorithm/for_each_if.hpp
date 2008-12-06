/// \author Mathias Laurin
/// \date 2008-07-24
#ifndef ML_ALGORITHM_FOR_EACH_IF_HPP
#define ML_ALGORITHM_FOR_EACH_IF_HPP

namespace ml_algorithm 
{
	template<class InputIterator, class Function, class Predicate>
	Function for_each_if (InputIterator first, InputIterator last, Function f, Predicate pred )
	{
		while (first != last) if (pred(*first)) f(*first++);
		return f;
	}
}

#endif

