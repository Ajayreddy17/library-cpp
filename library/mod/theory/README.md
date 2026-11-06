# Modular Arithmetic

## Chapters

| # | Chapter | Headers |
|---|---|---|
| 0 | [Modular arithmetic](00_modular_arithmetic.md): residues, inverses and Bézout, prime vs composite, Fermat / Euler / Wilson, linear congruences, order and primitive roots, quadratic residues, CRT, Lucas / Kummer, fast reduction | — |
| 1 | [Modular integers](01_modint.md): representations, inverse, NTT info, `2^61 − 1`, Barrett, Montgomery, arithmetic mod `2^64` | [`modint`](../modint.hpp), [`modint_64bit`](../modint_64bit.hpp), [`modint61`](../modint61.hpp), [`dynamic_modint`](../dynamic_modint.hpp), [`dynamic_modint_64`](../dynamic_modint_64.hpp), [`barrett`](../barrett.hpp), [`montgomery_modint`](../montgomery_modint.hpp), [`mod_pow`](../mod_pow.hpp), [`mod_pow_u64`](../mod_pow_u64.hpp), [`mod_inv_u64`](../mod_inv_u64.hpp) |
| 2 | [Inverses, power tables and CRT](02_inverses_tables.md): batch and linear-time inverses, `O(1)` inverse, log tables, Garner | [`mod_inv`](../mod_inv.hpp), [`all_inverse`](../all_inverse.hpp), [`O1_inverse`](../O1_inverse.hpp), [`modfast`](../modfast.hpp), [`powertable`](../powertable.hpp), [`crt3`](../crt3.hpp) |
| 3 | [Factorials and binomial coefficients](03_binomial.md): factorial tables, `n!` mod a large prime, any modulus (Granville), mod `2^64`, prefix sums of a row, q-analogues | [`modint_common`](../modint_common.hpp), [`factorial998`](../factorial998.hpp), [`factorial107`](../factorial107.hpp), [`binomial`](../binomial.hpp), [`binomial_u64`](../binomial_u64.hpp), [`prefix_sum_of_binom`](../prefix_sum_of_binom.hpp), [`q_analogue`](../q_analogue.hpp) |
| 4 | [Primitive roots, roots and logarithms](04_roots_logs.md): primitive roots, Cipolla, Adleman–Manders–Miller, baby-step giant-step, logs mod `2^64`, tetration | [`primitive_root`](../primitive_root.hpp), [`mod_sqrt`](../mod_sqrt.hpp), [`mod_kth_root`](../mod_kth_root.hpp), [`mod_log`](../mod_log.hpp), [`mod_log_u64`](../mod_log_u64.hpp), [`tetration`](../tetration.hpp) |
| 5 | [Floor sums and linear functions mod `m`](05_floor_sums.md): floor sums, residue counts, floor monoid product, polynomial floor sums, prefix minima of `(ax + b) mod m` | [`floor_sum_of_linear`](../floor_sum_of_linear.hpp), [`mod_sum_of_linear`](../mod_sum_of_linear.hpp), [`range_freq_of_linear`](../range_freq_of_linear.hpp), [`range_freq_mod`](../range_freq_mod.hpp), [`floor_monoid_product`](../floor_monoid_product.hpp), [`floor_sum_of_linear_polynomial`](../floor_sum_of_linear_polynomial.hpp), [`floor_sum_of_linear_polynomial_pq`](../floor_sum_of_linear_polynomial_pq.hpp), [`min_of_linear_segments`](../min_of_linear_segments.hpp), [`min_of_linear`](../min_of_linear.hpp), [`max_of_linear_segments`](../max_of_linear_segments.hpp), [`first_mod_range_of_linear`](../first_mod_range_of_linear.hpp) |
| 6 | [Multiplicative convolution and modular subset sum](06_multiplicative_convolution.md) | [`multiplicative_convolution_mod_p`](../multiplicative_convolution_mod_p.hpp), [`multiplicative_convolution_mod_2n`](../multiplicative_convolution_mod_2n.hpp), [`modular_subset_sum`](../modular_subset_sum.hpp) |

## Notation

`m` modulus, `p` prime, `g` primitive root, `φ` Euler's totient, `v_p(n)`
exponent of `p` in `n`, `⌊·⌋` floor, `a^{-1}` inverse mod the current modulus.
