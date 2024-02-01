#ifndef AJAY_KANPSACK01
#define AJAY_KANPSACK01

namespace mitsuha{
/*
Full enumeration of knapsack01
・In O(2^n), enumerate all (wt, val) + sort by wt
・remove extra = true → leave only the parts where val increases
*/
template <typename WT, typename VAL>
vector<pair<WT, VAL>> knapsack01_gen_all(vector<WT> weight, vector<VAL> val, bool remove_extra) {
    int n = len(weight);
    using P = pair<WT, VAL>;
    vector<P> dp;
    dp.emplace_back(0, 0);
    For(i, n) {
        vector<P> tmp(len(dp));
        For(j, len(dp)) tmp[j] = {dp[j].first + weight[i], dp[j].second + val[i]};
        vector<P> res;
        merge(dp.begin(), dp.end(), tmp.begin(), tmp.end(), back_inserter(res));
        swap(dp, res);
    }
    if (!remove_extra) return dp;
    vector<P> res;
    res.reserve(len(dp));
    for (auto&& [w, v]: dp) {
        while (len(res) && res.back().first == w) res.pop_back();
        if (len(res) && res.back().second >= v) continue;
        res.emplace_back(w, v);
    }
    return res;
}

template <typename WT, typename VAL>
VAL knapsack01(vector<WT> weight, vector<VAL> val, WT LIM) {
    chmin(LIM, accumulate(weight.begin(), weight.end(), WT(0)));
    int n = len(weight);
    assert(len(val) == n);

    // Half-full enumeration + shaku-tori method
    // O(2^{N/2})
    auto sol_1 = [&]() -> VAL {
        vector<WT> wt_l = {weight.begin(), weight.begin() + n / 2};
        vector<WT> wt_r = {weight.begin() + n / 2, weight.end()};
        vector<VAL> val_l = {val.begin(), val.begin() + n / 2};
        vector<VAL> val_r = {val.begin() + n / 2, val.end()};
        auto dp1 = knapsack01_gen_all(wt_l, val_l, true);
        auto dp2 = knapsack01_gen_all(wt_r, val_r, true);
        int q = len(dp2);
        VAL ANS = 0;
        For(p, len(dp1)) {
            while (q && (dp1[p].first + dp2[q - 1].first > LIM)) --q;
            if (q == 0) break;
            chmax(ANS, dp1[p].second + dp2[q - 1].second);
        }
        return ANS;
    };

    // dp per wt
    // O(N LIM)
    auto sol_2 = [&]() -> VAL {
        vector<VAL> dp(LIM + 1);
        long long total = 0;
        For(k, n) {
            long long mx = min<long long>(LIM - weight[k], total);
            Frr(i, mx + 1) chmax(dp[i + weight[k]], dp[i] + val[k]);
            total += weight[k];
        };
        return *max_element(dp.begin(), dp.end());
    };

    // dp per val
    // O(N sum val)
    auto sol_3 = [&]() -> VAL {
        // val -> min wt
        vector<WT> dp(accumulate(val.begin(), val.end(), VAL(0)) + 1, LIM + 1);
        dp[0] = 0;
        long long total = 0;
        For(k, n) {
            Frr(i, total + 1) { chmin(dp[i + val[k]], dp[i] + weight[k]); }
            total += val[k];
        }
        VAL ANS = 0;
        For(i, total + 1) if (dp[i] <= LIM) ANS = i;
        return ANS;
    };

    double t1 = log(2) * n * 0.5;
    double t2 = log((n + 1) * (LIM + 1));
    double t3 = log((n + 1) * (accumulate(val.begin(), val.end(), WT(0)) + 1));
    double t = min({t1, t2, t3});
    if (t == t1) return sol_1();
    if (t == t2) return sol_2();
    return sol_3();
}
} // namespace mitsuha
#endif // AJAY_KANPSACK01