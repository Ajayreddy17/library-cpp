#ifndef AJAY_NTH_ELEM_FROM_LISTS
#define AJAY_NTH_ELEM_FROM_LISTS

namespace mitsuha{
// We have n sorted lists in non dec order
// S[x] = len(xth list), f(i, j) = lists[i][j]
// Assume all elem in all lists were sorted in one list and resized as K
// starting ret[i] elems in list[i] were in those K elems
// O(N * (logN + logK))
template <typename T, typename F>
vector<long long> nth_element_from_sorted_lists(vector<long long> S, long long K, F f, int k = 0) {
    long long N = len(S);
    long long sm = 0;
    for (auto& x: S) sm += x >> k;
    assert(0 <= K && K <= sm);
    if (K == 0) return vector<long long>(N, 0);
    if (K == sm) return S;

    long long row = 0;
    for (auto& x: S) row += (x >= (1LL << k));

    auto g = [&](int i, long long j) -> T {
        j = ((j + 1) << k) - 1;
        return (j >= S[i] ? (numeric_limits<T>::max() / 2) : f(i, j));
    };
    vector<long long> A(N);
    if (K > row) {
        A = nth_element_from_sorted_lists<T>(S, (K - row) / 2, f, k + 1);
        for (auto& a: A) a *= 2;
        K = K - (K - row) / 2 * 2;
    }
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> que;

    For(i, N) que.emplace(g(i, A[i]), i);
    while (K) {
        --K;
        auto [x, i] = que.top(); que.pop();
        A[i]++;
        if (K) que.emplace(g(i, A[i]), i);
    }
    return A;
}
} // namespace mitsuha
#endif // AJAY_NTH_ELEM_FROM_LISTS