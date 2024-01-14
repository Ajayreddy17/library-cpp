#ifndef AJAY_PBDS
#define AJAY_PBDS

namespace mitsuha{
#include "ext/pb_ds/assoc_container.hpp"
#include "ext/pb_ds/tree_policy.hpp"
 
namespace pbds {
    using namespace __gnu_pbds;
#ifdef PB_DS_ASSOC_CNTNR_HPP
    template <class Key, class Value, class Hash>
    using unordered_map = gp_hash_table<Key, Value, Hash, std::equal_to<Key>, direct_mask_range_hashing<>, linear_probe_fn<>,
                                        hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>>;
    template <class Key, class Hash>
    using unordered_set = pbds::unordered_map<Key, null_type, Hash>;
#endif
#ifdef PB_DS_TREE_POLICY_HPP
    template <typename T>
    using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename T>
    using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
    template <class Key, class Value, class Compare = std::less<Key>>
    using ordered_map = tree<Key, Value, Compare, rb_tree_tag, tree_order_statistics_node_update>;
#endif
} // namespace pbds
} // namespace mitsuha
#endif // AJAY_PBDS