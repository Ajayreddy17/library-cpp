#ifndef AJAY_INDEX_COMPRESSION
#define AJAY_INDEX_COMPRESSION

namespace mitsuha{
template <typename T>
struct Index_Compression_DISTINCT_SMALL {
    static_assert(is_same_v<T, int>);
    int mi, ma;
    vector<int> dat;
    vector<int> build(vector<int> X) {
        mi = 0, ma = -1;
        if (!X.empty()) mi = *min_element(X.begin(), X.end()), ma = *max_element(X.begin(), X.end());
        dat.assign(ma - mi + 2, 0);
        for (auto& x: X) dat[x - mi + 1]++;
        For(i, len(dat) - 1) dat[i + 1] += dat[i];
        for (auto& x: X) { x = dat[x - mi]++; }
        Frr(i, 1, len(dat)) dat[i] = dat[i - 1];
        dat[0] = 0;
        return X;
    }
    int operator()(long long x) { return dat[clamp<long long>(x - mi, 0, ma - mi + 1)]; }
};

template <typename T>
struct Index_Compression_SAME_SMALL {
    static_assert(is_same_v<T, int>);
    int mi, ma;
    vector<int> dat;
    vector<int> build(vector<int> X) {
        mi = 0, ma = -1;
        if (!X.empty()) mi = *min_element(X.begin(), X.end()), ma = *max_element(X.begin(), X.end());
        dat.assign(ma - mi + 2, 0);
        for (auto& x: X) dat[x - mi + 1] = 1;
        For(i, len(dat) - 1) dat[i + 1] += dat[i];
        for (auto& x: X) { x = dat[x - mi]; }
        return X;
    }
    int operator()(long long x) { return dat[clamp<long long>(x - mi, 0, ma - mi + 1)]; }
};

template <typename T>
struct Index_Compression_SAME_LARGE {
    vector<T> dat;
    vector<int> build(vector<T> X) {
        auto I = argsort(X);
        vector<int> res(len(X));
        for (auto& i: I) {
            if (!dat.empty() && dat.back() == X[i]) {
                res[i] = len(dat) - 1;
            } else {
                res[i] = len(dat);
                dat.emplace_back(X[i]);
            }
        }
        dat.shrink_to_fit();
        return res;
    }
    int operator()(T x) { return lower_bound(dat.begin(), dat.end(), x) - dat.begin(); }
};

template <typename T>
struct Index_Compression_DISTINCT_LARGE {
    vector<T> dat;
    vector<int> build(vector<T> X) {
        auto I = argsort(X);
        vector<int> res(len(X));
        for (auto& i: I) { res[i] = len(dat), dat.emplace_back(X[i]); }
        dat.shrink_to_fit();
        return res;
    }
    int operator()(T x) { return lower_bound(dat.begin(), dat.end(), x) - dat.begin(); }
};

template <typename T, bool SMALL>
using Index_Compression_DISTINCT =
        typename std::conditional<SMALL, Index_Compression_DISTINCT_SMALL<T>,
                Index_Compression_DISTINCT_LARGE<T>>::type;
template <typename T, bool SMALL>
using Index_Compression_SAME =
        typename std::conditional<SMALL, Index_Compression_SAME_SMALL<T>,
                Index_Compression_SAME_LARGE<T>>::type;

// SAME: [2,3,2] -> [0,1,0]
// DISTINCT: [2,2,3] -> [0,2,1]
// (x): return lower_bound(X,x)
template <typename T, bool SAME, bool SMALL>
using Index_Compression =
        typename std::conditional<SAME, Index_Compression_SAME<T, SMALL>,
                Index_Compression_DISTINCT<T, SMALL>>::type;
} // namespace mitsuha
#endif // AJAY_INDEX_COMPRESSION
