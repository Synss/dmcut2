/// \author Mathias Laurin
/// \date 2008-07-24
#ifndef ML_ALGORITHM_PRINT_HPP
#define ML_ALGORITHM_PRINT_HPP

#include <ostream>
#include <iterator>
#include <algorithm>

namespace ml_algorithm {
	/*
	template <typename T, typename InputOperator>
	std::ostream& print(InputOperator first, InputOperator last, std::ostream& os)
	{
		std::copy(first, last, std::ostream_iterator<T>(os));
		return os;
	}
	*/

	template <typename T, typename InputOperator>
	std::ostream& print(InputOperator first, InputOperator last, std::ostream& os, const char* delim)
	{
		std::copy(first, last, std::ostream_iterator<T>(os, delim));
		return os;
	}

	/*
	template <typename T, typename InputOperator, typename UnaryFunction>
	std::ostream& print(InputOperator first, InputOperator last, std::ostream& os, UnaryFunction op)
	{
		std::transform(first, last, std::ostream_iterator<T>(os), op);
		return os;
	}
	*/

	template <typename T, typename InputOperator, typename UnaryFunction>
	std::ostream& print(InputOperator first, InputOperator last, std::ostream& os, const char* delim, UnaryFunction op)
	{
		std::transform(first, last, std::ostream_iterator<T>(os, delim), op);
		return os;
	}
}

#endif

