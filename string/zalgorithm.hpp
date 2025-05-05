#pragma once

#include <cstdint>
#include <vector>

template <class T>
std::vector<int64_t> zalgorithm(T s) {
    std::vector<int64_t> a(s.size());
    a[0] = s.size();
    int32_t i = 1, j = 0;
    while (i < s.size()) {
        while (i + j < s.size() && s[j] == s[i + j]) j++;
        a[i] = j;
        if (j == 0) {
            i++;
            continue;
        }
        int32_t k = 1;
        while (i + k < s.size() && k + a[k] < j) a[i + k] = a[k], k++;
        i += k;
        j -= k;
    }
    return a;
}
