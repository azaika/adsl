#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>
#include "adsl/segtree/lazy_segtree.hpp"

using i32 = std::int32_t;
using u32 = std::uint32_t;

using MaxM = adsl::make_monoid<u32, 0, [](auto&& x, auto&& y) { return std::max(x, y); }, true>;

int main() {
}