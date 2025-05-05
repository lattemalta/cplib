#pragma once

#include <cstdint>
#include <vector>
template <typename mint_t>
struct FactorialTable {
    std::vector<mint_t> fact, ifact, inv;
    FactorialTable(const int64_t n) : fact(n), ifact(n), inv(n) {
        inv[1] = 1;
        constexpr auto mod = mint_t::get_mod();
        for (int32_t i = 2; i < n; i++) {
            inv[i] = -inv[mod % i] * (mod / i);
        }
        fact[0] = 1;
        ifact[0] = 1;
        for (int i = 1; i < n; i++) {
            fact[i] = fact[i - 1] * i;
            ifact[i] = ifact[i - 1] * inv[i];
        }
    }
};