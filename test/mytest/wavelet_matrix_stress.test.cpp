#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/random/shuffle.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_old.hpp"
#include "library/datastructure/static_range_product_group.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_add.hpp"
#include "library/monoid/monoid_max.hpp"
#include "library/graph/datastructure/tree_wavelet_matrix.hpp"

using ll = long long;

// values of A[L, R) sorted, with their weights in index order for ties
vector<pair<ll, ll>> sorted_range(const vector<ll>& A, const vector<ll>& W, int L, int R) {
    vector<pair<ll, ll>> v;
    For(i, L, R) v.emplace_back(A[i], W[i]);
    stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first < b.first; });
    return v;
}

// WM over A with weights W (Monoid_Add<ll>); A, W may change through set/add on both sides
template <typename WMT>
void check_queries(WMT& WM, const vector<ll>& A, const vector<ll>& W, ll lo, ll hi, bool compressed) {
    int n = len(A);
    For(q, 60) {
        int L = RNG(0, n + 1), R = RNG(0, n + 1);
        if (L > R) swap(L, R);
        ll y1 = RNG(lo - 2, hi + 3), y2 = RNG(lo - 2, hi + 3);
        if (!compressed) chmax(y1, 0LL), chmax(y2, 0LL);
        if (y1 > y2) swap(y1, y2);
        int c = 0, pc = 0;
        ll s = 0, ps = 0;
        For(i, L, R) {
            if (y1 <= A[i] && A[i] < y2) c++, s += W[i];
            if (A[i] < y2) pc++, ps += W[i];
        }
        assert(WM.count(L, R, y1, y2) == c);
        assert(WM.prefix_count(L, R, y2) == pc);
        assert(WM.prod(L, R, y1, y2) == s);
        assert(WM.prefix_prod(L, R, y2) == ps);
        assert(WM.count_and_prod(L, R, y1, y2) == make_pair(c, s));
        assert(WM.prefix_count_and_prod(L, R, y2) == make_pair(pc, ps));
        ll all = 0;
        For(i, L, R) all += W[i];
        assert(WM.prod_all(L, R) == all);

        auto v = sorted_range(A, W, L, R);
        // next / prev
        ll nx = numeric_limits<ll>::max() / 2, pv = -nx;
        for (auto [a, w]: v) {
            if (a >= y1) chmin(nx, a);
            if (a <= y1) chmax(pv, a);
        }
        using Y = decltype(WM.kth(0, 1, 0));
        ll inf = numeric_limits<Y>::max() / 2;
        auto norm = [&](ll x) { return abs(x) == numeric_limits<ll>::max() / 2 ? (x > 0 ? inf : -inf) : x; };
        if (compressed || y1 >= 0) {
            assert(WM.next(L, R, y1) == norm(nx));
            assert(WM.prev(L, R, y1) == norm(pv));
        }
        if (L == R) continue;
        int k = RNG(0, R - L);
        assert(WM.kth(L, R, k) == v[k].first);
        assert(WM.template median<false>(L, R) == v[(R - L - 1) / 2].first);
        assert(WM.template median<true>(L, R) == v[(R - L) / 2].first);
        int k2 = RNG(0, R - L + 1);
        ll pre = 0;
        For(i, k2) pre += v[i].second;
        auto [val, t] = WM.kth_value_and_prod(L, R, k2);
        assert(t == pre && val == (k2 == R - L ? inf : v[k2].first));
        int k1 = RNG(0, k2 + 1);
        ll mid = 0;
        For(i, k1, k2) mid += v[i].second;
        assert(WM.prod_index_range(L, R, k1, k2) == mid);
    }
}

