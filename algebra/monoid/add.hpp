#pragma once

template <typename X>
struct Monoid_Add {
    using value_type = X;
    static constexpr X op(const X& x, const X& y) noexcept { return x + y; }
    static constexpr X inv(const X& x) noexcept { return -x; }
    static constexpr X pow(const X& x, const int64_t n) noexcept {
        return X(n) * x;
    }
    static constexpr X e() noexcept { return X(0); }
    static constexpr bool commutative = true;
    static constexpr bool invertible = true;
};
