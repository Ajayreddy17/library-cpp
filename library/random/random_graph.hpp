#ifndef AJAY_RANDOM_GRAPH
#define AJAY_RANDOM_GRAPH

#include "library/graph/base.hpp"
#include "library/random/base.hpp"
#include "library/random/shuffle.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/random/random_graph.hpp

void random_relabel(int N, vector<pair<int, int>>& G) {
    shuffle(G);
    vector<int> A(N);
    For(i, N) A[i] = i;
    shuffle(A);
    for (auto& [a, b] : G) a = A[a], b = A[b];
}

template <int DIRECTED>
vector<pair<int, int>> random_graph(int n, bool simple) {
    vector<pair<int, int>> G, cand;
    For(a, n) For(b, n) {
        if (simple && a == b) continue;
        if (!DIRECTED && a > b) continue;
        cand.emplace_back(a, b);
    }
    int m = RNG(0, len(cand) + 1);
    set<int> ss;
    For(m) {
        while (1) {
            int i = RNG(0, len(cand));
            if (simple && ss.count(i)) continue;
            ss.insert(i);
            auto [a, b] = cand[i];
            G.emplace_back(a, b);
            break;
        }
    }
    random_relabel(n, G);
    return G;
}

template <int DIRECTED>
vector<pair<int, int>> random_connected_graph(int n, bool simple) {
    while (1) {
        vector<pair<int, int>> G, cand;
        For(a, n) For(b, n) {
            if (simple && a == b) continue;
            if (!DIRECTED && a > b) continue;
            cand.emplace_back(a, b);
        }
        int m = RNG(0, len(cand) + 1);
        set<int> ss;
        For(m) {
            while (1) {
                int i = RNG(0, len(cand));
                if (simple && ss.count(i)) continue;
                ss.insert(i);
                auto [a, b] = cand[i];
                G.emplace_back(a, b);
                break;
            }
        }
        UnionFind uf(n);
        for (auto& [a, b] : G) uf.merge(a, b);
        if (uf.n_comp != 1) continue;
        random_relabel(n, G);
        return G;
    }
    assert(0);
}

vector<pair<int, int>> random_tree(int n) {
    vector<pair<int, int>> G;
    For(i, 1, n) { G.emplace_back(RNG(0, i), i); }
    random_relabel(n, G);
    return G;
}

// EDGE = true: every edge is on at most one cycle (cycles or edges meeting at articulation points)
// EDGE = false: every vertex is on at most one cycle (cycles or edges joined by bridges)
vector<pair<int, int>> random_cactus(int N, bool EDGE) {
    if (!EDGE) {
        // split n vertices into parts of size 1 or >= 3
        vector<vector<int>> A;
        int n = RNG(1, N + 1);
        vector<int> S(n, 1);
        int rest = N - n;
        while (rest > 0) {
            int k = RNG(0, n);
            if (S[k] == 1) {
                if (rest == 1) {
                    S.emplace_back(1), rest = 0;
                } else {
                    S[k] += 2, rest -= 2;
                }
            } else {
                S[k]++, rest--;
            }
        }
        n = len(S);
        int p = 0;
        For(i, n) {
            vector<int> C;
            For(v, p, p + S[i]) C.emplace_back(v);
            A.emplace_back(C);
            p += S[i];
        }
        int m = len(A);
        auto H = random_tree(m);
        vector<pair<int, int>> G;
        For(i, m) {
            vector<int>& V = A[i];
            if (len(V) == 1) continue;
            For(k, len(V)) { G.emplace_back(V[k], V[(1 + k) % len(V)]); }
        }
        for (auto& [c1, c2] : H) {
            int a = A[c1][RNG(0, len(A[c1]))];
            int b = A[c2][RNG(0, len(A[c2]))];
            G.emplace_back(a, b);
        }
        random_relabel(N, G);
        return G;
    }
    assert(EDGE);
    if (N == 1) return {};
    int n = RNG(1, N);
    vector<int> S(n, 2);
    int rest = N - 1 - n;
    while (rest > 0) {
        int k = RNG(0, n);
        S[k]++, --rest;
    }
    vector<vector<int>> A;
    int p = 0;
    For(i, n) {
        vector<int> C;
        For(v, p, p + S[i]) C.emplace_back(v);
        A.emplace_back(C);
        p += S[i];
    }
    assert(p == N + n - 1);
    UnionFind uf(p);
    auto H = random_tree(n);
    for (auto& [c1, c2] : H) {
        int a = A[c1][RNG(0, len(A[c1]))];
        int b = A[c2][RNG(0, len(A[c2]))];
        uf.merge(a, b);
    }
    vector<int> new_idx(p);
    int x = 0;
    For(i, p) if (uf[i] == i) new_idx[i] = x++;
    assert(x == N);
    For(i, p) new_idx[i] = new_idx[uf[i]];
    vector<pair<int, int>> G;
    For(i, n) {
        vector<int>& V = A[i];
        for (auto& v : V) v = new_idx[v];
        if (len(V) == 2) {
            G.emplace_back(V[0], V[1]);
        } else {
            For(k, len(V)) { G.emplace_back(V[k], V[(1 + k) % len(V)]); }
        }
    }
    random_relabel(N, G);
    return G;
}

// |child| = 0 or 2 or (1 if can1); labels are in topological order
// return: {lch,rch}
pair<vector<int>, vector<int>> random_binary_tree(int N, bool can_1) {
    if (can_1) {
        vector<pair<int, int>> S;
        S.emplace_back(0, 0), S.emplace_back(0, 1);
        vector<int> lch(N, -1), rch(N, -1);
        For(v, 1, N) {
            int k = RNG(0, len(S));
            swap(S[k], S.back());
            auto [r, side] = POP(S);
            (side == 0 ? lch[r] : rch[r]) = v;
            S.emplace_back(v, 0), S.emplace_back(v, 1);
        }
        return {lch, rch};
    }
    // 0 or 2
    assert(N % 2 == 1);
    vector<int> lch(N, -1), rch(N, -1);
    vector<int> S;
    For(v, N / 2, N) S.emplace_back(v);
    int nxt = N / 2 - 1;
    while (len(S) >= 2) {
        shuffle(S);
        int a = POP(S), b = POP(S);
        lch[nxt] = a, rch[nxt] = b;
        S.emplace_back(nxt--);
    }
    return {lch, rch};
}

// N>=3. biconnected.
vector<pair<int, int>> random_outerplanar(int N) {
    assert(N >= 3);
    vector<pair<int, int>> E;
    int M = RNG(0, (N - 3) + 1);
    For(M) {
        int a = RNG(0, N);
        int b = RNG(0, N);
        if (a == b) continue;
        if (a > b) swap(a, b);
        if (b == a + 1 || b == a + (N - 1)) continue;
        bool ok = 1;
        for (auto& [c, d] : E) {
            if (a == c && b == d) ok = 0;
            if (a == c || a == d || b == c || b == d) continue;
            if (a < c && c < b && b < d) ok = 0;
            if (c < a && a < d && d < b) ok = 0;
        }
        if (ok) E.emplace_back(a, b);
    }
    vector<int> label(N);
    For(i, N) label[i] = i;
    shuffle(label);
    For(i, N) E.emplace_back(i, (i + 1) % N);
    for (auto& [a, b] : E) {
        a = label[a], b = label[b];
        if (RNG(0, 2)) swap(a, b);
    }
    shuffle(E);
    return E;
}
} // namespace mitsuha
#endif // AJAY_RANDOM_GRAPH
