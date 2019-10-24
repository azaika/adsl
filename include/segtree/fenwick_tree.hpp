#ifndef ADSL_SEGTREE_FENWICK_TREE_HPP
#define ADSL_SEGTREE_FENWICK_TREE_HPP

#include <vector>
#include <optional>
#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"

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

        static constexpr bool is_group = CommutativeGroup<M>;

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

        // time complexity: Θ(logN)
        void append_at(size_type idx, const_reference inc) {
            if (idx >= size())
                return;

            ++idx;
            for (size_type i = idx + 1; i < node.size(); i += i & ~i + 1)
                node[i] = M::op(node[i], inc);
        }

        // time complexity: Θ(logN)
        std::optional<value_type> accumulate(size_type idx) const requires is_group {
            if (idx >= size())
                return std::nullopt;

            value_type res = M::unit();
            for (size_type i = idx; i > 0; i &= i - 1)
                res = M::op(res, node[i]);
            
            return res;
        }

        // update i-th value with updater(i-th value)
        // time complexity: Θ(logN)
        template <typename F>
        void update(size_type idx, F&& updater)
        requires is_group && requires { {updater(std::declval<value_type>())} -> std::convertible_to<value_type>; }
        {
            if (idx >= size())
                return;
            
            const value_type cur_val = *calc(idx);
            const value_type new_val = updater(cur_val);

            append_at(idx, M::op(new_val, M::inv(cur_val)));
        }

        // time complexity: Θ(logN)
        void set(size_type idx, const_reference v)
        noexcept(noexcept(recalc_at(idx)) && std::is_nothrow_copy_assignable_v<value_type>)
        requires is_group
        {
            update(idx, [=, &v](auto&&) noexcept { return v; });
        }

        // time complexity: Θ(logN)
        std::optional<value_type> accumulate(size_type l, size_type r) const requires is_group {
            if (l >= size() || r > size() || l >= r)
                return std::nullopt;

            return M::op(*accumulate(r), M::inv(*accumulate(l)));
        }

        // time complexity: Θ(logN)
        std::optional<value_type> calc(size_type idx) const requires is_group {
            return accumulate(idx, idx + 1);
        }

    };

}

#endif // !ADSL_SEGTREE_FENWICK_TREE_HPP