// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <optional>
#include <limits>

#include "adsl/segtree/lazy_segtree.hpp"

using i32 = std::int32_t;

constexpr i32 Inf = std::numeric_limits<i32>::max();

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    i32 n, q;
    std::cin >> n >> q;

    using O = adsl::make_monoid<std::optional<i32>, std::nullopt, [](auto&& x, auto&& y) { return (y ? y : x); }, true>;
    using M = adsl::make_monoid<i32, Inf, [](i32 x, i32 y) { return std::min(x, y); }, true>;
    using Act = adsl::make_action<O, M, [](auto&& p, i32 x) { return (p ? *p : x); }>;

    adsl::lazy_segtree<Act> seg{static_cast<size_t>(n)};

    for (int i = 0; i < q; ++i) {
        i32 k, s, t;
        std::cin >> k >> s >> t;
        ++t;

        if (k == 0) {
            i32 x;
            std::cin >> x;

            seg.append(s, t, x);
        }
        else
            std::cout << *seg.accumulate(s, t) << "\n";
    }

    std::cout << std::flush;
}