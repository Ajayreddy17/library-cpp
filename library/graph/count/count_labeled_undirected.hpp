#ifndef AJAY_COUNT_LABELED_UNDIRECTED
#define AJAY_COUNT_LABELED_UNDIRECTED


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_undirected.hpp
// https://oeis.org/A006125
template <typename mint>
vector<mint> count_labeled_undirected(int N) {
    vector<mint> F(N + 1);
    mint pow2 = 1;
    F[0] = 1;
    For(i, 1, N + 1) F[i] = F[i - 1] * pow2, pow2 += pow2;
    return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_UNDIRECTED
