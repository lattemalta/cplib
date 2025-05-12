#pragma once

template <typename X>
struct XorMonoid {
    using value_type = X;
    static constexpr X op(const X u, const X v) noexcept { return u ^ v; }
    static constexpr X inv(const X x) noexcept { return x; }
    static constexpr X pow(const X x, const int64_t exp) noexcept {
        return (exp & 1) ? x : e();
    }
    static constexpr X e() noexcept { return X(0); }
    static constexpr bool commutative = true;
    static constexpr bool invertible = true;
};
