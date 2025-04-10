#include <bits/stdc++.h>

#define int long long

#define eb emplace_back
#define all(v) (v).begin(), (v).end()
#define fi first
#define se second

using vint = std::vector<int>;
using pint = std::pair<int, int>;
using vpint = std::vector<pint>;

template <typename A, typename B>
inline void chmin(A& a, B b) {
    if (a > b) a = b;
}
template <typename A, typename B>
inline void chmax(A& a, B b) {
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

inline int topbit(unsigned long long x) {
    return x ? 63 - __builtin_clzll(x) : -1;
}

inline int popcount(unsigned long long x) { return __builtin_popcountll(x); }

inline int parity(unsigned long long x) { return __builtin_parity(x); }

using std::abs, std::pow, std::sqrt;
using std::array, std::vector, std::string, std::queue, std::deque;
using std::cin, std::cout, std::cerr, std::endl;
using std::min, std::max, std::swap;
using std::pair, std::tuple, std::bitset;
using std::popcount;
using std::priority_queue, std::set, std::multiset, std::map;
using std::views::iota;

signed main() {
    // torichan

    return 0;
}