void test_wavelet_matrix() {
    For(iter, 400) {
        int n = RNG(0, 40);
        ll hi = RNG(1, 3) == 1 ? RNG(1, 8) : RNG(1, 1000);
        // uncompressed int, weights through Static_Range_Product_Group
        {
            vector<ll> A(n), W(n);
            For(i, n) A[i] = RNG(0, hi), W[i] = RNG(-5, 10);
            Wavelet_Matrix<int, false, Static_Range_Product_Group<Monoid_Add<ll>>> WM(
                n, [&](int i) -> pair<int, ll> { return {A[i], W[i]}; });
            check_queries(WM, A, W, 0, hi, false);
            // explicit log
            Wavelet_Matrix<int, false, Static_Range_Product_Group<Monoid_Add<ll>>> WM2(
                n, [&](int i) -> pair<int, ll> { return {A[i], W[i]}; }, 12);
            assert(WM2.log == 12);
            check_queries(WM2, A, W, 0, hi, false);
        }
        // compressed long long with negatives and huge values, Fenwick weights with updates
        {
            vector<ll> A(n), W(n);
            ll big = RNG(0, 2) ? (ll)1e18 : 10;
            For(i, n) A[i] = RNG(-big, big), W[i] = RNG(0, 10);
            if (n && RNG(0, 2)) For(i, n) A[i] = A[RNG(0, i + 1)];  // duplicates
            Wavelet_Matrix<ll, true, FenwickTree<Monoid_Add<ll>>> WM(
                n, [&](int i) -> pair<ll, ll> { return {A[i], W[i]}; });
            check_queries(WM, A, W, -big, big, true);
            For(t, 10) if (n) {
                int i = RNG(0, n);
                ll x = RNG(-5, 10);
                if (t % 2) WM.add(i, x), W[i] += x;
                else WM.set(i, x), W[i] = x;
            }
            check_queries(WM, A, W, -big, big, true);
        }
        // uncompressed long long near 2^62, counting only
        {
            vector<ll> A(n), W(n, 0);
            For(i, n) A[i] = RNG(0, 2) ? RNG(0, 50) : (ll)RNG(0, 1LL << 61) * 2 + RNG(0, 2);
            Wavelet_Matrix<ll, false> WM(A);
            For(q, 40) {
                int L = RNG(0, n + 1), R = RNG(0, n + 1);
                if (L > R) swap(L, R);
                ll y = RNG(0, 2) ? RNG(0, 60) : (ll)RNG(0, 1LL << 62);
                int c = 0;
                For(i, L, R) c += A[i] < y;
                assert(WM.prefix_count(L, R, y) == c);
                if (L < R) {
                    auto v = sorted_range(A, W, L, R);
                    int k = RNG(0, R - L);
                    assert(WM.kth(L, R, k) == v[k].first);
                }
            }
        }
    }
}

// max_right / max_right_many / min_left_many with check = sum of weights (>= 0) <= B
void test_binary_search() {
    For(iter, 2000) {
        int n = RNG(0, 30), hi = RNG(1, 70);
        vector<ll> A(n), W(n);
        For(i, n) A[i] = RNG(0, hi), W[i] = RNG(0, 6);
        Wavelet_Matrix<int, false, Static_Range_Product_Group<Monoid_Add<ll>>> WM(
            n, [&](int i) -> pair<int, ll> { return {A[i], W[i]}; });
        const ll INF = numeric_limits<int>::max() / 2;
        int m = RNG(1, 4);
        vector<pair<int, int>> LR;
        For(m) {
            int L = RNG(0, n + 1), R = RNG(0, n + 1);
            if (L > R) swap(L, R);
            LR.emplace_back(L, R);
        }
        ll B = RNG(0, 40);
        auto stat = [&](ll y1, ll y2) {  // over LR x [y1, y2)
            int c = 0;
            ll s = 0;
            for (auto [L, R]: LR) For(i, L, R) if (y1 <= A[i] && A[i] < y2) c++, s += W[i];
            return make_pair(c, s);
        };
        // max_right: largest y in [0, limit) ∪ {INF} with sum over [0, y) <= B
        auto check = [&](int, int, ll s) { return s <= B; };
        auto [y, c, s] = WM.max_right_many(LR, check);
        if (stat(0, INF).second <= B) {
            assert(y == INF);
        } else {
            assert(0 <= y && y < WM.limit && stat(0, y).second <= B && stat(0, y + 1).second > B);
        }
        assert(make_pair(c, s) == stat(0, y));
        if (m == 1) assert(WM.max_right(LR[0].first, LR[0].second, check) == make_tuple(y, c, s));
        // min_left: smallest y with sum over [y, limit) <= B
        auto [y2, c2, s2] = WM.min_left_many(LR, check);
        assert(0 <= y2 && y2 <= WM.limit && stat(y2, WM.limit).second <= B);
        if (y2 > 0) assert(stat(y2 - 1, WM.limit).second > B);
        assert(make_pair(c2, s2) == stat(y2, WM.limit));
    }
}

