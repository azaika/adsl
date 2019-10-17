#ifndef ADSL_ALGEBRA_DATA_TYPE_HPP
#define ADSL_ALGEBRA_DATA_TYPE_HPP

#include <utility>
#include <type_traits>
#include <concepts>

namespace adsl {

	template <typename M>
	concept Monoid = requires(M m) {
		typename M::value_type;

		{M::unit()} -> std::convertible_to<typename M::value_type>;
		// M::op must be associative
		{M::op(M::unit(), M::unit())} -> std::convertible_to<typename M::value_type>;
	};

	template <typename T>
	concept MonoidallyAdditionable = requires(T x, T y) {
        requires std::is_default_constructible_v<T>;
		{x + y} -> std::convertible_to<T>;
	};

}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP