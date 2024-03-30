#ifndef AJAY_CUMSUM
#define AJAY_CUMSUM

namespace mitsuha{
template <class T>
struct Cumsum{
    int n;
    vector<T> data;
    Cumsum() = default;
    Cumsum(const vector<T> &v): n((int) v.size()), data(n + 1, T(0)) {
        for (int i = 0; i < n; i ++) data[i + 1] = data[i] + v[i];
    }

    T operator()(int l, int r) const{
        assert(0 <= l and l <= r and r <= n);
        return data[r] - data[l];
    }

    T query(int l, int r) const{
        return (*this)(l, r);
    }

    template<typename T_large = long long>
    T_large query_circular(long long l, long long r) const{
        assert(0 <= l and l <= r);
        if (l == r) return T(0);
        T_large blockL = l / n;
        T_large blockR = (r - 1) / n;
        if (blockL == blockR){
            return (*this)(l % n, (l % n) + r - l);
        }
        return (blockR - blockL - 1) * data.back() + (*this)(l % n, n) + (*this)(0, r - blockR * n);
    }

    void push_back(const T& v) {
        if (data.empty()) data.push_back(T(0));
        T new_sum = data.back() + v;
        ++n, data.push_back(std::move(new_sum));
    }
};
} // namespace mitsuha
#endif // AJAY_CUMSUM