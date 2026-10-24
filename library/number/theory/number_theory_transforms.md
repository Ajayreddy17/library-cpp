# Divisor / Multiple Transforms, Möbius, Multiplicative Functions

Study sheet for competitive programming. Sections 0–1 are the core idea,
2–6 are theory, 7–9 are formulas, techniques and pitfalls, 10 is a
recognition checklist, 11 is practice, 12 is a cheat sheet.

Notation: `n | m` means n divides m. `[P]` is 1 if P is true, else 0.
`⌊x⌋` is floor. `N` is the array size.

---

## 0. The one idea to remember

Two "prefix sums" live on the divisibility order instead of on `1..N`:

| Name | Definition | Direction |
|---|---|---|
| Sum over **multiples** | `F(j) = Σ_{j | k, k ≤ N} f(k)` | "suffix sum" over multiples |
| Sum over **divisors**  | `G(n) = Σ_{d | n} g(d)`        | "prefix sum" over divisors |

Both are invertible, and the inverse is the **Möbius transform**:

```
F(j) = Σ_{j|k} f(k)      ⇒   f(j) = Σ_{m≥1, jm≤N} μ(m) · F(jm)
G(n) = Σ_{d|n} g(d)      ⇒   g(n) = Σ_{d|n} μ(n/d) · G(d)
```

If a problem says "operate on all divisors of i" or "all multiples of i",
think: this is a zeta transform, and the answer comes from inverting it.

---

## 1. Fast transforms in code (memorise these)

### 1.1 Sum over multiples and its inverse, O(N log N)

```cpp
// F[j] = sum of f[k] for all multiples k of j (k <= N)
for (int j = N; j >= 1; --j)
    for (int k = 2 * j; k <= N; k += j) F[j] += f[k];   // uses ORIGINAL f

// inverse: recover f from F.  Go from the back, subtract what's already known.
for (int j = N; j >= 1; --j) {
    f[j] = F[j];
    for (int k = 2 * j; k <= N; k += j) f[j] -= f[k];   // f[k] already final
}
```

In the inverse, each `f[j]` depends only on `f` at strictly larger multiples,
so computing from the back to the front is forced.

### 1.2 Sum over divisors and its inverse

```cpp
// G[n] = sum of g[d] for d | n
for (int d = 1; d <= N; ++d)
    for (int n = d; n <= N; n += d) G[n] += g[d];

// inverse, from the front
for (int d = 1; d <= N; ++d)
    for (int n = 2 * d; n <= N; n += d) g[n] -= g[d];       // g starts as a copy of G
```

### 1.3 The O(N log log N) version (prime by prime)

Like subset-sum (SOS) DP, but one "dimension" per prime.

```cpp
// sum over divisors (zeta), in place
for (int p : primes)
    for (int i = 1; i * p <= N; ++i) a[i * p] += a[i];

// its inverse (Möbius), in place
for (int p : primes)
    for (int i = N / p; i >= 1; --i) a[i * p] -= a[i];

// sum over multiples, in place
for (int p : primes)
    for (int i = N / p; i >= 1; --i) a[i] += a[i * p];

// its inverse
for (int p : primes)
    for (int i = 1; i * p <= N; ++i) a[i] -= a[i * p];
```

Loop direction matters. In the divisor-zeta the update `a[i*p] += a[i]` must
see `a[i]` already updated for this prime, so go upward in `i`. The multiples
version reads from larger indices, so go downward.

---

## 2. Möbius function μ

```
μ(1) = 1
μ(n) = 0               if some prime squared divides n
μ(n) = (-1)^k          if n is a product of k distinct primes
```

Key identity (the reason inversion works):

```
Σ_{d | n} μ(d) = [n = 1]
```

Proof: for n > 1 with k distinct primes, only squarefree divisors count and
`Σ_{i} C(k,i)(-1)^i = (1-1)^k = 0`.

Use it as an indicator: `[gcd(a,b) = 1] = Σ_{d | gcd(a,b)} μ(d)`.

### Linear sieve for μ, φ, smallest prime factor

