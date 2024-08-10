#ifndef AJAY_STATIC_RANGE_PRODUCT_GROUP
#define AJAY_STATIC_RANGE_PRODUCT_GROUP

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename Monoid>
struct Static_Range_Product_Group {
    using MX = Monoid;
    using X = typename MX::value_type;
    int n;
    vector<X> dat;
    Static_Range_Product_Group() {}
    template <typename F>
    Static_Range_Product_Group(int m, F f) {
        build(m, f);
    }
    template <typename F>
    void build(int m, F f) {
        n = m;
        dat.assign(n + 1, MX::unit());
        for (int i = 0; i < n; ++i) dat[i + 1] = MX::op(dat[i], f(i));
    }
    void build(vector<X>& A) {
        n = len(A);
        dat.assign(n + 1, MX::unit());
        for (int i = 0; i < n; ++i) dat[i + 1] = MX::op(dat[i], A[i]);
    }
    X prod(int l, int r) { return MX::op(MX::inverse(dat[l]), dat[r]); }
};


template <typename T>
using Prefix_Sum = Static_Range_Product_Group<Monoid_Add<T>>;
} // namespace mitsuha
#endif // AJAY_STATIC_RANGE_PRODUCT_GROUP