#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

template <typename flow_t>
struct Dinic {
    static constexpr flow_t INF = std::numeric_limits<flow_t>::max();

    struct edge {
        int32_t to;
        flow_t cap;
        int32_t rev;
        bool isrev;
        int32_t idx;
    };

    std::vector<std::vector<edge> > graph;
    std::vector<int32_t> min_cost, iter;

    Dinic(const int32_t V) : graph(V) {}

    void add_edge(const int32_t from, const int32_t to, const flow_t cap,
                  const int32_t idx = -1) {
        graph[from].emplace_back(
            (edge){to, cap, (int32_t)graph[to].size(), false, idx});
        graph[to].emplace_back(
            (edge){from, 0, (int32_t)graph[from].size() - 1, true, idx});
    }

    bool bfs(const int32_t s, const int32_t t) {
        min_cost.assign(graph.size(), -1);
        std::queue<int32_t> que;
        min_cost[s] = 0;
        que.push(s);
        while (!que.empty() && min_cost[t] == -1) {
            const auto p = que.front();
            que.pop();
            for (auto &e : graph[p]) {
                if (e.cap > 0 && min_cost[e.to] == -1) {
                    min_cost[e.to] = min_cost[p] + 1;
                    que.push(e.to);
                }
            }
        }
        return min_cost[t] != -1;
    }

    flow_t dfs(const int32_t idx, const int32_t t, flow_t flow) {
        if (idx == t) return flow;
        for (int32_t &i = iter[idx]; i < graph[idx].size(); i++) {
            edge &e = graph[idx][i];
            if (e.cap > 0 && min_cost[idx] < min_cost[e.to]) {
                flow_t d = dfs(e.to, t, min(flow, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    flow_t max_flow(const int32_t s, const int32_t t) {
        flow_t flow = 0;
        while (bfs(s, t)) {
            iter.assign(graph.size(), 0);
            flow_t f = 0;
            while ((f = dfs(s, t, INF)) > 0) flow += f;
        }
        return flow;
    }

    void output() {
        for (int32_t i = 0; i < graph.size(); i++) {
            for (auto &e : graph[i]) {
                if (e.isrev) continue;
                auto &rev_e = graph[e.to][e.rev];
                std::cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/"
                          << e.cap + rev_e.cap << ")" << std::endl;
            }
        }
    }
};
