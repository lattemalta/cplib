#pragma once

#include <bit>
#include <vector>

template <typename ActedMonoid>
struct LazySegmentTree {
    using MX = typename ActedMonoid::Monoid_X;
    using MA = typename ActedMonoid::Monoid_A;
    using X = typename MX::value_type;
    using A = typename MA::value_type;

   private:
    int32_t n, sz, height;
    std::vector<X> data;
    std::vector<A> lazy;

    void update(const int32_t k) {
        data[k] = MX::op(data[2 * k], data[2 * k + 1]);
    }

    void all_apply(const int32_t k, const int32_t len, const A &a) {
        data[k] = ActedMonoid::act(data[k], len, a);
        lazy[k] = MA::op(a, lazy[k]);
    }

    void propagate(const int32_t k, const int32_t len) {
        if (lazy[k] == MA::e()) return;
        all_apply(2 * k, len >> 1, lazy[k]);
        all_apply(2 * k + 1, len >> 1, lazy[k]);
        lazy[k] = MA::e();
    }

   public:
    LazySegmentTree(int32_t n) : n(n) {
        sz = std::bit_ceil(static_cast<uint32_t>(n));
        height = std::countr_zero(static_cast<uint32_t>(sz));
        data.assign(2 * sz, MX::e());
        lazy.assign(2 * sz, MA::e());
    }

    void build(const std::vector<X> &v) {
        assert(v.size() <= n);
        for (int32_t i = 0; i < v.size(); ++i) data[sz + i] = v[i];
        for (int32_t i = sz - 1; i > 0; --i) update(i);
    }

    void set(int32_t k, const X &x) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i, 1 << i);
        data[k] = x;
        for (int32_t i = 1; i <= height; ++i) update(k >> i);
    }

    X get(int32_t k) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i, 1 << i);
        return data[k];
    }

    X prod(int32_t l, int32_t r) {
        if (l >= r) return MX::e();
        l += sz;
        r += sz;
        for (int32_t i = height; i > 0; --i) {
            if (((l >> i) << i) != l) propagate(l >> i, 1 << i);
            if (((r >> i) << i) != r) propagate((r - 1) >> i, 1 << i);
        }
        X res_l = MX::e(), res_r = MX::e();
        while (l < r) {
            if (l & 1) res_l = MX::op(res_l, data[l++]);
            if (r & 1) res_r = MX::op(data[--r], res_r);
            l >>= 1;
            r >>= 1;
        }
        return MX::op(res_l, res_r);
    }

    void apply(int32_t k, const A &a) {
        k += sz;
        for (int32_t i = height; i > 0; --i) propagate(k >> i, 1 << i);
        all_apply(k, 1, a);
        for (int32_t i = 1; i <= height; ++i) update(k >> i);
    }

    void range_apply(int32_t l, int32_t r, const A &a) {
        if (l >= r) return;
        l += sz;
        r += sz;
        for (int32_t i = height; i > 0; --i) {
            if (((l >> i) << i) != l) propagate(l >> i, 1 << i);
            if (((r >> i) << i) != r) propagate((r - 1) >> i, 1 << i);
        }
        int32_t l2 = l, r2 = r, len = 1;
        while (l < r) {
            if (l & 1) all_apply(l++, len, a);
            if (r & 1) all_apply(--r, len, a);
            l >>= 1;
            r >>= 1;
            len <<= 1;
        }
        l = l2, r = r2;
        for (int32_t i = 1; i <= height; ++i) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
};