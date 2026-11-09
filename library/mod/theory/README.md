# Modular Arithmetic

## Chapters

| # | Chapter | Headers |
|---|---|---|
| 0 | [Modular arithmetic](00_modular_arithmetic.md): residues, inverses and Bézout, prime vs composite, Fermat / Euler / Wilson, linear congruences, order and primitive roots (powers cover all units, orders of powers, roots of unity and NTT primes, discrete roots, power sums), quadratic residues, CRT and Carmichael, Legendre / Lucas / Kummer, fractions mod `p`, fast reduction, common pitfalls | — |
| 1 | [Modular integers](01_modint.md): representations, inverse, NTT info, `2^61 − 1`, Barrett, Montgomery, arithmetic mod `2^64` | [`modint`](../modint.hpp)<br>[`modint_64bit`](../modint_64bit.hpp)<br>[`modint61`](../modint61.hpp)<br>[`dynamic_modint`](../dynamic_modint.hpp)<br>[`dynamic_modint_64`](../dynamic_modint_64.hpp)<br>[`barrett`](../barrett.hpp)<br>[`montgomery_modint`](../montgomery_modint.hpp)<br>[`mod_pow`](../mod_pow.hpp)<br>[`mod_pow_u64`](../mod_pow_u64.hpp)<br>[`mod_inv_u64`](../mod_inv_u64.hpp) |
| 2 | [Inverses, power tables and CRT](02_inverses_tables.md): batch and linear-time inverses, `O(1)` inverse, log tables, Garner | [`mod_inv`](../mod_inv.hpp)<br>[`all_inverse`](../all_inverse.hpp)<br>[`O1_inverse`](../O1_inverse.hpp)<br>[`modfast`](../modfast.hpp)<br>[`powertable`](../powertable.hpp)<br>[`crt3`](../crt3.hpp) |
| 3 | [Factorials and binomial coefficients](03_binomial.md): factorial tables, `n!` mod a large prime, any modulus (Granville), mod `2^64`, prefix sums of a row, q-analogues | [`modint_common`](../modint_common.hpp)<br>[`factorial998`](../factorial998.hpp)<br>[`factorial107`](../factorial107.hpp)<br>[`binomial`](../binomial.hpp)<br>[`binomial_u64`](../binomial_u64.hpp)<br>[`prefix_sum_of_binom`](../prefix_sum_of_binom.hpp)<br>[`q_analogue`](../q_analogue.hpp) |
| 4 | [Primitive roots, roots and logarithms](04_roots_logs.md): primitive roots, Cipolla, Adleman–Manders–Miller, baby-step giant-step, logs mod `2^64`, tetration | [`primitive_root`](../primitive_root.hpp)<br>[`mod_sqrt`](../mod_sqrt.hpp)<br>[`mod_kth_root`](../mod_kth_root.hpp)<br>[`mod_log`](../mod_log.hpp)<br>[`mod_log_u64`](../mod_log_u64.hpp)<br>[`tetration`](../tetration.hpp) |
| 5 | [Floor sums and linear functions mod `m`](05_floor_sums.md): floor sums, residue counts, floor monoid product, polynomial floor sums, prefix minima of `(ax + b) mod m` | [`floor_sum_of_linear`](../floor_sum_of_linear.hpp)<br>[`mod_sum_of_linear`](../mod_sum_of_linear.hpp)<br>[`range_freq_of_linear`](../range_freq_of_linear.hpp)<br>[`range_freq_mod`](../range_freq_mod.hpp)<br>[`floor_monoid_product`](../floor_monoid_product.hpp)<br>[`floor_sum_of_linear_polynomial`](../floor_sum_of_linear_polynomial.hpp)<br>[`floor_sum_of_linear_polynomial_pq`](../floor_sum_of_linear_polynomial_pq.hpp)<br>[`min_of_linear_segments`](../min_of_linear_segments.hpp)<br>[`min_of_linear`](../min_of_linear.hpp)<br>[`max_of_linear_segments`](../max_of_linear_segments.hpp)<br>[`first_mod_range_of_linear`](../first_mod_range_of_linear.hpp) |
| 6 | [Multiplicative convolution and modular subset sum](06_multiplicative_convolution.md) | [`multiplicative_convolution_mod_p`](../multiplicative_convolution_mod_p.hpp)<br>[`multiplicative_convolution_mod_2n`](../multiplicative_convolution_mod_2n.hpp)<br>[`modular_subset_sum`](../modular_subset_sum.hpp) |

## Notation

`m` modulus, `p` prime, `g` primitive root, `φ` Euler's totient, `v_p(n)`
exponent of `p` in `n`, `⌊·⌋` floor, `a^{-1}` inverse mod the current modulus.
