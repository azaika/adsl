#ifndef ADSL_ALGEBRA_TYPE_UTIL_HPP
#define ADSL_ALGEBRA_TYPE_UTIL_HPP

#include <utility>
#include <type_traits>
#include <concepts>
#include <optional>
#include "data_type.hpp"

namespace adsl {

    namespace impl {
        template <typename ValueType, ValueType identity, auto func, bool is_commutative>
        struct make_monoid {
            using value_type = ValueType;
        
            static constexpr value_type unit() noexcept {
                return identity;
            }
        
            static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(func(x, y))){
                return func(x, y);
            }
        };

        template <typename ValueType, ValueType identity, auto func>
        struct make_monoid<ValueType, identity, func, true> : commutative_tag {
            using value_type = ValueType;
        
            static constexpr value_type unit() noexcept {
                return identity;
            }
        
            static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(func(x, y))){
                return func(x, y);
            }
        };

        template <MonoidallyAdditionable T, bool is_commutative>
	    struct default_monoid {
		    using value_type = T;

		    static constexpr value_type unit() noexcept(noexcept(value_type{})) {
			    return value_type{};
		    }

		    static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(x + y)) {
			    return x + y;
		    }
	    };

        template <MonoidallyAdditionable T>
	    struct default_monoid<T, true> : commutative_tag {
		    using value_type = T;

		    static constexpr value_type unit() noexcept(noexcept(value_type{})) {
			    return value_type{};
		    }

		    static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(x + y)) {
			    return x + y;
		    }
	    };

        
        template <typename ValueType, ValueType identity, auto func, auto inv_func, bool is_commutative>
        struct make_group {
            using value_type = ValueType;
        
            static constexpr value_type unit() noexcept {
                return identity;
            }
        
            static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(func(x, y))){
                return func(x, y);
            }

            static constexpr value_type inv(const value_type& v) noexcept(noexcept(inv_func(v))) {
                return inv_func(v);
            }
        };

        template <typename ValueType, ValueType identity, auto func, auto inv_func>
        struct make_group<ValueType, identity, func, inv_func, true> : commutative_tag {
            using value_type = ValueType;
        
            static constexpr value_type unit() noexcept {
                return identity;
            }
        
            static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(func(x, y))){
                return func(x, y);
            }

            static constexpr value_type inv(const value_type& v) noexcept(noexcept(inv_func(v))) {
                return inv_func(v);
            }
        };

        template <GrouplyAdditionable T, bool is_commutative>
	    struct default_group {
		    using value_type = T;

		    static constexpr value_type unit() noexcept(noexcept(value_type{})) {
			    return value_type{};
		    }

		    static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(x + y)) {
			    return x + y;
		    }

            static constexpr value_type inv(const value_type& v) noexcept(noexcept(-v)) {
                return -v;
            }
	    };

        template <GrouplyAdditionable T>
	    struct default_group<T, true> : commutative_tag {
		    using value_type = T;

		    static constexpr value_type unit() noexcept(noexcept(value_type{})) {
			    return value_type{};
		    }

		    static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(x + y)) {
			    return x + y;
		    }

            static constexpr value_type inv(const value_type& v) noexcept(noexcept(-v)) {
                return -v;
            }
	    };

    }

    template <SemiGroup G>
    struct to_monoid {
        using value_type = std::optional<typename G::value_type>;

        static value_type unit() noexcept {
            return std::nullopt;
        }

        static value_type op(const value_type& x, const value_type& y) noexcept(noexcept(G::op(*x, *y))) {
            if (x == std::nullopt)
                return y;
            if (y == std::nullopt)
                return x;
            
            return G::op(*x, *y);
        }
    };

    template <typename ValueType, ValueType identity, auto func, bool is_commutative = false>
    using make_monoid = impl::make_monoid<ValueType, identity, func, is_commutative>;

    template <MonoidallyAdditionable T, bool is_commutative = std::is_arithmetic_v<T>>
    using default_monoid = impl::default_monoid<T, is_commutative>;


    template <typename ValueType, ValueType identity, auto func, auto inv_func, bool is_commutative = false>
    using make_group = impl::make_group<ValueType, identity, func, inv_func, is_commutative>;

    template <GrouplyAdditionable T, bool is_commutative = std::is_signed_v<T>>
    using default_group = impl::default_group<T, is_commutative>;


    template <typename D, typename S, auto func>
    requires std::invocable<decltype(func), typename D::value_type, typename S::value_type>
    struct make_action {
        using domain = D;
        using space = S;

        static constexpr space::value_type act(const domain::value_type& v) noexcept(noexcept(func(v, std::declval<space::value_type>()))) {
            return [&v](const space::value_type& s) noexcept(noexcept(func(v, s))) { return func(v, s); };
        }
    };
}

#endif // !ADSL_ALGEBRA_TYPE_UTIL_HPP