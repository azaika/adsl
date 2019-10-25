#include <iostream>
#include <vector>
#include <cstdint>
#include "adsl/segtree/segtree.hpp"

using i64 = std::int64_t;

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    int N, Q;
    std::cin >> N >> Q;

    std::vector<i64> vec(N);
    for (auto&& e : vec)
        std::cin >> e;
    
    adsl::segtree<adsl::default_monoid<i64>> seg(vec);

    for (int i = 0; i < Q; ++i) {
        int kind, x, y;
        std::cin >> kind >> x >> y;

        if (kind == 0)
            seg.update(x, [=](auto&& v) constexpr noexcept { return v + y; });
        else
            std::cout << seg.accumulate(x, y).value() << "\n";
    }

    std::cout << std::flush;
}