#ifndef AJAY_COUNT_LABELED_FOREST
#define AJAY_COUNT_LABELED_FOREST

#include "library/poly/fps_exp.hpp"
#include "library/graph/count/count_labeled_tree.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_forest.hpp

// https://oeis.org/A001858
template <typename mint>
vector<mint> count_labeled_forest(int N) {
    vector<mint> f = count_labeled_tree<mint>(N);
    For(i, len(f)) f[i] *= fact_inv<mint>(i);
    f = fps_exp(f);
    For(i, len(f)) f[i] *= fact<mint>(i);
    return f;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_FOREST
