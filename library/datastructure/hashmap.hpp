#ifndef AJAY_HASHMAP
#define AJAY_HASHMAP

namespace mitsuha{
// unsigned long long -> Val
template <typename Val, int LOG = 20, bool KEEP_IDS = false>
struct HashMap {
    static constexpr int N = (1 << LOG);
    unsigned long long* key;
    Val* val;
    vector<int> IDS;
    bitset<N> used;
    const int shift;
    const unsigned long long r = 11995408973635179863ULL;
    HashMap() : key(new unsigned long long [N]), val(new Val[N]), shift(64 - LOG) {}
    unsigned int hash(unsigned long long x) {
        static const unsigned long long FIXED_RANDOM
                = std::chrono::steady_clock::now().time_since_epoch().count();
        return ((unsigned long long)(x + FIXED_RANDOM) * r) >> shift;
    }

    int index(const unsigned long long& k) {
        int i = 0;
        for (i = hash(k); used[i] && key[i] != k; (i += 1) &= (N - 1)) {}
        return i;
    }

    Val& operator[](const unsigned long long& k) {
        int i = index(k);
        if (!used[i]) {
            used[i] = 1, key[i] = k, val[i] = Val{};
            if constexpr (KEEP_IDS) IDS.emplace_back(i);
        }
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

    void reset() {
        static_assert(KEEP_IDS);
        for (auto&& i: IDS) used[i] = 0;
        IDS.clear();
    }

    // f(key, val)
    template <typename F>
    void enumerate_all(F f) {
        static_assert(KEEP_IDS);
        for (auto&& i: IDS) f(key[i], val[i]);
    }
};
} // namespace mitsuha
#endif // AJAY_HASHMAP