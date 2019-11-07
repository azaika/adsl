#ifndef ADSL_SEGTREE_LAZY_SEGTREE_HPP
#define ADSL_SEGTREE_LAZY_SEGTREE_HPP

#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"
#include <utility>
#include <vector>
#include <memory>
#include <utility>

namespace adsl {

    template <typename A>
    concept LazySegtreeAct = MonoidAction<A> && Monoid<typename A::space> && requires {
        { A::act(A::domain::unit()) } -> MonoidEndomorphism<typename A::space>;
    };

    template <LazySegtreeAct Act, template <typename T, typename Allocator = std::allocator<T>> typename Container = std::vector>
    requires (
        std::copyable<typename Act::domain::value_type> &&
        std::copyable<typename Act::space::value_type> )
    class lazy_segtree {
    public:
        using operator_type = Act::domain::value_type;
        using value_type = Act::space::value_type;
        using reference = value_type&;
        using const_reference = const reference;

    private:
        using container_type = Container<std::pair<operator_type, value_type>>;

    public:
        using size_type = container_type::size_type;



    };

}

#endif // !ADSL_SEGTREE_LAZY_SEGTREE_HPP