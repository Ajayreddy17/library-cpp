#ifndef AJAY_RANGE_FOLDABLE_MAP
#define AJAY_RANGE_FOLDABLE_MAP

#include "library/util/update_proxy_object.hpp"
#include "library/datastructure/splay_tree/splay_tree_map.hpp"

namespace mitsuha {
namespace internal::range_foldable_map {
template<typename Key, typename Monoid, typename Derived, typename Val = Monoid::value_type>
struct RangeFoldableMapNodeBase : public internal::splay_tree_map::MapNodeBase<Key, Val, Derived> {
        using Base = internal::splay_tree_map::MapNodeBase<Key, Val, Derived>;
        using node_ptr_t = typename Base::node_ptr_t;
        Val dat;
        RangeFoldableMapNodeBase() : RangeFoldableMapNodeBase(Key{}, Monoid::unit()) {}
        RangeFoldableMapNodeBase(const Key &key, const Val &val) : Base::MapNodeBase(key, val), dat(val) {}
        void update() {
            Base::update();
            dat = Monoid::op(Monoid::op(prod_all(this->ch[0]), this->val), prod_all(this->ch[1]));
        }
        static Val prod_all(node_ptr_t node) {
            return node == nullptr ? Monoid::unit() : node->dat;
        }
        static std::pair<node_ptr_t, Val> prod_by_index(node_ptr_t node, int l, int r) {
            auto [tl, tm, tr] = Base::split_by_index(node, l, r);
            Val res = prod_all(tm);
            return { Base::merge(tl, tm, tr), res };
        }
        static std::pair<node_ptr_t, Val> prod_by_key(node_ptr_t node, const Key &l, const Key &r) {
            auto [tl, tm, tr] = Base::split_by_key(node, l, r);
            Val res = prod_all(tm);
            return { Base::merge(tl, tm, tr), res };
        }
    };
    
    template <typename Key, typename Monoid>
    struct RangeFoldableMapNode : public RangeFoldableMapNodeBase<Key, Monoid, RangeFoldableMapNode<Key, Monoid>> {
        using Base = RangeFoldableMapNodeBase<Key, Monoid, RangeFoldableMapNode<Key, Monoid>>;
        using Base::RangeFoldableMapNodeBase;
        using node_ptr_t = typename Base::node_ptr_t;
    };
}

template <typename Key, typename Monoid>
class RangeFoldableMap {
    using Val = Monoid::value_type;
    using Node = internal::range_foldable_map::RangeFoldableMapNode<Key, Monoid>;
    using node_ptr_t = typename Node::node_ptr_t;
public:
    RangeFoldableMap() : root(nullptr) {}
    ~RangeFoldableMap() {
        delete root;
    }
    RangeFoldableMap& operator=(const RangeFoldableMap&) = delete;
    RangeFoldableMap& operator=(RangeFoldableMap&& other) {
        if (other.root == root) return *this;
        delete root;
        root = other.root;
        other.root = nullptr;
        return *this;
    }
    int size() {
        return Node::size(root);
    }
    bool contains(const Key &key) {
        auto [new_root, found] = Node::find_key(root, key);
        root = new_root;
        return found;
    }
    void insert(const Key &key, const Val &val) {
        root = Node::insert(root, key, val, true);
    }
    void insert_if_absent(const Key &key, const Val &val) {
        root = Node::insert(root, key, val, false);
    }
    bool erase_key(const Key &key) {
        auto [new_root, is_erased] = Node::erase_key(root, key);
        root = new_root;
        return is_erased;
    }
    void erase_index(int k) {
        index_bounds_check(k, size() + 1);
        root = Node::erase_index(root, k);
    }
    Val& get_or_create(const Key &key, const Val &val) {
        root = Node::insert(root, key, val, false);
        return root->val;
    }
    auto operator[](const Key &key) {
        get_or_create(key, Monoid::unit());
        return UpdateProxyObject { root->val, [this]{ root->update(); } };
    }
    Val operator()(const Key &l, const Key &r) {
        return prod(l, r);
    }
    Val prod_by_key(const Key &l, const Key &r) {
        auto [new_root, res] = Node::prod_by_key(root, l, r);
        root = new_root;
        return res;
    }
    Val prod_by_index(int l, int r) {
        auto [new_root, res] = Node::prod_by_index(root, l, r);
        root = new_root;
        return res;
    }
    Val prod_all() {
        return Node::prod_all(root);
    }
    Val get_or_default(const Key &key, const Val &default_value) {
        auto [new_root, res] = Node::get_or_default(root, key, default_value);
        root = new_root;
        return res;
    }
    std::pair<Key, Val> kth_entry(int k) {
        index_bounds_check(k, size());
        root = Node::splay_by_index(root, k);
        return { root->key, root->val };
    }
    RangeFoldableMap split_by_index(int k) {
        index_bounds_check(k, size() + 1);
        auto [l, r] = Node::split_by_index(root, k);
        root = l;
        return RangeFoldableMap(r);
    }
    RangeFoldableMap split_by_key(const Key &key) {
        auto [l, r] = Node::split_by_key(root, key);
        root = l;
        return RangeFoldableMap(r);
    }
    void merge(RangeFoldableMap &&r) {
        assert(root == nullptr or root != r.root);
        root = Node::merge(root, r.root);
        r.root = nullptr;
    }
    void swap(RangeFoldableMap &r) {
        std::swap(root, r.root);
    }
protected:
    Node *root;
    RangeFoldableMap(node_ptr_t root) : root(root) {}
    static void index_bounds_check(unsigned int k, unsigned int n) {
        assert(k < n);
    }
    static void range_bounds_check(unsigned int l, unsigned int r, unsigned int n) {
        assert(l <= r and r <= n);
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_FOLDABLE_MAP