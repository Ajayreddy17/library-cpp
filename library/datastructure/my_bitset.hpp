#ifndef AJAY_MY_BITSET
#define AJAY_MY_BITSET

namespace mitsuha{
// https://codeforces.com/contest/914/problem/F
// https://yukicoder.me/problems/no/142
// It seems that it is sometimes slightly slower than normal bitset,
// Use when you don't want a fixed length or when you need a slice operation
struct My_Bitset {
    using T = My_Bitset;
    int N;
    vector<unsigned long long> dat;

    // fill with x
    My_Bitset(int N = 0, int x = 0) : N(N) {
        assert(x == 0 || x == 1);
        unsigned long long v = (x == 0 ? 0 : -1);
        dat.assign((N + 63) >> 6, v);
        if (N) dat.back() >>= (64 * len(dat) - N);
    }

    int size() { return N; }

    void resize(int size) {
        dat.resize((size + 63) >> 6);
        int remainingBits = size & 63;
        if (remainingBits != 0) {
            unsigned long long mask = (1ULL << remainingBits) - 1;
            dat.back() &= mask;
        }
        N = size;
    }

    // thanks to chatgpt!
    class Proxy {
    public:
        Proxy(vector<unsigned long long> &d, int i) : dat(d), index(i) {}
        operator bool() const { return (dat[index >> 6] >> (index & 63)) & 1; }

        Proxy &operator=(unsigned long long value) {
            dat[index >> 6] &= ~(1ULL << (index & 63));
            dat[index >> 6] |= (value & 1) << (index & 63);
            return *this;
        }
        void flip() {
            dat[index >> 6] ^= (1ULL << (index & 63)); // XOR to flip the bit
        }

    private:
        vector<unsigned long long> &dat;
        int index;
    };

    Proxy operator[](int i) { return Proxy(dat, i); }

    bool operator==(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) if (dat[i] != p.dat[i]) return false;
        return true;
    }

