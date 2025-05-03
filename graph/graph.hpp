#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

template <typename T>
struct Edge {
    int from, to;
    T cost;
    int id;
};

template <typename T = int64_t, bool directed = false>
struct Graph {
    static constexpr bool is_directed = directed;
    int V, E;
    using cost_type = T;
    using edge_type = Edge<T>;

    std::vector<std::vector<edge_type>> adj;

    Graph() : V(0), E(0), adj() {}
    Graph(const int V) : V(V), E(0), adj(V) {}

    void add_edge(int from, int to, T cost = 1) {
        assert(0 <= from && from < V && 0 <= to && to < V);
        const auto id = E++;
        adj[from].emplace_back(edge_type{from, to, cost, id});
        if (!directed) {
            adj[to].emplace_back(edge_type{to, from, cost, id});
        }
    }

    std::vector<edge_type>& operator[](const int v) { return adj[v]; }
    std::vector<edge_type> const& operator[](const int v) const {
        return adj[v];
    }
    int32_t size() const { return V; };
};
