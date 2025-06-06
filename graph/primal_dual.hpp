#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

template <typename flow_t>
struct PrimalDual {
   public:
    static constexpr flow_t INF = std::numeric_limits<flow_t>::max();
    struct Result {
        bool feasible;
        flow_t cost;
        std::vector<flow_t> dual;
        std::vector<std::pair<flow_t, flow_t>> slope;
    };
    PrimalDual(const int32_t V)
        : graph(V + 2), b(V), slope{{flow_t(0), flow_t(0)}} {}

    void add_edge(const int32_t from, const int32_t to, const flow_t cap,
                  const flow_t cost) {
        graph[from].emplace_back(
            edge{to, cap, cost, static_cast<int32_t>(graph[to].size()), false});
        graph[to].emplace_back(
            edge{from, 0, -cost, static_cast<int32_t>(graph[from].size()) - 1,
                 true});
    }

    void add_supply(const int32_t v, const flow_t cap) {
        assert(cap >= 0);
        b[v] += cap;
    }
    void add_demand(const int32_t v, const flow_t cap) {
        assert(cap >= 0);
        b[v] -= cap;
    }

    Result min_cost_flow() {
        const int32_t V = static_cast<int32_t>(graph.size());
        const int32_t src = V - 2, sink = V - 1;
        flow_t positive = 0, negative = 0;
        for (int32_t i = 0; i < V - 2; i++) {
            if (b[i] > 0) {
                add_edge(src, i, b[i], 0);
                positive += b[i];
            } else if (b[i] < 0) {
                add_edge(i, sink, -b[i], 0);
                negative += -b[i];
            }
        }
        if (positive != negative) return {};
        flow_t ret = min_cost_flow_impl(src, sink, positive);
        Result res{};
        res.feasible = (ret != INF);
        res.cost = ret;
        bellman_ford();
        res.dual = potential;
        res.slope = slope;

        return res;
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

   private:
    struct edge {
        int32_t to;
        flow_t cap;
        flow_t cost;
        int32_t rev;
        bool isrev;
    };

    std::vector<std::vector<edge>> graph;
    std::vector<flow_t> potential, min_cost;
    std::vector<flow_t> b;
    std::vector<int32_t> prevv, preve;
    std::vector<pair<flow_t, flow_t>> slope;

    void dijkstra(const int32_t s) {
        std::priority_queue<std::pair<flow_t, int32_t>,
                            std::vector<std::pair<flow_t, int32_t>>,
                            std::greater<std::pair<flow_t, int32_t>>>
            que;
        const int32_t V = static_cast<int32_t>(graph.size());
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
                const flow_t next_cost =
                    min_cost[v] + e.cost + potential[v] - potential[e.to];
                if (e.cap > 0 && min_cost[e.to] > next_cost) {
                    min_cost[e.to] = next_cost;
                    prevv[e.to] = v, preve[e.to] = i;
                    que.emplace(min_cost[e.to], e.to);
                }
            }
        }
    }

    void bellman_ford() {
        const int32_t V = static_cast<int32_t>(graph.size());
        potential.assign(V, 0);
        for (int32_t i = 0; i < V - 1; i++) {
            for (int32_t v = 0; v < V; v++) {
                for (auto &e : graph[v]) {
                    if (e.cap == 0) continue;
                    potential[e.to] =
                        std::min(potential[e.to], potential[v] + e.cost);
                }
            }
        }
    }

    flow_t min_cost_flow_impl(const int32_t s, const int32_t t, flow_t f) {
        const int32_t V = static_cast<int32_t>(graph.size());
        flow_t ret = 0;

        bellman_ford();

        preve.assign(V, -1);
        prevv.assign(V, -1);

        while (f > 0) {
            dijkstra(s);
            if (min_cost[t] == INF) return INF;
            for (int32_t v = 0; v < V; v++) potential[v] += min_cost[v];
            flow_t addflow = f;
            for (int32_t v = t; v != s; v = prevv[v]) {
                addflow = std::min(addflow, graph[prevv[v]][preve[v]].cap);
            }
            f -= addflow;
            ret += addflow * potential[t];
            assert(slope.size());
            slope.emplace_back(slope.back().first + addflow,
                               slope.back().second + addflow * potential[t]);

            for (int32_t v = t; v != s; v = prevv[v]) {
                edge &e = graph[prevv[v]][preve[v]];
                e.cap -= addflow;
                graph[v][e.rev].cap += addflow;
            }
        }
        return ret;
    }
};
