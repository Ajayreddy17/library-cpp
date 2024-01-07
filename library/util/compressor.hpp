#ifndef AJAY_COMPRESSOR
#define AJAY_COMPRESSOR

namespace mitsuha{
template<class T>
struct Compressor {
public:
    static constexpr int absent = -1;

    // default constructor
    Compressor() : _xs(vector<T>{}) {}
    // Construct from strictly sorted vector
    Compressor(const vector<T> &xs) : _xs(xs) {
        assert(is_strictly_sorted(xs));
    }

    // Return the number of distinct keys.
    int size() const {
        return _xs.size();
    }
    // Check if the element is registered.
    bool has_key(const T &e) const {
        return binary_search(_xs.begin(), _xs.end(), e);
    }
    // Compress the element. if not registered, returns `default_value`. (default: Compressor::absent)
    int comp(const T &e, int default_value = absent) const {
        const int res = min_geq_index(e);
        return res != size() and _xs[res] == e ? res : default_value;
    }
    // Restore the element from the index.
    T decomp(const int compressed_index) const {
        return _xs[compressed_index];
    }
    // Compress the element. Equivalent to call `comp(e)`
    int operator[](const T &e) const {
        return comp(e);
    }
    // Return the minimum registered value greater than `e`. if not exists, return `default_value`.
    T min_gt(const T &e, const T &default_value) const {
        auto it = upper_bound(_xs.begin(), _xs.end(), e);
        return it == _xs.end() ? default_value : *it;
    }
    // Return the minimum registered value greater than or equal to `e`. if not exists, return `default_value`.
    T min_geq(const T &e, const T &default_value) const {
        auto it = lower_bound(_xs.begin(), _xs.end(), e);
        return it == _xs.end() ? default_value : *it;
    }
    // Return the maximum registered value less than `e`. if not exists, return `default_value`
    T max_lt(const T &e, const T &default_value) const {
        auto it = upper_bound(_xs.rbegin(), _xs.rend(), e, greater<T>());
        return it == _xs.rend() ? default_value : *it;
    }
    // Return the maximum registered value less than or equal to `e`. if not exists, return `default_value`
    T max_leq(const T &e, const T &default_value) const {
        auto it = lower_bound(_xs.rbegin(), _xs.rend(), e, greater<T>());
        return it == _xs.rend() ? default_value : *it;
    }
    // Return the compressed index of the minimum registered value greater than `e`. if not exists, return `compressor.size()`.
    int min_gt_index(const T &e) const {
        return upper_bound(_xs.begin(), _xs.end(), e) - _xs.begin();
    }
    // Return the compressed index of the minimum registered value greater than or equal to `e`. if not exists, return `compressor.size()`.
    int min_geq_index(const T &e) const {
        return lower_bound(_xs.begin(), _xs.end(), e) - _xs.begin();
    }
    // Return the compressed index of the maximum registered value less than `e`. if not exists, return -1.
    int max_lt_index(const T &e) const {
        return int(_xs.rend() - upper_bound(_xs.rbegin(), _xs.rend(), e, greater<T>())) - 1;
    }
    // Return the compressed index of the maximum registered value less than or equal to `e`. if not exists, return -1.
    int max_leq_index(const T &e) const {
        return int(_xs.rend() - lower_bound(_xs.rbegin(), _xs.rend(), e, greater<T>())) - 1;
    }
private:
    vector<T> _xs;
    static bool is_strictly_sorted(const vector<T> &v) {
        return adjacent_find(v.begin(), v.end(), greater_equal<T>()) == v.end();
    }
};
} // namespace mitsuha
#endif // AJAY_COMPRESSOR