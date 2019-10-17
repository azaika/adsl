#ifndef ADSL_SEGTREE_SEGTREE_HPP
#define ADSL_SEGTREE_SEGTREE_HPP

#include <vector>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <utility>
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
        Container node;
        size_type actual_size;

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
            
            node = container_type(len * 2, M::id());
        }
        explicit segtree(const container_type& src) : actual_size(src.size()) {
            if (src.size() == 0)
                return;
            
            size_type len = 1;
            while (len < src.size())
                len <<= 1;
            
            node = container_type(len * 2, M::id());

            auto it = node.begin();
            std::advance(it, len);
            
            std::copy(src.begin(), src.end(), it);

            for (size_type i = len - 1; i >= 0; --i)
                recalc_at(i);
        }

        size_type size() const noexcept {
            return actual_size;
        }

        template <typename F>
        requires requires (F&& f) { {f(M::id())} -> std::convertible_to<value_type>; }
        void update(size_type idx, F&& updater) noexcept(noexcept(updater(std::declval<value_type>()), recalc_at(idx))) {
            if (idx >= size())
                return;
            
            idx += size();

            node[idx] = updater(node[idx]);
            while (idx >>= 1)
                recalc_at(idx);
        }

        void update(size_type idx, const_reference v) noexcept(update(size_type{}, []() noexcept { return value_type{}; })) {
            update(idx, [=, &v](auto&&) noexcept { return v; });
        }
    };

}

#endif // !ADSL_SEGTREE_SEGTREE_HPP