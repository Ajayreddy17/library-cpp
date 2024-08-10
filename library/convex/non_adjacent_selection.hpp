#ifndef AJAY_NON_ADJACENT_SELECTION
#define AJAY_NON_ADJACENT_SELECTION

namespace mitsuha{
// ANS[x] - Optimal Seclection of x non adjacent elements
template <typename T, bool MINIMIZE>
struct Non_Adjacent_Selection {
    int N;
    vector<T> ANS;
    vector<pair<int, int>> history;

    Non_Adjacent_Selection(vector<T>& A) { calc(A); }

    void calc(vector<T> A) {
        if (MINIMIZE) {
            for (auto& x: A) x = -x;
        }
        N = len(A);
        vector<bool> rest(N + 2, 1);
        rest[0] = rest[N + 1] = 0;
        vector<pair<int, int>> range(N + 2);
        vector<int> left(N + 2), right(N + 2);
        vector<T> val(N + 2);
        priority_queue<pair<T, int>> que;
        For(i, N + 2) { left[i] = i - 1, right[i] = i + 1; }
        For(i, N) {
            val[i + 1] = A[i], range[i + 1] = {i, i + 1};
            que.emplace(val[i + 1], i + 1);
        }

        ANS = {0};
        while (len(que)) {
            auto [add, i] = que.top(); que.pop();
            if (!rest[i]) continue;
            ANS.emplace_back(ANS.back() + add);
            int L = left[i], R = right[i];
            history.emplace_back(range[i]);
            if (1 <= L) { right[left[L]] = i, left[i] = left[L]; }
            if (R <= N) { left[right[R]] = i, right[i] = right[R]; }
            if (rest[L] && rest[R]) {
                val[i] = val[L] + val[R] - val[i];
                que.emplace(val[i], i);
                range[i] = {range[L].first, range[R].second};
            } else {
                rest[i] = 0;
            }
            rest[L] = rest[R] = 0;
        }

        if (MINIMIZE) {
            for (auto& x: ANS) x = -x;
        }
    }

    // returns indices
    vector<int> restore(int n) {
        vector<int> F(N + 1);
        For(i, n) {
            auto [a, b] = history[i];
            F[a]++, F[b]--;
        }
        For(x, N) F[x + 1] += F[x];
        vector<int> I;
        For(i, N) if (F[i] & 1) I.emplace_back(i);
        return I;
    }
};
} // namespace mitsuha
#endif // AJAY_NON_ADJACENT_SELECTION