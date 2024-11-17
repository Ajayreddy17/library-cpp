#ifndef AJAY_ARITHMATIC_SEQUENCE_UPDATE_PROCESSOR
#define AJAY_ARITHMATIC_SEQUENCE_UPDATE_PROCESSOR

namespace mitsuha{
template<class T>
struct Arithmetic_Sequence_Update_Processor{
	bool reversed;
	int n, ptr;
	vector<T> data, res;
	Arithmetic_Sequence_Update_Processor(int n, bool reversed = false): reversed(reversed), n(n), ptr(0), data(n + 2, T{}){
		Assert(n >= 0);
	}
	// O(n)
	void clear(){
		ptr = 0;
		fill(data.begin(), data.end(), T{});
	}
	// Add start, start + step, start + step * 2, ... on [l, r)
	// l must be equal or greater than the largest index which query() has been called over if reverse = false
	// Otherwise, r must be equal or less than the smallest index which query() has been called over plus one
	// O(1)
	void update(int l, int r, T start, T step = T{}){
		if(reversed) tie(l, r, start, step) = tuple{n - r, n - l, start + (r - l - 1) * step, -step};
		Assert(ptr <= l && l <= r && r <= n);
		if(l == r) return;
		data[l] += start;
		data[l + 1] -= start - step;
		data[r] -= start + (r - l) * step;
		data[r + 1] += start + (r - l - 1) * step;
	}
	// Add rev_start + (r - l - 1) * rev_step, rev_start + (r - l - 2) * rev_step, ... on [l, r)
	// l must be equal or greater than the largest index which query() has been called over if reverse = false
	// Otherwise, r must be equal or less than the smallest index which query() has been called over plus one
	// O(1)
	void update_reverse(int l, int r, T rev_start, T rev_step = T{}){
		update(l, r, rev_start + (r - l - 1) * rev_step, -rev_step);
	}
	// Add x to position p
	// p must be equal or greater than the largest index which query() has been called over
	// O(1)
	void update(int p, T x){
		if(reversed) p = n - 1 - p;
		Assert(ptr <= p && p < n);
		data[p] += x;
		data[p + 1] -= 2 * x;
		data[p + 2] += x;
	}
	// O(max(1, p - ptr))
	T query(int p){
		if(reversed) p = n - 1 - p;
		Assert(0 <= p && p < n);
		for(; ptr < p; ++ ptr){
			data[ptr + 1] += data[ptr];
			if(ptr >= 1) data[ptr] += data[ptr - 1];
		}
		return p == ptr ? data[p] + (p >= 1 ? data[p - 1] : T{}) : data[p];
	}
	// O(n)
	const vector<T> &snapshot(){
		res.resize(n + 1);
		copy(data.begin(), data.end() - 1, res.begin());
		for(auto i = ptr; i < n; ++ i){
			res[i + 1] += res[i];
			if(1 <= i) res[i] += res[i - 1];
		}
		res.pop_back();
		if(reversed) reverse(res.begin(), res.end());
		return res;
	}
};
} // namespace mitsuha
#endif // AJAY_ARITHMATIC_SEQUENCE_UPDATE_PROCESSOR