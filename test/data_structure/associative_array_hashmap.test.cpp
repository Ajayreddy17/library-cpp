#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include <library/template.hpp>
#include <library/datastructure/hashmap.hpp>

void solve() {
    LL(Q);
    HashMap<long long> A;
    For(Q) {
        LL(t);
        if (t == 0) {
            LL(k, v);
            A[k] = v;
        } else {
            LL(k);
            print(A.get(k, 0));
        }
    }
}

signed main() {
    solve();

    return 0;
}
