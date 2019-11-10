#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>
#include "adsl/segtree/lazy_segtree.hpp"

#include <functional>

using i32 = std::int32_t;
using u32 = std::uint32_t;

int main() {
    int n, q;
    std::cin >> n >> q;

    using M = adsl::default_monoid<i32>;
    using Act = adsl::make_action<M, M, [](i32 a, i32 b) { return a + b; }>;

    adsl::lazy_segtree<Act> seg{static_cast<size_t>(n)};

    for (int i = 0; i < q; ++i) {
        int k, s, t;
        std::cin >> k >> s >> t;
        --s;

        if (k == 0) {
            int x;
            std::cin >> x;

            seg.append(s, t, x);
        }
        else
            std::cout << *seg.accumulate(s, t) << std::endl;
    }
}