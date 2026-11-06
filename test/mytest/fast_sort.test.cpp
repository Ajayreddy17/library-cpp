#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>
#include <library/util/fast_sort.hpp>

template <class T>
void check(vector<T> a, bool desc) {
    vector<T> b = a;
    if (desc) {
        fast_sort<true>(a);
        sort(b.begin(), b.end(), greater<T>());
    } else {
        fast_sort(a);
        sort(b.begin(), b.end());
    }
    assert(a == b);
}

void test() {
    int N = RNG(0, 1000) < 300 ? RNG(0, 400) : RNG(300, 6000);
    int mod = (int[]){4, 1000, 1 << 30}[RNG(0, 3)];
    vector<int> x(N);
    vector<unsigned char> c(N);
    vector<long long> y(N);
    vector<double> d(N);
    For(i, N) {
        x[i] = RNG(-mod, mod);
        c[i] = RNG(0, 256);
        y[i] = (long long)RNG(-mod, mod) * RNG(1, 1 << 20);
        d[i] = RNG(-mod, mod) / 7.0;
    }
    for (bool desc : {false, true}) {
        check(x, desc), check(c, desc), check(y, desc), check(d, desc);
    }
    // stable on radix path, argsort
    if (N >= 2048) {
        vector<pair<int, int>> p(N), q;
        For(i, N) p[i] = {RNG(0, 50), i};
        q = p;
        fast_sort(q, [](const pair<int, int>& v) { return v.first; });
        stable_sort(p.begin(), p.end(), [](auto& a, auto& b) { return a.first < b.first; });
        assert(p == q);
    }
    // std::pair keys (packed into one integer key)
    {
        vector<pair<int, int>> p(N);
        for (auto& v : p) v = {RNG(-mod, mod), RNG(-mod, mod)};
        for (bool desc : {false, true}) check(p, desc);
        vector<pair<short, unsigned>> q(N);
        for (auto& v : q) v = {(short)RNG(-1000, 1000), (unsigned)RNG(0, mod)};
        for (bool desc : {false, true}) check(q, desc);
        vector<pair<long long, long long>> r(N);  // too wide to pack: merge_sort
        for (auto& v : r) v = {RNG(-mod, mod), RNG(-mod, mod)};
        for (bool desc : {false, true}) check(r, desc);
    }
    // 16-byte struct by key, both directions
    {
        struct S { long long k; int a, b; };
        vector<S> v(N);
        For(i, N) v[i] = {RNG(-mod, mod), (int)i, 0};
        for (bool desc : {false, true}) {
            auto w = v;
            if (desc) fast_sort<true>(w, [](const S& s) { return s.k; });
            else fast_sort(w, [](const S& s) { return s.k; });
            For(i, 1, N) assert(desc ? w[i - 1].k >= w[i].k : w[i - 1].k <= w[i].k);
            vector<int> seen(N);
            for (auto& s : w) seen[s.a]++;
            For(i, N) assert(seen[i] == 1);
        }
    }
    auto id = fast_argsort<true>(x);
    For(i, 1, N) assert(x[id[i - 1]] >= x[id[i]]);
    vector<pair<int, int>> pr(N);
    for (auto& v : pr) v = {RNG(0, 10), RNG(0, 10)};
    auto pr2 = pr;
    merge_sort(pr);
    sort(pr2.begin(), pr2.end());
    assert(pr == pr2);
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    For(300) test();
    solve();
}
