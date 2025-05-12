#pragma once

#include <cstdint>
#include <tuple>

struct InversionMonoid {
    using value_type = std::tuple<int64_t, int64_t, int64_t>;
    using X = value_type;
    static constexpr X op(const X& u, const X& v) noexcept {
        auto [zero_u, one_u, inversion_u] = u;
        auto [zero_v, one_v, inversion_v] = v;
        return {zero_u + zero_v, one_u + one_v,
                inversion_u + inversion_v + one_u * zero_v};
    }
    static constexpr X e() noexcept { return {0, 0, 0}; }
    static constexpr bool commutative = false;
    static constexpr bool invertible = false;
};
