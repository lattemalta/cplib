#pragma once

#include <cstdint>
#include <queue>
#include <ranges>
#include <vector>

struct HopcroftKarp {
    HopcroftKarp(const int32_t n, const int32_t m)
        : graph(n), match(m, -1), used(n) {}

    void add_edge(int32_t u, int32_t v) { graph[u].push_back(v); }

    void bfs() {
        dist.assign(graph.size(), -1);
        std::queue<int32_t> que;
        for (int32_t i : std::views::iota(0uz, graph.size())) {
            if (!used[i]) {
                que.emplace(i);
                dist[i] = 0;
            }
        }

        while (!que.empty()) {
            const auto a = que.front();
            que.pop();
            for (auto &b : graph[a]) {
                const auto c = match[b];
                if (c >= 0 && dist[c] == -1) {
                    dist[c] = dist[a] + 1;
                    que.emplace(c);
                }
            }
        }
    }

    bool dfs(const int32_t a) {
        vv[a] = true;
        for (auto &b : graph[a]) {
            const auto c = match[b];
            if (c < 0 || (!vv[c] && dist[c] == dist[a] + 1 && dfs(c))) {
                match[b] = a;
                used[a] = true;
                return true;
            }
        }
        return false;
    }

    int32_t bipartite_matching() {
        int32_t ret = 0;
        while (true) {
            bfs();
            vv.assign(graph.size(), false);
            int32_t flow = 0;
            for (int32_t i : std::views::iota(0uz, graph.size())) {
                if (!used[i] && dfs(i)) ++flow;
            }
            if (flow == 0) return ret;
            ret += flow;
        }
    }

   private:
    std::vector<std::vector<int32_t>> graph;
    std::vector<int32_t> dist, match, used, vv;
};