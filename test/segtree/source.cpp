#include <iostream>
#include <vector>
#include <cstdint>
#include "../../include/segtree/segtree.hpp"

using i64 = std::int64_t;

int main() {
    int N, Q;
    std::cin >> N >> Q;

    std::vector<i64> vec(N);
    for (auto&& e : vec)
        std::cin >> e;
    
    adsl::segtree<adsl::default_monoid<i64>> seg(vec);

    for (int i = 0; i < Q; ++i) {
        int kind;
        std::cin >> kind;

        if (kind == 0) {
            i64 p, x;
            std::cin >> p >> x;

            seg.update(p, [=](auto&& v) noexcept { return v + x; });
        }
        else {
            int l, r;
            std::cin >> l >> r;

            std::cout << seg.accumulate(l, r).value() << std::endl;
        }
    }
}