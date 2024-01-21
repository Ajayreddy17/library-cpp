#ifndef AJAY_MEX
#define AJAY_MEX

namespace mitsuha{
int mex(const vector<int> &a){
	int n = (int)a.size();
	static vector<int> found;
	found.assign(n + 1, false);
	for(auto x: a) if(x <= n) found[x] = true;
	int res = 0;
	while(found[res]) ++ res;
	return res;
}
} // namespace mitsuha
#endif // AJAY_MEX