#ifndef AJAY_ADD_REMOVE_QUERY
#define AJAY_ADD_REMOVE_QUERY

namespace mitsuha{
/*
Add / Remove `x` at time `t`     
Convert them into "add `x` in the time interval [l, r)"  
add(t1, x1), add(t2, x2), add(t3, x2), .... is called , 
then t1 <= t2 <= t3 <= ... is guaranteed [Monotone]
To avoid conflicts assign an appropriate label.
*/
template <typename X, bool monotone>
struct Add_Remove_Query {
    map<X, int> MP;
    vector<tuple<int, int, X>> dat;
    map<X, vector<int>> ADD;
    map<X, vector<int>> RM;

    void add(int time, X x) {
        if (monotone) return add_monotone(time, x);
        ADD[x].emplace_back(time);
    }
    void remove(int time, X x) {
        if (monotone) return remove_monotone(time, x);
        RM[x].emplace_back(time);
    }

    // Pass the current time after all queries have finished
    // [l, r, X]
    vector<tuple<int, int, X>> calc(int time) {
        if (monotone) return calc_monotone(time);
        vector<tuple<int, int, X>> dat;
        for (auto&& [x, A] : ADD) {
            vector<int> B;
            if (RM.count(x)) {
                B = RM[x];
                RM.erase(x);
            }
            if (len(B) < len(A)) B.emplace_back(time);
            assert(len(A) == len(B));

            sort(A.begin(), A.end());
            sort(B.begin(), B.end());
            For(i, len(A)) {
                assert(A[i] <= B[i]);
                if (A[i] < B[i]) dat.emplace_back(A[i], B[i], x);
            }
        }
        assert(len(RM) == 0);
        return dat;
    }

  private:
    void add_monotone(int time, X x) {
        assert(!MP.count(x));
        MP[x] = time;
    }
    void remove_monotone(int time, X x) {
        auto it = MP.find(x);
        assert(it != MP.end());
        int t = (*it).second;
        MP.erase(it);
        if (t == time) return;
        dat.emplace_back(t, time, x);
    }
    vector<tuple<int, int, X>> calc_monotone(int time) {
        for (auto&& [x, t] : MP) {
            if (t == time) continue;
            dat.emplace_back(t, time, x);
        }
        return dat;
    }
};
} // namespace mitsuha
#endif // AJAY_ADD_REMOVE_QUERY