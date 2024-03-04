#ifndef AJAY_DEFAULTDICT
#define AJAY_DEFAULTDICT

namespace mitsuha{
template<typename Key, typename T>
struct defaultdict : map<Key, T>{
    using super = map<Key, T>;
    const function<T(Key)> f;

    defaultdict(T val): defaultdict([val](Key){ return val; }){ }
    defaultdict(function<T(Key)> _f): f(_f){ }

    T& operator[](const Key& key){
        if(!super::count(key))
            super::operator[](key) = f(key);
        return super::operator[](key);
    }
};
} // namespace mitsuha
#endif // AJAY_DEFAULTDICT