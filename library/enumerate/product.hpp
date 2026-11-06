#ifndef AJAY_ENUMERATE_PRODUCT
#define AJAY_ENUMERATE_PRODUCT

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/enumerate/product.hpp
template <typename F>
void enumerate_product(vector<int> A, F query) {
    int N = len(A);
    auto dfs = [&](auto& dfs, vector<int>& p) -> void {
        int n = len(p);
        if (n == N)
            return query(p);
        For(x, A[n]) {
            p.emplace_back(x);
            dfs(dfs, p);
            p.pop_back();
        }
    };
    vector<int> p;
    dfs(dfs, p);
}
} // namespace mitsuha
#endif // AJAY_ENUMERATE_PRODUCT