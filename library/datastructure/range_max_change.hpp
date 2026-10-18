#ifndef AJAY_RANGE_MAX_CHANGE
#define AJAY_RANGE_MAX_CHANGE

namespace mitsuha {
// Increment p while managing:
// right_change: ANS[i] := max of [i, p)
// left_change:  ANS[i] := max of [p, i)
// i is in [0, N+1), empty intervals get -numeric_limits<T>::max()/2
template <typename T>
struct Range_Max_Change {
    vector<T> A;
    int N, pl, pr;
    bool prepared;
    Range_Max_Change(vector<T>& A) : A(A), N(len(A)), pl(0), pr(0), prepared(0) {}
    template <typename F>
    Range_Max_Change(int N, F f) : N(N), pl(0), pr(0), prepared(0) {
        A.resize(N);
        For(i, N) A[i] = f(i);
    }

    vector<pair<int, T>> st_right;

    // p=i -> p=i+1
    // f(L, R, before, after)
    template <typename F>
    void right_change(int i, F f) {
        assert(0 <= i && i < N && pr == i);
        ++pr;
        const T neg_inf = -numeric_limits<T>::max() / 2;
        int R = i;
        f(i, i + 1, neg_inf, A[i]);
        while (len(st_right) && st_right.back().second <= A[i]) {
            auto [L, x] = st_right.back(); st_right.pop_back();
            if (x != A[i]) f(L, R, x, A[i]);
            R = L;
        }
        st_right.emplace_back(R, A[i]);
    }

    vector<tuple<int, int, int, T, T>> event_left;

    // p=i -> p=i+1
    // f(L, R, before, after)
    template <typename F>
    void left_change(int i, F f) {
        assert(0 <= i && i < N && pl == i && prepared);
        ++pl;
        while (len(event_left) && get<0>(event_left.back()) == i) {
            auto [j, L, R, a, b] = event_left.back(); event_left.pop_back();
            f(L, R, b, a);
        }
    }

    // ANS[i] = max of [0, i) for i in [0, N+1)
    // f(L, R, x)
    template <typename F>
    void left_init(F f) {
        assert(!prepared);
        prepared = 1;
        const T neg_inf = -numeric_limits<T>::max() / 2;

        vector<pair<int, T>> st;
        for (int i = N; i--;) {
            int R = i + 2;
            event_left.emplace_back(i, i + 1, R, neg_inf, A[i]);
            while (len(st) && st.back().second <= A[i]) {
                auto [RR, x] = st.back(); st.pop_back();
                if (x != A[i]) event_left.emplace_back(i, R, RR, x, A[i]);
                R = RR;
            }
            st.emplace_back(R, A[i]);
        }

        f(0, 1, neg_inf);
        int L = 1;
        for (int k = len(st); k--;) {
            auto [R, x] = st[k];
            f(L, R, x);
            L = R;
        }
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_MAX_CHANGE
