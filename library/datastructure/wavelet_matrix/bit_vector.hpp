#ifndef AJAY_BITVECTOR
#define AJAY_BITVECTOR

namespace mitsuha{
struct Bit_Vector {
    int n;
    vector<pair<unsigned long long, unsigned int>> dat;
    
    Bit_Vector(int n) : n(n) { dat.assign((n + 127) >> 6, {0, 0}); }

    void set(int i) { dat[i >> 6].first |= 1ULL << (i & 63); }

    void reset() { fill(dat.begin(), dat.end(), pair<unsigned long long, unsigned int>{0, 0}); }

    void build() {
        For(i, len(dat) - 1) dat[i + 1].second = dat[i].second + __builtin_popcountll(dat[i].first);
    }
    // number of 1s in [0, k)
    int count(int k, bool f) {
        auto [a, b] = dat[k >> 6];
        int ret = b + __builtin_popcountll(a & ((1ULL << (k & 63)) - 1));
        return (f ? ret : k - ret);
    }

    int count(int L, int R, bool f) { return count(R, f) - count(L, f); }

    string to_string() {
        string ans;
        For(i, n) ans += '0' + (dat[i / 64].first >> (i % 64) & 1);
        return ans;
    }
};
} // namespace mitsuha
#endif // AJAY_BITVECTOR
