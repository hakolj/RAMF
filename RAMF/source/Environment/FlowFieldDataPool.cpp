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

	//note: when load hit data of Nx,Ny,Nz grid points,
	//the scalar has a data of size Nx+1, Ny+1, Nz+1. example: U
	//Because the domain is periodic, U(0,:,:) = U(Nx+1,:,:). U(:,0,:) = U(:,Ny+1,:) ...
	//BUT IN THE CURRENT DATA U(0,:,:) IS SET TO ZERO BECAUSE IT IS NEVER USED.

	// for HIT flow data, all the velocities and gradients are stored at the center of a grid in 3 directions 
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
	//cout << upool[0](1, 2, 1) << endl;
	//cout << upool[1](1, 2, 1) << endl;
	return; 
}
