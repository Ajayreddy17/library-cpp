#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree.hpp"
#include "library/graph/tree/centroid_decomposition.hpp"
#include "library/graph/tree/centroid_decomposition_old.hpp"
#include "library/graph/tree/find_centroid.hpp"
#include "library/graph/tree/tree_all_distances.hpp"
#include "library/graph/datastructure/contour_query_range.hpp"
#include "library/graph/datastructure/incremental_centroid.hpp"

// random tree: random parent, or a path / star now and then
Graph<int, 0> random_tree(int N) {
    Graph<int, 0> G(N);
    int type = RNG(0, 4);
    vector<int> perm(N);
    iota(perm.begin(), perm.end(), 0);
    For(i, N) swap(perm[i], perm[RNG(0, i + 1)]);
    For(i, 1, N) {
        int p = (type == 0 ? i - 1 : type == 1 ? 0 : RNG(0, i));
        G.add(perm[p], perm[i]);
    }
    G.build();
    return G;
}

// max weight of a component of G - v
long long max_component(Graph<int, 0>& G, const vector<long long>& W, int v) {
    long long mx = 0;
    vector<int> vis(G.N);
    vis[v] = 1;
    for (auto& e: G[v]) {
        long long s = 0;
        vector<int> st = {e.to};
        vis[e.to] = 1;
        while (len(st)) {
            int x = st.back();
            st.pop_back();
            s += W[x];
            for (auto& f: G[x]) if (!vis[f.to]) vis[f.to] = 1, st.emplace_back(f.to);
        }
        chmax(mx, s);
    }
    return mx;
}

vector<vector<int>> all_dist(Graph<int, 0>& G) {
    int N = G.N;
    vector<vector<int>> D(N, vector<int>(N, -1));
    For(s, N) {
        deque<int> que = {(int)s};
        D[s][s] = 0;
        while (len(que)) {
            int v = que.front();
            que.pop_front();
            for (auto& e: G[v]) if (D[s][e.to] == -1) D[s][e.to] = D[s][v] + 1, que.push_back(e.to);
        }
    }
    return D;
}

// dist from local vertex 0 inside the local tree; par[i] < i
vector<int> local_depth(const vector<int>& par) {
    vector<int> d(len(par));
    For(i, 1, len(par)) {
        assert(0 <= par[i] && par[i] < i);
        d[i] = d[par[i]] + 1;
    }
    return d;
}

void test_modes() {
    For(iter, 300) {
        int N = RNG(1, 40);
        auto G = random_tree(N);
        auto D = all_dist(G);
        int LOG = 1;
        while ((1 << LOG) <= N) ++LOG;

        // MODE 0: every vertex is a centroid once; blocks are the subtrees around it
        {
            vector<int> seen(N);
            vector<vector<int>> cnt(N, vector<int>(N));
            auto f = [&](vector<int>& par, vector<int>& V, vector<int>& indptr) {
                int n = len(par);
                assert(V[0] == V[0] && seen[V[0]]++ == 0);
                assert(indptr[0] == 0 && indptr[1] == 1 && indptr.back() == n);
                auto d = local_depth(par);
                For(i, n) assert(d[i] == D[V[0]][V[i]]);
                // centroid: each block has at most n / 2 vertices
                For(k, 1, len(indptr) - 1) assert(2 * (indptr[k + 1] - indptr[k]) <= n);
                // paths through V[0]: endpoints in different blocks (block 0 = V[0])
                For(a, len(indptr) - 1) For(b, a + 1, len(indptr) - 1) {
                    For(i, indptr[a], indptr[a + 1]) For(j, indptr[b], indptr[b + 1]) {
                        assert(d[i] + d[j] == D[V[i]][V[j]]);
                        cnt[V[i]][V[j]]++, cnt[V[j]][V[i]]++;
                    }
                }
            };
            centroid_decomposition<0>(G, f);
            For(v, N) assert(seen[v] == 1);
            For(u, N) For(v, N) assert(cnt[u][v] == (u != v));
        }
        // MODE 1: every pair at distance >= 1 exactly once, as color 1 x color 2
        {
            vector<vector<int>> cnt(N, vector<int>(N));
            int calls = 0;
            auto f = [&](vector<int>& par, vector<int>& V, int L1, int R1, int L2, int R2) {
                ++calls;
                int n = len(par);
                auto d = local_depth(par);
                For(i, n) assert(d[i] == D[V[0]][V[i]]);
                assert(L1 < R1 && R1 <= L2 && L2 < R2 && R2 <= n);
                For(i, L1, R1) For(j, L2, R2) {
                    assert(d[i] + d[j] == D[V[i]][V[j]]);
                    cnt[V[i]][V[j]]++, cnt[V[j]][V[i]]++;
                }
            };
            centroid_decomposition<1>(G, f);
            For(u, N) For(v, N) assert(cnt[u][v] == (u != v));
            if (N == 1) assert(calls == 0);
        }
        // MODE 2: real vertices of different colors (0 / 1 / 2 = centroid) form each pair once
        {
            vector<vector<int>> cnt(N, vector<int>(N));
            auto f = [&](vector<int>& par, vector<int>& V, vector<int>& color) {
                int n = len(par);
                auto d = local_depth(par);
                For(i, n) assert(d[i] == D[V[0]][V[i]]);
                For(i, n) For(j, n) {
                    if (color[i] == -1 || color[j] == -1 || color[i] >= color[j]) continue;
                    assert(d[i] + d[j] == D[V[i]][V[j]]);
                    cnt[V[i]][V[j]]++, cnt[V[j]][V[i]]++;
                }
            };
            centroid_decomposition<2>(G, f);
            For(u, N) For(v, N) assert(cnt[u][v] == (u != v));
        }
        // tree_all_distances: unordered pairs
        {
            auto ANS = tree_all_distances(G);
            vector<long long> brute(N);
            For(u, N) For(v, u + 1, N) brute[D[u][v]]++;
            assert(ANS == brute);
        }
        // old: centroid tree depth, unique min-cdep vertex on each path
        {
            Centroid_Decomposition<decltype(G)> CD(G);
            For(v, N) assert(0 <= CD.cdep[v] && CD.cdep[v] < LOG);
            For(u, N) For(v, N) {
                int best = -1, cntmin = 0;
                For(w, N) if (D[u][w] + D[w][v] == D[u][v]) {
                    if (best == -1 || CD.cdep[w] < CD.cdep[best]) best = w, cntmin = 1;
                    else if (CD.cdep[w] == CD.cdep[best]) ++cntmin;
                }
                assert(cntmin == 1);
            }
            For(r, N) {
                auto [V, dist, indptr] = CD.collect_dist(r);
                assert(V[0] == r && indptr[0] == 0 && indptr[1] == 1 && indptr.back() == len(V));
                int expect = 0;
                For(w, N) if (CD.cdep[w] >= CD.cdep[r]) {
                    // w is in r's component iff no vertex of smaller cdep on the path
                    bool ok = true;
                    For(x, N) if (D[r][x] + D[x][w] == D[r][w] && CD.cdep[x] < CD.cdep[r]) ok = false;
                    expect += ok;
                }
                assert(len(V) == expect);
                For(i, len(V)) assert(dist[i] == D[r][V[i]]);
            }
        }
    }
}

