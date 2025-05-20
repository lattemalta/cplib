#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

template <typename F>
auto monotone_minima(const int64_t H, const int64_t W, F&& f) -> std::vector<
    std::pair<typename std::invoke_result_t<F&, int64_t, int64_t>, int64_t>> {
    using T = std::invoke_result_t<F&, int64_t, int64_t>;

    std::vector<std::pair<T, int64_t>> res(H);
    auto dfs = [&](auto&& self, const int64_t top, const int64_t bottom,
                   const int64_t left, const int64_t right) {
        const int64_t mid_line = (top + bottom) / 2;
        T min_val{};
        int64_t min_idx = -1;

        for (int64_t j = left; j < right; j++) {
            const T tmp = f(mid_line, j);
            if (min_idx == -1 || tmp < min_val) {
                min_val = tmp;
                min_idx = j;
            }
        }
        res[mid_line] = {min_val, min_idx};
        if (top + 1 >= bottom) return;
        self(self, top, mid_line, left, min_idx + 1);
        self(self, mid_line, bottom, min_idx, right);
    };
    dfs(dfs, 0, H, 0, W);
    return res;
}
