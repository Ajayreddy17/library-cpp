#ifndef AJAY_HASHMAP
#define AJAY_HASHMAP

namespace mitsuha{
// unsigned long long -> Val
template <typename Val>
struct HashMap {
    HashMap(unsigned int n = 0) { build(n); }
    void build(unsigned int n) {
        unsigned int k = 8;
        while (k * 0.8 < n) k *= 2;
        cap = k * 0.8, mask = k - 1;
        key.resize(k), val.resize(k), used.assign(k, 0);
    }
    void clear() { build(0); }
    int size() { return len(used) - cap; }

    int index(const unsigned long long& k) {
        int i = 0;
        for (i = hash(k); used[i] && key[i] != k; i = (i + 1) & mask) {}
        return i;
    }

    Val& operator[](const unsigned long long& k) {
        if (cap == 0) extend();
        int i = index(k);
        if (!used[i]) { used[i] = 1, key[i] = k, val[i] = Val{}, --cap; }
        return val[i];
    }

    Val get(const unsigned long long& k, Val default_value) {
        int i = index(k);
        return (used[i] ? val[i] : default_value);
    }

    bool count(const unsigned long long& k) {
        int i = index(k);
        return used[i] && key[i] == k;
    }

    // f(key, val)
    template <typename F>
    void enumerate_all(F f) {
        For(i, len(used)) if (used[i]) f(key[i], val[i]);
    }

private:
    unsigned int cap, mask;
    vector<unsigned long long> key;
    vector<Val> val;
    vector<bool> used;

    unsigned long long hash(unsigned long long x) {
        static const unsigned long long FIXED_RANDOM
                = std::chrono::steady_clock::now().time_since_epoch().count();
        x += FIXED_RANDOM;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return (x ^ (x >> 31)) & mask;
    }

    void extend() {
        vector<pair<unsigned long long, Val>> dat;
        dat.reserve(len(used) - cap);
        For(i, len(used)) {
            if (used[i]) dat.emplace_back(key[i], val[i]);
        }
        build(2 * len(dat));
        for (auto& [a, b]: dat) (*this)[a] = b;
    }
};
} // namespace mitsuha
#endif // AJAY_HASHMAP