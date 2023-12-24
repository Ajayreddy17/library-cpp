#ifndef AJAY_VECTOR_SPACE
#define AJAY_VECTOR_SPACE

#include "library/util/bit_utils.hpp"
#include "library/linear_algebra/xor/transpose.hpp"

namespace mitsuha{
template <typename UINT>
struct Vector_Space {
#define SP Vector_Space
    vector<UINT> dat;

    Vector_Space() {}
    Vector_Space(vector<UINT> dat, bool is_reduced = false) : dat(dat) {
        if (!is_reduced) reduce();
    }

    int size() { return dat.size(); }

    bool add_element(UINT v) {
        for (auto&& e: dat) {
            if (e == 0 || v == 0) break;
            chmin(v, v ^ e);
        }
        if (v) {
            dat.emplace_back(v);
            return true;
        }
        return false;
    }

    bool contain(UINT v) {
        for (auto&& w: dat) {
            if (v == 0) break;
            chmin(v, v ^ w);
        }
        return v == 0;
    }

    UINT get_max(UINT xor_val = 0) {
        UINT res = xor_val;
        for (auto&& x: dat) chmax(res, res ^ x);
        return res;
    }

    UINT get_min(UINT xor_val) {
        UINT res = xor_val;
        for (auto&& x: dat) chmin(res, res ^ x);
        return res;
    }

    static SP merge(SP x, SP y) {
        if (len(x) < len(y)) swap(x, y);
        for (auto v: y.dat) { x.add_element(v); }
        return x;
    }

    static SP intersection(SP& x, SP& y, int max_dim) {
        SP xx = x.orthogonal_space(max_dim);
        SP yy = y.orthogonal_space(max_dim);
        xx = merge(xx, yy);
        return xx.orthogonal_space(max_dim);
    }

    SP orthogonal_space(int max_dim) {
        normalize();
        int m = max_dim;
        vector<unsigned long long> tmp(m);
        for(int i = 0; i < len(dat); ++i) tmp[msb(dat[i])] = dat[i];
        tmp = transpose(m, m, tmp, 0);
        SP res;
        for(int j = 0; j < m; ++j) {
            if (tmp[j] >> j & 1) continue;
            res.add_element(tmp[j] | UINT(1) << j);
        }
        return res;
    }

    void normalize(bool dec = true) {
        int n = len(dat);
        for(int j = 0; j < n; ++j)
            for(int i = 0; i < j; ++i) chmin(dat[i], dat[i] ^ dat[j]);
        sort(dat.begin(), dat.end());
        if (dec) reverse(dat.begin(), dat.end());
    }

private:
    void reduce() {
        SP y;
        for (auto&& e: dat) y.add_element(e);
        (*this) = y;
    }
#undef SP
};
} // namespace mitsuha
#endif // AJAY_VECTOR_SPACE