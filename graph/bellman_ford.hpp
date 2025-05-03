#pragma once

#include <cstdint>
#include <limits>
#include <vector>

#include "graph/graph.hpp"

template <typename T, bool directed>
std::vector<T> bellman_ford(const Graph<T, directed>& G, const int32_t s) {
    int V = G.size();
    constexpr T INF = std::numeric_limits<T>::max();

    std::vector<T> dist(V, INF);
    dist[s] = 0;

    for (int32_t t = 0; t < V - 1; t++) {
        for (int32_t v = 0; v < V; v++) {
            for (auto& e : G[v]) {
                if (dist[e.from] == INF) continue;
                dist[e.to] = std::min(dist[e.to], dist[e.from] + e.cost);
            }
        }
    }
    for (int32_t v = 0; v < V; v++) {
        for (auto& e : G[v]) {
            if (dist[e.from] == INF) continue;
            if (dist[e.to] > dist[e.from] + e.cost) return {};
        }
    }
    return dist;
}
