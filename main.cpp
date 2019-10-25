#include <cstdint>
#include <algorithm>
#include <string>
#include "adsl/segtree/fenwick_tree.hpp"

using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }, true>;

int main() {
    // モノイドを指定
    adsl::fenwick_tree<MaxM> seg1;

    // 演算に足し算を使うモノイドを使う
    adsl::fenwick_tree<adsl::default_monoid<u32>> seg2;
}