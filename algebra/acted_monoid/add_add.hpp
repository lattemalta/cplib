#pragma once

#include "algebra/monoid/add.hpp"

template <typename E>
struct AddAddActedMonoid {
    using Monoid_X = AddMonoid<E>;
    using Monoid_A = AddMonoid<E>;
    static constexpr E act(const E& x, const int32_t len, const E& a) {
        return Monoid_X::op(x, Monoid_A::pow(a, len));
    }
};
