#ifndef AJAY_BRACKET_GRAPH
#define AJAY_BRACKET_GRAPH

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bracket_graph.hpp

// {G, LR}.
// turn a regular bracket sequence into a graph; also builds LR, the range of each vertex
// a root node for the whole sequence is added: N/2+1 vertices
// ()() → [0,4), [0,2), [2,4)
// for other inputs, wrap the sequence in () first
pair<Graph<int, 1>, vector<pair<int, int>>> bracket_graph(string& S) {
  int N = len(S) / 2;
  Graph<int, 1> G(N + 1);
  vector<pair<int, int>> LR(N + 1);
  int now = 0;
  int nxt = 1;
  LR[0] = {0, len(S)};
  vector<int> par(N + 1, -1);
  For(i, len(S)) {
    assert(S[i] == '(' || S[i] == ')');
    if (S[i] == '(') {
      G.add(now, nxt);
      par[nxt] = now;
      LR[nxt].first = i;
      now = nxt;
      nxt++;
    }
    if (S[i] == ')') {
      LR[now].second = i + 1;
      now = par[now];
    }
  }
  assert(now == 0);
  G.build();
  return {G, LR};
}
} // namespace mitsuha
#endif // AJAY_BRACKET_GRAPH
