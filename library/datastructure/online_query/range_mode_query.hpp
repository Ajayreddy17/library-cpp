#ifndef AJAY_RANGE_MODE_QUERY
#define AJAY_RANGE_MODE_QUERY

namespace mitsuha{
template<int B, class T>
struct Range_Mode_Query{
    int n;
    vector<T> cmpr;
    vector<int> a, pos;
    vector<vector<int>> indices, block_mode, block_mode_freq;
    // O(n * B) time
    // O(n + (n/B)^2) space
    Range_Mode_Query(const vector<T> &_a): n((int)_a.size()), cmpr(_a), a(n), pos(n){
        sort(cmpr.begin(), cmpr.end());
        cmpr.erase(unique(cmpr.begin(), cmpr.end()), cmpr.end());
        indices.resize((int)cmpr.size());
        for(auto i = 0; i < n; ++ i){
            a[i] = lower_bound(cmpr.begin(), cmpr.end(), _a[i]) - cmpr.begin();
            pos[i] = (int)indices[a[i]].size();
            indices[a[i]].push_back(i);
        }
        int C = (n + B - 1) / B;
        block_mode.resize(C + 1), block_mode_freq.resize(C + 1);
        vector<int> cnt((int)cmpr.size());
        for(auto l = 0; l <= C; ++ l){
            fill(cnt.begin(), cnt.end(), 0);
            block_mode[l].resize(C - l + 1);
            block_mode_freq[l].resize(C - l + 1);
            array<int, 2> mx{-1, -1};
            for(auto len = 1; len <= C - l; ++ len){
                for(auto i = (l + len - 1) * B; i < min((l + len) * B, n); ++ i) mx = max(mx, array{++ cnt[a[i]], a[i]});
                block_mode[l][len] = mx[1], block_mode_freq[l][len] = mx[0];
            }
        }
    }
    // Returns a pair (mode, frequency)
    // O(B)
    pair<T, int> query(int ql, int qr) const{
        assert(0 <= ql && ql <= qr && qr <= n);
        if(ql == qr) return {{}, 0};
        const int bl = ql / B, br = qr / B;
        int mode = -1, freq = 0;
        if(bl == br){
            for(auto i = ql; i < qr; ++ i){
                int x = a[i];
                if(pos[i] && ql <= indices[x][pos[i] - 1]) continue;
                if(freq && ((int)indices[x].size() <= pos[i] + freq - 1 || qr <= indices[x][pos[i] + freq - 1])) continue;
                mode = x;
                while(pos[i] + freq < (int)indices[x].size() && indices[x][pos[i] + freq] < qr) ++ freq;
            }
        }
        else{
            mode = block_mode[bl + 1][br - bl - 1], freq = block_mode_freq[bl + 1][br - bl - 1];
            for(auto i = ql; i < (bl + 1) * B; ++ i){
                int x = a[i];
                if(pos[i] && ql <= indices[x][pos[i] - 1]) continue;
                if(freq && ((int)indices[x].size() <= pos[i] + freq - 1 || qr <= indices[x][pos[i] + freq - 1])) continue;
                mode = x;
                while(pos[i] + freq < (int)indices[x].size() && indices[x][pos[i] + freq] < qr) ++ freq;
            }
            for(auto i = qr - 1; i >= br * B; -- i){
                int x = a[i];
                if(pos[i] + 1 < (int)indices[x].size() && indices[x][pos[i] + 1] < qr) continue;
                if(freq && (pos[i] - freq + 1 < 0 || indices[x][pos[i] - freq + 1] < ql)) continue;
                mode = x;
                while(0 <= pos[i] - freq && ql <= indices[x][pos[i] - freq]) ++ freq;
            }
        }
        return {cmpr[mode], freq};
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_MODE_QUERY