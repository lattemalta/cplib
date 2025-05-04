#pragma once

#include <cstdint>
#include <map>

std::map<int64_t, int64_t> prime_factor(int64_t n) {
    std::map<int64_t, int64_t> ret;
    for (int64_t i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            ret[i]++;
            n /= i;
        }
    }
    if (n > 1) {
        ret[n]++;
    }
    return ret;
}
