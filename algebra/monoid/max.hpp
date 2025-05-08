#pragma once

#include <algorithm>
#include <limits>

template <typename X>
struct Monoid_Max {
    using value_type = X;
    static constexpr X op(const X& x, const X& y) noexcept {
        return std::max(x, y);
    }
    static constexpr X e() noexcept { return std::numeric_limits<X>::lowest(); }
    static constexpr bool commutative = true;
    static constexpr bool invertible = false;
};
