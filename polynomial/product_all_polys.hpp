#pragma once

#include <cstdint>
#include <queue>
#include <utility>
#include <vector>

template <typename Poly>
Poly product_all_polys(std::vector<Poly> ps) {
    if (ps.size() == 0) return Poly{1};
    std::priority_queue<std::pair<int32_t, int32_t>,
                        std::vector<std::pair<int32_t, int32_t>>,
                        std::greater<std::pair<int32_t, int32_t>>>
        que;
    for (int32_t i = 0; i < ps.size(); i++) que.emplace(ps[i].size(), i);

    while (que.size() > 1) {
        auto i = que.top().second;
        que.pop();
        auto j = que.top().second;
        que.pop();
        ps[j] *= ps[i];
        que.emplace(ps[j].size(), j);
    }
    return ps[que.top().second];
}
