#ifndef AJAY_DUMMY_DATA_STRUCTURE
#define AJAY_DUMMY_DATA_STRUCTURE

#include "library/monoid/monoid_dummy.hpp"

namespace mitsuha{
// placeholder SEGTREE for structures that only count (e.g. Wavelet_Matrix without weights)
struct Dummy_Data_Structure {
    using MX = Monoid_Dummy;
    using T = typename MX::value_type;
    void build(const vector<T>&) {}
};
} // namespace mitsuha
#endif // AJAY_DUMMY_DATA_STRUCTURE
