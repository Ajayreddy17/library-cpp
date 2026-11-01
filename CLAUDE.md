# CLAUDE.md

Personal competitive-programming library (C++20). Only user: me.

## Git

- **Never commit or push unless I explicitly say so.** Make the change, show it, wait.
- When asked to push and the remote is ahead, it is usually only the
  `[auto-verifier]` commit: `git pull --rebase` then push.

## Header conventions

- Path `library/<area>/<name>.hpp`, included as `#include "library/..."`.
- Include guard `AJAY_<NAME>`; code inside `namespace mitsuha{ ... } // namespace mitsuha`.
- Loops: `For(n)`, `For(i, n)`, `For(i, a, b)`, `For(i, a, b, step)`;
  reverse `Frr(i, n)`, `Frr(i, a, b)`. Loop variables are `long long`.
- Write `v.begin(), v.end()` (no `All` macro); `len(v)` = `(long long)v.size()`.
- Input `INT(...)`, `LL(...)`, `STR(...)`, `VT(type, name, n)`; output `print(...)`.
- Monoids expose `X::unit()` (identity), `X::op`, `X::commute`.
- `Tree<GT> T(G)` — root 0, HLD on by default; use `T.lca` / `T.la` (no `LCA` / `LA`).

## Porting from maspypy (github.com/maspypy/library)

| maspy | ours |
|---|---|
| `FOR(n)`, `FOR(i, n)`, `FOR(i, a, b)`, `FOR1/2/3(...)` | `For(...)` (same args) |
| `FOR_R(n)` | `Frr(i, n)` |
| `FOR_R(i, n)`, `FOR_R(i, a, b)`, `FOR2_R/3_R` | `Frr(...)` (same args) |
| `ll`, `vi`, `pi` | `long long`, `vector<long long>`, `pair<long long, long long>` |
| `vc<T>`, `vvc<T>`, `vvvc<T>` | `vector<T>`, `vector<vector<T>>`, ... |
| `vv(T, a, h, w[, x])` | `vector<vector<T>> a(h, vector<T>(w[, x]))` (same for `vvv`, `vvvv`) |
| `VEC(T, a, n)` / `VV(T, a, h, w)` | `VT(T, a, n)` / `VVT(T, a, h, w)` |
| `u8 u16 u32 u64 u128 i128 f128` | `uint8_t ... uint64_t`, `uint128`, `int128`, `__float128` |
| `pq_min<T>` / `pq_max<T>` | `min_priority_queue<T>` / `max_priority_queue<T>` |
| `all(v)` | `v.begin(), v.end()` |
| `MIN(v)` / `MAX(v)` | `*min_element(v.begin(), v.end())` / `*max_element(...)` |
| `UNIQUE(v)` | `settify(v)` |
| `eb`, `mp`, `mt`, `.fi`, `.se` | `emplace_back`, `make_pair`, `make_tuple`, `.first`, `.second` |
| `floor(a, b)` / `ceil(a, b)` (integer) | `fld(a, b)` / `cld(a, b)` |
| `X::id()` (monoid identity) | `X::unit()` |
| `.LCA(` / `.LA(` | `.lca(` / `.la(` |
| `SHOW(...)` | `debug(...)` |
| member `void debug()` | `void debug_print()` |
| `Tree_AbelianGroup` | `Tree_AbelGroup` |
| `bfs_01` | `bfs01` |
| `fastio::` | `io::` |
| `#include "other/io.hpp"` | `#include "library/io/io2.hpp"` |

Directory map: `alg/monoid/`→`monoid/`, `alg/acted_monoid/`→`acted_monoid/`,
`alg/acted_set/`→`monoid/acted_set/`, `ds/`→`datastructure/` (`segtree`→`segment_tree`,
`fenwicktree`→`fenwick_tree`, `splaytree`→`splay_tree`, `randomized_bst`→`rbst`),
`graph/ds/`→`graph/datastructure/`, `graph/shortest_path/`→`graph/shortestpath/`,
`nt/`→`number/`, `linalg/`→`linear_algebra/`, `other/`→`util/`.
Some file names differ (`functional_graph`→`functional`, `twosat`→`two_sat`,
`unicyclic_graph`→`unicyclicgraph`, `convex_hull`→`convexhull`, `bellman_ford`→`bellmanford`) —
check that the target exists before adding a duplicate.

maspy template helpers → ours (no compat header; include the util you use):

| maspy | ours |
|---|---|
| `popcnt(x)`, `topbit(x)`, `lowbit(x)` | `popcount(x)`, `msb(x)`, `lsb(x)` — `library/util/bit_utils.hpp` |
| `bit_reverse(x)` | `bit_reverse(x)` — `library/util/bit_utils.hpp` |
| `all_bit(s)`, `all_subset(s)` | `all_setbit(s)`, `all_subset(s)` — `library/util/subset_iter.hpp` |
| `POP(que)` (deque / vector / any priority_queue) | `POP(que)` — `library/util/pop.hpp` |
| `binary_search(check, ok, ng)` | `binary_search<LastTrue / FirstTrue / ...>(l, r, f)` — `library/util/binary_serch.hpp` |
| `LB(v, x)` / `UB(v, x)` | `count_lt(v, x)` / `count_leq(v, x)` — `library/util/container_utils.hpp` |
| `SUM<T>(v)` | `accumulate(v.begin(), v.end(), T(0))` |
| `bmod(x, y)` | `rem(x, y)` (template) |
| `cumsum(v)` | `Cumsum<T>` — `library/util/cumsum.hpp` |
| `U32(a)` / `U64(a)` | `unsigned int a; read(a);` |
| `chmax(a, b)` with different types | ours needs the same type: cast `b` |

## Tests

- `test/**/*.test.cpp`, verified by oj-verify on CI (`-std=c++17`, `shell/verify-g++`).
  Library Checker, AOJ and yukicoder (`YUKICODER_TOKEN` repo secret, passed in `verify.yml`).
  Yukicoder tests live in `test/yukicoder/`, converted from maspy's `test/3_yukicoder/`.
- Self-checking stress tests go in `test/mytest/` with `PROBLEM` = Library Checker
  `aplusb`: run randomized asserts in `main`, then solve a+b.
- CI only re-verifies tests whose dependencies changed, so a broken old test can look green.
- Code is written for C++20, but CI verifies with `-std=c++17`, so headers must also build there.
  Check with `-std=c++17` locally too: it catches missing includes that C++20's deferred
  `name<T>(...)` lookup hides.
- Local machine is ARM Mac (g++-14): `template.hpp` has `#pragma GCC target("avx2")`,
  so compile against a pragma-free copy of the template. Don't run binaries with
  `-DLOCAL` from the repo — it freopens `input.txt`/`output.txt`/`error.txt` in the cwd.

## Theory docs (`library/**/theory/*.md`)

Concept and usage only: no intro/meta paragraphs, no header-review, comparison
or "when to use" sections. Images in `theory/images/`.
