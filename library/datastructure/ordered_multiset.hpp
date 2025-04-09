#ifndef AJAY_ORDERED_MULTISET
#define AJAY_ORDERED_MULTISET

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
using namespace __gnu_pbds;

namespace mitsuha{
template <typename T>
struct Ordered_Multiset {
    using Entry = std::pair<T, int>;
    using Tree = tree<
        Entry,
        __gnu_pbds::null_type,
        std::less<Entry>,
        __gnu_pbds::rb_tree_tag,
        __gnu_pbds::tree_order_statistics_node_update>;

    Tree data;
    int uid = 0;

    void insert(const T& value) {
        data.insert({value, uid++});
    }

    bool erase(const T& value) {
        auto it = data.lower_bound({value, 0});
        if (it != data.end() && it->first == value) {
            data.erase(it);
            return true;
        }
        return false;
    }

    int size() const { return data.size(); }
    void clear() { data.clear(); uid = 0; }

    int count_leq(const T& key) const { return data.order_of_key({key, uid + 1}); }
    int count_lt(const T& key) const { return data.order_of_key({key, 0}); }
    int count_geq(const T& key) const { return size() - count_lt(key); }
    int count_gt(const T& key) const { return size() - count_leq(key); }

    optional<T> min_geq(const T& key) const {
        auto it = data.lower_bound({key, 0});
        return it == data.end() ? nullopt : make_optional(it->first);
    }
    optional<T> min_gt(const T& key) const {
        auto it = data.upper_bound({key, uid});
        return it == data.end() ? nullopt : make_optional(it->first);
    }
    optional<T> max_leq(const T& key) const {
        auto it = data.upper_bound({key, uid});
        return it == data.begin() ? nullopt : make_optional((--it)->first);
    }
    optional<T> max_lt(const T& key) const {
        auto it = data.lower_bound({key, 0});
        return it == data.begin() ? nullopt : make_optional((--it)->first);
    }
};
} // namespace mitsuha
#endif // AJAY_ORDERED_MULTISET