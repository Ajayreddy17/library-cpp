#ifndef AJAY_BITVECTOR
#define AJAY_BITVECTOR

namespace mitsuha{
struct Bit_Vector {
    int n;
    bool prepared = 0;
    vector<pair<unsigned long long, unsigned int>> dat;

    Bit_Vector(int n = 0) : n(n) { dat.assign((n + 127) >> 6, {0, 0}); }

    void set(int i) {
        assert(!prepared && 0 <= i && i < n);
        dat[i >> 6].first |= 1ULL << (i & 63);
    }

    void reset() {
        fill(dat.begin(), dat.end(), pair<unsigned long long, unsigned int>{0, 0});
        prepared = 0;
    }

    void build() {
        prepared = 1;
        For(i, len(dat) - 1) dat[i + 1].second = dat[i].second + __builtin_popcountll(dat[i].first);
    }

    bool operator[](int i) const { return dat[i >> 6].first >> (i & 63) & 1; }

    // number of f in [0, k)
    int count_prefix(int k, bool f = true) const {
        assert(prepared);
        auto [a, b] = dat[k >> 6];
        int ret = b + __builtin_popcountll(a & ((1ULL << (k & 63)) - 1));
        return (f ? ret : k - ret);
    }

    // number of f in [L, R)
    int count(int L, int R, bool f = true) const { return count_prefix(R, f) - count_prefix(L, f); }

    string to_string() const {
        string ans;
        For(i, n) ans += '0' + (dat[i / 64].first >> (i % 64) & 1);
        return ans;
    }
};
} // namespace mitsuha
#endif // AJAY_BITVECTOR
