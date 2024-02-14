#ifndef AJAY_FIND_LINEAR_REC
#define AJAY_FIND_LINEAR_REC

namespace mitsuha{
template <typename mint>
vector<mint> find_linear_rec(vector<mint>& A) {
    int N = len(A);
    vector<mint> B = {1}, C = {1};
    int l = 0, m = 1;
    mint p = 1;
    For(i, N) {
        mint d = A[i];
        For(j, 1, l + 1) { d += C[j] * A[i - j]; }
        if (d == 0) {
            ++m;
            continue;
        }
        auto tmp = C;
        mint q = d / p;
        if (len(C) < len(B) + m) C.insert(C.end(), len(B) + m - len(C), 0);
        For(j, len(B)) C[j + m] -= q * B[j];
        if (l + l <= i) {
            B = tmp;
            l = i + 1 - l, m = 1;
            p = d;
        } else {
            ++m;
        }
    }
    return C;
}
} // namespace mitsuha
#endif // AJAY_FIND_LINEAR_REC