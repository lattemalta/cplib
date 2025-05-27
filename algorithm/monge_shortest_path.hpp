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
 * 最短路が複数存在する場合は、その中で通る辺の本数（経路長）が最小となるものを選ぶ。
 * 返り値の2番目の配列 len[i] は、頂点0から頂点iまでの最小本数の辺数を表す。
 *
 * @param N 頂点数（0からNまで）
 * @param f 辺の重みを返す関数オブジェクト。f(i, j) (i < j) のコストを返す。
 * @return std::pair<std::vector<T>, std::vector<int64_t>>
 *   - first: dp[i] = 頂点0から頂点iまでの最短路長
 *   - second: len[i] = そのとき通る辺の本数（最小）
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
 * auto [dp, len] = monge_shortest_path(N, cost);
 * // dp[j] が 0→j の最短路長, len[j] がそのときの辺の本数
 * ```
 */
template <typename F>
auto monge_shortest_path(const int64_t N, const F& f)
    -> std::pair<std::vector<std::invoke_result_t<F, int64_t, int64_t>>,
                 std::vector<int64_t>> {
    assert(N > 0);

    using T = std::invoke_result_t<F, int64_t, int64_t>;
    constexpr T INF = std::numeric_limits<T>::max();

    std::vector<T> dp(N + 1, INF);
    std::vector<int64_t> amin(N + 1, 0), len(N + 1, 0);

    auto check = [&](const int64_t i, const int64_t k) -> void {
        assert(k < i);
        const T tmp = dp[k] + f(k, i);
        if (tmp < dp[i] || (tmp == dp[i] && len[k] + 1 < len[i])) {
            dp[i] = tmp;
            amin[i] = k;
            len[i] = len[k] + 1;
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
    return {dp, len};
}

/**
 * @brief Monge重み上のK本制約付き最短路長計算
 *
 * 頂点0から頂点Nまで、ちょうどK本の辺を使う最短路長を計算する。
 * 各辺の重み f(i, j) (i < j) がMonge性（f(a, c) + f(b, d) <= f(a, d) + f(b, c)
 * for all a < b < c < d）を満たすことを仮定する。
 *
 * @param N 頂点数（0からNまで）
 * @param K 使う辺の本数（ちょうどK本）
 * @param f 辺の重みを返す関数オブジェクト。f(i, j) (i < j) のコストを返す。
 * @param C abs(f(i, j)) の最大値（探索範囲の上限として使う）
 * @return T 頂点0から頂点Nまで、ちょうどK本の辺を使う最短路長
 *
 * 計算量: O(N log N log C)
 *
 * @usage
 * ```cpp
 * // H[0..N] の高さ配列、コストCで、
 * // ちょうどK本のジャンプで0→Nの最小コスト
 * std::vector<int64_t> H = ...; int64_t C = ...; int64_t N = H.size() - 1;
 * int64_t K = ...;
 * auto cost = [&](int64_t i, int64_t j) {
 *     return (H[j] - H[i]) * (H[j] - H[i]) + C;
 * };
 * int64_t maxC = ...; // abs(cost(i, j)) の最大値
 * auto ans = k_monge_shortest_path(N, K, cost, maxC);
 * // ans が 0→N をちょうどK本の辺で行く最小コスト
 * ```
 */
template <typename F>
auto k_monge_shortest_path(const int64_t N, const int64_t K, const F& f,
                           const std::invoke_result_t<F, int64_t, int64_t>& C)
    -> std::invoke_result_t<F, int64_t, int64_t> {
    using T = std::invoke_result_t<F, int64_t, int64_t>;
    int64_t lb = -3 * static_cast<int64_t>(C),
            ub = 3 * static_cast<int64_t>(C) + 1;

    while (ub - lb > 1) {
        const int64_t mid = (lb + ub) / 2;
        auto f_mid = [&](int64_t i, int64_t j) -> T { return f(i, j) - mid; };
        auto [dp, len] = monge_shortest_path(N, f_mid);
        if (len[N] > K)
            ub = mid;
        else
            lb = mid;
    }
    auto f2 = [&](int64_t i, int64_t j) -> T { return f(i, j) - lb; };
    auto [dp, len] = monge_shortest_path(N, f2);
    return dp[N] + lb * K;
}
