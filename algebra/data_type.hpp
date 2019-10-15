#ifndef ADSL_ALGEBRA_DATA_TYPE_HPP
#define ADSL_ALGEBRA_DATA_TYPE_HPP

#include <utility>
#include <type_traits>

namespace adsl {

	template <typename M>
	concept Monoid = requires(M m) {
		typename M::value_type;

		{M::id()} -> M::value_type;
		// M::op must be associative
		{M::op(M::id(), M::id())} -> M::value_type;
	};
    
    template <typename ValueType, ValueType identity, auto func>
    struct make_monoid {
        using value_type = ValueType;
        
        static value_type id() {
            return identity;
        }
        
        static value_type op(const value_type& x, const value_type& y) {
            return func(x, y);
        }
    };

	template <typename T>
	concept MonoidallyAdditionable = requires(T x, T y) {
        requires std::is_default_constructible_v<T>;
		{x + y} -> T;
	};

	template <MonoidallyAdditionable T>
	struct default_monoid {
		using value_type = T;

		static value_type id() {
			return value_type{};
		}

		static value_type op(const value_type& x, const value_type& y) {
			return x + y;
		}
	};

}

#endif // !ADSL_ALGEBRA_DATA_TYPE_HPP