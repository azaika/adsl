#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>
#include "adsl/segtree/dual_segtree.hpp"

using i32 = std::int32_t;
using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }, true>;

int main() {
    // モノイドを指定
    adsl::dual_segtree<adsl::default_monoid<i32>> sd{7};

    sd.append(1, 4, 3);
    sd.append(3, 7, -1);
    sd.append(6, 7, 5);

    for (size_t i = 0; i < 7; ++i)
        std::cout << *sd.calc(i) << std::endl;
}