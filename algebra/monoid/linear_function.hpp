#pragma once

#include <utility>

template <typename T>
struct LinearFunctionMonoid {
    using value_type = std::pair<T, T>;

    // g(f(x))
    static constexpr value_type op(const value_type& f,
                                   const value_type& g) noexcept {
        return {f.first * g.second + g.first, f.second * g.second};
    }
    static constexpr value_type e() noexcept { return {T(0), T(1)}; }
    static constexpr bool commutative = false;
    static constexpr bool invertible = false;
};
