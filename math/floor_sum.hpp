#include <cstdint>

/**
 * @brief
 * 次の式の値を求める
 *
 * \sum_{0 <= k < N} floor((A * k + B) / M)
 */
int64_t floor_sum(const int64_t N, const int64_t M, const int64_t A,
                  const int64_t B) {
    const int64_t a1 = A / M;
    const int64_t a2 = A % M;
    const int64_t s = N * (N - 1) / 2 * a1;
    const int64_t b1 = B / M;
    const int64_t b2 = B % M;

    if (a2 == 0) return s + b1 * N;

    const int64_t k = (a2 * (N - 1) + b2) / M;
    return s + N * (k + b1) - floor_sum(k, a2, M, M + a2 - b2 - 1);
}
