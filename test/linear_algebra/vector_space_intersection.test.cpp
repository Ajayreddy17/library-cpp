#define PROBLEM \
    "https://judge.yosupo.jp/problem/intersection_of_f2_vector_spaces"
#include <library/template.hpp>

#include <library/linear_algebra/xor/vector_space.hpp>

using SP = Vector_Space<uint32_t, 32>;

void solve() {
    auto get = [&]() -> SP {
        INT(n);
        SP V;
        For(n) {
            U32(a);
            V.add_element(a);
        }
        return V;
    };
    auto x = get(), y = get();
    x = SP::intersection(x, y);
    auto basis = x.to_vec();
    print(len(basis), basis);
}

signed main() {
    INT(T);
    For(T) solve();
}
