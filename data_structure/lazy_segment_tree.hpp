#pragma once

#include <bit>
#include <vector>

// https://ei1333.github.io/library/structure/segment-tree/lazy-segment-tree.hpp.html

template <typename S2, typename Op, typename E, typename F2, typename Act,
          typename Composition, typename Id>
struct LambdaActedMonoid {
    using S = S2;
    using F = F2;

    S op(const S &a, const S &b) const { return _op(a, b); }

    S e() const { return _e(); }

    S act(const S &x, const F &f) const { return _act(x, f); }

    F composition(const F &f, const F &g) const { return _composition(f, g); }

    F id() const { return _id(); }

    LambdaActedMonoid(Op _op, E _e, Act _act, Composition _composition, Id _id)
        : _op(_op), _e(_e), _act(_act), _composition(_composition), _id(_id) {}

   private:
    Op _op;

    E _e;

    Act _act;

    Composition _composition;

    Id _id;
};

// Deduction guide for LambdaActedMonoid
// This allows the compiler to deduce the template parameters of
// LambdaActedMonoid based on the types of the arguments passed to its
// constructor. It simplifies the instantiation of LambdaActedMonoid by avoiding
// the need to explicitly specify the template arguments.
template <typename Op, typename E, typename Act, typename Composition,
          typename Id>
LambdaActedMonoid(Op _op, E _e, Act _act, Composition _composition, Id _id)
    -> LambdaActedMonoid<decltype(_e()), Op, E, decltype(_id()), Act,
                         Composition, Id>;

template <typename ActedMonoid>
struct LazySegmentTree {
    using S = typename ActedMonoid::S;
    using F = typename ActedMonoid::F;

   private:
    ActedMonoid m;
    int32_t n, sz, height;
    std::vector<S> data;
    std::vector<F> lazy;

    void update(const int32_t k) {
        data[k] = m.op(data[2 * k], data[2 * k + 1]);
    }

    void all_apply(const int32_t k, const F &x) {
        data[k] = m.act(data[k], x);
        lazy[k] = m.composition(x, lazy[k]);
    }

    void propagate(const int32_t k) {
        if (lazy[k] == m.id()) return;
        all_apply(2 * k, lazy[k]);
        all_apply(2 * k + 1, lazy[k]);
        lazy[k] = m.id();
    }

   public:
    LazySegmentTree(ActedMonoid m, int32_t n) : m(m), n(n) {
        sz = std::bit_ceil(static_cast<uint32_t>(n));
        height = std::countr_zero(static_cast<uint32_t>(sz));
        data.assign(2 * sz, m.e());
        lazy.assign(2 * sz, m.id());
    }

    void build(const std::vector<S> &v) {
        assert(v.size() <= n);
        for (int32_t i = 0; i < v.size(); ++i) data[sz + i] = v[i];
        for (int32_t i = sz - 1; i > 0; --i) update(i);
    }

    void set(int32_t k, const S &x) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i);
        data[k] = x;
        for (int32_t i = 1; i <= height; ++i) update(k >> i);
    }

    S get(int32_t k) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i);
        return data[k];
    }

    S prod(int32_t l, int32_t r) {
        if (l >= r) return m.e();
        l += sz;
        r += sz;
        for (int32_t i = height; i > 0; --i) {
            if (((l >> i) << i) != l) propagate(l >> i);
            if (((r >> i) << i) != r) propagate((r - 1) >> i);
        }
        S res_l = m.e(), res_r = m.e();
        while (l < r) {
            if (l & 1) res_l = m.op(res_l, data[l++]);
            if (r & 1) res_r = m.op(data[--r], res_r);
            l >>= 1;
            r >>= 1;
        }
        return m.op(res_l, res_r);
    }

    void apply(int32_t k, const F &x) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i);
        all_apply(k, x);
        for (int32_t i = 1; i <= height; ++i) update(k >> i);
    }

    void range_apply(int32_t l, int32_t r, const F &x) {
        if (l >= r) return;
        l += sz;
        r += sz;
        for (int32_t i = height; i > 0; --i) {
            if (((l >> i) << i) != l) propagate(l >> i);
            if (((r >> i) << i) != r) propagate((r - 1) >> i);
        }
        int32_t l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) all_apply(l++, x);
            if (r & 1) all_apply(--r, x);
            l >>= 1;
            r >>= 1;
        }
        l = l2, r = r2;
        for (int32_t i = 1; i <= height; ++i) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
};