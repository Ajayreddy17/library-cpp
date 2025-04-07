#ifndef AJAY_ARRAY_ON_DIVISORS
#define AJAY_ARRAY_ON_DIVISORS

#include "library/number/factor.hpp"
#include "library/datastructure/hashmap.hpp"

namespace mitsuha{
template <typename T>
struct Array_On_Divisors {
    vector<pair<long long, int>> pf;
    vector<long long> divs;
    vector<T> dat;
    HashMap<int> MP;

    Array_On_Divisors(long long N = 1) { build(N); }
    Array_On_Divisors(vector<pair<long long, int>> pf) { build(pf); }

    void build(long long N) { build(factor(N)); }
    void build(vector<pair<long long, int>> pfs) {
        if (not pf.empty() && pf == pfs)
            return;
        pf = pfs;
        long long n = 1;
        for (auto&& [p, e] : pf) n *= (e + 1);
        divs.assign(n, 1);
        dat.assign(n, T{});
        int nxt = 1;
        for (auto&& [p, e] : pf) {
            int L = nxt;
            long long q = p;
            For(e){
                For(i, L) divs[nxt++] = divs[i] * q;
                q *= p;
            }
        }
        MP.build(n);
        For(i, n) MP[divs[i]] = i;
    }

    T& operator[](long long d) { return dat[MP[d]]; }

    // f(p, k) Given -> Extend multiplicatively
    template <typename F>
    void set_multiplicative(F f) {
        dat.reserve(int(divs.size()));
        dat = {T(1)};
        for (auto&& [p, e] : pf) {
            int n = int(divs.size());
            For(k, 1, e + 1) For(i, n) dat.emplace_back(dat[i] * f(p, k));
        }
    }

    void set_euler_phi() {
        dat.resize(int(divs.size()));
        For(i, int(divs.size())) dat[i] = T(divs[i]);
        divisor_mobius();
    }

    void set_mobius() {
        set_multiplicative([&](long long p, int k) -> T {
            if (k >= 2) return T(0);
            return (k == 1 ? T(-1) : T(0));
        });
    }

    void multiplier_zeta() {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                Frr(j, mod - k) dat[mod * i + j] += dat[mod * i + j + k];
            }
            k *= (e + 1);
        }
    }

    void multiplier_mobius() {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod) {
                For(j, mod - k) dat[mod * i + j] -= dat[mod * i + j + k];
            }
            k *= (e + 1);
        }
    }

    void divisor_zeta() {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                For(j, mod - k) dat[mod * i + j + k] += dat[mod * i + j];
            }
            k *= (e + 1);
        }
    }

    void divisor_mobius() {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod) {
                Frr(j, mod - k) dat[mod * i + j + k] -= dat[mod * i + j];
            }
            k *= (e + 1);
        }
    }

    // (T a, T b) -> T: a - b
    template <typename F>
    void divisor_mobius(F SUB) {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                Frr(j, mod - k) {
                    dat[mod * i + j + k] = SUB(dat[mod * i + j + k], dat[mod * i + j]);
                }
            }
            k *= (e + 1);
        }
    }

    // ADD(T a, T b) -> T: a + b
    template <typename F>
    void multiplier_zeta(F ADD) {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                Frr(j, mod - k) {
                    dat[mod * i + j] = ADD(dat[mod * i + j], dat[mod * i + j + k]);
                }
            }
            k *= (e + 1);
        }
    }

    // SUB(T a, T b) -> T: a -= b
    template <typename F>
    void multiplier_mobius(F SUB) {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                For(j, mod - k) {
                    dat[mod * i + j] = SUB(dat[mod * i + j], dat[mod * i + j + k]);
                }
            }
            k *= (e + 1);
        }
    }

    // ADD(T a, T b) -> T: a += b
    template <typename F>
    void divisor_zeta(F ADD) {
        long long k = 1;
        for (auto&& [p, e] : pf) {
            long long mod = k * (e + 1);
            For(i, int(divs.size()) / mod){
                For(j, mod - k) {
                    dat[mod * i + j + k] = ADD(dat[mod * i + j + k], dat[mod * i + j]);
                }
            }
            k *= (e + 1);
        }
    }

    template <typename F>
    void set(F f) {
        For(i, int(divs.size())) dat[i] = f(divs[i]);
    }

    // (d, fd)
    // &fd by reference, can also modify it
    template <typename F>
    void enumerate(F f) {
        For(i, int(divs.size())) f(divs[i], dat[i]);
    }
};
} // namespace mitsuha
#endif // AJAY_ARRAY_ON_DIVISORS