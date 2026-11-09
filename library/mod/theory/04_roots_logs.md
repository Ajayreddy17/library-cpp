# 4. Primitive roots, roots and logarithms

Headers:

- [`mod/primitive_root.hpp`](../primitive_root.hpp)
- [`mod/mod_sqrt.hpp`](../mod_sqrt.hpp)
- [`mod/mod_kth_root.hpp`](../mod_kth_root.hpp)
- [`mod/mod_log.hpp`](../mod_log.hpp)
- [`mod/mod_log_u64.hpp`](../mod_log_u64.hpp)
- [`mod/tetration.hpp`](../tetration.hpp)

`p` is prime throughout unless stated.

---

## 1. Primitive roots

`(Z/p)^×` is cyclic of order `p − 1`. `g` generates it iff

```
g^{(p−1)/q} ≠ 1   for every prime q | p − 1.
```

* `primitive_root(p)`: the smallest one, by scanning `g = 1, 2, …` (it is
  always small in practice). `primitive_root(p, false)` instead draws random
  `g`, each a generator with probability `φ(p−1)/(p−1)`.
* `primitive_root_64(p)`: random, for `p < 2^63`.

```
primitive_root(998244353) = 3,   primitive_root(10^9 + 7) = 5
```

**Prime powers.** For odd `p`, `(Z/p^e)^×` is cyclic too. If `g` generates
mod `p` and `g^{p−1} ≢ 1 (mod p²)`, then `g` generates mod every `p^e`;
otherwise `g + p` does. `primitive_root_prime_power_64(p, e)` applies this
check at each level. For `2^e` (`e ≥ 3`) there is no generator:
`(Z/2^e)^× = {±1} × ⟨5⟩` (§5).

---

## 2. Square roots: Cipolla

`mod_sqrt(a, p)` / `mod_sqrt_64(a, p)` return some `x` with `x² ≡ a`, or `−1`.

1. Euler's criterion: `a` is a square iff `a^{(p−1)/2} = 1` (or `a = 0`).
2. Pick random `b` until `D = b² − a` is a non-square.
3. In `F_p(√D) = {u + v√D}`, `(b + √D)^p = b − √D` (Frobenius), so
   `(b + √D)^{p+1} = b² − D = a`. Hence `x = (b + √D)^{(p+1)/2}` has `x² = a`
   and lies in `F_p`.

Expected two tries for `b`, then `O(log p)` multiplications in `F_p(√D)`.

```
mod_sqrt(5, 11) = 7   (49 = 4·11 + 5)        mod_sqrt(3, 7) = −1
```

---

## 3. `k`-th roots: Adleman–Manders–Miller

`mod_kth_root(k, a, p)` / `mod_kth_root_64`: some `x` with `x^k ≡ a`, or `−1`.

**Reduce to `k | p − 1`.** Let `g = gcd(k, p − 1)`. A solution exists iff
`a^{(p−1)/g} = 1`. With `c = (k/g)^{-1} mod (p−1)/g`, replace `a` by `a^c`
and `k` by `k·c mod (p − 1)`. The new `k` divides `p − 1` up to a unit, and
the same `x` works.

**One prime power at a time.** Factor `k = ∏ q^e` and take `q^e`-th roots
successively. For a single `q^e` write `p − 1 = q^f · m` with `q ∤ m`.

* Keep `b = a / x^{q^e}`, starting with `x = 1`, together with `c` such that
  `b^{m q^c} = 1`. Initially `c = f − e`, because `a` is a `q^e`-th power.
* While `c > 0`: `B = b^{m q^{c−1}}` is a `q`-th root of unity. Find `B = G^j`,
  where `G` is a primitive `q`-th root of unity, by baby-step giant-step in
  `O(√q)`. Multiply `x` by the power of the primitive root whose `q^e`-th
  power cancels `B`, and decrease `c`.
* Now `b^m = 1`. Since `m` is invertible mod `q^e`, `b = (b^t)^{q^e}` with
  `t = (k m + 1) / q^e` and `k ≡ −m^{-1} (mod q^e)`, so `x ← x · b^t`.

Cost: `O(Σ_{q^e ∥ k} (e·log p + √q) · log p)`.

```
mod_kth_root(3, 5, 13) = 8   (512 = 39·13 + 5)        mod_kth_root(3, 6, 13) = −1
```

---

## 4. Discrete logarithm: baby-step giant-step

`mod_log(mod, a, b)`: the smallest `n ≥ 0` with `a^n ≡ b (mod mod)`, or `−1`.
`mod` need not be prime, and `a` need not be invertible.

It is `discrete_log_monoid` ([`number/discrete_log.hpp`](../../number/discrete_log.hpp))
over the multiplicative monoid. To find `n ∈ [lb, ub)` with `x^n s = t`:

```
K = ⌈√(ub − lb)⌉
baby steps: store H(x^i t) for i = 1..K
giant steps: s, x^K s, x^{2K} s, …; when x^{(j+1)K} s matches some stored x^i t,
             scan x^{jK} s, x^{jK+1} s, … for the first exact hit of t
```

Without inverses a match may be a false positive (`x` not injective). The
scan tolerates one, since after the first collision the sequence is periodic.
Cost: `O(√(ub − lb))` operations and a hash map.

```
mod_log(13, 2, 11) = 7       mod_log(12, 2, 8) = 3       (2 not invertible mod 12)
```

---

## 5. Discrete log mod `2^64`

`mod_log_u64(a, b)`: smallest `n` with `a^n ≡ b (mod 2^64)`.

Odd residues mod `2^64` form `{±1} × ⟨5⟩`, with `5` of order `2^62`.
`mod_log_u64_primitive_root(a)` returns `(s, K)` with `a = (−1)^s 5^K`:

* `s = 1` iff `a ≡ 3 (mod 4)`; then replace `a` by `−a`.
* Bits of `K` are found from low to high. Keep `X = 5^K mod 2^64` and compare
  with `a` one bit position higher each step. `5^{2^{n−2}}` is the factor that
  fixes bit `n`.

Then `a^n = b` becomes `s_a·n ≡ s_b (mod 2)` and `K_a·n ≡ K_b (mod 2^62)`, a
linear congruence solved with `mod_inv_u64`. Even `a` satisfies `a^{64} ≡ 0`,
so it is brute-forced over `n ≤ 64`.

---

## 6. Tetration

`tetration(a, m) = a[0]^(a[1]^(a[2]^⋯)) mod m`, all `a[i] ≥ 1`.

Generalised Euler theorem: for `x ≥ log₂ m`,

```
a^x ≡ a^{(x mod φ(m)) + φ(m)}   (mod m)        (any a, any m)
```

So the exponent only matters mod `φ(m)`, as long as we remember whether it is
`≥ φ(m)`. The chain `m, φ(m), φ(φ(m)), …` reaches `1` in `O(log m)` steps,
which bounds how many levels of the tower are used. Evaluate from the top,
storing each value as `v` if `v < mod`, else `v mod mod + mod`, so that the
"large" flag survives each reduction.

```
tetration({2, 3, 2}, 1000) = 2^9 = 512        tetration({3, 3, 3}, 100) = 3^27 mod 100 = 87
```