```cpp
vector<int> lp(N + 1), primes, mu(N + 1), phi(N + 1);
mu[1] = phi[1] = 1;
for (int i = 2; i <= N; ++i) {
    if (!lp[i]) { lp[i] = i; primes.push_back(i); mu[i] = -1; phi[i] = i - 1; }
    for (int p : primes) {
        if (p > lp[i] || (long long)i * p > N) break;
        lp[i * p] = p;
        if (i % p == 0) { mu[i * p] = 0;      phi[i * p] = phi[i] * p;       }
        else            { mu[i * p] = -mu[i]; phi[i * p] = phi[i] * (p - 1); }
    }
}
```

---

## 3. Multiplicative functions

`f` is **multiplicative** if `f(1) = 1` and `f(ab) = f(a) f(b)` whenever
`gcd(a,b) = 1`. It is **completely multiplicative** if that holds for all a, b.

So a multiplicative function is determined by its values on prime powers:
`f(n) = Π f(p^e)` over `n = Π p^e`.

| Function | Meaning | Value at p^e | Type |
|---|---|---|---|
| `ε(n) = [n=1]` | identity for convolution | `0` (e ≥ 1) | completely mult. |
| `1(n) = 1` | constant | `1` | completely mult. |
| `id(n) = n` | identity map | `p^e` | completely mult. |
| `id_k(n) = n^k` | power | `p^{ek}` | completely mult. |
| `μ(n)` | Möbius | `-1, 0, 0, …` | multiplicative |
| `φ(n)` | Euler totient | `p^e - p^{e-1}` | multiplicative |
| `τ(n)` or `d(n)` | number of divisors | `e + 1` | multiplicative |
| `σ(n)` | sum of divisors | `(p^{e+1}-1)/(p-1)` | multiplicative |

Useful closed forms:

```
φ(n) = n · Π_{p | n} (1 - 1/p)
τ(n) = Π (e_i + 1)
σ(n) = Π (p_i^{e_i+1} - 1) / (p_i - 1)
```

To compute a multiplicative function for all n ≤ N, use the linear sieve:
when `i % p == 0`, you need `f(i·p)` from the exponent of p in i. Keep an
auxiliary array (the p-power part of i, or the exponent) to update it in O(1).

---

## 4. Dirichlet convolution

```
(f * g)(n) = Σ_{d | n} f(d) · g(n/d)
```

Properties:

- Commutative, associative, distributes over `+`.
- Identity is `ε`.
- If `f` and `g` are multiplicative, so is `f * g`.
- If `f(1) ≠ 0`, `f` has a unique inverse `f⁻¹` under `*`.
- `μ` is the inverse of `1`: **`μ * 1 = ε`** (that is the Section 2 identity).

### Möbius inversion in this language

```
G = g * 1     ⇔     g = G * μ
```

That is the divisor-sum form from Section 0.

### Identities worth knowing cold

```
1 * 1   = τ            (number of divisors)
1 * id  = σ            (sum of divisors)
μ * 1   = ε
φ * 1   = id           i.e.  Σ_{d | n} φ(d) = n
μ * id  = φ            (Möbius inversion of the line above)
μ * τ   = 1
μ * σ   = id
```

---

## 5. The Mertens function

```
M(n) = Σ_{k ≤ n} μ(k)
```

Two facts:

1. `M(1) = 1`, but `M(n)` is mostly zero or negative for small n. The first
   `n > 1` with `M(n) = 1` is **94**. Past that, `|M(n)|` stays small (it is
   roughly √n in size in practice), e.g. over `n ≤ 2·10^5` it ranges about
   from −134 to +132.
2. **`Σ_{j=1}^{n} M(⌊n/j⌋) = 1`.**

Proof of 2: `Σ_j M(⌊n/j⌋) = Σ_j Σ_{m ≤ n/j} μ(m) = Σ_{k ≤ n} Σ_{m | k} μ(m) =
Σ_{k ≤ n} [k = 1] = 1`.

