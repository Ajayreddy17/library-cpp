#ifndef AJAY_PYVECTOR
#define AJAY_PYVECTOR

namespace mitsuha{
template<typename T>
struct pyvector{
    vector<T> vec;
    pyvector(int _n, T val = T{}) {
        n = _n;
        vec.assign(n, val);
    }
    T& operator[](int i){
        assert(i < n and i >= -n);
        return i >= 0 ? vec[i]: vec[n - i];
    }
    void push_back(T v){
        vec.push_back(v);
        ++n;
    }
    void pop_back(T v){
        assert(n)
        vec.pop_back(v);
        --n;
    }
    T &front(){ return vec[0]; }
    T &back() { return vec[n - 1]; }
};
} // namespace mitsuha
#endif // AJAY_PYVECTOR