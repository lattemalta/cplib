#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>

#include "algorithm/monotone_minima.hpp"

template <typename T>
std::vector<T> convex_max_plus_convolution(const std::vector<T>& a,
                                           const std::vector<T>& b) {
    // a: convex

    assert(a.size() > 0);
    assert(b.size() > 0);

    constexpr T INF = std::numeric_limits<T>::max();

    auto res = monotone_minima(a.size() + b.size() - 1, b.size(),
                               [&](int64_t k, int64_t j) -> T {
                                   const int64_t i = k - j;
                                   if (i < 0 || i >= (int64_t)a.size())
                                       return INF;
                                   return -(a[i] + b[j]);
                               });
    std::vector<T> c(a.size() + b.size() - 1);
    for (int64_t i = 0; i < (int64_t)c.size(); ++i) c[i] = -res[i].first;
    return c;
}
