#ifndef ADSL_SEGTREE_DUAL_SEGTREE_HPP
#define ADSL_SEGTREE_DUAL_SEGTREE_HPP

#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"
#include <utility>
#include <vector>

namespace adsl {

    template <Monoid M, typename Container = std::vector<typename M::value_type>>
    requires (
        std::copyable<typename M::value_type> &&
        std::same_as<typename Container::value_type, typename M::value_type> )
    class dual_segtree {
    public:
        using value_type = M::value_type;
        using size_type = Container::size_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;
        using container_type = Container;
    
    private:
        container_type node;
        size_type actual_size = 0;
        size_type height = 0;

        // contracts: idx <- [0, node.size() / 2)
        void prop_at(size_type idx) noexcept(noexcept(M::op(std::declval<value_type>(), std::declval<value_type>()))) {
            node[idx << 1] = M::op(node[idx << 1], node[idx]);
            node[(idx << 1) + 1] = M::op(node[(idx << 1) + 1], node[idx]);

            node[idx] = M::unit();
        }

        void prop_to(size_type idx) noexcept(noexcept(prop_at(idx))) {
            for (size_type i = height; i >= 1; --i)
                prop_at(idx >> i);
        }

    public:
        dual_segtree() = default;
        dual_segtree(const dual_segtree&) = default;

        explicit dual_segtree(size_type _size) : actual_size(_size) {
            size_type len = 1;
            while (len < _size) {
                len <<= 1;
                ++height;
            }

            node = container_type(len * 2, M::unit());
        }

        size_type size() const noexcept {
            return actual_size;
        }

        bool is_empty() const noexcept {
            return size() == 0;
        }

        void append(size_type l, size_type r, const_reference inc) {
            if (l >= size() || r > size() || l >= r)
                return;

            l += node.size() / 2;
            r += node.size() / 2;

            prop_to(l);
            prop_to(r - 1);

            while (l < r) {
                if (l & 1) {
                    node[l] = M::op(node[l], inc);
                    ++l;
                }
                if (r & 1)
                    node[r - 1] = M::op(node[r - 1], inc);
                
                l >>= 1;
                r >>= 1;
            }
        }

        std::optional<value_type> get(size_type idx) {
            if (idx >= size())
                return std::nullopt;

            idx += node.size() / 2;

            prop_to(idx);
            return node[idx];
        }

    };

}

#endif // !ADSL_SEGTREE_DUAL_SEGTREE_HPP