void test_2d() {
    For(iter, 300) {
        int n = RNG(0, 40);
        ll R = RNG(1, 30);
        vector<ll> X(n), Y(n), W(n);
        For(i, n) X[i] = RNG(-R, R), Y[i] = RNG(-R, R), W[i] = RNG(0, 10);
        auto brute = [&](ll x1, ll x2, ll y1, ll y2) {
            int c = 0;
            ll s = 0;
            For(i, n) if (x1 <= X[i] && X[i] < x2 && y1 <= Y[i] && Y[i] < y2) c++, s += W[i];
            return make_pair(c, s);
        };
        Wavelet_Matrix_2D_Range<ll, true, true, FenwickTree<Monoid_Add<ll>>> WM(
            n, [&](int i) -> tuple<ll, ll, ll> { return {X[i], Y[i], W[i]}; });
        // small x (dense compression), compressed y
        Wavelet_Matrix_2D_Range<int, false, true, FenwickTree<Monoid_Add<ll>>> WS(
            n, [&](int i) -> tuple<int, int, ll> { return {X[i], Y[i], W[i]}; });
        For(q, 60) {
            if (n && q % 4 == 0) {
                int i = RNG(0, n);
                ll x = RNG(0, 10);
                if (q % 8) WM.add(i, x), WS.add(i, x), W[i] += x;
                else WM.set(i, x), WS.set(i, x), W[i] = x;
            }
            ll x1 = RNG(-R - 2, R + 3), x2 = RNG(-R - 2, R + 3), y1 = RNG(-R - 2, R + 3), y2 = RNG(-R - 2, R + 3);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            auto [c, s] = brute(x1, x2, y1, y2);
            assert(WM.count(x1, x2, y1, y2) == c && WM.prod(x1, x2, y1, y2) == s);
            assert(WS.count(x1, x2, y1, y2) == c && WS.prod(x1, x2, y1, y2) == s);
            assert(WM.count_and_prod(x1, x2, y1, y2) == make_pair(c, s));
            assert(WM.prefix_count_and_prod(x1, x2, y2) == brute(x1, x2, -linf, y2));
            assert(WM.prefix_count(x1, x2, y2) == brute(x1, x2, -linf, y2).first);
            assert(WM.prefix_prod(x1, x2, y2) == brute(x1, x2, -linf, y2).second);
            assert(WM.prod_all(x1, x2) == brute(x1, x2, -linf, linf).second);
        }
    }
    // non-invertible monoid: max through SegTree
    For(iter, 200) {
        int n = RNG(0, 30);
        vector<ll> X(n), Y(n), W(n);
        For(i, n) X[i] = RNG(0, 20), Y[i] = RNG(0, 20), W[i] = RNG(-100, 100);
        Wavelet_Matrix_2D_Range<ll, true, false, SegTree<Monoid_Max<ll>>> WM(
            n, [&](int i) -> tuple<ll, ll, ll> { return {X[i], Y[i], W[i]}; });
        For(q, 40) {
            if (n && q % 3 == 0) {
                int i = RNG(0, n);
                W[i] = RNG(-100, 100), WM.set(i, W[i]);
            }
            ll x1 = RNG(-2, 23), x2 = RNG(-2, 23), y1 = RNG(0, 23), y2 = RNG(0, 23);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            ll mx = Monoid_Max<ll>::unit();
            For(i, n) if (x1 <= X[i] && X[i] < x2 && y1 <= Y[i] && Y[i] < y2) chmax(mx, W[i]);
            assert(WM.prod(x1, x2, y1, y2) == mx);
        }
    }
}

