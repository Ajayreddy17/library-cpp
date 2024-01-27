#ifndef AJAY_KNAPSACK_BRANCH_BOUND
#define AJAY_KNAPSACK_BRANCH_BOUND

namespace mitsuha{
template <typename T>
vector<T> __rearrange(const vector<T> &A, const vector<int> &I) {
    vector<T> B(len(I));
    For(i, len(I)) B[i] = A[I[i]];
    return B;
}

// Branch limitation method. yukicoder 626
template <typename WT, typename VAL>
VAL knapsack_branch_bound(vector<WT> wt, vector<VAL> val, WT LIM) {
    int N = len(val);
    using Re = long double;
    vector<Re> sort_key(N);
    For(i, N) sort_key[i] = Re(val[i]) / wt[i];
    auto I = sorted_indices(N, [&](int i){ return -sort_key[i]; });
    wt = __rearrange(wt, I);
    val = __rearrange(val, I);

    VAL ANS = 0;
    auto dfs = [&](auto dfs, int nxt, WT now_wt, VAL now_val) -> void {
        if (now_wt > LIM) return;
        chmax(ANS, now_val);
        if (nxt == N) return;
        Re bd = now_val;
        Re rest = LIM - now_wt;
        For(i, nxt, N) {
        if (rest >= wt[i]) {
            rest -= wt[i];
            bd += val[i];
            continue;
        }
        bd += rest / wt[i] * val[i];
        break;
    }
        if (bd <= ANS) return;
        dfs(dfs, nxt + 1, now_wt + wt[nxt], now_val + val[nxt]);
        dfs(dfs, nxt + 1, now_wt, now_val);
    };

    dfs(dfs, 0, 0, 0);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_KNAPSACK_BRANCH_BOUND