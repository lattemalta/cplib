#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

class SuffixArray {
    void create_begin_bucket(const std::vector<int32_t> &v,
                             std::vector<int32_t> &bucket) {
        fill(bucket.begin(), bucket.end(), 0);
        for (int32_t i = 0; i < v.size(); i++) bucket[v[i]]++;
        int32_t sum = 0;
        for (int32_t i = 0; i < bucket.size(); i++) {
            bucket[i] += sum;
            std::swap(sum, bucket[i]);
        }
    }

    void create_end_bucket(const std::vector<int32_t> &v,
                           std::vector<int32_t> &bucket) {
        fill(bucket.begin(), bucket.end(), 0);
        for (int32_t i = 0; i < v.size(); i++) bucket[v[i]]++;
        for (int32_t i = 1; i < bucket.size(); i++) bucket[i] += bucket[i - 1];
    }

    void induced_sort(const std::vector<int32_t> &v, std::vector<int32_t> &sa,
                      std::vector<int32_t> &bucket,
                      const std::vector<int32_t> &is_l) {
        create_begin_bucket(v, bucket);
        for (int32_t i = 0; i < v.size(); i++)
            if (sa[i] > 0 && is_l[sa[i] - 1])
                sa[bucket[v[sa[i] - 1]]++] = sa[i] - 1;
    }

    void invert_induced_sort(const std::vector<int32_t> &v,
                             std::vector<int32_t> &sa,
                             std::vector<int32_t> &bucket,
                             const std::vector<int32_t> &is_l) {
        create_end_bucket(v, bucket);
        for (int32_t i = (int32_t)v.size() - 1; i >= 0; i--)
            if (sa[i] > 0 && !is_l[sa[i] - 1])
                sa[--bucket[v[sa[i] - 1]]] = sa[i] - 1;
    }

    std::vector<int32_t> sa_is_impl(const std::vector<int32_t> &v,
                                    const int32_t mv) {
        if (v.size() == 1) return {0};

        std::vector<int32_t> is_l(v.size());
        std::vector<int32_t> bucket(mv + 1);
        std::vector<int32_t> sa(v.size(), -1);
        auto is_lms = [&](const int32_t x) -> bool {
            return x > 0 && is_l[x - 1] && !is_l[x];
        };

        is_l[v.size() - 1] = 0;
        for (int32_t i = (int32_t)v.size() - 2; i >= 0; i--)
            is_l[i] = v[i] > v[i + 1] || (v[i] == v[i + 1] && is_l[i + 1]);
        create_end_bucket(v, bucket);
        for (int32_t i = 0; i < v.size(); i++)
            if (is_lms(i)) sa[--bucket[v[i]]] = i;
        induced_sort(v, sa, bucket, is_l);
        invert_induced_sort(v, sa, bucket, is_l);

        int32_t cur = 0;
        std::vector<int32_t> order(v.size());
        for (int32_t i = 0; i < v.size(); i++)
            if (is_lms(i)) order[i] = cur++;

        std::vector<int32_t> next_v(cur);
        cur = -1;
        int32_t prev = -1;
        for (int32_t i = 0; i < v.size(); i++) {
            if (!is_lms(sa[i])) continue;
            bool diff = false;
            for (int32_t d = 0; d < v.size(); d++) {
                if (prev == -1 || v[sa[i] + d] != v[prev + d] ||
                    is_l[sa[i] + d] != is_l[prev + d]) {
                    diff = true;
                    break;
                } else if (d > 0 && is_lms(sa[i] + d))
                    break;
            }
            if (diff) {
                cur++;
                prev = sa[i];
            }
            next_v[order[sa[i]]] = cur;
        }

        std::vector<int32_t> re_order(next_v.size());
        for (int32_t i = 0; i < v.size(); i++)
            if (is_lms(i)) re_order[order[i]] = i;
        std::vector<int32_t> next_sa = sa_is_impl(next_v, cur);
        create_end_bucket(v, bucket);
        for (int32_t i = 0; i < sa.size(); i++) sa[i] = -1;
        for (int32_t i = next_sa.size() - 1; i >= 0; i--)
            sa[--bucket[v[re_order[next_sa[i]]]]] = re_order[next_sa[i]];
        induced_sort(v, sa, bucket, is_l);
        invert_induced_sort(v, sa, bucket, is_l);
        return sa;
    }

