#pragma once

#include <cstdint>
#include <vector>
template <typename mint_t>
struct FactorialTable {
    FactorialTable(const int64_t n) : fact_(n), ifact_(n), inv_(n) {
        inv_[1] = 1;
        constexpr auto mod = mint_t::get_mod();
        for (int32_t i = 2; i < n; i++) {
            inv_[i] = -inv_[mod % i] * (mod / i);
        }
        fact_[0] = 1;
        ifact_[0] = 1;
        for (int i = 1; i < n; i++) {
            fact_[i] = fact_[i - 1] * i;
            ifact_[i] = ifact_[i - 1] * inv_[i];
        }
    }
    mint_t fact(const int64_t n) const { return fact_[n]; }
    mint_t ifact(const int64_t n) const { return ifact_[n]; }
    mint_t inv(const int64_t n) const { return inv_[n]; }
    mint_t binom(const int64_t n, const int64_t k) const {
        if (n < k || k < 0) return 0;
        return fact_[n] * ifact_[k] * ifact_[n - k];
    }

   private:
    std::vector<mint_t> fact_, ifact_, inv_;
};