Computing prefix sums of multiplicative functions faster than O(n) uses the
convolution identity `(f * g)` prefix sum, the hyperbola method, and
`⌊n/i⌋` blocks. For `M(n)` the standard recursion is
`M(n) = 1 − Σ_{i=2}^{n} M(⌊n/i⌋)` with memoisation, giving `O(n^{2/3})` after
sieving up to `n^{2/3}`.

---

## 6. Floor-division blocks (needed for almost every Möbius sum)

`⌊n/i⌋` takes only about `2√n` distinct values. Iterate by block:

```cpp
for (long long l = 1, r; l <= n; l = r + 1) {
    long long q = n / l;
    r = n / q;                  // largest r with n/r == q
    // all i in [l, r] share quotient q
    ans += (S[r] - S[l - 1]) * q;
}
```

---

## 7. Common formulas built from these tools

**Coprime pairs.**

```
#{(a,b) : 1 ≤ a,b ≤ n, gcd(a,b)=1} = Σ_{d=1}^{n} μ(d) · ⌊n/d⌋²
```

**Count by exact gcd** (the CSES "Counting Coprime Pairs" idea). Let
`cnt[d]` = how many array elements are divisible by d (the multiples-sum).
Pairs with `d | gcd` number `C(cnt[d], 2)`; with exact gcd `g` use inversion:

```
exact[g] = Σ_{m ≥ 1} μ(m) · pairs_div[g·m]        (this is Section 1.1 inverse)
```

**Sum of gcds.**

```
Σ_{i=1}^{n} gcd(i, n) = Σ_{d | n} d · φ(n/d)
Σ_{i,j ≤ n} gcd(i,j) = Σ_{d=1}^{n} φ(d) · ⌊n/d⌋²
```

(use `gcd(a,b) = Σ_{d | gcd(a,b)} φ(d)`.)

**LCM sums.** `lcm(a,b) = ab / gcd(a,b)`. Pull the gcd out as `g`, set
`a = g a'`, `b = g b'` with `gcd(a', b') = 1`, then replace the coprime
condition with `Σ_{e | gcd(a',b')} μ(e)`.

**GCD convolution and LCM convolution.** Given `a` and `b` over `1..N`:

```
c[g] = Σ_{gcd(i,j) = g} a[i] b[j]
```

Take the multiples-zeta of `a` and `b`, multiply pointwise, then take the
inverse. For lcm convolution use the divisors-zeta instead.

**Subset analogues** (same structure, different lattice):

| Lattice | Zeta | Inverse |
|---|---|---|
| Subsets of a set (SOS DP) | `F[S] = Σ_{T ⊆ S} f[T]` | subtract |
| Supersets | `F[S] = Σ_{T ⊇ S} f[T]` | subtract |
| Divisors | Section 1.3 | Section 1.3 |
| Multiples | Section 1.3 | Section 1.3 |
| XOR | Walsh–Hadamard | divide by `2^k` after |

Same trick everywhere: transform, combine pointwise, invert.

---

## 8. Euler's totient, short

```
φ(n) = #{1 ≤ k ≤ n : gcd(k,n)=1}
Σ_{d | n} φ(d) = n
φ(n) = n · Π (1 - 1/p),  p | n
φ(p^e) = p^e - p^{e-1}
a^φ(n) ≡ 1 (mod n)  if gcd(a,n)=1     (Euler)
```

Sum of `φ` up to `n` uses the Section 5 style recursion:
`Φ(n) = n(n+1)/2 − Σ_{i=2}^{n} Φ(⌊n/i⌋)`.

---

## 9. Pitfalls

- **Floor division with negatives.** C++ `/` truncates toward zero. For a floor
  with positive divisor use `q = x / y; if (x % y != 0 && x < 0) q--;`
  and `ceil(a/b) = -floor(-a/b)`.
- **Overflow.** Möbius sums combine up to N terms of size ~1e9: use `long long`
  at least, `__int128` when multiplying by a coefficient and a bound.
- **Index base.** Divisor transforms are 1-indexed. `a[0]` must not be part
  of the loops.
