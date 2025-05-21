#pragma once

#include <cstdint>
#include <utility>
#include <vector>

template <typename T, uint64_t K>
struct TopK {
    using value_type = vector<pair<T, int64_t>>;
    static constexpr value_type op(const value_type &a, const value_type &b) {
        std::vector<std::pair<T, int64_t>> c;
        int32_t ia = 0, ib = 0;
        while (ia < a.size() || ib < b.size()) {
            if (ib == b.size()) {
                c.emplace_back(a[ia++]);
                continue;
            }
            if (ia == a.size()) {
                c.emplace_back(b[ib++]);
                continue;
            }
            if (a[ia].first < b[ib].first) {
                c.emplace_back(b[ib++]);
                continue;
            }
            if (a[ia].first > b[ib].first) {
                c.emplace_back(a[ia++]);
                continue;
            }
            c.emplace_back(a[ia].first, a[ia].second + b[ib].second);

            ia++;
            ib++;
        }
        while (c.size() > K) c.pop_back();
        return c;
    }
    static constexpr value_type e() { return {}; }
    static constexpr bool is_commutative = true;
};
