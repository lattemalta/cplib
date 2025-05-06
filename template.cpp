#include <bits/stdc++.h>

template <class A, class B>
std::ostream& operator<<(std::ostream& ost, const std::pair<A, B>& p) {
    ost << "(" << p.first << ", " << p.second << ")";
    return ost;
}

template <class T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& v) {
    ost << "[";
    for (int i = 0; i < v.size(); i++) {
        if (i) ost << ", ";
        ost << v[i];
    }
    ost << "]";
    return ost;
}

template <class T>
std::ostream& operator<<(std::ostream& ost, const std::set<T>& s) {
    ost << "{";
    bool first = true;
    for (const auto& v : s) {
        if (!first) ost << ", ";
        ost << v;
        first = false;
    }
    ost << "}";
    return ost;
}

template <class T, class U>
std::ostream& operator<<(std::ostream& ost, const std::map<T, U>& m) {
    ost << "{";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) ost << ", ";
        ost << k << ":" << v;
        first = false;
    }
    ost << "}";
    return ost;
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
#define SHOW1(x) println(#x, "=", (x))
#define SHOW2(x, y) println(#x, "=", (x), #y, "=", (y))
#define SHOW3(x, y, z) println(#x, "=", (x), #y, "=", (y), #z, "=", (z))
#define SHOW4(x, y, z, w) \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w))
#define SHOW5(x, y, z, w, v)                                                 \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
            (v))
#define SHOW6(x, y, z, w, v, u)                                              \
    println(#x, "=", (x), #y, "=", (y), #z, "=", (z), #w, "=", (w), #v, "=", \
            (v), #u, "=", (u))

int Yes() {
    println("Yes");
    return 0;
}
int No() {
    println("No");
    return 0;
}

#define FOR1(a) for (int64_t _ = 0; _ < int64_t(a); ++_)
#define FOR2(i, a) for (int64_t i = 0; i < int64_t(a); ++i)
#define FOR3(i, a, b) for (int64_t i = a; i < int64_t(b); ++i)
#define FOR4(i, a, b, c) for (int64_t i = a; i < int64_t(b); i += (c))
#define FOR1_R(a) for (int64_t i = (a) - 1; i >= int64_t(0); --i)
#define FOR2_R(i, a) for (int64_t i = (a) - 1; i >= int64_t(0); --i)
#define FOR3_R(i, a, b) for (int64_t i = (b) - 1; i >= int64_t(a); --i)
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define rep(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define rep_r(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

template <typename A, typename B>
inline void chmin(A& a, const B b) {
    if (a > b) a = b;
}
template <typename A, typename B>
inline void chmax(A& a, const B b) {
    if (a < b) a = b;
}

inline int32_t __popcount(int64_t x) {
    return std::popcount(static_cast<uint64_t>(x));
}

#define eb emplace_back
#define all(v) (v).begin(), (v).end()
#define fi first
#define se second

#define int long long  // <= 64bit ミ(°_°)ミ

using vint = std::vector<int>;
using pint = std::pair<int, int>;
using vpint = std::vector<pint>;
using namespace std;

constexpr int INF = 1ll << 61;

signed main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // torichan

    return 0;
}
