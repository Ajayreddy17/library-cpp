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

    void push_back(const T& v) {
        if (data.empty()) data.push_back(T(0));
        T new_sum = data.back() + v;
        ++n, data.push_back(std::move(new_sum));
    }
    /*
    // Assuming all a[i] >= 0, Max r such that sum(a[l:r]) <= / < sum;
    int max_pref(int idx, T sum, bool less_equal = true) {
        assert(0 <= idx and idx < n and (less_equal ? T(0) <= sum: T(0) < sum));
        return *ranges::partition_point(ranges::iota_view(idx, n + 1), [&](int r){
            return less_equal ? query(idx, r) <= sum: query(idx, r) < sum;
        }) - 1;
    }
    */
};
} // namespace mitsuha
#endif // AJAY_CUMSUM