- **Order of loops.** In place transforms are only correct in the direction
  shown in Section 1.
- **Only the first N entries matter.** `k ≤ N` is part of the definition of
  "multiples"; do not read past it.

---

## 10. Recognition checklist

Reach for Möbius / divisor transforms when you see:

1. An operation touching **all divisors** or **all multiples** of an index.
2. A count over pairs/tuples with `gcd = 1`, `gcd = g`, or `lcm` conditions.
3. A sum like `Σ f(gcd(i,j))` or `Σ f(i·j)`.
4. A condition "d divides x" that you want to turn into "exactly".
5. A recurrence where answer for `j` depends on the answers of `2j, 3j, …`
   (process in decreasing order).
6. Coprime counting up to `n ≤ 1e12` → blocks + Mertens/Phi recursion.

Decision shortcut:

```
need "exactly g"            → count "g divides" for each g, then Möbius-invert
need "gcd = 1"              → Σ_d μ(d) · (count with d | everything)
sum over divisors or multiples of everything → zeta transform (Section 1.3)
f(n) = Σ_{d|n} g(d)         → g = μ * f
multiplicative, want table  → linear sieve
multiplicative, want prefix sum for huge n → convolution trick + blocks
```

---

## 11. Practice (verify each statement on the judge)

- AtCoder ABC162 E, "Sum of gcd of Tuples (Hard)": exact-gcd counting via
  `x^N` and inversion.
- CSES 2417, "Counting Coprime Pairs": Section 7.
- Codeforces 1043F, "Make It One": minimum subset with gcd 1 via inclusion–
  exclusion over multiples.
- Codeforces 1139D, "Steps to One": Möbius on an expectation DP.
- For the harder tier: sums of `φ`, `μ`, `σ` up to 1e10–1e12 using
  Dirichlet-hyperbola / min_25 / Du's sieve.

---

## 12. One-page cheat sheet

```
Zeta over multiples   F(j) = Σ_{j|k} f(k)
Inverse               f(j) = F(j) − Σ_{k=2j,3j,…} f(k)  =  Σ_m μ(m) F(jm)

Zeta over divisors    G(n) = Σ_{d|n} g(d)
Inverse               g(n) = Σ_{d|n} μ(n/d) G(d)

μ:   Σ_{d|n} μ(d) = [n=1]
φ:   Σ_{d|n} φ(d) = n          φ = μ * id
τ = 1*1      σ = 1*id          Convolution identity = ε = [n=1]

Mertens:  M(n) = Σ_{k≤n} μ(k)     Σ_{j=1}^{n} M(⌊n/j⌋) = 1     M(94) = 1

Coprime pairs ≤ n:   Σ_d μ(d) ⌊n/d⌋²
Σ_{i,j≤n} gcd(i,j) = Σ_d φ(d) ⌊n/d⌋²

floor(x/y), y>0:   q = x/y; if (x%y && x<0) q--;      ceil(a/b) = −floor(−a/b)
```

---

## 13. Mapping theory to `library/number`

Everything in Sections 1–8 already exists in `library/number`. This section
links each piece of theory to the function that implements it.

### 13.1 The four transforms: `zeta.hpp`

All take `vector<T>& A`, **1-indexed** (`A[0]` must be 0, asserted), and
work in place in `O(N log log N)` using the prime-by-prime loops of
Section 1.3.

| Library function | Theory (Section 0 / 1) | Formula | Loop |
|---|---|---|---|
| `divisor_zeta(A)`     | sum over divisors   | `G(n) = Σ_{d│n} A(d)`     | `A[p·x] += A[x]`, x ascending |
| `divisor_mobius(A)`   | inverse of the above | `g(n) = Σ_{d│n} μ(n/d) A(d)` | `A[p·x] -= A[x]`, x descending |
| `multiplier_zeta(A)`  | sum over multiples  | `F(j) = Σ_{j│k} A(k)`      | `A[x] += A[p·x]`, x descending |
| `multiplier_mobius(A)`| inverse of the above | `f(j) = Σ_m μ(m) A(jm)`    | `A[x] -= A[p·x]`, x ascending |

