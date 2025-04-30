#pragma once

#include <cstdint>
#include <vector>

template <class T>
struct BinaryIndexedTree {
    std::vector<T> data;

    BinaryIndexedTree(int32_t sz) { data.assign(++sz, 0); }

    T sum(int32_t k) const {
        T ret = 0;
        for (++k; k > 0; k -= k & -k) ret += data[k];
        return (ret);
    }
    T sum(const int32_t l, const int32_t r) const {
        return sum(r) - sum(l - 1);
    }

    void add(int32_t k, const T x) {
        for (++k; k < data.size(); k += k & -k) data[k] += x;
    }
};
