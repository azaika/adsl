#ifndef ADSL_ALGEBRA_DATA_TYPE_HPP
#define ADSL_ALGEBRA_DATA_TYPE_HPP

#include <utility>
#include <type_traits>
#include <concepts>

namespace adsl {

	template <typename M>
	concept Monoid = requires {
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
	concept Group = Monoid<G> && requires {
		// G::op(a, G::inv(a)) must equal to G::unit()
		{G::inv(G::unit())} -> std::convertible_to<typename G::value_type>;
	};

	template <typename T>
	concept GrouplyAdditionable = requires(T x, T y) {
        requires std::is_default_constructible_v<T>;
		{x + y} -> std::convertible_to<T>;
		{-x} -> std::convertible_to<T>;
	};


	class commutative_tag {};

	// M::op must be commutative
	template <typename M>
	concept CommutativeMonoid = Monoid<M> && std::is_base_of_v<commutative_tag, M>;

	// G::op must be commutative
	template <typename G>
	concept CommutativeGroup = Group<G> && std::is_base_of_v<commutative_tag, G>;


	template <typename A>
	concept LeftAction = requires {
		typename A::domain;
		typename A::space;

		{ A::act(std::declval<typename A::domain>(), std::declval<typename A::space>()) } -> std::convertible_to<typename A::space>;
	};

	// A::act(M::unit(), x) must equal to x
	// A::act(m1, A::act(m2, x)) must equal to A::act(M::op(m1, m2), x)
	template <typename A>
	concept MonoidAct = LeftAction<A> && CommutativeMonoid<typename A::domain>;
}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP