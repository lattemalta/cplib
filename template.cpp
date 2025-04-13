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

int Yes() {
    std::cout << "Yes" << std::endl;
    return 0;
}
int No() {
    std::cout << "No" << std::endl;
    return 0;
}

void print() {}

template <class Head, class... Tail>
void print(Head&& head, Tail&&... tail) {
    std::cout << head;
    if (sizeof...(Tail)) {
        std::cout << " ";
    }
    print(std::forward<Tail>(tail)...);
}

template <class... Args>
void println(Args&&... args) {
    print(args..., '\n');
}

#define SHOW(...)                                             \
    SHOW_IMPL(__VA_ARGS__, SHOW6, SHOW5, SHOW4, SHOW3, SHOW2, \
              SHOW1)(__VA_ARGS__)
#define SHOW_IMPL(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define SHOW1(x) println(#x, "=", (x)), flush()
#define SHOW2(x, y) println(#x, "=", (x), #y, "=", (y)), flush()
#define SHOW3(x, y, z) \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z)), flush()
#define SHOW4(x, y, z, w) \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w)), flush()
#define SHOW5(x, y, z, w, v)                                                 \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
            (v)),                                                            \
        flush()
#define SHOW6(x, y, z, w, v, u)                                              \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
            (v), #u, "=", (u)),                                              \
        flush()

#define int long long

#define eb emplace_back
#define all(v) (v).begin(), (v).end()
#define fi first
#define se second

using vint = std::vector<int>;
using pint = std::pair<int, int>;
using vpint = std::vector<pint>;

using std::cin, std::cout, std::cerr, std::endl;

using std::abs, std::pow, std::sqrt;

using std::min, std::max, std::swap;

using std::bit_ceil, std::bit_floor, std::bit_width, std::countr_zero,
    std::countr_one, std::popcount;

using std::pair, std::tuple;

using std::array, std::bitset, std::string, std::vector, std::deque;
using std::queue, std::priority_queue;
using std::set, std::multiset, std::map;

using std::ranges::sort, std::ranges::reverse;
using std::views::iota;

signed main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // torichan

    return 0;
}
