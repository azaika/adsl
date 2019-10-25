#ifndef ADSL_UTILITY_CONCEPTS_HPP
#define ADSL_UTILITY_CONCEPTS_HPP

#include <concepts>
#include <type_traits>

namespace adsl {

    template <typename From, typename To>
    concept NothrowConvertibleTo = std::convertible_to<From, To> && std::is_nothrow_constructible_v<From, To>;

}

#endif // !ADSL_UTILITY_CONCEPTS_HPP