#ifndef AJAY_DELETION_OFFLINE
#define AJAY_DELETION_OFFLINE

namespace mitsuha{
// For a commutative data structure supporting insertion,
// when a update couldn't be reversed, but DS can be rollbacked
// support deletion offline.
template<typename T>
struct Deletion_Offline{
	int qn; // Elements should lie in range [0, n).
	vector<vector<int>> queue;
	Deletion_Offline(int qn):qn(qn), queue(qn << 1){
		Assert(qn >= 0);
	}
	// Update i exists during interval [ql, qr).
	void insert(int ql, int qr, T i){
		Assert(0 <= ql && ql <= qr && qr <= qn);
		auto recurse = [&](auto self, int u, int l, int r)->void{
			if(qr <= l || r <= ql) return;
			if(ql <= l && r <= qr) queue[u].push_back(i);
			else{
				int m = l + (r - l >> 1), v = u + 1, w = u + (m - l << 1);
				self(self, v, l, m), self(self, w, m, r);
			}
		};
		recurse(recurse, 0, 0, qn);
	}
	// insert(i): insert update i.
	// state(): return the state of the DS as an integer.
	// reverse_to(s): reverse the state of the DS to s.
	// answer(qi): return the answer to the time qi.
	// O(qn * log(qn)) insert() calls and O(qn) answer() calls.
	template<class I, class S, class R, class A>
	void solve(I insert, S state, R reverse_to, A answer){
        if (qn == 0) return;
		auto recurse = [&](auto self, int u, int l, int r)->void{
			auto s = state();
			for(auto i: queue[u]) insert(i);
			if(r - l == 1) answer(l);
			else{
				int m = l + (r - l >> 1);
				self(self, u + 1, l, m), self(self, u + (m - l << 1), m, r);
			}
			reverse_to(s);
		};
		recurse(recurse, 0, 0, qn);
	}
};
} // namespace mitsuha
#endif // AJAY_DELETION_OFFLINE