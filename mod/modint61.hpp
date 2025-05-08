#pragma once

#include <cstdint>

struct ModInt61 {
    static constexpr uint64_t mod = (1ULL << 61) - 1;
    static constexpr uint64_t MASK61 = mod;
    uint64_t a;

    constexpr ModInt61& s(uint64_t vv) {
        a = vv < mod ? vv : vv - mod;
        return *this;
    }

    constexpr ModInt61() : a(0ULL) {}
    constexpr ModInt61(uint32_t x) : a(x) {}
    constexpr ModInt61(uint64_t x) { s((x >> 61) + (x & MASK61)); }
    constexpr ModInt61(int32_t x)
        : a((x < 0 ? x + static_cast<int64_t>(mod) : x)) {}
    constexpr ModInt61(int64_t x)
        : a(((x %= static_cast<int64_t>(mod)) < 0)
                ? (x + static_cast<int64_t>(mod))
                : x) {}

    ModInt61& operator+=(const ModInt61& x) { return s(a + x.a); }
    ModInt61& operator-=(const ModInt61& x) { return s(a + mod - x.a); }
    ModInt61& operator*=(const ModInt61& x) {
        const unsigned __int128 y = static_cast<unsigned __int128>(a) * x.a;
        return s((y >> 61) + (y & mod));
    }
    ModInt61& operator/=(const ModInt61& x) {
        *this *= x.inv();
        return *this;
    }

    ModInt61 operator+(const ModInt61& x) const { return ModInt61(*this) += x; }
    ModInt61 operator-(const ModInt61& x) const { return ModInt61(*this) -= x; }
    ModInt61 operator*(const ModInt61& x) const { return ModInt61(*this) *= x; }
    ModInt61 operator/(const ModInt61& x) const { return ModInt61(*this) /= x; }
    bool operator==(const ModInt61& x) const { return a == x.a; }
    bool operator!=(const ModInt61& x) const { return a != x.a; }
    bool operator<(const ModInt61& x) const { return a < x.a; }

    ModInt61 operator-() const { return ModInt61() - *this; }
    ModInt61 pow(int64_t n) const {
        ModInt61 res(1), x(*this);
        while (n) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }

    ModInt61 inv() const { return pow(mod - 2); }
    static constexpr uint64_t get_mod() { return mod; }
};
std::istream& operator>>(std::istream& in, ModInt61& a) { return (in >> a.a); }
std::ostream& operator<<(std::ostream& out, const ModInt61& a) {
    return (out << a.a);
}

using modint61 = ModInt61;
