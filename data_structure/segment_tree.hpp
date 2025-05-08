#pragma once

#include <cstdint>
#include <functional>
#include <vector>

template <typename Monoid>
struct SegmentTree {
    using G = Monoid;
    using E = typename G::value_type;

    int32_t sz;
    std::vector<E> seg;

    SegmentTree(const int32_t n) {
        sz = 1;
        while (sz < n) sz <<= 1;
        seg.assign(2 * sz, G::e());
    }

    void set(int32_t k, const E &x) {
        k += sz;
        seg[k] = x;
        while (k >>= 1) {
            seg[k] = G::op(seg[2 * k + 0], seg[2 * k + 1]);
        }
    }

    E prod(int32_t a, int32_t b) {
        E L = G::e(), R = G::e();
        for (a += sz, b += sz; a < b; a >>= 1, b >>= 1) {
            if (a & 1) L = G::op(L, seg[a++]);
            if (b & 1) R = G::op(seg[--b], R);
        }
        return G::op(L, R);
    }

    E operator[](const int32_t &k) const { return seg[k + sz]; }

    /**
     * @brief
     * find_first(a, check) returns the first index i such that
     * check(prod(a, i+1)) == true.
     * returns -1 if no such i exists.
     */
    template <typename C>
    int32_t find_first(int32_t a, const C &check) {
        E L = G::e();
        if (a <= 0) {
            if (check(G::op(L, seg[1])))
                return find_subtree(1, check, L, false);
            return -1;
        }
        int32_t b = sz;
        for (a += sz, b += sz; a < b; a >>= 1, b >>= 1) {
            if (a & 1) {
                E nxt = G::op(L, seg[a]);
                if (check(nxt)) return find_subtree(a, check, L, false);
                L = nxt;
                ++a;
            }
        }
        return -1;
    }

    /**
     * @brief
     * find_last(b, check) returns the last index i such that
     * check(prod(i, b)) == true.
     * returns -1 if no such i exists.
     */
    template <typename C>
    int32_t find_last(int32_t b, const C &check) {
        E R = G::e();
        if (b >= sz) {
            if (check(f(seg[1], R))) return find_subtree(1, check, R, true);
            return -1;
        }
        int32_t a = sz;
        for (b += sz; a < b; a >>= 1, b >>= 1) {
            if (b & 1) {
                E nxt = G::op(seg[--b], R);
                if (check(nxt)) return find_subtree(b, check, R, true);
                R = nxt;
            }
        }
        return -1;
    }

   private:
    template <typename C>
    int32_t find_subtree(int32_t a, const C &check, E &M, bool type) {
        while (a < sz) {
            E nxt = type ? G::op(seg[2 * a + type], M)
                         : G::op(M, seg[2 * a + type]);
            if (check(nxt))
                a = 2 * a + type;
            else
                M = nxt, a = 2 * a + 1 - type;
        }
        return a - sz;
    }
};