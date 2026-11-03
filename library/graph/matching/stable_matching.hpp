#ifndef AJAY_STABLE_MATCHING
#define AJAY_STABLE_MATCHING

#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/matching/stable_matching.hpp

// returns the stable matching that is optimal for group 1 (it always exists)
// O((N+M)logM)
struct Stable_Matching {
    int N1, N2;
    vector<vector<tuple<int, int, int>>> dat;
    vector<int> match_1, match_2;
    vector<int> val_1, val_2;

    Stable_Matching(int N1, int N2) : N1(N1), N2(N2), dat(N1) {}

    // x is the value; larger is preferred
    void add(int v1, int v2, int x1, int x2) { dat[v1].emplace_back(v2, x1, x2); }

    vector<pair<int, int>> calc() {
        For(v1, N1) {
            sort(dat[v1].begin(), dat[v1].end(),
                      [&](auto& a, auto& b) -> bool { return get<1>(a) < get<1>(b); });
        }
        match_1.assign(N1, -1), match_2.assign(N2, -1);
        val_1.assign(N1, -iinf), val_2.assign(N2, -iinf);
        vector<int> que(N1);
        For(v, N1) que[v] = v;
        while (len(que)) {
            auto v1 = POP(que);
            match_1[v1] = -1, val_1[v1] = -iinf;
            if (dat[v1].empty()) continue;
            auto [v2, x1, x2] = POP(dat[v1]);
            if (!chmax(val_2[v2], x2)) {
                que.emplace_back(v1);
                continue;
            }
            if (match_2[v2] != -1) que.emplace_back(match_2[v2]);
            match_1[v1] = v2, match_2[v2] = v1, val_1[v1] = x1, val_2[v2] = x2;
        }
        vector<pair<int, int>> match;
        For(v1, N1) {
            int v2 = match_1[v1];
            if (v2 != -1) match.emplace_back(v1, v2);
        }
        return match;
    }
};
} // namespace mitsuha
#endif // AJAY_STABLE_MATCHING
