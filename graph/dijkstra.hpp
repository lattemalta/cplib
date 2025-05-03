#pragma once

#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

#include "graph/graph.hpp"

template <typename T, bool directed>
std::vector<T> dijkstra(const Graph<T, directed>& G, const int32_t s) {
    using P = std::pair<T, int32_t>;
    const T inf = std::numeric_limits<T>::max();
    std::vector<T> dist(G.size(), inf);
    dist[s] = 0;
    std::priority_queue<P, std::vector<P>, std::greater<P>> que;
    que.emplace(0, s);
    while (!que.empty()) {
        auto [d, v] = que.top();
        que.pop();
        if (dist[v] < d) continue;
        for (const auto& e : G[v]) {
            if (dist[e.to] > d + e.cost) {
                dist[e.to] = d + e.cost;
                que.emplace(dist[e.to], e.to);
            }
        }
    }
    return dist;
}