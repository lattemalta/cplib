#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

template <typename flow_t, typename cost_t>
struct PrimalDual {
    struct edge {
        int32_t to;
        flow_t cap;
        cost_t cost;
        int32_t rev;
        bool isrev;
    };

    std::vector<std::vector<edge>> graph;
    std::vector<cost_t> potential, min_cost;
    std::vector<int32_t> prevv, preve;
    static constexpr cost_t INF = std::numeric_limits<cost_t>::max();

    PrimalDual(const int32_t V) : graph(V) {}

    void add_edge(const int32_t from, const int32_t to, const flow_t cap,
                  const cost_t cost) {
        graph[from].emplace_back(
            edge{to, cap, cost, static_cast<int32_t>(graph[to].size()), false});
        graph[to].emplace_back(
            edge{from, 0, -cost, static_cast<int32_t>(graph[from].size()) - 1,
                 true});
    }

    cost_t min_cost_flow(const int32_t s, const int32_t t, flow_t f) {
        const int32_t V = static_cast<int32_t>(graph.size());
        cost_t ret = 0;
        std::priority_queue<std::pair<cost_t, int32_t>,
                            std::vector<std::pair<cost_t, int32_t>>,
                            std::greater<std::pair<cost_t, int32_t>>>
            que;
        potential.assign(V, 0);
        preve.assign(V, -1);
        prevv.assign(V, -1);

        while (f > 0) {
            min_cost.assign(V, INF);
            que.emplace(0, s);
            min_cost[s] = 0;
            while (!que.empty()) {
                const auto [d, v] = que.top();
                que.pop();
                if (min_cost[v] < d) continue;
                for (int32_t i = 0; i < static_cast<int32_t>(graph[v].size());
                     i++) {
                    edge &e = graph[v][i];
                    const cost_t next_cost =
                        min_cost[v] + e.cost + potential[v] - potential[e.to];
                    if (e.cap > 0 && min_cost[e.to] > next_cost) {
                        min_cost[e.to] = next_cost;
                        prevv[e.to] = v, preve[e.to] = i;
                        que.emplace(min_cost[e.to], e.to);
                    }
                }
            }
            if (min_cost[t] == INF) return -1;
            for (int32_t v = 0; v < V; v++) potential[v] += min_cost[v];
            flow_t addflow = f;
            for (int32_t v = t; v != s; v = prevv[v]) {
                addflow = std::min(addflow, graph[prevv[v]][preve[v]].cap);
            }
            f -= addflow;
            ret += addflow * potential[t];
            for (int32_t v = t; v != s; v = prevv[v]) {
                edge &e = graph[prevv[v]][preve[v]];
                e.cap -= addflow;
                graph[v][e.rev].cap += addflow;
            }
        }
        return ret;
    }

    void output() const {
        for (int32_t i = 0; i < static_cast<int32_t>(graph.size()); i++) {
            for (const auto &e : graph[i]) {
                if (e.isrev) continue;
                const auto &rev_e = graph[e.to][e.rev];
                std::cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/"
                          << rev_e.cap + e.cap << ")" << std::endl;
            }
        }
    }
};
