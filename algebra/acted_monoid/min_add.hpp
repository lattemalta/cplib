#pragma once

#include "algebra/monoid/add.hpp"
#include "algebra/monoid/min.hpp"

template <typename E>
struct MinAddActedMonoid {
    using Monoid_X = MinMonoid<E>;
    using Monoid_A = AddMonoid<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X& x, const int32_t len, const A& a) {
        if (x == Monoid_X::e()) return x;
        return x + a;
    }
};