"multiplier" here means "multiple". (Other libraries call the last one
`multiple_mobius`; it is the same function.)

Rules for the loop direction (the reason the library uses `For` for some and
`Frr` for others):

- A zeta must use each value **after** the same prime has already updated it
  (divisor zeta) or **before** it has (multiples zeta), so the two zetas run in
  opposite directions.
- Each Möbius is the exact reverse of its zeta, so it runs the opposite
  direction of the matching zeta and subtracts.

Quick sanity picture: the four are two pairs of inverse operations.

```
divisor_zeta     <-->  divisor_mobius        (g  <->  G = g * 1)
multiplier_zeta  <-->  multiplier_mobius     (f  <->  F = Σ over multiples)
```

### 13.2 Functions built from them

| Library | Theory used | How |
|---|---|---|
| `euler_phi_table<T>(n)` in `euler_phi.hpp` | `φ * 1 = id`, so `φ = μ * id` (Section 3, 4) | fill `A[i] = i`, then `divisor_mobius(A)` |
| `euler_phi(n)` in `euler_phi.hpp` | `φ(n) = n·Π(1 − 1/p)` (Section 8) | factor `n`, then `n -= n/p` for each distinct prime `p` |
| `gcd_convolution(A, B)` in `gcd_convolution.hpp` | GCD convolution (Section 7) | `multiplier_zeta` both, multiply pointwise, `multiplier_mobius` |

`gcd_convolution` computes `c[g] = Σ_{gcd(i,j)=g} A[i]·B[j]`. It works
because the multiples-sum of `c` is the pointwise product of the multiples-sums.
Pairs with gcd divisible by `d` are exactly the pairs of multiples of `d`, so

```
(Σ_{d│i} A[i]) · (Σ_{d│j} B[j])  =  Σ_{d│gcd(i,j)} A[i]·B[j]
```

**LCM convolution** is not in the library, but it is the same recipe with the
divisor transforms (pairs with `lcm(i,j) │ n` are pairs with both `i│n`, `j│n`):

```cpp
vector<T> lcm_convolution(vector<T> A, vector<T> B) {
    divisor_zeta(A);
    divisor_zeta(B);
    For(i, len(A)) A[i] *= B[i];
    divisor_mobius(A);
    return A;                       // c[l] = Σ_{lcm(i,j)=l} A[i]·B[j]
}
```

(Entries beyond `N` are dropped by both transforms, so `c[l]` for `l ≤ N`
only sums pairs whose lcm is at most `N`, which is what you want.)

### 13.3 Recipes by what you need

```
count of x with d | x        →  cnt[d] via multiplier_zeta on a frequency array
count with gcd exactly g     →  multiplier_mobius on "count with g | everything"
count with gcd = 1           →  entry [1] of that result (or Σ_d μ(d)·cnt[d])
Σ over divisors of f         →  divisor_zeta
recover g from Σ_{d|n} g(d)  →  divisor_mobius
φ table, μ table             →  euler_phi_table, or set_mobius (below)
```

Exact-gcd counting with the library:

```cpp
vector<long long> cnt(N + 1);
for (int x : values) cnt[x]++;        // frequency array, cnt[0] = 0
multiplier_zeta(cnt);                 // cnt[d] = #values divisible by d
// pairs with d | gcd:
vector<long long> pairs(N + 1);
For(d, 1, N + 1) pairs[d] = cnt[d] * (cnt[d] - 1) / 2;
multiplier_mobius(pairs);             // pairs[g] = #pairs with gcd exactly g
```

### 13.4 Transforms on the divisors of one huge number: `array_on_divisors.hpp`

When `n` is up to `1e18` you cannot index `1..n`, but you only need the
divisors of `n` (at most about 100 000). `Array_On_Divisors<T>` stores an
array indexed by the divisors of `n`, using the factorisation `n = Π p_i^{e_i}`.

