#ifndef ADSL_SEGTREE_SEGTREE_HPP
#define ADSL_SEGTREE_SEGTREE_HPP

#include "../algebra/data_type.hpp"

namespace adsl {
    
    // can not use
    template <typename T>
    class segtree;
    
    template <Monoid M>
	class segtree<M> {
    public:
	};
    
    template <MonoidallyAdditionable T> requires (!Monoid<T>)
    class segtree<T> : public segtree<default_monoid<T>> {};
}

#endif // !ADSL_SEGTREE_SEGTREE_HPP