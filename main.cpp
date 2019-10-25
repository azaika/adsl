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
    adsl::fenwick_tree<MaxM> seg1;

    // 演算に足し算を使うモノイドを使う
    adsl::fenwick_tree<adsl::default_group<i32>> seg2{{1, 2, 3, 4, 5}};
}