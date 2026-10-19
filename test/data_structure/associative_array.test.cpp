#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include <library/template.hpp>

void sol_1() {
    LL(Q);
    map<long long, long long> A;
    For(_, Q) {
        LL(t);
        if (t == 0) {
            LL(k, v);
            A[k] = v;
        } else {
            LL(k);
            print(A[k]);
        }
    }
}

void sol_2() {
    LL(Q);
    unordered_map<long long, long long> A;
    A.reserve(Q);
    For(_, Q) {
        LL(t);
        if (t == 0) {
            LL(k, v);
            A[k] = v;
        } else {
            LL(k);
            print(A[k]);
        }
    }
}

void sol_3() {
    LL(Q);
    using T = tuple<long long, long long, long long>;
    vector<T> query(Q);
    vector<long long> key;
    key.reserve(Q);
    For(q, Q) {
        LL(t);
        if (t == 0) {
            LL(k, v);
            query[q] = {t, k, v};
            key.emplace_back(k);
        } else {
            LL(k);
            query[q] = {t, k, 0};
            key.emplace_back(k);
        }
    }
    settify(key);
    vector<long long> A(len(key));
    For(q, Q) {
        auto [t, k, v] = query[q];
        k = lower_bound(key.begin(), key.end(), k) - key.begin();
        if (t == 0) {
            A[k] = v;
        } else {
            print(A[k]);
        }
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    // sol_1();
    // sol_2();
    sol_3();

    return 0;
}
