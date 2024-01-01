#ifndef AJAY_WALK_ON_TREE
#define AJAY_WALK_ON_TREE

#include "library/graph/tree.hpp"

namespace mitsuha{
template<typename TREE>
vector<int> walk_on_tree(TREE &tree){
    vector<int> walk = {tree.V[0]};
    auto &V = tree.V;
    for(int x = 1; x < tree.N; ++x){
        while (walk.back() != tree.parent[V[x]]) 
            walk.push_back(tree.parent[walk.back()]);
        walk.push_back(V[x]);
    }
    return walk;
}
} // namespace mitsuha
#endif // AJAY_WALK_ON_TREE