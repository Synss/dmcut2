/// \author Mathias Laurin
/// \date 2008-07-24
#ifndef ML_ALGORITHM_TRANSFORM_IF_HPP
#define ML_ALGORITHM_TRANSFORM_IF_HPP

namespace ml_algorithm
{
	template< class InputIterator, class OutputIterator, class UnaryOperator, class Predicate >
	OutputIterator transform_if (InputIterator first1, InputIterator last1,
			OutputIterator result, UnaryOperator op, Predicate pred)
	{
		while (first1 != last1) if(pred(*first1)) 
			*results++ = op(*first++);
		return results;
	}

	template< class InputIterator, class OutputIterator, class UnaryOperator, class Predicate >
	OutputIterator transform_if (InputIterator first1, InputIterator last1,
			InputIterator first2, InputIterator last2,
			OutputIterator result, BinaryOperator op, Predicate pred)
	{
		while (first1 != last1) if(pred(*first1)) 
			*results++ = op(*first++, *first2++);
		return results;
	}
}

#endif

