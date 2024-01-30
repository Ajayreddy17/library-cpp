#ifndef AJAY_DEFAULTDICT
#define AJAY_DEFAULTDICT

namespace mitsuha{
template<typename Key, typename T>
struct defaultdict : map<Key, T>{
    using super = map<Key, T>;
    const T val;
    defaultdict(T val_): val(val_){ }
    T& operator[](const Key& key){
        if(!super::count(key))
            super::operator[](key) = val;
        return super::operator[](key);
    }
};
} // namespace mitsuha
#endif // AJAY_DEFAULTDICT