#include "pch.h"
#include "FlowFieldDataPool.h"

using namespace std;
FlowFieldDataPool::FlowFieldDataPool() :
	upool(), vpool(), wpool(), _datapath()
{

}

void FlowFieldDataPool::LoadData(const Mesh& ms, const std::string& datapath, const std::vector<int>& idxlist) {
	Scalar u(ms);
	Scalar v(ms);
	Scalar w(ms);
	_datapath = datapath;
	upool.resize(idxlist.size(), Scalar(ms));
	vpool.resize(idxlist.size(), Scalar(ms));
	wpool.resize(idxlist.size(), Scalar(ms));


	for (int i = 0; i < idxlist.size(); i++) {
		char idx[256];
		string fname;
		sprintf(idx, "%.7d", idxlist[i]);
		string idxstr = string(idx);
		fname = "u-" + idxstr;
		upool[i].FileIO(_datapath.c_str(), fname.c_str(), 'r');
		fname = "v-" + idxstr;
		vpool[i].FileIO(_datapath.c_str(), fname.c_str(), 'r');
		fname = "w-" + idxstr;
		wpool[i].FileIO(_datapath.c_str(), fname.c_str(), 'r');
		
	}
	cout << upool[0](1, 2, 1) << endl;
	cout << upool[1](1, 2, 1) << endl;
	return; 
}
