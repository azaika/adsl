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

	// M::op must be commutative
	template <typename M>
	concept CommutativeMonoid = Monoid<M> && true;


	template <typename G>
	concept Group = requires(G g) {
		typename M::value_type;

		{G::unit()} -> std::convertible_to<typename G::value_type>;
		// G::op(a, G::inv(a)) must equal to G::unit()
		{G::inv(G::unit())} -> std::convertible_to<typename G::value_type>;
		// G::op must be associative
		{G::op(G::unit(), G::unit())} -> std::convertible_to<typename G::value_type>;
	};

	// G::op must be commutative
	template <typename G>
	concept CommutativeGroup = Group<G> && true;
}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP