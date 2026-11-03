# Network Flow

## Chapters

| # | Chapter | Headers |
|---|---|---|
| 1 | [Flow fundamentals](01_flow_fundamentals.md): networks, cuts, max-flow min-cut, residual graph, augmenting paths, integrality, flow decomposition | — |
| 2 | [Dinic and `MaxFlow`](02_dinic_maxflow.md): level graph, blocking flow, complexity, CSR implementation, capacity changes, incremental flow | [`maxflow`](../maxflow.hpp), [`incremental_maxflow`](../incremental_maxflow.hpp) |
| 3 | [Min cut](03_min_cut.md): extracting cuts, which edges lie in min cuts, Menger, Gomory–Hu tree | [`maxflow`](../maxflow.hpp), [`gomory_hu_tree`](../gomory_hu_tree.hpp) |
| 4 | [Lower bounds and circulations](04_lower_bounds.md): feasible circulation, s–t flow with lower bounds | [`maxflow_with_lowerbound`](../maxflow_with_lowerbound.hpp) |
| 5 | [Modelling with max flow](05_flow_modelling.md): standard reductions, Dilworth, path cover, antichains | [`dag_path_cover`](../../graph/matching/dag_path_cover.hpp), [`maximum_antichain`](../../graph/matching/maximum_antichain.hpp), [`maximum_weighted_antichain`](../../graph/matching/maximum_weighted_antichain.hpp) |
| 6 | [Bipartite matching](06_bipartite_matching.md): Berge, König, Hall, Hopcroft–Karp, covers, DM decomposition, dense and rank-maximal matching | [`bipartite`](../bipartite.hpp), [`bipartite_matching`](../bipartite_matching.hpp), [`bipartite_matching_dense`](../bipartite_matching_dense.hpp), [`rank_maximal_bipartite_matching`](../rank_maximal_bipartite_matching.hpp), [`bipartite_edge_coloring`](../../graph/coloring/bipartite_edge_coloring.hpp) |
| 7 | [Min cost flow](07_min_cost_flow.md): optimality conditions, potentials, successive shortest paths, cost slope | [`mincostflow`](../mincostflow.hpp) |
| 8 | [b-flow and duality](08_b_flow_duality.md): supplies and demands, cost scaling, LP duality, potential problems | [`bflow`](../bflow.hpp), [`dual_mcf`](../dual_mcf.hpp), [`longest_shortest_path`](../longest_shortest_path.hpp) |
| 9 | [Assignment, transportation and special structures](09_assignment_transportation.md) | [`hungarian`](../hungarian.hpp), [`unbalanced_transportation`](../unbalanced_transportation.hpp), [`convex_min_cost_flow`](../convex_min_cost_flow.hpp), [`cyclic_b_flow`](../cyclic_b_flow.hpp), [`min_cost_matching_on_line`](../min_cost_matching_on_line.hpp), [`dynamic_mcf_on_line`](../dynamic_mcf_on_line.hpp) |
| 10 | [Cut optimization](10_cut_optimization.md): submodular energies, project selection, k-valued labels | [`binary_optimization`](../binary_optimization.hpp), [`k_ary_optimization`](../k_ary_optimization.hpp) |

## Which header

| Problem | Use | Chapter |
|---|---|---|
| max flow / min cut, static graph | `MaxFlow<Cap>` | 2, 3 |
| add edges between flow queries | `Incremental_MaxFlow<Cap>` | 2 |
| change one capacity, keep the flow | `MaxFlow::change_capacity` | 2 |
| all-pairs min cut, undirected | `gomory_hu_tree` | 3 |
| edge flow bounded below | `MaxFlow_With_LowerBound<Cap>` | 4 |
| min chain cover / max antichain of a DAG or poset | `dag_path_cover`, `maximum_antichain`, `maximum_weighted_antichain` | 5 |
| max bipartite matching, vertex cover, independent set, edge cover | `BipartiteMatching<GT>` | 6 |
| which edges / vertices are in every / some max matching | `DM_decomposition()` | 6 |
| dense bipartite graph given as bitsets | `Bipartite_Matching_Dense<BS>` | 6 |
| matching that maximizes count of best-ranked edges first | `Rank_Maximal_Bipartite_Matching` | 6 |
| s–t min cost flow, cost for every flow amount | `Min_Cost_Flow<Cap, Cost, DAG>` | 7 |
| supplies / demands, lower bounds, negative costs | `MinCostFlow<Flow, Cost>` (`bflow.hpp`) | 8 |
| minimize `Σ b_v p_v + Σ c·max(0, p_v − p_u − w)` over integers `p` | `Dual_MCF` | 8 |
| lengthen edges so that `dist(s, t) ≥ L` at min cost | `Longest_Shortest_Path` | 8 |
| `N × M` cost matrix assignment | `hungarian<T, MINIMIZE>` | 9 |
| few sources (`K`), many unit sinks (`N`) | `Unbalanced_Transportation_Problem` | 9 |
| convex cost per edge | `Convex_Min_Cost_Flow` | 9 |
| flow on a cycle / on a line | `cyclic_b_flow`, `min_cost_matching_on_line`, `Dynamic_MCF_on_Line` | 9 |
| choose 0/1 per item with unary + pairwise (+ triple) costs | `Binary_Optimization<T, MINIMIZE>` | 10 |
| choose a value in `{0..k−1}` per item, Monge pairwise costs | `K_ary_Optimization<T, MINIMIZE>` | 10 |

## Notation

`N` vertices, `M` edges, `s` source, `t` sink, `c(e)` capacity, `f(e)` flow,
`w(e)` cost, `|f|` flow value, `F` max flow value, `U` max capacity, `C` max |cost|.
