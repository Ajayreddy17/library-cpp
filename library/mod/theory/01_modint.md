# 1. Modular integers

Headers:

- [`mod/modint.hpp`](../modint.hpp)
- [`mod/modint_64bit.hpp`](../modint_64bit.hpp)
- [`mod/modint61.hpp`](../modint61.hpp)
- [`mod/dynamic_modint.hpp`](../dynamic_modint.hpp)
- [`mod/dynamic_modint_64.hpp`](../dynamic_modint_64.hpp)
- [`mod/barrett.hpp`](../barrett.hpp)
- [`mod/montgomery_modint.hpp`](../montgomery_modint.hpp)
- [`mod/mod_pow.hpp`](../mod_pow.hpp)
- [`mod/mod_pow_u64.hpp`](../mod_pow_u64.hpp)
- [`mod/mod_inv_u64.hpp`](../mod_inv_u64.hpp)

---

## 1. Representation

Every modint stores `val ∈ [0, m)`.

* `+` / `−`: add (or add `m − y`), subtract `m` once if the result is `≥ m`.
  Needs `2m` to fit in the storage type.
* `×`: multiply in a type twice as wide, then `% m`.
* construction from a signed value: `x %= m`, add `m` if negative.

| type | modulus | storage / product |
|---|---|---|
| `modint<mod>` | compile time, `0 < mod < 2^31` | `u32` / `u64` |
| `modint_64bit<mod>` | compile time, `mod < 2^63` | `u64` / `u128` |
| `modint61` | `2^61 − 1` | `u64` / `u128`, no division |
| `Dynamic_Modint<id>` | runtime, `< 2^31` | `u32` / Barrett |
| `Dynamic_Modint_64<id>` | runtime, `< 2^63` | `u64` / Barrett_64 |
| `Montgomery_modint_32/64<id>` | runtime, odd, `≤ 2^30` / `≤ 2^62` | Montgomery form |

`modint998 = modint<998244353>`, `modint107 = modint<1000000007>`,
`dmint = Dynamic_Modint<-1>`, `dmint64 = Dynamic_Modint_64<-1>`.

---

## 2. Inverse and powers

`x.inverse()` runs the extended Euclidean algorithm on `(val, m)` with the
invariants

```
a ≡ u · val,   b ≡ v · val   (mod m)        start: (a, u) = (val, 1), (b, v) = (m, 0)
```

When `b` reaches 0, `a = gcd(val, m)`. If it is `1`, then `u ≡ val^{-1}`.
The inverse exists exactly when `gcd(val, m) = 1`; for a prime `m` that
means `val ≠ 0`. The values `|u|, |v|` never exceed `m`, so `int` suffices
for `m < 2^31` and `long long` for `m < 2^63`.

`x.pow(n)` is binary exponentiation; `n < 0` means `inverse().pow(−n)`.

---

## 3. NTT information

`mint::ntt_info()` returns `(n, r)` with `r` a primitive `2^n`-th root of
unity, or `(−1, −1)`; `can_ntt()` tests it. A prime `p = c · 2^n + 1` has
`2^n`-th roots of unity. Known primes:

| p | n | p | n |
|---|---|---|---|
| 998244353 | 23 | 167772161 | 25 |
| 469762049 | 26 | 754974721 | 24 |
| 1224736769 | 24 | 2013265921 | 27 |

For `Dynamic_Modint`, call `set_ntt_info()` after `set_mod`. It computes
`n = v₂(m − 1)` and `r = g^{(m−1)/2^n}` from a primitive root `g`.
`set_mod` resets it to `(−1, −1)`.

---

## 4. `modint61`: the Mersenne prime `2^61 − 1`

Since `2^61 ≡ 1`, a 122-bit product `y = hi · 2^61 + lo` is congruent to
`hi + lo`:

```
val = (y >> 61) + (y & mod);   if (val >= mod) val -= mod;
```

There is no division at all. A prime this large makes random collisions in
polynomial hashing negligible (`modint61` is the field for rolling hashes and
the hash trees of chapter 6).

---

## 5. Barrett reduction

For a runtime modulus `m < 2^32`, precompute `im = ⌈2^64 / m⌉`. For `z < 2^64`:

```
x = (z · im) >> 64          // floor(z / m) or floor(z / m) + 1
y = x · m
z mod m = z − y  (+ m if z < y)
```

`floor(z)` and `divmod(z)` return the quotient with the same correction.
`Barrett_64` does the same for 64-bit moduli with
`⌊2^128 / m⌋` split into two 64-bit halves (a 128 × 128 → high 128 product
built from four 64 × 64 products).

`Dynamic_Modint<id>` keeps one static `Barrett` per `id`: `set_mod(m)` replaces
it. Different `id`s are independent types, so two runtime moduli can be used
at once.

---

## 6. Montgomery multiplication

For an odd modulus `m` and `R = 2^W` (`W = 32` or `64`), store `x̃ = x · R mod m`
instead of `x`. Products need the reduction `REDC(T) = T · R^{-1} mod m`:

```
r = −m^{-1} mod R
REDC(T) = (T + ((T · r) mod R) · m) / R
```

The numerator is divisible by `R` by the choice of `r`, so the division is a
shift. Then `x̃ ỹ → REDC(x̃ ỹ) = (xy)~`. Conversion in is `REDC(x · (R² mod m))`,
conversion out is `REDC(x̃)`.

`m^{-1} mod 2^W` comes from Newton's iteration `r ← r (2 − m r)`, which
doubles the number of correct low bits. Start with `r = m`, already correct
mod 8 because every odd `m` has `m² ≡ 1 (mod 8)`.

Values are kept lazily in `[0, 2m)`. That is why `m ≤ 2^{W−2}` is required.

`mod_pow(a, n, mod)` (`int`) and `mod_pow_64` use Montgomery for odd moduli
below `2^30` / `2^62` and Barrett otherwise; `mod = 1` gives `0`.

---

## 7. Arithmetic mod `2^64`

Unsigned 64-bit overflow is arithmetic mod `2^64`.

* `mod_pow_u64(a, n)`: binary exponentiation in `u64`.
* `mod_inv_u64(a)` for odd `a`: Newton `x ← x (2 − a x)` from `x = 1`
  (correct mod 2), six steps give `1 → 2 → … → 64` correct bits.

Chapter 3 (`Binomial_u64`) and chapter 4 (`mod_log_u64`) build on these.
