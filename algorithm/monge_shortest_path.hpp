#pragma once

#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

/**
 * @brief 簡易版LARSCHによるMonge重み上の最短路長計算
 *
 * 頂点0から頂点Nまでの最短路長を計算する。
 * 各辺の重み f(i, j) (i < j) がMonge性（f(a, c) + f(b, d) <= f(a, d) + f(b, c)
 * for all a < b < c < d）を満たすことを仮定する。
 *
 * @param N 頂点数（0からNまで）
 * @param f 辺の重みを返す関数オブジェクト。f(i, j) (i < j) のコストを返す。
 * @return std::vector<T> dp : dp[i] = 頂点0から頂点iまでの最短路長
 *
 * 計算量: O(N log N)
 *
 * @usage
 * ```cpp
 * // H[0..N] の高さ配列、コストCで、
 * // dp[j] = min_{i<j} dp[i] + (H[j]-H[i])^2 + C
 * std::vector<int64_t> H = ...; int64_t C = ...; int64_t N = H.size() - 1;
 * auto cost = [&](int64_t i, int64_t j) {
 *     return (H[j] - H[i]) * (H[j] - H[i]) + C;
 * };
 * auto dp = monge_shortest_path(N, cost);
 * // dp[j] が 0→j の最短路長
 * ```
 */
template <typename F>
auto monge_shortest_path(const int64_t N, const F& f)
    -> std::vector<std::invoke_result_t<F, int64_t, int64_t>> {
    assert(N > 0);

    using T = std::invoke_result_t<F, int64_t, int64_t>;
    constexpr T INF = std::numeric_limits<T>::max();

    std::vector<T> dp(N + 1, INF);
    std::vector<int64_t> amin(N + 1, 0);

    auto check = [&](const int64_t i, const int64_t k) -> void {
        assert(k < i);
        const T tmp = dp[k] + f(k, i);
        if (tmp < dp[i]) {
            dp[i] = tmp;
            amin[i] = k;
        }
    };
    auto solve = [&](auto&& self, const int64_t l, const int64_t r) -> void {
        if (l + 1 == r) return;

        const int64_t m = (l + r) / 2;
        for (int64_t k = amin[l]; k <= amin[r]; k++) check(m, k);
        self(self, l, m);
        for (int64_t k = l + 1; k <= m; k++) check(r, k);
        self(self, m, r);
    };
    dp[0] = 0;
    check(N, 0);
    solve(solve, 0, N);
    return dp;
}
