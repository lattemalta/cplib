#pragma once

#include <algorithm>
#include <bit>
#include <cstdint>
#include <vector>

// TOOD: calc root from mint_t::get_mod()
template <class mint_t, int32_t root = 3>
void ntt(std::vector<mint_t> &f) {
    const int32_t n = f.size();
    for (int32_t i = 0, j = 1; j < n - 1; ++j) {
        for (int32_t k = n >> 1; k > (i ^= k); k >>= 1);
        if (i > j) std::swap(f[i], f[j]);
    }
    const auto s = std::bit_width(static_cast<uint32_t>(n)) - 1;
    for (int32_t m = 1; m <= s; ++m) {
        const mint_t wr = mint_t(root).pow((mint_t::get_mod() - 1) >> m);
        for (int32_t i = 0; i < n; i += 1 << m) {
            mint_t w = 1;
            for (int32_t j = 0; j < 1 << (m - 1); ++j, w *= wr) {
                const mint_t f0 = f[i + j], f1 = w * f[i + j + (1 << (m - 1))];
                f[i + j] = f0 + f1;
                f[i + j + (1 << (m - 1))] = f0 - f1;
            }
        }
    }
}

template <class mint_t, uint32_t root = 3>
void intt(std::vector<mint_t> &f) {
    const int32_t n = f.size();

    std::reverse(f.begin() + 1, f.end());
    ntt(f);

    const mint_t in = mint_t(n).inv();
    for (int32_t i = 0; i < n; ++i) f[i] *= in;
}

template <class mint_t, uint32_t root = 3>
std::vector<mint_t> convolute(const std::vector<mint_t> &a,
                              const std::vector<mint_t> &b) {
    if (a.size() == 0 || b.size() == 0) return {};
    const int32_t n = std::bit_ceil(a.size() + b.size() - 1);
    std::vector<mint_t> g(n), h(n);
    for (int32_t i = 0; i < a.size(); ++i) g[i] = a[i];
    for (int32_t i = 0; i < b.size(); ++i) h[i] = b[i];
    ntt<mint_t, root>(g);
    ntt<mint_t, root>(h);
    for (int32_t i = 0; i < n; ++i) g[i] *= h[i];
    intt<mint_t, root>(g);

    std::vector<mint_t> c(a.size() + b.size() - 1);
    for (int32_t i = 0; i < c.size(); ++i) c[i] = g[i];
    return c;
}
