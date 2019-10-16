#ifndef ADSL_ALGEBRA_TYPE_UTIL_HPP
#define ADSL_ALGEBRA_TYPE_UTIL_HPP

#include <type_traits>
#include "./data_type.hpp"

namespace adsl {

    template <typename ValueType, ValueType identity, auto func>
    struct make_monoid {
        using value_type = ValueType;
        
        static constexpr value_type id() noexcept {
            return identity;
        }
        
        static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(func(x, y))){
            return func(x, y);
        }
    };

    template <MonoidallyAdditionable T>
	struct default_monoid {
		using value_type = T;

		static constexpr value_type id() noexcept(noexcept(value_type{})) {
			return value_type{};
		}

		static constexpr value_type op(const value_type& x, const value_type& y) noexcept(noexcept(x + y)) {
			return x + y;
		}
	};

}

#endif // !ADSL_ALGEBRA_TYPE_UTIL_HPP