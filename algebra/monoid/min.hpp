#pragma once

#include <algorithm>
#include <limits>

template <typename X>
struct MinMonoid {
    using value_type = X;
    static constexpr X op(const X& x, const X& y) noexcept {
        return std::min(x, y);
    }
    static constexpr X e() noexcept { return std::numeric_limits<X>::max(); }
    static constexpr bool commutative = true;
    static constexpr bool invertible = false;
};
