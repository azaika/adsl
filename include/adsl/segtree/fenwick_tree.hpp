#ifndef ADSL_SEGTREE_FENWICK_TREE_HPP
#define ADSL_SEGTREE_FENWICK_TREE_HPP

#include <vector>
#include <optional>
#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"
#include "../utility/concepts.hpp"

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

        static constexpr bool is_nothrow_unit = noexcept(M::unit());
        static constexpr bool is_nothrow_op = noexcept(M::op(std::declval<value_type>(), std::declval<value_type>()));

        value_type accumulate_impl(size_type idx) const noexcept(is_nothrow_unit && is_nothrow_op) {
            value_type res = M::unit();
            for (size_type i = idx; i > 0; i &= i - 1)
                res = M::op(res, node[i]);
            
            return res;
        }

    public:
        fenwick_tree() = default;
        fenwick_tree(const fenwick_tree&) = default;

        explicit fenwick_tree(size_type _size) : actual_size(_size) {
            size_type len = 1;
            while (len < _size)
                len <<= 1;
            
            node = container_type(len + 1, M::unit());
        }
        fenwick_tree(const container_type& src) : actual_size(src.size()) {
            if (src.size() == 0)
                return;
            
            size_type len = 1;
            while (len < src.size())
                len <<= 1;
            
            node = container_type(len + 1, M::unit());

            for (size_type i = 0; i + 1 < len; ++i) {
                if (i < src.size())
                    node[i + 1] = M::op(node[i + 1], src[i]);

                const size_type par = (i + 1) + ((i + 1) & (~(i + 1) + 1));
                node[par] = M::op(node[par], node[i + 1]);
            }
            if (len == src.size())
                node[len] = M::op(node[len], src.back());
        }

        size_type size() const noexcept {
            return actual_size;
        }

        bool is_empty() const noexcept {
            return size() == 0;
        }

        // time complexity: Θ(logN)
        void append_at(size_type idx, const_reference inc) noexcept(is_nothrow_op) {
            if (idx >= size())
                return;

            ++idx;
            for (size_type i = idx + 1; i < node.size(); i += i & (~i + 1))
                node[i] = M::op(node[i], inc);
        }

        // time complexity: Θ(logN)
        std::optional<value_type> accumulate(size_type idx) const noexcept(std::is_nothrow_move_constructible_v<value_type> && noexcept(accumulate_impl(idx))) {
            if (idx >= size())
                return std::nullopt;

            return accumulate_impl(idx);
        }
        
        // accumulate [l, r), returns std::nullopt if the given range is invalid
        // time complexity: Θ(logN)
        std::optional<value_type> accumulate(size_type l, size_type r) const
        noexcept(is_nothrow_op && noexcept(std::optional<value_type>(M::unit()), M::inv(accumulate_impl(l))))
        requires CommutativeGroup<M>
        {
            if (l >= size() || r > size() || l >= r)
                return std::nullopt;

            return M::op(accumulate_impl(r), M::inv(accumulate_impl(l)));
        }

        // time complexity: Θ(logN)
        std::optional<value_type> calc(size_type idx) const noexcept(noexcept(accumulate(idx, idx + 1))) requires CommutativeGroup<M> {
            return accumulate(idx, idx + 1);
        }

        // update i-th value with updater(i-th value)
        // time complexity: Θ(logN)
        template <typename F>
        void update(size_type idx, F&& updater)
        noexcept(
            NothrowConvertibleTo<value_type, value_type> &&
            NothrowConvertibleTo<decltype(updater(std::declval<value_type>())), value_type> &&
            is_nothrow_op &&
            noexcept(M::inv(accumulate_impl(idx)), updater(std::declval<value_type>())))
        requires CommutativeGroup<M> && requires { {updater(std::declval<value_type>())} -> std::convertible_to<value_type>; }
        {
            if (idx >= size())
                return;
            
            const value_type cur_val = M::op(accumulate_impl(idx), M::inv(accumulate_impl(idx + 1)));
            const value_type new_val = updater(cur_val);

            append_at(idx, M::op(new_val, M::inv(cur_val)));
        }

        // time complexity: Θ(logN)
        void set(size_type idx, const_reference v)
        noexcept(noexcept(this->update(idx, [=, &v](auto&&) noexcept { return v; })))
        requires CommutativeGroup<M>
        {
            update(idx, [=, &v](auto&&) noexcept { return v; });
        }

    };

}

#endif // !ADSL_SEGTREE_FENWICK_TREE_HPP