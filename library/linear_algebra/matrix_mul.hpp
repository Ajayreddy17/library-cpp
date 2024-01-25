#ifndef AJAY_MATRIX_MUL
#define AJAY_MATRIX_MUL

namespace mitsuha{
template <class T, typename enable_if<has_mod<T>::value>::type* = nullptr>
vector<vector<T>> matrix_mul(const vector<vector<T>>& A, const vector<vector<T>>& B, int N1 = -1, int N2 = -1, int N3 = -1) {
    if (N1 == -1) { N1 = len(A), N2 = len(B), N3 = len(B[0]); }

    vector<vector<unsigned int>> b(N3, vector<unsigned int>(N2));
    For(i, N2) For(j, N3) b[j][i] = B[i][j].val;
    vector<vector<T>> C(N1, vector<T>(N3))

    if ((T::get_mod() < (1 << 30)) && N2 <= 16) {
        For(i, N1) For(j, N3) {
            unsigned long long sm = 0;
            For(m, N2) sm += (unsigned long long)(A[i][m].val) * b[j][m];
            C[i][j] = sm;
        }
    } else {
        For(i, N1) For(j, N3) {
            unsigned __int128 sm = 0;
            FOR(m, N2) sm += u64(A[i][m].val) * b[j][m];
            C[i][j] = T::raw(sm % (T::get_mod()));
        }
    }
    return C;
}

template <class T, typename enable_if<!has_mod<T>::value>::type* = nullptr>
vector<vector<T>> matrix_mul(const vector<vector<T>>& A, const vector<vector<T>>& B, int N1 = -1, int N2 = -1, int N3 = -1) {
    if (N1 == -1) { N1 = len(A), N2 = len(B), N3 = len(B[0]); }
    vector<vector<T>> b(N2, vector<T>(N3));
    For(i, N2) For(j, N3) b[j][i] = B[i][j];
    vector<vector<T>> C(N1, vector<T>(N3));
    For(n, N1) For(m, N2) FOR(k, N3) C[n][k] += A[n][m] * b[k][m];
    return C;
}

// square-matrix defined as array
template <class T, int N>
array<array<T, N>, N> matrix_mul(const array<array<T, N>, N>& A,
                                 const array<array<T, N>, N>& B) {
    array<array<T, N>, N> C{};

    if ((T::get_mod() < (1 << 30)) && N <= 16) {
        For(i, N) For(k, N) {
            unsigned long long sm = 0;
            For(j, N) sm += (unsigned long long)(A[i][j].val) * (B[j][k].val);
            C[i][k] = sm;
        }
    } else {
        For(i, N) For(k, N) {
            unsigned __int128 sm = 0;
            For(j, N) sm += ((unsigned long long))(A[i][j].val) * (B[j][k].val);
            C[i][k] = sm;
        }
    }
    return C;
}
} // namespace mitsuha
#endif // AJAY_MATRIX_MUL