#include <iostream>
#include <vector>
#include <cstdint>
#include "adsl/segtree/fenwick_tree.hpp"

using i64 = std::int64_t;

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    int N, Q;
    std::cin >> N >> Q;

    std::vector<i64> vec(N);
    for (auto&& e : vec)
        std::cin >> e;
    
    adsl::fenwick_tree<adsl::default_group<i64>> bit(vec);

    for (int i = 0; i < Q; ++i) {
        int kind, x, y;
        std::cin >> kind >> x >> y;

        if (kind == 0)
            bit.append_at(x, y);
        else
            std::cout << bit.accumulate(x, y).value() << "\n";
    }

    std::cout << std::flush;
}