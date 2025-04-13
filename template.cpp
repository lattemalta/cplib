#include <bits/stdc++.h>

template <typename A, typename B>
inline void chmin(A& a, const B b) {
    if (a > b) a = b;
}
template <typename A, typename B>
inline void chmax(A& a, const B b) {
    if (a < b) a = b;
}

template <class A, class B>
std::ostream& operator<<(std::ostream& ost, const std::pair<A, B>& p) {
    ost << "{" << p.first << "," << p.second << "}";
    return ost;
}

template <class T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& v) {
    ost << "{";
    for (int i = 0; i < v.size(); i++) {
        if (i) ost << ",";
        ost << v[i];
    }
    ost << "}";
    return ost;
}

inline int32_t parity(uint64_t x) { return __builtin_parity(x); }

#define int long long

#define eb emplace_back
#define all(v) (v).begin(), (v).end()
#define fi first
#define se second

using vint = std::vector<int>;
using pint = std::pair<int, int>;
using vpint = std::vector<pint>;

using std::abs, std::pow, std::sqrt;
using std::array, std::vector, std::string, std::queue, std::deque;
using std::bit_ceil, std::bit_floor, std::bit_width, std::countr_zero,
    std::countr_one, std::popcount;
using std::cin, std::cout, std::cerr, std::endl;
using std::min, std::max, std::swap;
using std::pair, std::tuple, std::bitset;
using std::priority_queue, std::set, std::multiset, std::map;
using std::views::iota;

signed main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // torichan

    return 0;
}
