#pragma once

template <typename X>
struct AddMonoid {
    using value_type = X;
    static constexpr X op(const X& x, const X& y) noexcept { return x + y; }
    static constexpr X inv(const X& x) noexcept { return -x; }
    static constexpr X pow(const X& x, const int64_t exp) noexcept {
        return x * exp;
    }
    static constexpr X e() noexcept { return X(0); }
    static constexpr bool commutative = true;
    static constexpr bool invertible = true;
};
