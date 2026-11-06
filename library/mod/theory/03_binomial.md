# 3. Factorials and binomial coefficients

Headers: [`modint_common`](../modint_common.hpp),
[`factorial998`](../factorial998.hpp), [`factorial107`](../factorial107.hpp),
[`binomial`](../binomial.hpp), [`binomial_u64`](../binomial_u64.hpp),
[`prefix_sum_of_binom`](../prefix_sum_of_binom.hpp),
[`q_analogue`](../q_analogue.hpp).

---

## 1. Factorial tables (prime modulus)

`fact<mint>(n)`, `fact_inv<mint>(n)` and `inv<mint>(n)` keep static tables:

* `fact` grows by doubling, up to `min(mod, next power of two > n)`.
* `fact_inv` inverts the last entry once and fills down with
  `(i!)^{-1} = ((i+1)!)^{-1} · (i+1)`.
* `inv` uses the linear recurrence of chapter 2 §3 and accepts `n ≥ mod`
  (reduced mod `mod`).

All tables are rebuilt when `mint::get_mod()` changes, so they stay valid for
`Dynamic_Modint` after `set_mod`.

| call | value | method |
|---|---|---|
| `C<mint>(n, k)` | `binom(n, k)`, `n < mod` | `n! / (k! (n−k)!)` |
| `C<mint, true>(n, k)` | `n` huge, `k` small | `n (n−1) ⋯ (n−k+1) / k!`, `O(k)` |
| `C<mint, false, true>(n, k)` | any modulus (no inverses) | Pascal's triangle, cached |
| `C_inv<mint>(n, k)` | `binom(n, k)^{-1}` | |
| `C_negative<mint>(n, d)` | `[x^d] (1 − x)^{−n} = binom(n+d−1, d)` | `d` balls into `n` boxes |
| `P<mint>(n, k)` | `n! / (n−k)!` | |
| `multinomial<mint>(n, k1, k2, …)` | `n! / (k1! k2! ⋯)` | |

`C` returns `0` for `k < 0` or `k > n`.

---

## 2. One factorial below a large prime

`factorial998(n)`, `factorial107(n)`: `n! mod p` for `n < p ≈ 10^9`, using an
embedded table of `(q · 2^20)!` for every `q`. A query multiplies at most
`2^20` further terms: `n! = (q·2^20)! · ∏_{i ≤ r} (q·2^20 + i)` with
`n = q·2^20 + r`. For `n ≥ p` the answer is `0`.

---

## 3. `Binomial(mod)`: any modulus

`Binomial X(m); X.C(n, k)` for `n, k` up to `10^18` and a fixed `m`.
`m = ∏ p^e`; compute `binom(n, k) mod p^e` for each factor and combine by CRT
(`crt_coef[s] = (m/p^e) · ((m/p^e)^{-1} mod p^e)`). Each prime power uses a
table of size `p^e`, so this suits `m` up to about `10^7`.

### Modulo a prime power: Granville

Write `(n!)_p` for the product of the integers in `[1, n]` that are **not**
divisible by `p`. Then

```
n! = p^{ν(n!)} · (n!)_p · (⌊n/p⌋!)_p · (⌊n/p²⌋!)_p ⋯
```

and `(n!)_p` modulo `p^e` only depends on the full blocks of length `p^e` and
the remainder `n mod p^e`:

```
(n!)_p ≡ (±1)^{⌊n / p^e⌋} · (units in [1, n mod p^e])    (mod p^e)
```

The product of all units mod `p^e` is `−1`, except for `p = 2`, `e ≥ 3`, where
it is `+1` (generalised Wilson). Hence, digit by digit in base `p`:

```
binom(n, k) ≡ p^c · ± · ∏_digits  F(n_i) / (F(k_i) F(r_i))     (r = n − k)
```

where `F(t)` is the product of units in `[1, t]`, `t < p^e`, and `c` is the
number of carries when adding `k + r` in base `p` (Kummer). If `c ≥ e` the
answer is `0`.

The code keeps everything in **logarithms** to avoid divisions by units:

* odd `p`: units mod `p^e` are cyclic. A generator comes from
  `primitive_root_prime_power_64`, which checks lifting (chapter 4 §1).
  `log_fact[t] = Σ_{units u ≤ t} log u`, so each digit adds
  `log_fact[n_i] − log_fact[k_i] − log_fact[r_i]`, and the block signs add the
  parities of `⌊·/p^e⌋`.
* `p = 2`, `e ≥ 2`: units mod `2^e` are `±5^j`. `log` stores `j` and drops the
  sign. The sign of the product of units in `[1, t]` is `(−1)^{#{u ≤ t : u ≡ 3 mod 4}}`,
  and that count is `⌊(t + 1) / 4⌋`.
* `p^e = 2`: `binom(n, k)` is odd iff `k & (n − k) = 0` (Lucas).

```
Binomial X(12);   X.C(10, 4) = 6          // 210 mod 12
Binomial Y(1000000);  Y.C(10^18, 5·10^17) = 400000
```

---

## 4. `Binomial_u64`: modulo `2^64`

Only odd numbers are invertible mod `2^64`. Store for every `i ≤ LIM`:

```
fact[i] = odd part of i!     exp[i] = v₂(i!)     ifact[i] = fact[i]^{-1} mod 2^64
binom(n, k) = fact[n] · ifact[k] · ifact[n−k] · 2^{exp[n] − exp[k] − exp[n−k]}
```

Only one inverse is computed (`mod_inv_u64` of `fact[LIM]`); the others go
backwards with `ifact[i−1] = ifact[i] · odd(i)`.

---

## 5. Prefix sums of a binomial row

`Prefix_Sum_of_Binom<mint> P(MAX_N); P.query(n, m) = Σ_{k<m} binom(n, k)`
(static prime modulus), `O(N √N)` precomputation and `O(√N)` per query.

Let `S(n, m) = Σ_{k<m} binom(n, k)`. By Pascal,

```
S(n + 1, m) = 2 S(n, m) − binom(n, m − 1).
```

With `B ≈ √N`, the constructor stores `S(n, kB + 1)` for every block boundary
`kB` and every `n` (one pass of the recurrence per `k`). A query starts from
the nearest boundary and adds or removes at most `B / 2` terms. Terms are
accumulated as raw products in `u128` and reduced once. `m > (n+1)/2` uses the
symmetry `S(n, m) = 2^n − S(n, n + 1 − m)`.

`Prefix_Sum_of_Binom_Offline<mint>`: `add(n, m)` queries, then `calc()`.
It answers them in Mo order (`O(N √Q)` moves):

| move | update |
|---|---|
| `n → n + 1` | `S ← 2S − binom(n, m − 1)` |
| `n → n − 1` | `S ← (S + binom(n − 1, m − 1)) / 2` |
| `m → m ± 1` | add / remove `binom(n, m)` |

```
P.query(10, 3) = 1 + 10 + 45 = 56
```

---

## 6. q-analogues

`q_Analogue<mint> X(q, LIM)` (prime modulus `p`, `LIM < p`):

```
[n]_q = 1 + q + ⋯ + q^{n−1}         [n]_q! = [1]_q [2]_q ⋯ [n]_q
[n choose k]_q = [n]_q! / ([k]_q! [n−k]_q!)
```

`[n choose k]_q` counts `k`-dimensional subspaces of `F_q^n` when `q` is a
prime power, and satisfies
`[n choose k]_q = [n−1 choose k−1]_q + q^k [n−1 choose k]_q`.

Let `D` be the smallest `n` with `[n]_q ≡ 0`. For `q ≠ 1` this is the
multiplicative order of `q`, and for `q = 1` it is `p`. Factorials are only
invertible below `D`. Beyond `D`, **q-Lucas** applies:

```
[N choose K]_q ≡ binom(⌊N/D⌋, ⌊K/D⌋) · [N mod D choose K mod D]_q      (mod p)
```

`X.fact(N)` is `0` for `N ≥ D`.

```
q = 2 mod 7:  [1] = 1, [2] = 3, [3] = 7 ≡ 0  →  D = 3
[6 choose 3]_2 ≡ binom(2, 1) · [0 choose 0]_2 = 2
```