    void sa_is() {
        std::vector<int32_t> v(s.size() + 1);
        for (int32_t i = 0; i < s.size(); i++) v[i] = s[i];
        auto sa_ = sa_is_impl(v, *std::max_element(v.begin(), v.end()));
        sa.assign(sa_.begin(), sa_.end());
    }

    void construct_lcp() {
        lcp.resize(s.size());
        rank.resize(s.size() + 1);
        const int32_t n = s.size();
        for (int32_t i = 0; i <= n; i++) rank[sa[i]] = i;
        int32_t h = 0;
        lcp[0] = 0;
        for (int32_t i = 0; i < n; i++) {
            int32_t j = sa[rank[i] - 1];

            if (h > 0) h--;
            for (; j + h < n && i + h < n; h++) {
                if (s[j + h] != s[i + h]) break;
            }
            lcp[rank[i] - 1] = h;
        }
    }

    struct segtree {
        int32_t N;
        std::vector<int32_t> dat;
        void init(std::vector<int32_t> &v) {
            for (N = 1; N < v.size(); N <<= 1);
            dat.resize(N * 2, 1001001001);
            for (int32_t i = 0; i < v.size(); i++) dat[i + N - 1] = v[i];
            for (int32_t i = N - 2; i >= 0; i--)
                dat[i] = std::min(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
        int32_t get_min(int32_t a, int32_t b, int32_t k, int32_t l, int32_t r) {
            if (r <= a || b <= l) return 1001001001;
            if (a <= l && r <= b) return dat[k];
            return std::min(get_min(a, b, k * 2 + 1, l, (l + r) / 2),
                            get_min(a, b, k * 2 + 2, (l + r) / 2, r));
        }
        int32_t get_min(int32_t a, int32_t b) { return get_min(a, b, 0, 0, N); }
    };
    class sparse_table {
        std::vector<std::vector<int32_t> > dat;

       public:
        void init(const std::vector<int64_t> &vec) {
            int32_t b;
            for (b = 0; (1 << b) <= vec.size(); b++);
            dat.assign(b, std::vector<int32_t>(1 << b));
            for (int32_t i = 0; i < vec.size(); i++)
                dat[0][i] = static_cast<int32_t>(vec[i]);

            for (int32_t i = 1; i < b; i++) {
                for (int32_t j = 0; j + (1 << i) <= (1 << b); j++) {
                    dat[i][j] =
                        std::min(dat[i - 1][j], dat[i - 1][j + (1 << (i - 1))]);
                }
            }
        }
        int32_t get_min(const int32_t l, const int32_t r) const {
            assert(l < r);
            const int32_t b = 32 - __builtin_clz(r - l) - 1;
            return std::min(dat[b][l], dat[b][r - (1 << b)]);
        }
        sparse_table() {}
        sparse_table(const std::vector<int64_t> &vec) { init(vec); }
    };

   public:
    sparse_table st;
    std::string s;
    std::vector<int64_t> sa, lcp, rank;

    SuffixArray(const std::string &s) : s(s) {
        sa_is();
        construct_lcp();
        st.init(lcp);
    }

    bool contain(const std::string &t) {
        int32_t lb = 0, ub = s.size();
        while (ub - lb > 1) {
            int32_t mid = (lb + ub) / 2;
            if (s.compare(sa[mid], t.size(), t) < 0)
                lb = mid;
            else
                ub = mid;
        }
        return s.compare(sa[ub], t.size(), t) == 0;
    }

    int32_t get_lcp(int32_t i, int32_t j) {
        assert(i != j);
        if (rank[i] > rank[j]) std::swap(i, j);
        return st.get_min(rank[i], rank[j]);
    }
    int32_t operator[](const int32_t idx) const { return sa[idx]; }
};
