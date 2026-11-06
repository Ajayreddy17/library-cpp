# 0. Modular arithmetic

The concepts behind every header in `library/mod/`. Each section ends with
the templates that use it.

---

## 1. Congruences and residues

`a ≡ b (mod m)` means `m | a − b`. Every integer is congruent to exactly one
**residue** `r ∈ [0, m)`, namely `r = a − m ⌊a/m⌋`. The integers wrap around a
clock of length `m`:

![clock](images/ch00_clock.png)

`Z/m` is the set of residues with `+`, `−`, `×` taken mod `m`. These are
well defined: if `a ≡ a'` and `b ≡ b'`, then `a + b ≡ a' + b'` and `ab ≡ a'b'`.
So reducing in the middle of a computation never changes the final residue.
That is why a modint can reduce after every operation.

**Negative numbers.** C++ `%` truncates toward zero: `-7 % 12 == -7`. The
residue is `rem(-7, 12) = 5` (`rem` / `fld` in `template.hpp`), and modint
constructors add `m` when the remainder is negative.

→ every modint type ([chapter 1](01_modint.md)).

---

## 2. Division and inverses

Division is not automatic: `2x ≡ 4 (mod 8)` has the solutions `x = 2` and
`x = 6`, so "`4 / 2`" is not one number mod 8. The **inverse** `a^{-1}` is a
residue `x` with `ax ≡ 1`. When it exists, `b / a` means `b · a^{-1}` and is
unique.

**`a` is invertible mod `m` ⟺ `gcd(a, m) = 1`.**

* If `ax ≡ 1`, then `ax − my = 1`, so every common divisor of `a` and `m`
  divides `1`.
* Conversely, **Bézout's identity** gives integers `x, y` with
  `ax + my = gcd(a, m)`. If the gcd is `1`, then `ax ≡ 1`.

The extended Euclidean algorithm finds `x`. Run Euclid on `(m, a)`, keeping
each remainder as a multiple of `a` mod `m`: `r_i ≡ s_i · a`. Example
`17^{-1} mod 43`:

| `q` | `r` | `s` (so `r ≡ s · 17`) |
|---|---|---|
| | 43 | 0 |
| 2 | 17 | 1 |
| 1 | 9 | `0 − 2·1 = −2` |
| 1 | 8 | `1 − 1·(−2) = 3` |
| 8 | 1 | `−2 − 1·3 = −5` |

So `1 ≡ −5 · 17`, i.e. `17^{-1} ≡ −5 ≡ 38 (mod 43)`. There are
`O(log m)` steps, and `|s| ≤ m` throughout.

→ `modint::inverse`, `mod_inv(val, mod)` ([chapter 2](02_inverses_tables.md) §1).

---

## 3. Prime vs composite moduli

![multiplication tables](images/ch00_mult_tables.png)

* **`m = p` prime.** Every nonzero residue is coprime to `p`, so every row of
  the multiplication table contains a `1`. `Z/p` is a **field**: you can divide
  by anything nonzero, solve linear equations, interpolate polynomials, and
  run NTT. This is why contest moduli are primes (`998244353`, `10^9 + 7`).
