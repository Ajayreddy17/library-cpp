#ifndef AJAY_SPLIT_SLIDE_SUM
#define AJAY_SPLIT_SLIDE_SUM

#include "library/datastructure/removable_queue.hpp"

namespace mitsuha{
/*
・Handling multiple sets
・You can get the sum of [0,k)th and [k,N)th]
・O(sum of changes in k x log N)
*/
template <typename T>
struct Slide_Split_Sum {
    Removable_Queue<priority_queue<T>> ql;
    Removable_Queue<priority_queue<T, vector<T>, greater<T>>> qr;
    T sl, sr;
    Slide_Split_Sum() : sl(0), sr(0) {}

    inline int size() { return len(ql) + len(qr); }
    void insert(T x) { (x <= lmax() ? push_l(x) : push_r(x)); }
    void erase(T x) { (x <= lmax() ? erase_l(x) : erase_r(x)); }
    pair<T, T> query(int k) {
        assert(0 <= k && k <= size());
        while (len(ql) < k) { push_l(pop_r()); }
        while (len(ql) > k) { push_r(pop_l()); }
        return {sl, sr};
    }
    T query_l(int k) { return query(k).fi; }
    T query_r(int k) { return query(size() - k).se; }

private:
    inline T lmax() { return (ql.empty() ? numeric_limits<T>::min() / 2 : ql.top()); }
    inline T rmin() { return (qr.empty() ? numeric_limits<T>::max() / 2 : qr.top()); }
    inline T pop_l() {
        T x = ql.pop();
        sl -= x;
        return x;
    }
    inline T pop_r() {
        T x = qr.pop();
        sr -= x;
        return x;
    }
    inline void push_l(T x) { ql.push(x), sl += x; }
    inline void push_r(T x) { qr.push(x), sr += x; }
    inline void erase_l(T x) { ql.remove(x), sl -= x; }
    inline void erase_r(T x) { qr.remove(x), sr -= x; }
};
} // namespace mitsuha
#endif // AJAY_SPLIT_SLIDE_SUM