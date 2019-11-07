#ifndef ADSL_ALGEBRA_DATA_TYPE_HPP
#define ADSL_ALGEBRA_DATA_TYPE_HPP

#include <utility>
#include <type_traits>
#include <concepts>

namespace adsl {

	template <typename M>
	concept Magma = requires {
		typename M::value_type;

		{ M::op(std::declval<typename M::value_type>(), std::declval<typename M::value_type>()) } -> std::convertible_to<typename M::value_type>;
	};

	// G::op must be associative
	template <typename G>
	concept SemiGroup = Magma<G> && true;

	template <typename M>
	concept Monoid = SemiGroup<M> && requires {
		{M::unit()} -> std::convertible_to<typename M::value_type>;
	};

	template <typename G>
	concept Group = Monoid<G> && requires {
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


	template <typename T>
	concept MonoidallyAdditionable = requires(T x, T y) {
        requires std::is_default_constructible_v<T>;
		{x + y} -> std::convertible_to<T>;
	};

	template <typename T>
	concept GrouplyAdditionable = requires(T x, T y) {
        requires std::is_default_constructible_v<T>;
		{x + y} -> std::convertible_to<T>;
		{-x} -> std::convertible_to<T>;
	};


	template <typename A>
	concept LeftAction = requires {
		typename A::domain;
		typename A::space;

		{ A::act(std::declval<typename A::domain>())(std::declval<typename A::space>()) } -> std::convertible_to<typename A::space>;
	};

	// A::act(M::unit(), x) must equal to x
	// A::act(m1, A::act(m2, x)) must equal to A::act(M::op(m1, m2), x)
	template <typename A>
	concept MonoidAction = LeftAction<A> && CommutativeMonoid<typename A::domain>;

	// F.operator () (Domain::unit()) must equal to the unit of codomain 
	template <typename F, typename Domain>
	concept MonoidHomomorphism = Monoid<Domain> && Monoid<std::remove_cvref_t<std::invoke_result_t<F, Domain>>>;

	template <typename F, typename M>
	concept MonoidEndomorphism = MonoidHomomorphism<F> && std::same_as<M, std::remove_cvref_t<std::invoke_result_t<F, Domain>>>;

}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP