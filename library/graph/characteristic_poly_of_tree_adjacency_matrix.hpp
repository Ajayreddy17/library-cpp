#ifndef AJAY_CHARACTERISTIC_POLY_OF_TREE_ADJACENCY_MATRIX
#define AJAY_CHARACTERISTIC_POLY_OF_TREE_ADJACENCY_MATRIX

#include "library/graph/datastructure/static_toptree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/poly/convolution.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/characteristic_poly_of_tree_adjacency_matrix.hpp

template <typename mint>
struct TREE_ADJ_MATRIX_DP {
    using poly = vector<mint>;
    using Data = array<array<poly, 2>, 2>;
    using value_type = Data;

    static void add(poly& f, poly g) {
        if (len(f) < len(g)) f.resize(len(g));
        For(i, len(g)) f[i] += g[i];
    };

    static Data rake(Data L, Data R) {
        Data Z;
        add(Z[0][0], convolution(L[0][0], R[0][1]));
        add(Z[0][1], convolution(L[0][1], R[0][1]));
        add(Z[1][0], convolution(L[0][0], R[1][1]));
        add(Z[1][1], convolution(L[0][1], R[1][1]));
        add(Z[1][0], convolution(L[1][0], R[0][1]));
        add(Z[1][1], convolution(L[1][1], R[0][1]));
        return Z;
    }
    static Data compress(Data L, Data R) {
        Data Z;
        For(p, 2) For(q, 2) For(r, 2) {
            add(Z[p][r], convolution<mint>(L[p][q], R[1 - q][r]));
        }
        return Z;
    }
};

// computes det(I-xA) (the reversed characteristic polynomial)
// weight(i,j): A[i][j]
// only even degrees appear, so faster than the version with loops
template <typename mint, typename F>
vector<mint> characteristic_poly_of_tree_adjacency_matrix_not_allow_loop(
        Graph<int, 0>& G, F weight) {
    using poly = vector<mint>;
    Tree<Graph<int, 0>> tree(G);
    Static_TopTree<decltype(tree)> STT(tree);

    // whether u, v are already computed
    using Data = array<array<poly, 2>, 2>;
    auto single = [&](int v) -> Data {
        Data X;
        int p = tree.parent[v];
        mint wt = (p == -1 ? mint(0) : weight(p, v) * weight(v, p));
        X[0][0] = poly{mint(1)};
        X[0][1] = poly{mint(1)};                    // loop
        if (p != -1) X[1][1] = poly{mint(0), -wt};  // match
        return X;
    };
    Data X = STT.tree_dp<TREE_ADJ_MATRIX_DP<mint>>(single);
    vector<mint> ANS(G.N + 1);
    For(i, len(X[0][1])) { ANS[2 * i] += X[0][1][i]; }
    return ANS;
}

template <typename mint, typename F>
vector<mint> characteristic_poly_of_tree_adjacency_matrix_allow_loop(
        Graph<int, 0>& G, F weight) {
    using poly = vector<mint>;
    Tree<Graph<int, 0>> tree(G);
    Static_TopTree<decltype(tree)> STT(tree);

    using Data = array<array<poly, 2>, 2>;
    auto single = [&](int v) -> Data {
        Data X;
        int p = tree.parent[v];
        mint wt = (p == -1 ? mint(0) : weight(p, v) * weight(v, p));
        X[0][0] = poly{mint(1)};
        X[0][1] = poly{mint(1), -weight(v, v)};              // loop
        if (p != -1) X[1][1] = poly{mint(0), mint(0), -wt};  // match
        return X;
    };
    Data X = STT.tree_dp<TREE_ADJ_MATRIX_DP<mint>>(single);
    vector<mint> ANS(G.N + 1);
    For(i, len(X[0][1])) { ANS[i] += X[0][1][i]; }
    return ANS;
}

// computes det(I-xA) (the reversed characteristic polynomial)
// weight(i,j): A[i][j]
template <bool ALLOW_LOOP, typename mint, typename F>
vector<mint> characteristic_poly_of_tree_adjacency_matrix(Graph<int, 0>& G,
                                                                                                            F weight) {
    if constexpr (ALLOW_LOOP) {
        return characteristic_poly_of_tree_adjacency_matrix_allow_loop<mint>(
                G, weight);
    } else {
        return characteristic_poly_of_tree_adjacency_matrix_not_allow_loop<mint>(
                G, weight);
    }
}
} // namespace mitsuha
#endif // AJAY_CHARACTERISTIC_POLY_OF_TREE_ADJACENCY_MATRIX
