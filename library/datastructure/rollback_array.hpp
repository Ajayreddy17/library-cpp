#ifndef AJAY_ROLLBACK_ARRAY
#define AJAY_ROLLBACK_ARRAY

namespace mitsuha{
template <typename T>
struct Rollback_Array {
    int N;
    vector<T> dat;
    vector<pair<int, T>> history;
 
    Rollback_Array() {}
    Rollback_Array(vector<T> x) : N(len(x)), dat(x) {}
    Rollback_Array(int N) : N(N), dat(N) {}
    template <typename F>
    Rollback_Array(int N, F f) : N(N) {
        dat.reserve(N);
        for(int i = 0; i < N; i++) dat.emplace_back(f(i));
    }
 
    int time() { return len(history); }
    void rollback(int t) {
        for(int i = time() - 1; i >= t; --i) {
            auto& [idx, v] = history[i];
            dat[idx] = v;
        }
        history.resize(t);
    }
    T get(int idx) { return dat[idx]; }
    void set(int idx, T x) {
        history.emplace_back(idx, dat[idx]);
        dat[idx] = x;
    }
 
    vector<T> get_all() {
        vector<T> res(N);
        for(int i = 0; i < N; i++) res[i] = get(i);
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_ROLLBACK_ARRAY