#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph/graph.hpp"
template <typename T, typename index_t>
std::vector<T> minimum_steiner_tree(const Graph<T>& G,
                                    const std::vector<index_t>& terminals) {
    const int32_t N = static_cast<int32_t>(G.size());
    const int32_t K = static_cast<int32_t>(terminals.size());

    constexpr T INF = std::numeric_limits<T>::max();

    std::vector<std::vector<T>> dp(1 << K, std::vector<T>(N, INF));
    for (int32_t i = 0; i < K; i++) dp[1 << i][terminals[i]] = 0;

    for (int32_t s = 1; s < (1 << K); s++) {
        // t | u = s , t & u = 0, t != 0, u != 0
        int32_t t = (s - 1) & s;
        while (t) {
            const int32_t u = t ^ s;
            for (int32_t i = 0; i < N; i++) {
                if (dp[t][i] == INF || dp[u][i] == INF) continue;
                dp[s][i] = std::min(dp[s][i], dp[t][i] + dp[u][i]);
            }
            t = (t - 1) & s;
        }

        std::priority_queue<std::pair<T, int32_t>,
                            std::vector<std::pair<T, int32_t>>,
                            std::greater<std::pair<T, int32_t>>>
            que;
        for (int32_t i = 0; i < N; i++) que.emplace(dp[s][i], i);
        while (que.size()) {
            auto [d, v] = que.top();
            que.pop();
            if (dp[s][v] < d) continue;
            for (auto& e : G[v]) {
                if (dp[s][e.to] > d + e.cost) {
                    dp[s][e.to] = d + e.cost;
                    que.emplace(dp[s][e.to], e.to);
                }
            }
        }
    }

    return dp[(1 << K) - 1];
}