void test_find_centroids() {
    For(iter, 2000) {
        int N = RNG(1, 30);
        auto G = random_tree(N);
        vector<long long> W(N);
        bool weighted = RNG(0, 2);
        For(v, N) W[v] = weighted ? RNG(0, 5) : 1;
        if (accumulate(W.begin(), W.end(), 0LL) == 0) W[0] = 1;
        long long FULL = accumulate(W.begin(), W.end(), 0LL);
        vector<int> good;
        For(v, N) if (2 * max_component(G, W, v) <= FULL) good.emplace_back(v);
        auto [a, b] = (weighted ? find_centroids(G, W) : find_centroids(G));
        vector<int> got;
        if (a != -1) got.emplace_back(a);
        if (b != -1) got.emplace_back(b);
        assert(1 <= len(got) && len(got) <= 2);
        // zero weights can make more than two vertices qualify; it reports two of them
        if (len(good) <= 2) assert(got == good);
        for (int v: got) assert(find(good.begin(), good.end(), v) != good.end());
    }
}

void test_contour() {
    For(iter, 200) {
        int N = RNG(1, 40);
        auto G = random_tree(N);
        auto D = all_dist(G);
        Contour_Query_Range<decltype(G), false> CQ(G);
        vector<long long> A(N);
        For(v, N) A[v] = RNG(0, 1000);
        vector<long long> dat(CQ.size());
        For(v, N) for (auto& p: CQ.get_indices(v)) dat[p] += A[v];
        For(q, 100) {
            int v = RNG(0, N), l = RNG(0, N + 1), r = RNG(0, N + 2);
            long long got = 0, brute = 0;
            for (auto& [a, b]: CQ.get_contour_range(v, l, r)) For(i, a, b) got += dat[i];
            For(w, N) if (w != v && l <= D[v][w] && D[v][w] < r) brute += A[w];
            assert(got == brute);
        }
    }
}

void test_incremental() {
    For(iter, 300) {
        int N = RNG(1, 30);
        auto G = random_tree(N);
        Tree<decltype(G)> tree(G);
        Incremental_Centroid<decltype(tree)> X(tree);
        vector<long long> W(N);
        For(step, 3 * N) {
            int v = RNG(0, N);
            X.add(v), W[v]++;
            long long FULL = accumulate(W.begin(), W.end(), 0LL);
            assert(2 * max_component(G, W, X.cent) <= FULL);
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_modes();
    test_find_centroids();
    test_contour();
    test_incremental();
    solve();
    return 0;
}
