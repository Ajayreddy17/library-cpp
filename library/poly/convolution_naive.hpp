#ifndef AJAY_CONV_NAIVE
#define AJAY_CONV_NAIVE

namespace mitsuha{
template <class T, typename enable_if<!has_mod<T>::value>::type* = nullptr>
vector<T> convolution_naive(const vector<T>& a, const vector<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (n > m) return convolution_naive<T>(b, a);
    if (n == 0) return {};
    vector<T> ans(n + m - 1);
    For(i, n) For(j, m) ans[i + j] += a[i] * b[j];
    return ans;
}

template <class T, typename enable_if<has_mod<T>::value>::type* = nullptr>
vector<T> convolution_naive(const vector<T>& a, const vector<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (n > m) return convolution_naive<T>(b, a);
    if (n == 0) return {};
    vector<T> ans(n + m - 1);
    if (n <= 16 && (T::get_mod() < (1 << 30))) {
        for (int k = 0; k < n + m - 1; ++k) {
            int s = max(0, k - m + 1);
            int t = min(n, k + 1);
            unsigned long long sm = 0;
            for (int i = s; i < t; ++i) { sm += (unsigned long long)(a[i].val) * (b[k - i].val); }
            ans[k] = sm;
        }
    } else {
        for (int k = 0; k < n + m - 1; ++k) {
            int s = max(0, k - m + 1);
            int t = min(n, k + 1);
            unsigned __int128 sm = 0;
            for (int i = s; i < t; ++i) { sm += (unsigned long long)(a[i].val) * (b[k - i].val); }
            ans[k] = T::raw(sm % T::get_mod());
        }
    }
    return ans;
}
} // namespace mitsuha
#endif // AJAY_CONV_NAIVE