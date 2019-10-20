#ifndef ADSL_SEGTREE_FENWICK_TREE_HPP
#define ADSL_SEGTREE_FENWICK_TREE_HPP

#include <vector>
#include <optional>
#include "../algebra/data_type.hpp"

namespace adsl {
    
    template <CommutativeMonoid M, typename Container = std::vector<typename M::value_type>>
    requires (
        std::copyable<typename M::value_type> &&
        std::same_as<typename Container::value_type, typename M::value_type> )
    class fenwick_tree {
    public:
        using value_type = M::value_type;
        using size_type = Container::size_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;
        using container_type = Container;

    private:
        container_type node;
        size_type actual_size;

    public:
        fenwick_tree() = default;
        fenwick_tree(const fenwick_tree&) = default;

        explicit fenwick_tree(size_type _size) : actual_size(_size) {
            size_type len = 1;
            while (len < _size)
                len <<= 1;
            
            node = container_type(len + 1, M::unit());
        }
        explicit fenwick_tree(const container_type& src) : actual_size(src.size()) {
            if (src.size() == 0)
                return;
            
            size_type len = 1;
            while (len < src.size())
                len <<= 1;
            
            node = container_type(len + 1, M::unit());

            for (size_type i = 0; i < src.size(); ++i)
                append_at(i, src[i]);
        }

        size_type size() const noexcept {
            return actual_size;
        }

        bool is_empty() const noexcept {
            return size() == 0;
        }

        void append_at(size_type idx, const_reference inc) {
            if (idx >= size())
                return;

            ++idx;
            for (size_type i = idx + 1; i < node.size(); i += i & ~i + 1)
                node[i] = M::op(node[i], diff);
        }

        std::optional<value_type> accumulate(size_type idx) const {
            if (idx >= size())
                return std::nullopt;

            value_type res = M::unit();
            for (size_type i = idx; i > 0; i &= i - 1)
                res = M::op(res, node[i]);
            
            return res;
        }

    };

}

#endif // !ADSL_SEGTREE_FENWICK_TREE_HPP