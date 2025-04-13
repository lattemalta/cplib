#pragma once

#include <cstdint>
#include <vector>

struct UnionFindTree {
    UnionFindTree(const int32_t n) : data(n, -1) {}

    int32_t find(const int32_t x) {
        if (data[x] < 0) return x;
        return data[x] = find(data[x]);
    }
    bool unite(int32_t x, int32_t y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (data[x] > data[y]) std::swap(x, y);
        data[x] += data[y];
        data[y] = x;
        return true;
    }
    int32_t size(const int32_t x) { return -data[find(x)]; }

   private:
    std::vector<int32_t> data;
};
