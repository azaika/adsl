#include "segtree/segtree.hpp"
#include <cstdint>
#include <algorithm>

using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }>;

int main() {
    // モノイドを指定
    adsl::segtree<MaxM> seg1;
    // モノイドでない場合、足し算を演算とするモノイドを使う
    adsl::segtree<int> seg2;
}