#ifndef AJAY_BITVECTOR
#define AJAY_BITVECTOR

namespace mitsuha{
struct Bit_Vector {
    vector<pair<unsigned int, unsigned int>> dat;
    Bit_Vector(int n) { dat.assign((n + 63) >> 5, {0, 0}); }

    void set(int i) { dat[i >> 5].first |= (unsigned int)(1) << (i & 31); }

    void build() {
        for(int i = 0; i < (int) dat.size() - 1; i ++) dat[i + 1].second = dat[i].second + __builtin_popcount(dat[i].first);
    }

    int rank(int k, bool f = 1) {
        auto [a, b] = dat[k >> 5];
        int ret = b + __builtin_popcount(a & (((unsigned int)(1) << (k & 31)) - 1));
        return (f ? ret : k - ret);
    }
};
} // namespace mitsuha
#endif // AJAY_BITVECTOR
