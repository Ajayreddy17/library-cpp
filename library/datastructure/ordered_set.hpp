#ifndef AJAY_ORDERED_SET
#define AJAY_ORDERED_SET

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
using namespace __gnu_pbds;

namespace mitsuha{
template <typename T>
struct Ordered_Set {
    using Tree = tree<
        T,
        null_type,
        less<T>,
        rb_tree_tag,
        tree_order_statistics_node_update
    >;

    Tree data;

    void insert(const T& value) { data.insert(value); }

    bool erase(const T& value) {
        auto it = data.find(value);
        if (it == data.end()) return false;
        data.erase(it);
        return true;
    }

    bool contains(const T& value) const { return data.find(value) != data.end(); }

    int count_lt(const T& value) const { return data.order_of_key(value); }
    int count_leq(const T& value) const { return count_lt(value) + contains(value); }
    int count_gt(const T& value) const { return data.size() - count_leq(value); }
    int count_geq(const T& value) const { return data.size() - count_lt(value); }

    std::optional<T> min_geq(const T& value) const {
        auto it = data.lower_bound(value);
        return it == data.end() ? std::nullopt : std::make_optional(*it);
    }
    std::optional<T> min_gt(const T& value) const {
        auto it = data.upper_bound(value);
        return it == data.end() ? std::nullopt : std::make_optional(*it);
    }
    std::optional<T> max_leq(const T& value) const {
        auto it = data.upper_bound(value);
        return it == data.begin() ? std::nullopt : std::make_optional(*std::prev(it));
    }
    std::optional<T> max_lt(const T& value) const {
        auto it = data.lower_bound(value);
        return it == data.begin() ? std::nullopt : std::make_optional(*std::prev(it));
    }
};
} // namespace mitsuha
#endif // AJAY_ORDERED_SET