# 2. Inverses, power tables and CRT

Headers:

- [`mod/mod_inv.hpp`](../mod_inv.hpp)
- [`mod/all_inverse.hpp`](../all_inverse.hpp)
- [`mod/O1_inverse.hpp`](../O1_inverse.hpp)
- [`mod/modfast.hpp`](../modfast.hpp)
- [`mod/powertable.hpp`](../powertable.hpp)
- [`mod/crt3.hpp`](../crt3.hpp)

---

## 1. One inverse, any modulus

`mod_inv(val, mod)` returns `x ∈ [0, mod)` with `val · x ≡ 1 (mod mod)` by
extended Euclid (chapter 1 §2), for `long long` values. It needs
`gcd(val, mod) = 1`. For `mod = 0` it returns `0`.

---

## 2. Many inverses with one division

`all_inverse(X)` inverts every `X[i]` (all nonzero) with **one** inversion:

```
P[0] = 1, P[i+1] = P[i] · X[i]          prefix products
t = P[N]^{-1}
for i = N−1 .. 0:  X[i]^{-1} = P[i] · t,   t ← t · X[i]
```

Invariant: before step `i`, `t = (X[0] ⋯ X[i])^{-1}`. Cost: `3(N − 1)`
multiplications and one inverse.

---

## 3. Inverses of `1..n` in linear time

For a prime `p` and `2 ≤ i < p`, let `q = ⌈p / i⌉`. Then

```
t = i · q − p  satisfies  0 ≤ t < i   and   i · q ≡ t  (mod p),
so  i^{-1} = q · t^{-1}.
```

`t < i` was computed earlier, so the table fills left to right in `O(n)`.
This is `inv<mint>(n)` in [`modint_common`](../modint_common.hpp), and the
table inside `O1_inverse` and `ModFast`.

---

## 4. `O1_inverse<mint>(a)`: `O(1)` inverse for `p ≤ 2^30`

Precomputation is `O(p^{2/3})`, and every query after that is `O(1)`.

Take `n = 2^10`, so `n³ ≥ p`. Approximate `a / p` by a fraction `x / y` with
`y < n`. Dirichlet / Farey gives one with `|a/p − x/y| ≤ 1 / (n y)`, hence

```
c = a·y − p·x,    |c| ≤ p / n ≤ 2^20,     c ≡ a·y (mod p)
a^{-1} = y · c^{-1}
```

and `c^{-1}` is in the linear table of size `2^21` (§3).

To find `x / y` in `O(1)`: split `[0, 1)` into `2^20` buckets. Bucket `k`
stores the fraction (`y < 1024`) that is closest to the bucket's position
among the Farey neighbours. A query looks at bucket `⌊a · 2^20 / p⌋`. The
tables are rebuilt when `mint::get_mod()` changes.

---

## 5. `ModFast<mint>`: logs, powers and inverses by table

For a prime `p < 2^30` with primitive root `r`, `ModFast` stores discrete
logarithms, so that

```
a^e = r^{(log a · e) mod (p−1)}        a^{-1} = r^{−log a}
r^e = POW0[e mod 2^15] · POW1[e >> 15]     (two tables of 2^15 + 1 entries)
```

* **Small `p`** (`p ≤ 2^21`): `LOG[x]` and `INV[x]` for every `x`.
* **Large `p`**: `LOG[K + i]` and `INV[K + i]` for `|i| ≤ K = 2^21`. A query
  `x` is reduced with the same Farey trick as §4: `x · b − a · p = t`, with
  `|t| ≤ K` and `b < 2048`, so `x ≡ t / b`. Then
  `log x = log t − log b` and `x^{-1} = b · t^{-1}`.

The `LOG` table for `|i| ≤ K` is built multiplicatively:

* composite `i`: `log i = log lpf(i) + log(i / lpf(i))`.
* prime `i < 100`: baby-step giant-step.
* prime `i` with `i² > p`: write `p = j·i + k` with `j, k < i`. Then
  `i ≡ −k / j`, so `log i = log k + (p−1)/2 − log j`, where `log(−1) = (p−1)/2`.
* other primes: index calculus. Pick random `k` until `i · r^k mod p`
  factors into primes smaller than `i`, whose logs are already known.
* negative `i`: `log(−i) = log i + (p−1)/2`.

`log_r(x)` returns a value in `[0, 2p − 2)`; reduce it mod `p − 1` if needed.
`get_log_table(n)` returns `log 1 .. log n`.

---

## 6. Power tables

* `powertable_1(a, N)`: `a^0, …, a^N`.
* `powertable_2<mint>(e, N)`: `0^e, 1^e, …, N^e`. `x ↦ x^e` is completely
  multiplicative, so only primes need a `pow`. Every prime power `p^k ≤ N`
  multiplies `p^e` into all of its multiples, giving `O(N log log N)`
  multiplications plus `π(N)` exponentiations.

```
powertable_2<mint>(3, 6)  =  0 1 8 27 64 125 216
```

---

## 7. CRT by Garner

`CRT2 … CRT5<T, p0, p1, …>(a0, a1, …)` recombine residues modulo distinct
primes `p0 < p1 < …` (compile time) into `x mod p0 p1 ⋯`, computed in type `T`:

```
x = a0 + c1·p0 + c2·p0p1 + c3·p0p1p2 + …
c_k = (a_k − (x so far)) · (p0 ⋯ p_{k−1})^{-1}   (mod p_k)
```

Every `c_k < p_k`, so all intermediate values stay below the product and the
inverses are compile-time constants. This is how convolution with an
arbitrary modulus combines three NTT-prime results.

```
CRT3<u64, 3, 5, 7>(2, 3, 2) = 23      (23 ≡ 2 mod 3, ≡ 3 mod 5, ≡ 2 mod 7)
```
