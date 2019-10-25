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

	
	template <typename G>
	concept Group = Monoid<G> && requires(G g) {
		// G::op(a, G::inv(a)) must equal to G::unit()
		{G::inv(G::unit())} -> std::convertible_to<typename G::value_type>;
	};

	class commutative_tag {};

	// M::op must be commutative
	template <typename M>
	concept CommutativeMonoid = Monoid<M> && std::is_base_of_v<commutative_tag, M>;

	// G::op must be commutative
	template <typename G>
	concept CommutativeGroup = Group<G> && std::is_base_of_v<commutative_tag, G>;
}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP