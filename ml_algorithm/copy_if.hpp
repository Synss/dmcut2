/// \author Mathias Laurin
/// \date 2008-07-24
#ifndef ML_ALGORITHM_COPY_IF_HPP
#define ML_ALGORITHM_COPY_IF_HPP

namespace ml_algorithm
{
	template <class InputIterator, class OutputIterator, class Predicate>
	OutputIterator copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
	{
		while (first != last) if (pred(*first)) *result++ = *first++;
		return result;
	}
}

#endif

