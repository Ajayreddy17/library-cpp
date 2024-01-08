#ifndef AJAY_NTT
#define AJAY_NTT

namespace mitsuha{
template <class mint>
void ntt(vector<mint>& a, bool inverse) {
    assert(mint::can_ntt());
    const int rank2 = mint::ntt_info().first;
    const int mod = mint::get_mod();
    static array<mint, 30> root, iroot;
    static array<mint, 30> rate2, irate2;
    static array<mint, 30> rate3, irate3;

    assert(rank2 != -1 && len(a) <= (1 << max(0, rank2)));

    static bool prepared = 0;
    if (!prepared) {
        prepared = 1;
        root[rank2] = mint::ntt_info().second;
        iroot[rank2] = mint(1) / root[rank2];
        for(int i = rank2 - 1; i >= 0; --i) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
        mint prod = 1, iprod = 1;
        for (int i = 0; i <= rank2 - 2; i++) {
            rate2[i] = root[i + 2] * prod;
            irate2[i] = iroot[i + 2] * iprod;
            prod *= iroot[i + 2];
            iprod *= root[i + 2];
        }
        prod = 1, iprod = 1;
        for (int i = 0; i <= rank2 - 3; i++) {
            rate3[i] = root[i + 3] * prod;
            irate3[i] = iroot[i + 3] * iprod;
            prod *= iroot[i + 3];
            iprod *= root[i + 3];
        }
    }

    int n = int(a.size());
    int h = (n == 0 ? -1 : 31 - __builtin_clz(n));
    assert(n == 1 << h);
    if (!inverse) {
        int len = 0;
        while (len < h) {
            if (h - len == 1) {
                int p = 1 << (h - len - 1);
                mint rot = 1;
                For(s, 1 << len) {
                    int offset = s << (h - len);
                    For(i, p) {
                        auto l = a[i + offset];
                        auto r = a[i + offset + p] * rot;
                        a[i + offset] = l + r;
                        a[i + offset + p] = l - r;
                    }
                    rot *= rate2[((~s & -~s) == 0 ? -1 : 31 - __builtin_clz(~s & -~s))];
                }
                len++;
            } else {
                int p = 1 << (h - len - 2);
                mint rot = 1, imag = root[2];
                for (int s = 0; s < (1 << len); s++) {
                    mint rot2 = rot * rot;
                    mint rot3 = rot2 * rot;
                    int offset = s << (h - len);
                    for (int i = 0; i < p; i++) {
                        unsigned long long mod2 = (unsigned long long)(mod) * mod;
                        unsigned long long a0 = a[i + offset].val;
                        unsigned long long a1 = (unsigned long long)(a[i + offset + p].val) * rot.val;
                        unsigned long long a2 = (unsigned long long)(a[i + offset + 2 * p].val) * rot2.val;
                        unsigned long long a3 = (unsigned long long)(a[i + offset + 3 * p].val) * rot3.val;
                        unsigned long long a1na3imag = (a1 + mod2 - a3) % mod * imag.val;
                        unsigned long long na2 = mod2 - a2;
                        a[i + offset] = a0 + a2 + a1 + a3;
                        a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                        a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                        a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                    }
                    rot *= rate3[((~s & -~s) == 0 ? -1 : 31 - __builtin_clz(~s & -~s))];
                }
                len += 2;
            }
        }
    } else {
        mint coef = mint(1) / mint(len(a));
        For(i, len(a)) a[i] *= coef;
        int len = h;
        while (len) {
            if (len == 1) {
                int p = 1 << (h - len);
                mint irot = 1;
                For(s, 1 << (len - 1)) {
                    int offset = s << (h - len + 1);
                    For(i, p) {
                        unsigned long long l = a[i + offset].val;
                        unsigned long long r = a[i + offset + p].val;
                        a[i + offset] = l + r;
                        a[i + offset + p] = (mod + l - r) * irot.val;
                    }
                    irot *= irate2[((~s & -~s) == 0 ? -1 : 31 - __builtin_clz(~s & -~s))];
                }
                len--;
            } else {
                int p = 1 << (h - len);
                mint irot = 1, iimag = iroot[2];
                For(s, (1 << (len - 2))) {
                    mint irot2 = irot * irot;
                    mint irot3 = irot2 * irot;
                    int offset = s << (h - len + 2);
                    for (int i = 0; i < p; i++) {
                        unsigned long long a0 = a[i + offset + 0 * p].val;
                        unsigned long long a1 = a[i + offset + 1 * p].val;
                        unsigned long long a2 = a[i + offset + 2 * p].val;
                        unsigned long long a3 = a[i + offset + 3 * p].val;
                        unsigned long long x = (mod + a2 - a3) * iimag.val % mod;
                        a[i + offset] = a0 + a1 + a2 + a3;
                        a[i + offset + 1 * p] = (a0 + mod - a1 + x) * irot.val;
                        a[i + offset + 2 * p] = (a0 + a1 + 2 * mod - a2 - a3) * irot2.val;
                        a[i + offset + 3 * p] = (a0 + 2 * mod - a1 - x) * irot3.val;
                    }
                    irot *= irate3[((~s & -~s) == 0 ? -1 : 31 - __builtin_clz(~s & -~s))];
                }
                len -= 2;
            }
        }
    }
}
} // namespace mitsuha
#endif // AJAY_NTT