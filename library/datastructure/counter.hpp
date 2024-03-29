#ifndef AJAY_COUNTER
#define AJAY_COUNTER

namespace mitsuha{
// n: # of items
// m: maximum possible freq
// order: list of items sorted by freq (tie broken arbitrarily)
// pos[x]: position of the item x in the list
// freq[x]: frequency of the item x
// fpos[f]: starting position of items for freqeuncy f
struct Counter{
    int n, m;
    vector<int> order, pos, freq, fpos;
    // O(n + m)
    Counter(int n, int m): n(n), m(m), order(n), pos(n), freq(n), fpos(m + 2, n){
        fpos[0] = 0;
        iota(order.begin(), order.end(), 0);
        iota(pos.begin(), pos.end(), 0);
    }
    // O(n + m)
    Counter(int n, int m, const vector<int> &freq): n(n), m(m), pos(n), freq(freq), fpos(m + 2, n){
        assert((int)freq.size() == n && *max_element(freq.begin(), freq.end()) <= m);
        iota(order.begin(), order.end(), 0);
        vector<vector<int>> occur(m + 1);
        for(auto i = 0; i < n; ++ i) occur[freq[i]].push_back(i);
        for(auto f = 0; f <= m; ++ f){
            fpos[f] = (int)order.size();
            order.insert(order.end(), occur[f].begin(), occur[f].end());
            for(auto t = fpos[f]; t < (int)order.size(); ++ t) pos[order[t]] = t;
        }
    }
    // O(1)
    void insert(int x){
        assert(freq[x] + 1 <= m);
        -- fpos[++ freq[x]];
        int y = order[fpos[freq[x]]];
        swap(pos[x], pos[y]);
        swap(order[pos[x]], order[pos[y]]);
    }
    // O(1)
    void erase(int x){
        assert(freq[x]);
        int y = order[fpos[freq[x]]];
        swap(pos[x], pos[y]);
        swap(order[pos[x]], order[pos[y]]);
        ++ fpos[freq[x] --];
    }
    // O(1)
    int maximum_frequency() const{
        return freq[order.back()];
    }
};
} // namespace mitsuha
#endif // AJAY_COUNTER