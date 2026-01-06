#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <vector>

#include "convolution/number_theoretic_transform.hpp"

// todo: calc root from mint_t::get_mod()
template <class mint_t>
struct Polynomial : private std::vector<mint_t> {
    using std::vector<mint_t>::vector;
    using std::vector<mint_t>::size;
    using std::vector<mint_t>::resize;
    using std::vector<mint_t>::operator[];
    using std::vector<mint_t>::begin;
    using std::vector<mint_t>::end;

    Polynomial& operator+=(const Polynomial& x) {
        if (this->size() < x.size()) this->resize(x.size());
        for (auto i = 0; i < this->size(); ++i) (*this)[i] += x[i];
        return *this;
    }
    Polynomial& operator-=(const Polynomial& x) {
        if (this->size() < x.size()) this->resize(x.size());
        for (auto i = 0; i < this->size(); ++i) (*this)[i] -= x[i];
        return *this;
    }
    Polynomial& operator*=(const Polynomial& x) {
        auto res = convolute(*this, x);
        return *this = Polynomial(res.begin(), res.end());
    }
    Polynomial& operator*=(const mint_t& x) {
        for (auto& e : *this) e *= x;
        return *this;
    }
    Polynomial operator/=(const mint_t& x) { return (*this) *= x.inv(); }
    Polynomial operator+(const Polynomial& x) const {
        return Polynomial(*this) += x;
    }
    Polynomial operator-(const Polynomial& x) const {
        return Polynomial(*this) -= x;
    }
    Polynomial operator*(const Polynomial& x) const {
        return Polynomial(*this) *= x;
    }
    Polynomial operator*(const mint_t& x) const {
        return Polynomial(*this) *= x;
    }
    Polynomial operator/(const mint_t& x) const {
        return Polynomial(*this) /= x;
    }
    Polynomial operator-() const { return Polynomial() - *this; }

    mint_t coef(const std::size_t i) const {
        return (i < this->size()) ? (*this)[i] : 0;
    }

    Polynomial pre(const std::size_t n) const {
        Polynomial res(this->begin(),
                       this->begin() + std::min(n, this->size()));
        res.resize(n);
        return res;
    }

    Polynomial inv(const std::optional<std::size_t> n_ = std::nullopt) const {
        const auto n = n_.value_or(this->size());
        assert(this->coef(0) != 0);
        Polynomial res{this->coef(0).inv()};

        for (auto d = 1; d < n; d <<= 1) {
            /*
            g = inv(f) mod x^d
            g':= inv(f) mod x^2d
            (g' - g)^2 = 0 (mod x^2d)
            => g'^2 - 2g'g + g^2 = 0 (mod x^2d)
            => g' - 2g + fg^2 = 0 (mod x^2d)
            => g' = 2g - fg^2 = g - (fg - 1)g  (mod x^2d)
            */
            auto &&f = this->pre(2 * d), &&g = res.pre(2 * d);

            // calc -(fg - 1)
            ntt(f);
            ntt(g);
            for (auto i = 0; i < 2 * d; i++) f[i] *= g[i];
            intt(f);
            for (auto i = 0; i < d; i++) {
                f[i] = 0;
                f[i + d] = -f[i + d];
            }

            // calc g - (fg - 1)g
            ntt(f);
            for (auto i = 0; i < 2 * d; i++) f[i] *= g[i];
            intt(f);
            for (auto i = 0; i < d; i++) f[i] = res[i];
            std::swap(res, f);
        }
        return res.pre(n);
    }

    Polynomial differential(
        const std::optional<std::size_t> n_ = std::nullopt) const {
        const auto n = n_.value_or(this->size() ? this->size() - 1 : 0);
        Polynomial res(n);
        for (auto i = 1; i < this->size() && i <= n; i++) {
            res[i - 1] = (*this)[i] * i;
        }
        return res;
    }

    Polynomial integral(
        const std::optional<std::size_t> n_ = std::nullopt) const {
        const auto n = n_.value_or(this->size() + 1);
        Polynomial res(n);

        // calc i^-1
        const auto mod = mint_t::get_mod();
        if (n > 1) res[1] = 1;
        for (auto i = 2; i < n; ++i) res[i] = -res[mod % i] * (mod / i);

        for (auto i = 0; i < size() && i + 1 < n; i++) {
            res[i + 1] *= (*this)[i];
        }
        return res;
    }

    Polynomial log(const std::optional<std::size_t> n_ = std::nullopt) const {
        const auto n = n_.value_or(this->size());
        assert(this->coef(0) == 1);

        return (this->differential(n - 1) * this->inv(n - 1)).integral(n);
    }

