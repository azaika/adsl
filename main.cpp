#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>
#include "adsl/segtree/fenwick_tree.hpp"

using i32 = std::int32_t;
using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }, true>;

int main() {
    // モノイドを指定
    adsl::fenwick_tree<MaxM> seg1{{1u, 6u, 2u, 9u, 3u}};

    for (size_t i = 0; i < seg1.size(); ++i)
        std::cout << *seg1.accumulate(i) << std::endl;


    // 演算に足し算を使うモノイドを使う
    adsl::fenwick_tree<adsl::default_group<i32>> seg2{{1, 2, 3, 4, -4, -3, -2, -1}};

    for (size_t i = 0; i < seg2.size(); ++i)
        std::cout << *seg2.accumulate(i) << std::endl;
}