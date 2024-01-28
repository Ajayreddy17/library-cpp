#ifndef AJAY_STATIC_RANGE_FREQUENCY
#define AJAY_STATIC_RANGE_FREQUENCY

#include "library/datastructure/to_small_key.hpp"

namespace mitsuha{
template <int LOG, bool USE_RESET = false>
struct Static_Range_Frequency {
    vector<int> pos, indptr;
    To_Small_Key S;

    template <typename T>
    Static_Range_Frequency(vector<T>& A) {
        build(len(A), [&](int i) -> unsigned long long{ return A[i]; });
    }

    template <typename F>
    Static_Range_Frequency(int N, F f) {
        build(N, f);
    }

    template <typename F>
    void build(int N, F f) {
        S.reserve(N);
        pos.resize(N);
        vector<int> cnt(N + 1), dat(N);
        For(i, N) {
            unsigned long long x = f(i);
            int k = S.set_key(x);
            cnt[1 + k]++, dat[i] = k;
        }
        For(k, N) cnt[1 + k] += cnt[k];
        indptr = cnt;
        For(i, N) pos[cnt[dat[i]]++] = i;
    }

    int query(int L, int R, unsigned long long x) {
        int k = S.query(x);
        if (k == -1) return 0;
        int a = indptr[k], b = indptr[k + 1];
        auto nl = lower_bound(pos.begin() + a, pos.begin() + b, L);
        auto nr = lower_bound(pos.begin() + a, pos.begin() + b, R);
        return nr - nl;
    }
};
} // namespace mitsuha
#endif // AJAY_STATIC_RANGE_FREQUENCY