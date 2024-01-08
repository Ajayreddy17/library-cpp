#ifndef AJAY_COUNT_TERMS
#define AJAY_COUNT_TERMS

namespace mitsuha{
template<typename mint>
int count_terms(const vector<mint>& f){
    int t = 0;
    for(int i = 0; i < (int) f.size(); i++) if(f[i] != mint(0)) ++t;
    return t;
}
} // namespace mitsuha
#endif // AJAY_COUNT_TERMS