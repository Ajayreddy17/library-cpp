#ifndef AJAY_ROLLBACK_MO
#define AJAY_ROLLBACK_MO

namespace mitsuha{
// https://codeforces.com/contest/620/problem/F
// (10^5,3*10^5), mo+fastset 1300ms
// https://codeforces.com/problemset/submission/765/240821486
struct Rollback_Mo {
    vector<pair<int, int>> LR;
    void add(int L, int R) { LR.emplace_back(L, R); }

    template <typename AL, typename AR, typename F1, typename F2, typename F3, typename O>
    void calc(AL add_left, AR add_right, F1 reset, F2 save, F3 rollback, O query) {
        const int Q = len(LR);
        if (Q == 0) return;
        int N = 0;
        for (auto &&[L, R]: LR) chmax(N, R);
        const int b_num = sqrt(Q);
        const int b_sz = ceil(N, b_num);
        vector<vector<int>> QID((N - 1) / b_sz + 1);
        // Sort by block to which the left end belongs
        // Items whose left and right ends belong to the same block are processed first.
        auto naive = [&](int qid) -> void {
            save();
            auto [L, R] = LR[qid];
            For(i, L, R) add_right(i);
            query(qid);
            rollback();
        };

        For(qid, Q) {
            auto [L, R] = LR[qid];
            int iL = L / b_sz, iR = R / b_sz;
            if (iL == iR) {
                naive(qid);
                continue;
            }
            QID[iL].emplace_back(qid);
        }

        For(iL, len(QID)) {
            auto &I = QID[iL];
            if (I.empty()) continue;
            sort(I.begin(), I.end(), [&](auto &a, auto &b) -> bool { return LR[a].second < LR[b].second; });
            int LMAX = 0;
            for (auto &&qid: I) {
                auto [L, R] = LR[qid];
                chmax(LMAX, L);
            }
            reset();
            int l = LMAX, r = LMAX;
            for (auto &&qid: I) {
                auto [L, R] = LR[qid];
                while (r < R) add_right(r++);
                save();
                while (L < l) add_left(--l);
                query(qid);
                rollback();
                l = LMAX;
            }
        }
    }
};
} // namespace mitsuha
#endif // AJAY_ROLLBACK_MO