// Wavelet_Matrix_Old: segments and xor_val
void test_old() {
    For(iter, 500) {
        int n = RNG(0, 30), lg = RNG(1, 6);
        vector<ll> A(n), W(n);
        For(i, n) A[i] = RNG(0, 1 << lg), W[i] = RNG(-5, 10);
        Wavelet_Matrix_Old<ll, false> WM(A, W, lg);
        Wavelet_Matrix_Old<ll, true> WC(A, W);
        For(q, 40) {
            int segs = RNG(1, 4);
            vector<pair<int, int>> S;
            For(segs) {
                int L = RNG(0, n + 1), R = RNG(0, n + 1);
                if (L > R) swap(L, R);
                S.emplace_back(L, R);
            }
            ll xv = RNG(0, 1 << lg);
            vector<pair<ll, ll>> v;
            for (auto [L, R]: S) For(i, L, R) v.emplace_back(A[i] ^ xv, W[i]);
            stable_sort(v.begin(), v.end(), [](auto& p, auto& q) { return p.first < q.first; });
            ll a = RNG(-1, (1 << lg) + 2), b = RNG(-1, (1 << lg) + 2);
            if (a > b) swap(a, b);
            int c = 0;
            for (auto [x, w]: v) c += a <= x && x < b;
            assert(WM.count(S, a, b, xv) == c);
            auto [L, R] = S[0];
            int c0 = 0, cc = 0;
            For(i, L, R) c0 += a <= (A[i] ^ xv) && (A[i] ^ xv) < b, cc += a <= A[i] && A[i] < b;
            assert(WM.count(L, R, a, b, xv) == c0);
            assert(WC.count(L, R, a, b) == cc);
            ll all = 0;
            for (auto [x, w]: v) all += w;
            assert(WM.sum_all(S) == all);
            if (v.empty()) continue;
            int k = RNG(0, len(v));
            assert(WM.kth(S, k, xv) == v[k].first);
            assert(WM.median(false, S, xv) == v[(len(v) - 1) / 2].first);
            // prefix sums by rank: equal values are taken in segment order, then index order
            int k2 = RNG(0, len(v) + 1);
            ll pre = 0;
            For(i, k2) pre += v[i].second;
            assert(WM.kth_value_and_sum(S, k2, xv).second == pre);
            if (L < R) {
                vector<pair<ll, ll>> u, uc;
                For(i, L, R) u.emplace_back(A[i] ^ xv, W[i]), uc.emplace_back(A[i], W[i]);
                stable_sort(u.begin(), u.end(), [](auto& p, auto& q) { return p.first < q.first; });
                stable_sort(uc.begin(), uc.end(), [](auto& p, auto& q) { return p.first < q.first; });
                int j = RNG(0, R - L);
                assert(WM.kth(L, R, j, xv) == u[j].first);
                assert(WC.kth(L, R, j) == uc[j].first);
                int j1 = RNG(0, R - L + 1), j2 = RNG(0, R - L + 1);
                if (j1 > j2) swap(j1, j2);
                ll s = 0;
                For(i, j1, j2) s += u[i].second;
                assert(WM.sum(L, R, j1, j2, xv) == s);
            }
        }
    }
}

void test_tree() {
    For(iter, 300) {
        int N = RNG(1, 15);
        Graph<int, 0> G(N);
        vector<int> perm(N);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm);
        For(i, 1, N) G.add(perm[RNG(0, i)], perm[i]);
        G.build();
        Tree<decltype(G)> tree(G);
        vector<ll> A(N), S(N), EA(N - 1), ES(N - 1);
        For(v, N) A[v] = RNG(0, 8), S[v] = RNG(0, 10);
        For(e, N - 1) EA[e] = RNG(0, 8), ES[e] = RNG(0, 10);
        Tree_Wavelet_Matrix<decltype(tree), false, ll, false> TV(tree, A, S, 3);
        Tree_Wavelet_Matrix<decltype(tree), true, ll, false> TE(tree, EA, ES, 3);
        auto path = [&](int s, int t) {
            vector<int> P;
            int c = tree.lca(s, t);
            for (int x = s; x != c; x = tree.parent[x]) P.emplace_back(x);
            vector<int> Q;
            for (int x = t; x != c; x = tree.parent[x]) Q.emplace_back(x);
            P.emplace_back(c);
            P.insert(P.end(), Q.rbegin(), Q.rend());
            return P;
        };
        For(q, 30) {
            int s = RNG(0, N), t = RNG(0, N), u = RNG(0, N);
            ll xv = RNG(0, 8), a = RNG(0, 9), b = RNG(0, 9);
            if (a > b) swap(a, b);
            auto P = path(s, t);
            vector<pair<ll, ll>> vv, ve;
            for (int x: P) vv.emplace_back(A[x] ^ xv, S[x]);
            For(i, len(P) - 1) {
                int x = P[i], y = P[i + 1];
                int e = (tree.parent[x] == y ? tree.VtoE[x] : tree.VtoE[y]);
                ve.emplace_back(EA[e] ^ xv, ES[e]);
            }
            sort(vv.begin(), vv.end()), sort(ve.begin(), ve.end()); // only kth / count / sum over all ties
            auto cnt = [&](auto& v) {
                int c = 0;
                for (auto [x, w]: v) c += a <= x && x < b;
                return c;
            };
            assert(TV.count_path(s, t, a, b, xv) == cnt(vv));
            assert(TE.count_path(s, t, a, b, xv) == cnt(ve));
            int k = RNG(0, len(vv));
            assert(TV.kth_path(s, t, k, xv) == vv[k].first);
            int k1 = RNG(0, len(vv) + 1), k2 = RNG(0, len(vv) + 1);
            if (k1 > k2) swap(k1, k2);
            // equal values are taken in HLD order: only compare at boundaries between distinct values
            auto cut = [&](int k) { return k == 0 || k == len(vv) || vv[k - 1].first != vv[k].first; };
            if (cut(k1) && cut(k2)) {
                ll sm = 0;
                For(i, k1, k2) sm += vv[i].second;
                assert(TV.sum_path(s, t, k1, k2, xv) == sm);
            }
            if (!ve.empty()) {
                int j = RNG(0, len(ve));
                assert(TE.kth_path(s, t, j, xv) == ve[j].first);
            }
            // subtree of u, and subtree of u when the tree is rerooted at r
            vector<ll> sub, sube;
            for (int x: tree.collect_subtree(u)) {
                sub.emplace_back(A[x] ^ xv);
                if (x != u) sube.emplace_back(EA[tree.VtoE[x]] ^ xv);
            }
            int cs = 0, cse = 0;
            for (ll x: sub) cs += a <= x && x < b;
            for (ll x: sube) cse += a <= x && x < b;
            assert(TV.count_subtree(u, a, b, xv) == cs);
            assert(TE.count_subtree(u, a, b, xv) == cse);
            int r = RNG(0, N), cr = 0;
            For(x, N) {
                // x is in the subtree of u when rooted at r iff u is on the path r .. x
                auto Q = path(r, x);
                if (find(Q.begin(), Q.end(), u) != Q.end()) cr += a <= (A[x] ^ xv) && (A[x] ^ xv) < b;
            }
            assert(TV.count_subtree_root(u, a, b, r, xv) == cr);
        }
    }
}

