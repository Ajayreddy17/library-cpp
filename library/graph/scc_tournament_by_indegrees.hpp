#ifndef AJAY_SCC_TOURNAMENT_BY_INDEGREES
#define AJAY_SCC_TOURNAMENT_BY_INDEGREES

#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/scc_tournament_by_indegrees.hpp
pair<int, vector<int>> scc_tournament_by_indegrees(vector<int> indeg) {
  int N = len(indeg);
  auto I = argsort(indeg);
  vector<int> ANS(N);
  long long sm = 0;
  int nxt = 0;
  For(i, N) {
    int v = I[i];
    ANS[v] = nxt;
    // whether I[0:i] is one component
    sm += indeg[v];
    long long TS = sm - (i + 1) * i / 2;
    if (TS == 0) ++nxt;
  }
  return {nxt, ANS};
}
} // namespace mitsuha
#endif // AJAY_SCC_TOURNAMENT_BY_INDEGREES
