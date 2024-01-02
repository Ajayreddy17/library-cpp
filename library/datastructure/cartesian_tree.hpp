#ifndef AJAY_CARTESIAN_TREE
#define AJAY_CARTESIAN_TREE

namespace mitsuha{
/*
    ##Examples
    data - 1 2 3 4 5 4 3 2 1 6
    Min Cartesian Tree - [0 10] [1 8] [2 7] [3 6] [4 5] [4 6] [3 7] [2 8] [1 10] [9 10]
    Max Cartesian Tree - [0 1] [0 2] [0 3] [0 4] [0 9] [5 9] [6 9] [7 9] [8 9] [0 10]
*/
template <typename T, bool IS_MIN>
struct CartesianTree {
    int n;
    vector<T>& A;
    vector<pair<int, int>> range;
    vector<int> lch, rch, par;
    int root;

    CartesianTree(vector<T>& A) : n(A.size()), A(A) {
        range.assign(n, {-1, -1});
        lch.assign(n, -1);
        rch.assign(n, -1);
        par.assign(n, -1);
        if (n == 1) {
            range[0] = {0, 1};
            root = 0;
            return;
        }
        auto is_sm = [&](int i, int j) -> bool {
            if (IS_MIN) return (A[i] < A[j]) || (A[i] == A[j] && i < j);
            return (A[i] > A[j]) || (A[i] == A[j] && i < j);
        };
        vector<int> st;
        for(int i = 0; i < n; ++i) {
            while (!st.empty() && is_sm(i, st.back())) {
                lch[i] = st.back();
                st.pop_back();
            }
            range[i].first = (st.empty() ? 0 : st.back() + 1);
            st.emplace_back(i);
        }
        st.clear();
        for(int i = n - 1; i >= 0; --i) {
            while (!st.empty() && is_sm(i, st.back())) {
                rch[i] = st.back();
                st.pop_back();
            }
            range[i].second = (st.empty() ? n : st.back());
            st.emplace_back(i);
        }
        for(int i = 0; i < n; ++i) if (lch[i] != -1) par[lch[i]] = i;
        for(int i = 0; i < n; ++i) if (rch[i] != -1) par[rch[i]] = i;
        for(int i = 0; i < n; ++i) if (par[i] == -1) root = i;
    }

    // (l, r, h)
    tuple<int, int, T> maximum_rectangle(int i) {
        auto [l, r] = range[i];
        return {l, r, A[i]};
    }

    // (l, r, h)
    T max_rectangle_area() {
        assert(IS_MIN);
        T res = 0;
        for(int i = 0; i < n; ++i) {
            auto [l, r, h] = maximum_rectangle(i);
            chmax(res, (r - l) * h);
        }
        return res;
    }

    long long count_subrectangle(bool baseline) {
        assert(IS_MIN);
        long long res = 0;
        for(int i = 0; i < n; ++i) {
            auto [l, r, h] = maximum_rectangle(i);
            long long x = (baseline ? h : h * (h + 1) / 2);
            res += x * (i - l + 1) * (r - i);
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_CARTESIAN_TREE