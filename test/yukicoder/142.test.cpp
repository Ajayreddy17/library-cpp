#define PROBLEM "https://yukicoder.me/problems/no/142"
#include "library/template.hpp"
#include "library/datastructure/bit_array.hpp"

void solve() {
    LL(N, S, X, Y, Z);
    LL(Q);
    using BS = Bit_Array;
    BS A(N);
    For(i, N) {
        A[i] = S & 1;
        S = (X * S + Y) % Z;
    }

    For(Q) {
        LL(a, b, c, d);
        --a, --c;
        BS x = A.slice(a, b);
        A.xor_to_range(c, d, x);
    }
    string ANS = A.to_string();
    for (auto&& ch : ANS) {
        ch = (ch == '0' ? 'E' : 'O');
    }
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
