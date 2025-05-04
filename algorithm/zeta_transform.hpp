#include <cassert>
#include <cstdint>
#include <vector>

template <typename T>
std::vector<T> zeta_transform(std::vector<T>& f) {
    const int32_t n = f.size();
    assert((n & (n - 1)) == 0);

    std::vector<T> g = f;
    for (int32_t i = 1; i < n; i <<= 1) {
        for (int32_t j = 0; j < n; j += i << 1) {
            for (int32_t k = 0; k < i; k++) {
                g[j + k + i] += g[j + k];
            }
        }
    }
    return g;
}

template <typename T>
std::vector<T> mobius_transform(std::vector<T>& f) {
    const int32_t n = f.size();
    assert((n & (n - 1)) == 0);

    std::vector<T> g = f;
    for (int32_t i = 1; i < n; i <<= 1) {
        for (int32_t j = 0; j < n; j += i << 1) {
            for (int32_t k = 0; k < i; k++) {
                g[j + k + i] -= g[j + k];
            }
        }
    }
    return g;
}