    Polynomial exp(const std::optional<std::size_t> n_ = std::nullopt) {
        const auto n = n_.value_or(this->size());
        Polynomial f0{1}, g0{1};
        Polynomial F0{1};
        for (auto d = 1; d < n; d <<= 1) {
            auto G0 = g0;
            ntt(G0);
            Polynomial Delta(d);
            for (auto i = 0; i < d; i++) Delta[i] = F0[i] * G0[i];
            intt(Delta);
            Delta[0] -= 1;
            Polynomial delta(2 * d);
            for (auto i = 0; i < d; i++) delta[d + i] = Delta[i];

            Polynomial epsilon(2 * d);

            auto DF0 = f0.differential(d);
            ntt(DF0);
            for (auto i = 0; i < d; i++) DF0[i] *= G0[i];
            intt(DF0);
            for (auto i = 0; i < d - 1; i++) {
                epsilon[i] += this->coef(i + 1) * (i + 1);
                epsilon[i + d] += DF0[i] - this->coef(i + 1) * (i + 1);
            }
            epsilon[d - 1] += DF0[d - 1];

            Delta = delta;
            ntt(Delta);
            Polynomial DH0 = this->differential(d - 1);
            DH0.resize(2 * d);
            ntt(DH0);
            for (auto i = 0; i < 2 * d; i++) Delta[i] *= DH0[i];
            intt(Delta);
            for (auto i = 0; i < d; i++) epsilon[i + d] -= Delta[i + d];

            epsilon = epsilon.integral(2 * d) - pre(2 * d);

            Polynomial Epsilon = epsilon;
            ntt(Epsilon);
            for (auto i = 0; i < d; i++) {
                DH0[i] = f0[i];
                DH0[i + d] = 0;
            }
            ntt(DH0);
            for (auto i = 0; i < 2 * d; i++) Epsilon[i] *= DH0[i];
            intt(Epsilon);
            f0.resize(2 * d);
            for (auto i = 0; i < d; i++) f0[i + d] -= Epsilon[i + d];
            // f0=(f0-epsilon*f0).pre(2*d);

            if (2 * d >= n) break;

            G0.resize(2 * d);
            for (auto i = 0; i < d; i++) G0[i] = g0[i];
            ntt(G0);
            F0 = f0;
            ntt(F0);
            Polynomial T(2 * d);
            for (auto i = 0; i < 2 * d; i++) T[i] = F0[i] * G0[i];
            intt(T);
            for (auto i = 0; i < d; i++) {
                T[i] = 0;
                T[i + d] = -T[i + d];
            }
            ntt(T);
            for (auto i = 0; i < 2 * d; i++) T[i] *= G0[i];
            intt(T);
            for (auto i = 0; i < d; i++) T[i] = g0[i];
            g0 = T;
        }
        return f0.pre(n);
    }
    Polynomial pow(const int64_t M,
                   const std::optional<std::size_t> n_ = std::nullopt) {
        const auto n = n_.value_or(this->size());

        if (M == 0) {
            Polynomial res(n);
            res[0] = 1;
            return res;
        }

        int32_t nz = 0;
        while (nz < this->size() && (*this)[nz] == 0) ++nz;
        if (nz == this->size()) return Polynomial(n);
        if (nz && (n + nz - 1) / nz <= M) {
            return Polynomial(n);
        }

        Polynomial res((*this).begin() + nz,
                       (*this).begin() + std::min(n, this->size()));
        auto c = res[0];
        res *= c.inv();
        res = res.log(n);
        res *= M;
        res = res.exp(n);
        res *= c.pow(M);

        Polynomial res2(n);
        for (auto i = 0; i < n - nz * M; i++) res2[nz * M + i] = res[i];
        return res2;
    }
    Polynomial inv_sparse(const std::optional<std::size_t> n_ = std::nullopt) {
        const auto n = n_.value_or(this->size());
        assert(this->coef(0) != 0);

        if (n == 0) {
            return Polynomial();
        }

        const auto inv_val = this->coef(0).inv();
        Polynomial res(n);
        res[0] = inv_val;

        std::vector<std::size_t> non_zero_pos;
        for (int32_t i = 1; i < this->size(); i++)
            if (this->coef(i) != 0) non_zero_pos.emplace_back(i);

        for (int32_t i = 1; i < n; i++) {
            for (auto j : non_zero_pos) {
                if (j <= i) res[i] += this->coef(j) * res[i - j];
            }
            res[i] *= -inv_val;
        }
        return res;
    }
};

template <class T>
std::ostream& operator<<(std::ostream& ost, const Polynomial<T>& v) {
    ost << "[";
    for (int32_t i = 0; i < v.size(); i++) {
        if (i) ost << ", ";
        ost << v[i];
    }
    ost << "]";
    return ost;
}