Layout: divisors are listed in **mixed radix** order. With digits
`(k_1, k_2, …)` for the exponent of each prime, index
`= k_1 + (e_1+1)·k_2 + (e_1+1)(e_2+1)·k_3 + …`. Moving by `k` positions
(where `k = Π_{earlier primes}(e+1)`) inside a block of size `mod = k·(e+1)`
means "multiply by this prime once". That is exactly the prime-by-prime loop
of Section 1.3, with the same four directions:

| Method | Same as | Direction inside the block |
|---|---|---|
| `divisor_zeta()`    | `divisor_zeta`    | `dat[j+k] += dat[j]`, j ascending |
| `divisor_mobius()`  | `divisor_mobius`  | `dat[j+k] -= dat[j]`, j descending |
| `multiplier_zeta()` | `multiplier_zeta` | `dat[j] += dat[j+k]`, j descending |
| `multiplier_mobius()` | `multiplier_mobius` | `dat[j] -= dat[j+k]`, j ascending |

Also provided:

- Versions taking a lambda (`ADD`/`SUB`), for non-numeric `T` (min, max, mod
  arithmetic, matrices): the "zeta" can be any associative combine, the
  "Möbius" needs a real inverse (`SUB`).
- `set_multiplicative(f)`: pass `f(p, k) = f(p^k)`, and the array is filled
  with `f(d)` for every divisor `d` using multiplicativity (Section 3):
  `f(d) = Π f(p^e)`.
- `set_mobius()`: `μ(p) = −1`, `μ(p^k) = 0` for `k ≥ 2`, built via
  `set_multiplicative`.
- `set_euler_phi()`: `A[d] = d` then `divisor_mobius()`, the same
  `φ = μ * id` as `euler_phi_table`.
- `operator[](d)`: access by the divisor value (hash lookup).
- `set(f)` and `enumerate(f)` to fill from / iterate over `(d, value)`.

Typical use: answer a question that sums over `d | n` (number of divisors with
some property, `Σ_{d│n} f(d)·g(n/d)`, a divisor-lattice DP) in
`O(τ(n)·ω(n))` instead of `O(τ(n)²)`.

### 13.5 The supporting pieces

| Library | Theory | Notes |
|---|---|---|
| `primetable.hpp` : `primetable(LIM)` | primes up to `LIM` | segmented sieve, result cached in a `static`, used by every transform |
| `lpf_table.hpp` : `lpf_table(LIM)` | least prime factor table | needed for fast factorisation of many numbers `≤ LIM` |
| `factor.hpp` : `factor`, `factor_by_lpf` | prime factorisation | `factor` uses Pollard rho (any `n ≤ 1e18`); `factor_by_lpf` uses the LPF table, `O(log n)` per number |
| `divisors.hpp` : `divisors`, `divisors_by_pf`, `divisors_by_lpf` | enumerate divisors | unsorted; from `factor` output |

Use `lpf_table` + `factor_by_lpf` when you need to factor many small numbers;
use `factor` for a few large ones.

### 13.6 What the library does not have (write when needed)

- Linear sieve for `μ` and `φ` of every `n ≤ N` as one table (Section 2). The
  library gets `φ` from `divisor_mobius` instead, which is fine for `n ≤ 1e7`.
  A `μ` table can also come from the transform: `μ * 1 = ε`, so set
  `A[1] = 1` and all other entries 0 (that is `ε`), then call
  `divisor_mobius(A)`; the result is `μ`.
- Dirichlet convolution of two arbitrary functions `f * g` in
  `O(N log N)`: `for d: for k: h[dk] += f[d]·g[k]`.
- Prefix sums of `μ` / `φ` for huge `n` (Mertens, Section 5).
- LCM convolution (snippet in 13.2).

### 13.7 Known issue in the library headers

`euler_phi.hpp` opens with `#ifndef AJAY_EULER_PHI` but then does
`#define AJAY_EULAR_PHI` (spelling differs). The guard never becomes active,
so including the file twice (directly or through two other headers) will
redefine `euler_phi` and fail to compile. Fix the second line to
`#define AJAY_EULER_PHI`.