// every number quoted in datastructure/theory/wavelet_matrix.md
void test_doc_examples() {
    vector<int> A = {5, 4, 5, 5, 2, 1, 5, 6, 1, 3, 5, 0};
    int n = len(A);
    Wavelet_Matrix<int, false, Static_Range_Product_Group<Monoid_Add<long long>>> WM(
        n, [&](int i) -> pair<int, long long> { return {A[i], A[i]}; });
    assert(WM.log == 3);
    assert(WM.bv[2].to_string() == "111100110010" && WM.mid[2] == 5);
    assert(WM.bv[1].to_string() == "100100000010" && WM.mid[1] == 9);
    assert(WM.bv[0].to_string() == "110101111010" && WM.mid[0] == 4);
    assert(WM.get_subtree(3, 2, 9) == make_tuple(0, 3, 7, 11));
    assert(WM.kth(2, 9, 3) == 5 && WM.count(2, 9, 2, 6) == 4 && WM.prod(2, 9, 2, 6) == 17);
    assert(WM.prefix_count(2, 9, 5) == 3);
    assert(WM.kth_value_and_prod(2, 9, 3) == make_pair(5, 4LL));
    assert(WM.next(2, 9, 3) == 5 && WM.prev(2, 9, 4) == 2 && WM.median<false>(2, 9) == 5);
    assert(WM.max_right(2, 9, [](int, int, long long s) { return s <= 10; }) == make_tuple(5, 3, 4LL));
    vector<long long> B = {-7, 300, 42, -7, 1000000000000LL, 42};
    Wavelet_Matrix<long long, true> C(B);
    assert(C.key == vector<long long>({-7, 42, 300, 1000000000000LL}));
    assert(C.kth(0, 6, 4) == 300 && C.count(0, 6, 0, 500) == 3);
    // figure 4: [2, 5) x [1, 6)
    vector<pair<int, int>> P = {{1, 4}, {3, 1}, {2, 6}, {5, 3}, {4, 5}, {6, 2}, {7, 6}, {3, 4}};
    Wavelet_Matrix_2D_Range<int, true, true> W2(len(P), [&](int i) -> tuple<int, int, char> { return {P[i].first, P[i].second, 0}; });
    assert(W2.count(2, 5, 1, 6) == 3);
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_wavelet_matrix();
    test_binary_search();
    test_2d();
    test_old();
    test_tree();
    test_doc_examples();
    solve();
    return 0;
}
