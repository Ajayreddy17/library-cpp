#ifndef AJAY_PARTITION
#define AJAY_PARTITION

namespace mitsuha{
/*
partition enumerates in lexicographical descending 
order as a "decreasing sequence".
N = 50（204226）：12 ms
N = 60（966467）：60 ms
N = 70（4087968）：270 ms
N = 80（15796476）：1100 ms
N = 90（56634173）：4800 ms
N = 100 (190569292) : 15600 ms
*/
template <typename F>
void enumerate_partition(int N, F query, int LIM_len = -1, int LIM_val = -1) {
    assert(N >= 0);
    auto dfs = [&](auto self, vector<int>& p, int sum) -> void {
        if (sum == N) {
            query(p);
            return;
        }
        if (LIM_len != -1 && len(p) == LIM_len) return;
        int nxt = (len(p) == 0 ? N : p.back());
        if (LIM_val != -1) chmin(nxt, LIM_val);
        chmin(nxt, N - sum);
        p.emplace_back(0);
        For(x, 1, nxt + 1) {
            p.back() = x;
            self(self, p, sum + x);
        }
        p.pop_back();
    };
    vector<int> p;
    dfs(dfs, p, 0);
}
} // namespace mitsuha
#endif // AJAY_PARTITION