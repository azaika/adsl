#ifndef ADSL_SEGTREE_LAZY_SEGTREE_HPP
#define ADSL_SEGTREE_LAZY_SEGTREE_HPP

#include "../algebra/data_type.hpp"
#include "../algebra/type_util.hpp"
#include <cstddef>
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
        using size_type = std::size_t;
        using operator_container_type = Container<operator_type>;
        using value_container_type = Container<value_type>;

    private:
        using domain = Act::domain;
        using space = Act::space;
        
        operator_container_type lazy;
        value_container_type node;

        size_type actual_size = 0;
        size_type height = 0;

        void append_op_at(size_type idx, const operator_type& o) {
            lazy[idx] = domain::op(lazy[idx], o);
        }

        // contracts: idx <- [0, node.size() / 2)
        void prop_at(size_type idx) {
            append_op_at(idx << 1, lazy[idx]);
            append_op_at((idx << 1) + 1, lazy[idx]);

            lazy[idx] = domain::unit();
        }

        void prop_to(size_type idx) noexcept(noexcept(prop_at(idx))) {
            for (size_type i = height; i >= 1; --i)
                prop_at(idx >> i);
        }

        void calc_at(size_type idx) const {
            return A::act(lazy[idx])(node[idx]);
        }

        void reflect(size_type idx) {
            for (size_type i = idx; i > 0; i >>= 1)
                node[idx] = space::op(calc_at(i << 1), calc_at((i << 1) + 1));
        }

        void evaluate_at(size_type idx) {
            node[idx] = calc_at(idx >> i);
            prop_at(idx);
        }

        void evaluate(size_type idx) {
            for (size_type i = height; i >= 1; --i)
                evaluate_at(idx);
        }

    public:
        lazy_segtree() = default;
        lazy_segtree(const lazy_segtree&) = default;

        explicit lazy_segtree(size_type _size) : actual_size(_size) {
            static_assert(sizeof(size_type) <= sizeof(operator_container_type::size_type));
            static_assert(sizeof(size_type) <= sizeof(value_container_type::size_type));

            if (_size == 0)
                return;

            size_type len = 1;
            while (len < _size) {
                len <<= 1;
                ++height;
            }

            lazy = operator_container_type(len * 2, domain::unit());
            node = value_container_type(len * 2, space::unit());
        }
        lazy_segtree(const value_container_type& src) : lazy_segtree(src.size()) {
            if (src.size() == 0)
                return;
            
            auto it = node.begin();
            std::advance(it, src.size() / 2);
            
            std::copy(src.begin(), src.end(), it);

            for (size_type i = src.size() / 2 - 1; i > 0; ++i)
                node[i] = space::op(node[i << 1], node[(i << 1) + 1]);
        }

        size_type size() const noexcept {
            return actual_size;
        }

        bool is_empty() const noexcept {
            return size() == 0;
        }

        void append(size_type l, size_type r, const operator_type& inc) {
            if (l >= size() || r > size() || l >= r)
                return;

            l += lazy.size() / 2;
            r += lazy.size() / 2;

            prop_to(l);
            prop_to(r - 1);

            for (size_type _l = l, _r = r; _l < _r; _l >>= 1, _r >>= 1) {
                if (_l & 1) {
                    append_op_at(_l, inc);
                    ++_l;
                }
                if (_r & 1)
                    append_op_at(_r - 1, inc);
            }

            reflect(l);
            reflect(r - 1);
        }

        template <typename F>
        void update(size_type idx, F&& updater)
        requires requires{ {updater(std::declval<value_type>())} -> std::convertible_to<value_type>; }
        {
            if (idx >= size())
                return;

            idx += node.size() / 2;

            prop_to(idx);

            node[idx] = updater(calc_at(idx));
            lazy[idx] = domain::unit();

            reflect(idx);
        }

        void set(size_type idx, const_reference v) {
            update(idx, [=, &v](auto&&) noexcept { return v; });
        }

        std::optional<value_type> accumulate(size_type l, size_type r) const {
            if (l >= size() || r > size() || l >= r)
                return std::nullopt;

            l += node.size() / 2;
            r += node.size() / 2;

            evaluate(l);
            evaluate(r - 1);

            value_type res_l = space::unit(), res_r = space::unit();
            while (l < r) {
                if (l & 1) {
                    res_l = space::op(res_l, calc_at(l));
                    ++l;
                }
                if (r & 1)
                    res_r = space::op(calc_at(r - 1), res_r);

                l >>= 1;
                r >>= 1;
            }

            return space::op(res_l, res_r);
        }

    };

}

#endif // !ADSL_SEGTREE_LAZY_SEGTREE_HPP