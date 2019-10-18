#include <cstdint>
#include <algorithm>
#include <string>
#include "include/segtree/segtree.hpp"

using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }>;

int main() {
    // モノイドを指定
    adsl::segtree<MaxM> seg1;
    // 演算に足し算を使うモノイドを使う
    adsl::segtree<adsl::default_monoid<std::string>> seg2;
}