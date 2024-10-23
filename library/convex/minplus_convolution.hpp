#ifndef AJAY_MINPLUS_CONVOLUTION
#define AJAY_MINPLUS_CONVOLUTION

#include "library/convex/monotone_minima.hpp"

namespace mitsuha{
template <typename T>
vector<T> minplus_convolution_convex_convex(vector<T>& A, vector<T>& B) {
    int n = len(A), m = len(B);
    if (n == 0 && m == 0) return {};
    vector<T> C(n + m - 1, numeric_limits<T>::max() / 2);
    while (n > 0 && A[n - 1] == numeric_limits<T>::max() / 2) --n;
    while (m > 0 && B[m - 1] == numeric_limits<T>::max() / 2) --m;
    if (n == 0 || m == 0) return C;
    int a = 0, b = 0;
    while (a < n && A[a] == numeric_limits<T>::max() / 2) ++a;
    while (b < m && B[b] == numeric_limits<T>::max() / 2) ++b;
    C[a + b] = A[a] + B[b];
    for (int i = a + b + 1; i < n + m - 1; ++i) {
        if (b == m - 1 || (a != n - 1 && A[a + 1] + B[b] < A[a] + B[b + 1])) {
            chmin(C[i], A[++a] + B[b]);
        } else {
            chmin(C[i], A[a] + B[++b]);
        }
    }
    return C;
}

template <typename T>
vector<T> minplus_convolution_arbitrary_convex(vector<T>& A, vector<T>& B) {
    int n = len(A), m = len(B);
    if (n == 0 && m == 0) return {};
    vector<T> C(n + m - 1, numeric_limits<T>::max() / 2);
    while (m > 0 && B[m - 1] == numeric_limits<T>::max() / 2) --m;
    if (m == 0) return C;
    int b = 0;
    while (b < m && B[b] == numeric_limits<T>::max() / 2) ++b;

    auto select = [&](int i, int j, int k) -> bool {
        if (i < k) return false;
        if (i - j >= m - b) return true;
        return A[j] + B[b + i - j] >= A[k] + B[b + i - k];
    };
    vector<int> J = monotone_minima(n + m - b - 1, n, select);
    For(i, n + m - b - 1) {
        T x = A[J[i]], y = B[b + i - J[i]];
        if (x < numeric_limits<T>::max() / 2 && y < numeric_limits<T>::max() / 2) C[b + i] = x + y;
    }
    return C;
}

template <typename T, bool convA, bool convB>
vector<T> minplus_convolution(vector<T>& A, vector<T>& B) {
    static_assert(convA || convB);
    if constexpr (convA && convB) return minplus_convolution_convex_convex(A, B);
    if constexpr (convA && !convB)
        return minplus_convolution_arbitrary_convex(B, A);
    if constexpr (convB && !convA)
        return minplus_convolution_arbitrary_convex(A, B);
    return {};
}
} // namespace mitsuha
#endif // AJAY_MINPLUS_CONVOLUTION