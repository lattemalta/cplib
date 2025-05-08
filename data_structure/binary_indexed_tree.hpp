#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

template <class Monoid>
struct BinaryIndexedTree {
    using G = Monoid;
    using E = typename G::value_type;
    std::vector<E> data;

    BinaryIndexedTree(const int32_t sz) { data.assign(sz + 1, G::e()); }

    // [0, k)
    E prefix_prod(int32_t k) const {
        E res = G::e();
        for (; k > 0; k -= k & -k) {
            res = G::op(data[k], res);
        }
        return res;
    }

    E prod(int32_t l, int32_t r) const {
        static_assert(G::invertible);
        return G::op(G::inv(prefix_prod(l)), prefix_prod(r));
    }

    void multiply(int32_t k, const E& x) {
        static_assert(G::commutative);
        for (k++; k < data.size(); k += k & -k) {
            data[k] = G::op(data[k], x);
        }
    }
};
