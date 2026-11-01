#define PROBLEM \
    "https://judge.yosupo.jp/problem/intersection_of_f2_vector_spaces"
#include <library/template.hpp>

#include <library/linear_algebra/xor/vector_space.hpp>

using SP = Vector_Space<uint32_t>;

void solve() {
    auto get = [&]() -> SP {
        INT(n);
        SP V;
        For(n) {
            unsigned int a;
        read(a);
            V.add_element(a);
        }
        return V;
    };
    auto x = get(), y = get();
    x = SP::intersection(x, y, 32);
    auto basis = x.dat;
    print(len(basis), basis);
}

signed main() {
    INT(T);
    For(T) solve();
}
