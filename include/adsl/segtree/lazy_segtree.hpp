#ifndef ADSL_SEGTREE_LAZY_SEGTREE_HPP
#define ADSL_SEGTREE_LAZY_SEGTREE_HPP

#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"
#include <utility>
#include <vector>

namespace adsl {

    template <Monoid M>
    class lazy_segtree {
    public:
        using value_type = M::value_type;
        using size_type = Container::size_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;
        using container_type = Container;

    private:

    public:

    }

}

#endif // !ADSL_SEGTREE_LAZY_SEGTREE_HPP