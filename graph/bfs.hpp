#pragma once

#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

#include "graph/graph.hpp"

template <typename T = int64_t, bool directed = false>
std::vector<T> bfs(const Graph<T, directed>& G, const int32_t s) {
    constexpr T INF = std::numeric_limits<T>::max();
    const int32_t V = G.size();

    std::vector<T> dist(V, INF);
    dist[s] = 0;

    std::queue<int32_t> que;
    que.emplace(s);

    while (!que.empty()) {
        const auto v = que.front();
        que.pop();

        for (const auto& e : G[v]) {
            if (dist[e.to] == INF) {
                dist[e.to] = dist[v] + 1;
                que.emplace(e.to);
            }
        }
    }
    return dist;
}
