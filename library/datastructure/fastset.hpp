#ifndef AJAY_FASTSET
#define AJAY_FASTSET

namespace mitsuha{
// 64-ary tree
// space: (N/63) * u64
struct FastSet {
    static constexpr unsigned int B = 64;
    int n, log;
    vector<vector<unsigned long long>> seg;

    FastSet() {}
    FastSet(int n) { build(n); }
    template <typename F> FastSet(int n, F f) { build(n, f); }

    void build(int m) {
        seg.clear();
        n = m;
        do {
            seg.push_back(vector<unsigned long long>((m + B - 1) / B));
            m = (m + B - 1) / B;
        } while (m > 1);
        log = (int) seg.size();
    }
    template <typename F>
    void build(int n, F f) {
        build(n);
        for(int i = 0; i < n; ++i) { seg[0][i / B] |= (unsigned long long)(f(i)) << (i % B); }
        for(int h = 0; h < log - 1; ++h) {
            for(int i = 0; i < (int) seg[h].size(); ++i) {
                seg[h + 1][i / B] |= (unsigned long long)(bool(seg[h][i])) << (i % B);
            }
        }
    }

    bool operator[](int i) const { return seg[0][i / B] >> (i % B) & 1; }
    void insert(int i) {
        for (int h = 0; h < log; h++) {
            seg[h][i / B] |= 1ULL << (i % B), i /= B;
        }
    }
    void add(int i) { insert(i); }
    void erase(int i) {
        unsigned long long x = 0;
        for (int h = 0; h < log; h++) {
            seg[h][i / B] &= ~(1ULL << (i % B));
            seg[h][i / B] |= x << (i % B);
            x = bool(seg[h][i / B]);
            i /= B;
        }
    }
    void remove(int i) { erase(i); }

    // min[x,n) or n
    int next(int i) {
        assert(i <= n);
        i = max(0, i);
        for (int h = 0; h < log; h++) {
            if (i / B == seg[h].size()) break;
            unsigned long long d = seg[h][i / B] >> (i % B);
            if (not d) {
                i = i / B + 1;
                continue;
            }
            i += lowbit(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += lowbit(seg[g][i / B]);
            }
            return i;
        }
        return n;
    }

    // max [0,x], or -1
    int prev(int i) {
        assert(i >= -1);
        if (i >= n) i = n - 1;
        for (int h = 0; h < log; h++) {
            if (i == -1) break;
            unsigned long long d = seg[h][i / B] << (63 - i % B);
            if (not d) {
                i = i / B - 1;
                continue;
            }
            i -= __builtin_clzll(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += topbit(seg[g][i / B]);
            }
            return i;
        }
        return -1;
    }

    // [l, r)
    template <typename F>
    void enumerate(int l, int r, F f) {
        for (int x = next(l); x < r; x = next(x + 1)) f(x);
    }
private:
    int lowbit(unsigned long long x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
    int topbit(unsigned long long x) { return (x == 0 ? -1 : 63 - __builtin_clzll(x)); }
};

std::ostream &operator<<(std::ostream &out, const FastSet &fs){
    for (int i = 0; i < fs.n - 1; ++i){
        out << fs[i] << " ";
    }
    return out;
}
} // namespace mitsuha
#endif // AJAY_FASTSET