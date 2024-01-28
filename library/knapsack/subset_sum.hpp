#ifndef AJAY_SUBSET_SUM
#define AJAY_SUBSET_SUM

#include "library/enumerate/bits.hpp"
#include "library/datastructure/my_bitset.hpp"

namespace mitsuha{
// O(N MAX(vals))
template <typename T>
vector<int> subset_sum_solution_1(vector<T>& vals, int target) {
    int n = len(vals);
    if (n == 0) return {};
    int mx = MAX(vals);
    int b = 0, sb = 0;
    while (b < n && sb + vals[b] <= target) { sb += vals[b++]; }
    if (b == n && sb != target) return {};

    int off = target - mx + 1;
    vector<int> dp(2 * mx, -1);
    vector<vector<int>> PAR(n, vector<int>(2 * mx, -1));
    dp[sb - off] = b;
    For(i, b, n) {
        auto newdp = dp;
        auto& par = PAR[i];
        int a = vals[i];
        For(j, mx) {
            if (chmax(newdp[j + a], dp[j])) { par[j + a] = -2; }
        }
        for(int j = 2 * mx - 1; j >= mx; --j) {
            for(int k = newdp[j] - 1; k >= max(dp[j], 0); --k) {
                if (chmax(newdp[j - vals[k]], k)) par[j - vals[k]] = k;
            }
        }
        swap(dp, newdp);
    }
    if (dp[mx - 1] == -1) return {};
    vector<bool> use(n);
    int i = n - 1, j = mx - 1;
    while (i >= b) {
        int p = PAR[i][j];
        if (p == -2) {
            use[i] = !use[i];
            j -= vals[i--];
        }
        elif (p == -1) { --i; }
        else {
            use[p] = !use[p];
            j += vals[p];
        }
    }
    while (i >= 0) {
        use[i] = !use[i];
        --i;
    }
    vector<int> I;
    For(i, n) if (use[i]) I.emplace_back(i);

    long long sm = 0;
    for (auto&& i: I) sm += vals[i];
    assert(sm == target);
    return I;
}

// O(N target / w)
template <typename T>
vector<int> subset_sum_solution_2(vector<T>& vals, int target) {
    int n = len(vals);
    auto I = argsort(vals);
    My_Bitset dp(1, 1);
    vector<int> last(target + 1, -1);
    For(k, n) {
        int v = vals[I[k]];
        if (v > target) continue;
        My_Bitset newdp = dp;
        int new_size = len(dp) + v;
        newdp.resize(new_size);
        newdp.or_to_range(v, new_size, dp);
        if (len(newdp) > target + 1) newdp.resize(target + 1);
        // update Note what you did
        For(i, len(newdp.dat)) {
            unsigned long long upd = (i < len(dp.dat) ? dp.dat[i] : 0ULL) ^ newdp.dat[i];
            enumerate_bits_64(upd, [&](int p) -> void { last[(i << 6) | p] = I[k]; });
        }
        swap(dp, newdp);
    }
    if (target >= len(dp) || !dp[target]) return {};
    vector<int> ANS;
    while (target > 0) {
        int i = last[target];
        ANS.emplace_back(i);
        target -= vals[i];
    }
    return ANS;
}

// O(sum^{1.5} / w)
// sum=10^6 in 150ms：https://codeforces.com/contest/755/problem/F
template <typename T>
vector<int> subset_sum_solution_3(vector<T>& vals, int target) {
    int SM = SUM<int>(vals);
    int N = len(vals);
    vector<vector<int>> IDS(SM + 1);
    For(i, N) IDS[vals[i]].emplace_back(i);
    vector<pair<int, int>> par(N, {-1, -1});
    vector<int> grp_vals;
    vector<int> raw_idx;
    For(x, 1, SM + 1) {
        auto& I = IDS[x];
        while (len(I) >= 3) {
            int a = I.back(); I.pop_back();
            int b = I.back(); I.pop_back();
            int c = len(par);
            par.emplace_back(a, b);
            IDS[2 * x].emplace_back(c);
        }
        for (auto& i: I) {
            grp_vals.emplace_back(x);
            raw_idx.emplace_back(i);
        }
    }
    auto I = subset_sum_solution_2<int>(grp_vals, target);
    vector<int> ANS;
    for (auto& i: I) {
        vector<int> st = {raw_idx[i]};
        while (len(st)) {
            auto c = st.back(); st.pop_back();
            if (c < N) {
                ANS.emplace_back(c);
                continue;
            }
            auto [a, b] = par[c];
            st.emplace_back(a), st.emplace_back(b);
        }
    }
    return ANS;
}

template <typename T>
vector<int> subset_sum_solution_4(vector<T>& vals, T target) {
    if (target <= 0) return {};
    int N = len(vals);
    int M = N / 2;

    auto calc = [&](int L, int R) -> vector<T> {
        int n = R - L;
        vector<T> dp = {0};
        For(i, n) {
        T a = vals[L + i];
        vector<T> dp1(len(dp));
        For(k, len(dp)) dp1[k] = dp[k] + a;
            vector<T> newdp;
            merge(dp.begin(), dp.end(), dp1.begin(), dp1.end(), back_inserter(newdp));
            swap(dp, newdp);
        }
        return dp;
    };

    auto restore = [&](int L, int R, T v) -> vector<int> {
        int n = R - L;
        vector<T> dp(1 << n);
        For(i, n) For(s, 1 << i) dp[s | 1 << i] = dp[s] + vals[L + i];
        For(s, 1 << n) {
            if (dp[s] == v) {
                vector<int> I;
                For(i, n) if (s >> i & 1) I.emplace_back(L + i);
                return I;
            }
        }
        assert(0);
        return {};
    };

    auto dp1 = calc(0, M);
    auto dp2 = calc(M, N);
    int t = 0;
    Frr(s, len(dp1)) {
        while (t + 1 < len(dp2) && dp1[s] + dp2[t + 1] <= target) { ++t; }
        if (dp1[s] + dp2[t] == target) {
            vector<int> I1 = restore(0, M, dp1[s]);
            vector<int> I2 = restore(M, N, dp2[t]);
            I1.insert(I1.end(), I2.begin(), I2.end());
            return I1;
        }
    }
    return {};
}

template <typename T>
vector<int> subset_sum(vector<T>& vals, T target) {
    if (target <= 0) return {};
    int n = len(vals);
    if (n == 0) return {};
    int mx = *max_element(vals);

    // Threshold has not been adjusted
    // solution 1: O(N mx))
    // solution 2: O(N target / w)
    // solution 3: O(sum^1.5 / w)
    // solution 4: O(2^(N/2))
    double x1 = double(len(vals)) * mx;
    double x2 = double(len(vals)) * target / 500.0;
    double x3 = pow(accumulate(vals.begin(), vals.end(), 0.), 1.5) / 500.0;
    double x4 = pow(2.0, 0.5 * len(vals));
    double mi = min({x1, x2, x3, x4});
    if (x1 == mi) return subset_sum_solution_1(vals, target);
    if (x2 == mi) return subset_sum_solution_2(vals, target);
    if (x3 == mi) return subset_sum_solution_3(vals, target);
    return subset_sum_solution_4(vals, target);
}
} // namespace mitsuha
#endif // AJAY_SUBSET_SUM