    T &operator&=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] &= p.dat[i];
        return *this;
    }
    T &operator|=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] |= p.dat[i];
        return *this;
    }
    T &operator^=(const T &p) {
        assert(N == p.N);
        For(i, len(dat)) dat[i] ^= p.dat[i];
        return *this;
    }
    T operator&(const T &p) const { return T(*this) &= p; }
    T operator|(const T &p) const { return T(*this) |= p; }
    T operator^(const T &p) const { return T(*this) ^= p; }
    T operator~() const {
        T p = (*this);
        p.flip_range(0, N);
        return p;
    }

    int count() {
        int ans = 0;
        for (unsigned long long val: dat) ans += __builtin_popcountll(val);
        return ans;
    }

    int next(int i) {
        chmax(i, 0);
        if (i >= N) return N;
        int k = i >> 6;
        {
            unsigned long long x = dat[k];
            int s = i & 63;
            x = (x >> s) << s;
            if (x) return (k << 6) | (x == 0 ? -1 : __builtin_ctzll(x));
        }
        For(idx, k + 1, len(dat)) {
            if (dat[idx] == 0) continue;
            return (idx << 6) | (dat[idx] == 0 ? -1 : __builtin_ctzll(dat[idx]));
        }
        return N;
    }

    int prev(int i) {
        chmin(i, N - 1);
        if (i <= -1) return -1;
        int k = i >> 6;
        if ((i & 63) < 63) {
            unsigned long long x = dat[k];
            x &= (1ULL << ((i & 63) + 1)) - 1;
            if (x) return (k << 6) | (x == 0 ? -1 : 63 - __builtin_clzll(x));
            --k;
        }
        Frr(idx, k + 1) {
            if (dat[idx] == 0) continue;
            return (idx << 6) | (dat[idx] == 0 ? -1 : 63 - __builtin_clzll(dat[idx]));
        }
        return -1;
    }

    My_Bitset range(int L, int R) {
        assert(L <= R);
        My_Bitset p(R - L);
        int rm = (R - L) & 63;
        Loop(rm) {
            p[R - L - 1] = bool((*this)[R - 1]);
            --R;
        }
        int n = (R - L) >> 6;
        int hi = L & 63;
        int lo = 64 - hi;
        int s = L >> 6;
        if (hi == 0) {
            For(i, n) { p.dat[i] ^= dat[s + i]; }
        } else {
            For(i, n) { p.dat[i] ^= (dat[s + i] >> hi) ^ (dat[s + i + 1] << lo); }
        }
        return p;
    }

    int count_range(int L, int R) {
        assert(L <= R);
        int cnt = 0;
        while ((L < R) && (L & 63)) cnt += (*this)[L++];
        while ((L < R) && (R & 63)) cnt += (*this)[--R];
        int l = L >> 6, r = R >> 6;
        For(i, l, r) cnt += __builtin_popcountll(dat[i]);
        return cnt;
    }

    // Assign p to [L,R)
    void assign_to_range(int L, int R, My_Bitset &p) {
        assert(p.N == R - L);
        int a = 0, b = p.N;
        while (L < R && (L & 63)) { (*this)[L++] = bool(p[a++]); }
        while (L < R && (R & 63)) { (*this)[--R] = bool(p[--b]); }
        // p[a:b] to [L:R]
        int l = L >> 6, r = R >> 6;
        int s = a >> 6, t = b >> t;
        int n = r - l;
        if (!(a & 63)) {
            For(i, n) dat[l + i] = p.dat[s + i];
        } else {
            int hi = a & 63;
            int lo = 64 - hi;
            For(i, n) dat[l + i] = (p.dat[s + i] >> hi) | (p.dat[1 + s + i] << lo);
        }
    }

    // xor p to [L,R)
    void xor_to_range(int L, int R, My_Bitset &p) {
        assert(p.N == R - L);
        int a = 0, b = p.N;
        while (L < R && (L & 63)) {
            dat[L >> 6] ^= (unsigned long long)(p[a]) << (L & 63);
            ++a, ++L;
        }
        while (L < R && (R & 63)) {
            --b, --R;
            dat[R >> 6] ^= (unsigned long long)(p[b]) << (R & 63);
        }
        // p[a:b] to [L:R]
        int l = L >> 6, r = R >> 6;
        int s = a >> 6, t = b >> t;
        int n = r - l;
        if (!(a & 63)) {
            For(i, n) dat[l + i] ^= p.dat[s + i];
        } else {
            int hi = a & 63;
            int lo = 64 - hi;
            For(i, n) dat[l + i] ^= (p.dat[s + i] >> hi) | (p.dat[1 + s + i] << lo);
        }
    }

    // and p to [L,R)
    void and_to_range(int L, int R, My_Bitset &p) {
        assert(p.N == R - L);
        int a = 0, b = p.N;
        while (L < R && (L & 63)) {
            if (!p[a++]) (*this)[L++] = 0;
        }
        while (L < R && (R & 63)) {
            if (!p[--b]) (*this)[--R] = 0;
        }
        // p[a:b] to [L:R]
        int l = L >> 6, r = R >> 6;
        int s = a >> 6, t = b >> t;
        int n = r - l;
        if (!(a & 63)) {
            For(i, n) dat[l + i] &= p.dat[s + i];
        } else {
            int hi = a & 63;
            int lo = 64 - hi;
            For(i, n) dat[l + i] &= (p.dat[s + i] >> hi) | (p.dat[1 + s + i] << lo);
        }
    }

    // [L,R) に p を or
    void or_to_range(int L, int R, My_Bitset &p) {
        assert(p.N == R - L);
        int a = 0, b = p.N;
        while (L < R && (L & 63)) {
            dat[L >> 6] |= (unsigned long long)(p[a]) << (L & 63);
            ++a, ++L;
        }
        while (L < R && (R & 63)) {
            --b, --R;
            dat[R >> 6] |= (unsigned long long)(p[b]) << (R & 63);
        }
        // p[a:b] to [L:R]
        int l = L >> 6, r = R >> 6;
        int s = a >> 6, t = b >> t;
        int n = r - l;
        if (!(a & 63)) {
            For(i, n) dat[l + i] |= p.dat[s + i];
        } else {
            int hi = a & 63;
            int lo = 64 - hi;
            For(i, n) dat[l + i] |= (p.dat[s + i] >> hi) | (p.dat[1 + s + i] << lo);
        }
    }

    // Change [L,R) to 1
    void set_range(int L, int R) {
        while (L < R && (L & 63)) { set(L++); }
        while (L < R && (R & 63)) { set(--R); }
        For(i, L >> 6, R >> 6) dat[i] = (unsigned long long)(-1);
    }

    // Change [L,R) to 0
    void reset_range(int L, int R) {
        while (L < R && (L & 63)) { reset(L++); }
        while (L < R && (R & 63)) { reset(--R); }
        For(i, L >> 6, R >> 6) dat[i] = (unsigned long long)(0);
    }

    // flip [L,R)
    void flip_range(int L, int R) {
        while (L < R && (L & 63)) { flip(L++); }
        while (L < R && (R & 63)) { flip(--R); }
        For(i, L >> 6, R >> 6) dat[i] ^= (unsigned long long)(-1);
    }

    string to_string() const {
        string S;
        For(i, N) S += '0' + (dat[i >> 6] >> (i & 63) & 1);
        return S;
    }

    // Match specifications to bitset
    void set(int i) { (*this)[i] = 1; }
    void reset(int i) { (*this)[i] = 0; }
    void flip(int i) { (*this)[i].flip(); }
    void set() {
        fill(dat.begin(), dat.end(), (unsigned long long)(-1));
        resize(N);
    }
    void reset() { fill(dat.begin(), dat.end(), 0); }
    void flip() {
        For(i, len(dat) - 1) { dat[i] = (unsigned long long)(-1) ^ dat[i]; }
        int i = len(dat) - 1;
        For(k, 64) {
            if (64 * i + k >= size()) break;
            flip(64 * i + k);
        }
    }
    bool any() {
        For(i, len(dat)) {
            if (dat[i]) return true;
        }
        return false;
    }

    int _Find_first() { return next(0); }
    int _Find_next(int p) { return next(p + 1); }
};
} // namespace mitsuha
#endif // AJAY_MY_BITSET