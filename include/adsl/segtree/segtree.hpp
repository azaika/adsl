#ifndef ADSL_SEGTREE_SEGTREE_HPP
#define ADSL_SEGTREE_SEGTREE_HPP

#include <vector>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <utility>
#include <optional>
#include <type_traits>
#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"

namespace adsl {

    template <Monoid M, typename Container = std::vector<typename M::value_type>>
    requires (
        std::copyable<typename M::value_type> &&
        std::same_as<typename Container::value_type, typename M::value_type> )
    class segtree {
    public:
        using value_type = M::value_type;
        using size_type = Container::size_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;
        using container_type = Container;
        
    private:
        container_type node;
        size_type actual_size = 0;

        // contracts: idx <- [0, node.size() / 2)
        void recalc_at(size_type idx) noexcept(noexcept(M::op(std::declval<value_type>(), std::declval<value_type>()))) {
            node[idx] = M::op(node[idx << 1], node[(idx << 1) + 1]);
        }

    public:
        segtree() = default;
        segtree(const segtree&) = default;

        explicit segtree(size_type _size) : actual_size(_size) {
            size_type len = 1;
            while (len < _size)
                len <<= 1;
            
            node = container_type(len * 2, M::unit());
        }
        segtree(const container_type& src) : actual_size(src.size()) {
            if (src.size() == 0)
                return;
            
            size_type len = 1;
            while (len < src.size())
                len <<= 1;
            
            node = container_type(len * 2, M::unit());

            auto it = node.begin();
            std::advance(it, len);
            
            std::copy(src.begin(), src.end(), it);

            for (size_type i = len - 1; i > 0; --i)
                recalc_at(i);
        }

        size_type size() const noexcept {
            return actual_size;
        }

        bool is_empty() const noexcept {
            return size() == 0;
        }

        // update i-th value with updater(i-th value)
        // time complexity: Θ(logN)
        template <typename F>
        void update(size_type idx, F&& updater)
        noexcept(noexcept(updater(std::declval<value_type>()), recalc_at(idx)) && std::is_nothrow_copy_assignable_v<value_type>)
        requires requires { {updater(std::declval<value_type>())} -> std::convertible_to<value_type>; }
        {
            if (idx >= size())
                return;
            
            idx += node.size() / 2;

            node[idx] = updater(node[idx]);
            while (idx >>= 1)
                recalc_at(idx);
        }

        // time complexity: Θ(logN)
        void set(size_type idx, const_reference v) noexcept(noexcept(recalc_at(idx)) && std::is_nothrow_copy_assignable_v<value_type>) {
            update(idx, [=, &v](auto&&) noexcept { return v; });
        }

        // accumulate [l, r), returns std::nullopt if the given range is invalid
        // time complexity: Θ(logN)
       std::optional<value_type> accumulate(size_type l, size_type r) const noexcept(noexcept(std::optional<value_type>(M::op(M::unit(), M::unit()))) && std::is_nothrow_copy_assignable_v<value_type>) {
            if (l >= size() || r > size() || l >= r)
                return std::nullopt;
            
            l += node.size() / 2;
            r += node.size() / 2;

            value_type res_l = M::unit(), res_r = M::unit();
            while (l < r) {
                if (l & 1) {
                    res_l = M::op(res_l, node[l]);
                    ++l;
                }
                if (r & 1)
                    res_r = M::op(node[r - 1], res_r);

                l >>= 1;
                r >>= 1;
            }

            return M::op(res_l, res_r);
        }
    };

}

#endif // !ADSL_SEGTREE_SEGTREE_HPP