* **Composite `m`.** **Zero divisors** appear: `2 · 4 ≡ 0 (mod 8)`, although
  neither factor is `0`. Zero divisors have no inverse. The invertible
  residues, the **units** `(Z/m)^×`, number `φ(m)` (Euler's totient), e.g.
  `{1, 3, 5, 7}` mod 8.

Practical consequences:

* `n!` and binomials mod a composite cannot be computed by division. Use
  Pascal's triangle (`C<mint, false, true>`), or CRT over prime powers
  (`Binomial`, [chapter 3](03_binomial.md) §3).
* Mod `2^64` (unsigned overflow) only odd numbers are invertible
  (`mod_inv_u64`, `Binomial_u64`).

---

## 4. Fermat, Euler, Wilson

**Fermat.** For prime `p` and `a ≢ 0`: `a^{p−1} ≡ 1 (mod p)`.
Multiplying all units by `a` permutes them, so
`∏ (a·u) = ∏ u` gives `a^{p−1} = 1`.

Consequences:

* `a^{-1} ≡ a^{p−2}`. Example: `3^{-1} mod 7 = 3^5 = 243 ≡ 5`, and `3 · 5 = 15 ≡ 1`.
* Exponents can be reduced mod `p − 1`: `2^{100} ≡ 2^{100 mod 6} = 2^4 ≡ 2 (mod 7)`.

**Euler.** For `gcd(a, m) = 1`: `a^{φ(m)} ≡ 1 (mod m)`, by the same argument
on the `φ(m)` units. Example: `φ(12) = 4`, and `5^4 = 625 ≡ 1 (mod 12)`.

**Not coprime.** Then the exponent can **not** simply be reduced mod `φ(m)`:
`2^5 = 32 ≡ 8`, but `2^{5 mod 4} = 2 (mod 12)`. The generalised form still
holds for large exponents:

```
a^x ≡ a^{(x mod φ(m)) + φ(m)}   (mod m)    whenever x ≥ log₂ m
```

→ `tetration` ([chapter 4](04_roots_logs.md) §6).

**Wilson.** `(p − 1)! ≡ −1 (mod p)`: every unit pairs with its inverse except
`1` and `−1`, which are their own inverses. Example: `6! = 720 ≡ −1 (mod 7)`.
The analogue for prime powers drives `Binomial`'s sign bookkeeping
([chapter 3](03_binomial.md) §3).

---

## 5. Linear congruences

`ax ≡ b (mod m)` with `g = gcd(a, m)`:

* no solution if `g ∤ b`;
* otherwise divide through, `(a/g) x ≡ b/g (mod m/g)`, and invert `a/g`.
  This gives `g` solutions mod `m`.

```
6x ≡ 4 (mod 10):  g = 2  →  3x ≡ 2 (mod 5)  →  x ≡ 4 (mod 5)  →  x ∈ {4, 9} mod 10
```

→ the first step of `mod_kth_root`, the final step of `mod_log_u64`
([chapter 4](04_roots_logs.md)).

---

## 6. Order and primitive roots

The **order** of a unit `a` is the smallest `k > 0` with `a^k ≡ 1`. Powers of
`a` cycle with period `ord(a)`, and `ord(a) | φ(m)` (Lagrange's theorem).

![power cycles](images/ch00_power_cycles.png)

A **primitive root** is a unit whose order is `φ(m)`: its powers visit
**every** unit. They exist exactly for `m = 1, 2, 4, p^k, 2p^k` (`p` an odd
prime). Mod a prime there are `φ(p − 1)` of them; mod 7 they are `3` and `5`.

With a primitive root `g`, the unit group is a cycle, `(Z/p)^× ≅ Z/(p − 1)`,
via the **discrete logarithm** `g^i ↦ i`. Multiplication becomes addition of
exponents, so:

| problem mod `p` | becomes, in exponents mod `p − 1` |
|---|---|
| `x^k = a` | `k · i ≡ log a` (linear congruence, §5) |
| `a^n = b` | `n · log a ≡ log b` |
| multiplicative convolution | additive (cyclic) convolution |
| `a^{-1}`, `a^e` | `−log a`, `e · log a` (table lookups) |

**Testing a primitive root.** `ord(g)` divides `p − 1`. If it were a proper
divisor, it would divide `(p − 1)/q` for some prime `q | p − 1`. So `g` is
primitive iff `g^{(p−1)/q} ≠ 1` for every prime `q | p − 1`.

**Mod `2^e`** (`e ≥ 3`) there is no primitive root. Instead every odd number
is `±5^j`, where `5` has order `2^{e−2}`.

→ `primitive_root`, `mod_kth_root`, `mod_log`, `mod_log_u64`
([chapter 4](04_roots_logs.md)); `ModFast` ([chapter 2](02_inverses_tables.md) §5);
multiplicative convolutions ([chapter 6](06_multiplicative_convolution.md));
NTT roots of unity ([chapter 1](01_modint.md) §3).

---

## 7. Quadratic residues

Mod an odd prime `p`, the map `x ↦ x²` is 2-to-1 on units, because `x` and
`−x` have the same square. So exactly `(p − 1)/2` units are squares.

![squares](images/ch00_squares.png)

**Euler's criterion.** For a unit `a`, `a^{(p−1)/2} ≡ +1` if `a` is a square
and `≡ −1` if not. With `a = g^i`, `a` is a square iff `i` is even. Example
mod 11: `3^5 ≡ 1` (`3 = 5²`), while `2^5 = 32 ≡ −1`, so 2 is not a square.

Half of all residues are non-squares, so a random one is found in about two
tries. Cipolla's method needs one.

→ `mod_sqrt` ([chapter 4](04_roots_logs.md) §2).

---

## 8. Chinese remainder theorem

For coprime `m, n`, the map `x ↦ (x mod m, x mod n)` is a bijection
`Z/mn → Z/m × Z/n` that respects `+` and `×`:

![CRT grid](images/ch00_crt_grid.png)

So a computation mod a composite `m = ∏ p_i^{e_i}` can be done independently
mod each prime power and recombined. Recombination (Garner): solve for the
digits of `x = a_0 + c_1 m_0 + c_2 m_0 m_1 + …` one modulus at a time.

```
x ≡ 2 (mod 3), x ≡ 3 (mod 5)  →  x = 2 + 3·c,  2 + 3c ≡ 3 (mod 5)  →  c = 2  →  x = 8
```

Non-coprime moduli: `x ≡ a (mod m)`, `x ≡ b (mod n)` is solvable iff
`a ≡ b (mod gcd(m, n))`. The solution is unique mod `lcm(m, n)`.

→ `CRT2..CRT5` ([chapter 2](02_inverses_tables.md) §7), convolution with an
arbitrary modulus (three NTT primes), `Binomial(mod)`.

---

## 9. Binomials mod `p`: Lucas and Kummer

**Lucas.** Write `n = Σ n_i p^i` and `k = Σ k_i p^i` in base `p`. Then

```
binom(n, k) ≡ ∏ binom(n_i, k_i)   (mod p).
```

This is why a factorial table up to `p` suffices for any `n`.

**Kummer.** `v_p(binom(n, k))` equals the number of carries when adding `k`
and `n − k` in base `p`.

Both generalise to prime powers (Granville) → `Binomial`; to `q`-binomials →
`q_Analogue` ([chapter 3](03_binomial.md)).

---

## 10. Computing `a mod m` fast

Hardware division is slow (tens of cycles, more for 128-bit). The modint types
avoid it in three ways.

**Fixed modulus.** When `m` is a compile-time constant (`modint<998244353>`),
the compiler replaces `% m` with a multiplication by a precomputed reciprocal.

**Barrett (runtime `m`).** Precompute `im = ⌈2^k / m⌉` once. Then

```
⌊z / m⌋ ≈ (z · im) >> k
```

For `z < 2^k` the estimate is `⌊z/m⌋` or `⌊z/m⌋ + 1`, never anything else.
Write `im = 2^k/m + e` with `0 ≤ e < 1`; then
`z · im / 2^k = z/m + z·e/2^k`, and the error term is `< 1`. One comparison
fixes it:

![Barrett](images/ch00_barrett.png)

The library uses `k = 64`, with a 128-bit product → `Barrett`,
`Dynamic_Modint` ([chapter 1](01_modint.md) §5).

**Montgomery (odd `m`).** Store `x · 2^W mod m` instead of `x`. Then
reduction is a multiplication and a shift, with no division at all →
`Montgomery_modint`, used by `mod_pow` ([chapter 1](01_modint.md) §6).

**Special primes.** For `p = 2^61 − 1`, `2^61 ≡ 1`, so reducing is "add the
high bits to the low bits" → `modint61` ([chapter 1](01_modint.md) §4).

**Powers.** Binary exponentiation computes `a^n` in `O(log n)`
multiplications: `a^n = (a^{⌊n/2⌋})² · a^{n mod 2}`.
