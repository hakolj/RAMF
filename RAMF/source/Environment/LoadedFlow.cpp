#include "pch.h"
#include "LoadedFlow.h"
#include "Random.h"
#include <fstream>

using namespace std;


LoadedFlow::LoadedFlow(const Mesh& ms) :
	Nx(ms.Nx), Ny(ms.Ny), Nz(ms.Nz), LD(ms.Lx), ms(ms),
	u(NULL), v(NULL), w(NULL), dudx(ms), dudy(ms), dudz(ms), dvdx(ms), dvdy(ms), dvdz(ms),
	dwdx(ms), dwdy(ms), dwdz(ms), indexlist(0, 0), interpolater(),datapool()
{
	interpolater.interpCoef(ms.Lx / ms.Nx, ms.Ly / ms.Ny, ms.Lz / ms.Nz);
	//u = Scalar(ms); v = Scalar(ms); w = Scalar(ms);
	//dudx = Scalar(ms); dudy = Scalar(ms); dudz = Scalar(ms);
	//dvdx = Scalar(ms); dvdy = Scalar(ms); dvdz = Scalar(ms);
	//dwdx = Scalar(ms); dwdy = Scalar(ms); dwdz = Scalar(ms);

	return;
}

void LoadedFlow::infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) {
#pragma omp parallel for
	for (int pn = 0; pn < pos.size(); pn++) {
		vec3d temppos;
		temppos[0] = fmod(pos[pn][0], ms.Lx);
		temppos[1] = fmod(pos[pn][1], ms.Ly);
		temppos[2] = fmod(pos[pn][2], ms.Lz);
		//cout << temppos << endl;
		temppos[0] = fmod(temppos[0] + ms.Lx, ms.Lx);
		temppos[1] = fmod(temppos[1] + ms.Ly, ms.Ly);
		temppos[2] = fmod(temppos[2] + ms.Lz, ms.Lz);
		//cout << temppos << endl;
		interpolater.interp3d(temppos, *u, *v, *w, uf[pn]);
		interpolater.interp3d(temppos, dudx, dudy, dudz, gradu[pn]);
		interpolater.interp3d(temppos, dvdx, dvdy, dvdz, gradv[pn]);
		interpolater.interp3d(temppos, dwdx, dwdy, dwdz, gradw[pn]);
	}


}
void LoadedFlow::initialize(const std::string& path, const Config& config) {

	string stpstr = config.Read<string>("step range", "NULL");
	if (stpstr == "NULL") {
		std::cout << "Wrong flow field index list in HomoIsoTurb" << std::endl;
		//step = config.Read<int>("step");
		//indexlist = vector<int>(1, step);
	}
	else {		
		stringstream ss;
		int start, end, interv;
		ss << stpstr;
		ss >> start >> end >> interv;
		int curridx = start;
		while (curridx <= end) {
			indexlist.push_back(curridx);
			curridx += interv;
		}
	}
	ifrozen = config.Read<bool>("random frozen", false);
	_nextFieldCount = config.Read<int>("steps for next field", 1);

	flowfieldpath = config.Read<string>("data path");
	flowfieldpath = flowfieldpath + "/";

	datapool.LoadData(ms, flowfieldpath,indexlist); //load flow data to datapool
	loadFlowData(indexlist[0]);
	return;
}

void LoadedFlow::reset() {

	_flowIndexCount = 0;
	_updateStepCount = 0;
	int chosen = ifrozen ? rd::randi(0, indexlist.size()) : 0;
	//loadFlowData(flowfieldpath.c_str(), indexlist[chosen]);
	cout << chosen << endl;
	loadFlowData(chosen);
	makeGradient();
}

// to make an instance of the class from the config. 
// cannot be done in constructor because member "mesh" don't have default constructor
std::shared_ptr<LoadedFlow> LoadedFlow::makeInstance(const Config& config) {
	string str = config.Read<string>("Mesh Number");
	stringstream ss;
	int Nx, Ny, Nz;
	double L;
	ss << str;
	ss >> Nx >> Ny >> Nz;

	str = config.Read<string>("Domain Size", "2PI");
	str = str.substr(0, str.find("PI"));
	ss.str("");
	ss.clear();
	ss << str;
	ss >> L;

	Geometry_prdXYZ geo(Nx, Ny, Nz, L * PI, L * PI, L * PI);
	geo.InitMesh();
	geo.InitIndices();
	geo.InitInterval();
	geo.InitWaveNumber();
	Mesh mesh(geo);

	return make_shared<LoadedFlow>(mesh);
}

void LoadedFlow::loadFlowData(int loadstep) {

	u = &(datapool.upool[loadstep]);
	v = &(datapool.vpool[loadstep]);
	w = &(datapool.wpool[loadstep]);

	if (u->ms.Nx != 96) {
		cout << loadstep << endl;
	}

	return;
}

void LoadedFlow::makeGradient() {
	u->GradientAtCenter(dudx, dudy, dudz);
	v->GradientAtCenter(dvdx, dvdy, dvdz);
	w->GradientAtCenter(dwdx, dwdy, dwdz);
	return;
}


void LoadedFlow::update(double dt) {
	if (ifrozen) return;
	if (++_updateStepCount % _nextFieldCount != 0) return;

	_updateStepCount = 0;
	_flowIndexCount++;
	if (_flowIndexCount >= indexlist.size()) {
		cout << "Error: indexcount is out of the bound of indexlist while updating env" << endl;
		return;
	}

	loadFlowData(_flowIndexCount);
	makeGradient();
	if (_flowIndexCount % 10 == 0)	cout << indexlist[_flowIndexCount] << " ";

	return;
}