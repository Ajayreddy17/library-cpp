#ifndef AJAY_CSR
#define AJAY_CSR


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/ds/csr.hpp

template <typename T>
struct CSR {
    int n;
    bool prepared;
    vector<int> ptr;
    vector<int> I;
    vector<T> dat;

    CSR(int n = 0) : n(n), prepared(false) {}
    void reserve(int n) { dat.reserve(n); }

    void add(int i, const T& x) {
        assert(0 <= i && i < n && !prepared);
        I.emplace_back(i), dat.emplace_back(x);
    }

    void build() {
        assert(!prepared);
        prepared = 1;
        ptr.assign(n + 1, 0);
        for (auto& i : I) ptr[1 + i]++;
        For(i, len(ptr) - 1) ptr[i + 1] += ptr[i];
        vector<T> tmp(len(dat));
        For(k, len(dat)) {
            int i = I[k];
            tmp[ptr[i]++] = dat[k];
        }
        swap(dat, tmp);
        ptr.pop_back();
        ptr.insert(ptr.begin(), 0);
        I.clear();
    }

    struct range {
        T *first, *last;
        T* begin() const { return first; }
        T* end() const { return last; }
        bool empty() const { return first == last; }
        int size() const { return last - first; }
    };

    range operator[](int i) {
        assert(prepared);
        return range{dat.data() + ptr[i], dat.data() + ptr[i + 1]};
    }
};
} // namespace mitsuha
#endif // AJAY_CSR
