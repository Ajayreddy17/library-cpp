#ifndef AJAY_SETFUNC_ZETA
#define AJAY_SETFUNC_ZETA

namespace mitsuha{
template <typename T>
void superset_zeta(vector<T>& A) {
    int log = (len(A) == 0 ? -1 : 31 - __builtin_clz(len(A)));
    assert(1 << log == len(A));
    For(n, log) For(s, 1 << log) {
        int t = s ^ (1 << n);
        if (s < t) A[s] += A[t];
    }
}

template <typename T>
void superset_mobius(vector<T>& A) {
    int log = (len(A) == 0 ? -1 : 31 - __builtin_clz(len(A)));
    assert(1 << log == len(A));
    For(n, log) For(s, 1 << log) {
        int t = s ^ (1 << n);
        if (s < t) A[s] -= A[t];
    }
}

template <typename T>
void subset_zeta(vector<T>& A) {
    int log = (len(A) == 0 ? -1 : 31 - __builtin_clz(len(A)));
    assert(1 << log == len(A));
    For(n, log) For(s, 1 << log) {
        int t = s ^ (1 << n);
        if (s > t) A[s] += A[t];
    }
}

template <typename T>
void subset_mobius(vector<T>& A) {
    int log = (len(A) == 0 ? -1 : 31 - __builtin_clz(len(A)));
    assert(1 << log == len(A));
    For(n, log) For(s, 1 << log) {
        int t = s ^ (1 << n);
        if (s > t) A[s] -= A[t];
    }
}
} // namespace mitsuha
#endif // AJAY_SETFUNC_ZETA