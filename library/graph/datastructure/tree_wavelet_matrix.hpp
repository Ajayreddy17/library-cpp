#ifndef AJAY_TREE_WAVELET_MATRIX
#define AJAY_TREE_WAVELET_MATRIX

#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"
#include "library/graph/tree.hpp"

namespace mitsuha{
// https://atcoder.jp/contests/pakencamp-2022-day1/tasks/pakencamp_2022_day1_j
// https://atcoder.jp/contests/utpc2011/tasks/utpc2011_12
template <typename TREE, bool edge, typename T, bool COMPRESS, typename Monoid = Monoid_Add<T>>
struct Tree_Wavelet_Matrix {
    TREE& tree;
    int N;
    using WM = Wavelet_Matrix<T, COMPRESS, Monoid_Add<T>>;
    using X = typename Monoid::value_type;
    WM wm;

    Tree_Wavelet_Matrix(TREE& tree, vector<T> A, vector<X> SUM_data = {}, int log = -1)
                        : tree(tree), N(tree.N) {
        vector<X>& S = SUM_data;
        vector<T> A1;
        vector<X> S1;
        A1.resize(N);
        if (!S.empty()) S1.resize(N);
        if (!edge) {
            assert(len(A) == N && (len(S) == 0 || len(S) == N));
            For(v, N) A1[tree.LID[v]] = A[v];
            if (len(S) == N) { For(v, N) S1[tree.LID[v]] = S[v]; }
            wm.build(A1, S1, log);
        } else {
            assert(len(A) == N - 1 && (len(S) == 0 || len(S) == N - 1));
            if (!S.empty()) {
                For(e, N - 1) { S1[tree.LID[tree.e_to_v(e)]] = S[e]; }
            }
            For(e, N - 1) { A1[tree.LID[tree.e_to_v(e)]] = A[e]; }
            wm.build(A1, S1, log);
        }
    }

    // Count the xor results that fit in [a, b)
    int count_path(int s, int t, T a, T b, T xor_val = 0) {
        return wm.count(get_segments(s, t), a, b, xor_val);
    }

    // Count the xor results that fit in [a, b)
    int count_subtree(int u, T a, T b, T xor_val = 0) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.count(l + edge, r, a, b, xor_val);
    }

    // xor result, k>=0th and prefix sum in [L, R)
    pair<T, X> kth_value_and_sum_path(int s, int t, int k, T xor_val = 0) {
        return wm.kth_value_and_sum(get_segments(s, t), k, xor_val);
    }

    // xor result, k>=0th and prefix sum in [L, R)
    pair<T, X> kth_value_and_sum_subtree(int u, int k, T xor_val = 0) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.kth_value_and_sum(l + edge, r, k, xor_val);
    }

    // xor result, k>=0th in [L, R)
    T kth_path(int s, int t, int k, T xor_val = 0) {
        return wm.kth(get_segments(s, t), k, xor_val);
    }

    // xor result, k>=0th in [L, R)
    T kth_subtree(int u, int k, T xor_val = 0) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.kth(l + edge, r, k, xor_val);
    }

    // The median value in [L, R) as a result of xoring.
    // LOWER = true: lower median, false: upper median
    T median_path(bool UPPER, int s, int t, T xor_val = 0) {
        return wm.median(UPPER, get_segments(s, t), xor_val);
    }

    T median_subtree(bool UPPER, int u, T xor_val = 0) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.median(UPPER, l + edge, r, xor_val);
    }

    // The sum of SUM_data at the [k1, k2)th point in the xor result
    X sum_path(int s, int t, int k1, int k2, T xor_val = 0) {
        return wm.sum(get_segments(s, t), k1, k2, xor_val);
    }

    // The sum of SUM_data at the [k1, k2)th point in the xor result
    X sum_subtree(int u, int k1, int k2, T xor_val = 0) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.sum(l + edge, r, k1, k2, xor_val);
    }

    X sum_all_path(int s, int t) { return wm.sum_all(get_segments(s, t)); }

    X sum_all_subtree(int u) {
        int l = tree.LID[u], r = tree.RID[u];
        return wm.sum_all(l + edge, r);
    }

private:
    vector<pair<int, int>> get_segments(int s, int t) {
        vector<pair<int, int>> segments = tree.get_path_decomposition(s, t, edge);
        for (auto&& [a, b]: segments) {
            if (a >= b) swap(a, b);
            ++b;
        }
        return segments;
    }
};
} // namespace mitsuha
#endif // AJAY_TREE_WAVELET_MATRIX
