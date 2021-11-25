#include "pch.h"
#include "HomoIsoTurb.h"
#include "Random.h"
using namespace std;

HomoIsoTurb::HomoIsoTurb(const Mesh& ms) :
	Nx(ms.Nx), Ny(ms.Ny), Nz(ms.Nz), LD(ms.Lx), ms(ms),
	u(ms), v(ms), w(ms), dudx(ms), dudy(ms), dudz(ms), dvdx(ms), dvdy(ms), dvdz(ms),
	dwdx(ms), dwdy(ms), dwdz(ms), step(0),indexlist(0,0),interpolater()
{
	interpolater.interpCoef(ms.Lx / ms.Nx, ms.Ly / ms.Ny, ms.Lz / ms.Nz);
	//u = Scalar(ms); v = Scalar(ms); w = Scalar(ms);
	//dudx = Scalar(ms); dudy = Scalar(ms); dudz = Scalar(ms);
	//dvdx = Scalar(ms); dvdy = Scalar(ms); dvdz = Scalar(ms);
	//dwdx = Scalar(ms); dwdy = Scalar(ms); dwdz = Scalar(ms);

	return;
}

void HomoIsoTurb::infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw){
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
		interpolater.interp3d(temppos, u, v, w, uf[pn]);
		interpolater.interp3d(temppos, dudx, dudy, dudz, gradu[pn]);
		interpolater.interp3d(temppos, dvdx, dvdy, dvdz, gradv[pn]);
		interpolater.interp3d(temppos, dwdx, dwdy, dwdz, gradw[pn]);
	}


}
void HomoIsoTurb::initialize(const std::string& path, const Config& config) {
	
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
	
	flowfieldpath = config.Read<string>("data path");
	flowfieldpath = flowfieldpath + "/";
	
	//if (ifrozen) {
	//	// random chosen frozen flow
	//	loadFlowData("F:/winscp下载/velfield-convert/", indexlist[rd::randi(0, indexlist.size())]);
	//}
	//else {
	//	// continueous flow, read the first one
	//	loadFlowData("F:/winscp下载/velfield-convert/", indexlist[0]);
	//}

	//makeGradient();
	//dudx.FileIO("D:/Homework/研3-1/2020-10-10-AMFcpp/tools/", "dudx", 'w');
	//dvdy.FileIO("D:/Homework/研3-1/2020-10-10-AMFcpp/tools/", "dvdy", 'w');
	//dwdz.FileIO("D:/Homework/研3-1/2020-10-10-AMFcpp/tools/", "dwdz", 'w');
	//dwdx.FileIO("D:/Homework/研3-1/2020-10-10-AMFcpp/tools/", "dwdx", 'w');

	return;
}

void HomoIsoTurb::reset() {
	int chosen = ifrozen ? rd::randi(0, indexlist.size()) : 0;
	loadFlowData(flowfieldpath.c_str(), indexlist[chosen]);
	cout << chosen << endl;
	makeGradient();
	_flowIndexCount = 0;
	_updateStepCount = 0;
}

// to make an instance of the class from the config. 
// cannot be done in constructor because member "mesh" don't have default constructor
std::shared_ptr<HomoIsoTurb> HomoIsoTurb::makeInstance(const Config& config) {
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

	Geometry_prdXYZ geo(Nx, Ny, Nz, L*PI, L*PI, L*PI);
	geo.InitMesh();
	geo.InitIndices();
	geo.InitInterval();	
	geo.InitWaveNumber();
	Mesh mesh(geo);

	return make_shared<HomoIsoTurb>(mesh);
}

void HomoIsoTurb::loadFlowData(const char* path, int loadstep) {
	char idx[256];
	string fname;	
	sprintf(idx, "%.7d", loadstep);
	string idxstr = string(idx);
	fname = "u-" + idxstr;
	u.FileIO(path, fname.c_str(), 'r');
	fname = "v-" + idxstr;
	v.FileIO(path, fname.c_str(), 'r');
	fname = "w-" + idxstr;
	w.FileIO(path, fname.c_str(), 'r');

	//fname = "dudx-" + string(idx);
	//dudx.FileIO(path, fname.c_str(), 'r');
	//fname = "dudy-" + string(idx);
	//dudy.FileIO(path, fname.c_str(), 'r');
	//fname = "dudz-" + string(idx);
	//dudz.FileIO(path, fname.c_str(), 'r');

	//fname = "dvdx-" + string(idx);
	//dvdx.FileIO(path, fname.c_str(), 'r');
	//fname = "dvdy-" + string(idx);
	//dvdy.FileIO(path, fname.c_str(), 'r');
	//fname = "dvdz-" + string(idx);
	//dvdz.FileIO(path, fname.c_str(), 'r');

	//fname = "dwdx-" + string(idx);
	//dwdx.FileIO(path, fname.c_str(), 'r');
	//fname = "dwdy-" + string(idx);
	//dwdy.FileIO(path, fname.c_str(), 'r');
	//fname = "dwdz-" + string(idx);
	//dwdz.FileIO(path, fname.c_str(), 'r');

	//u.Set(0.0);
	//v.Set(0.0);
	//w.Set(0.0);

	//dudx.Set(0.0);
	//dudy.Set(0.0);
	//dudz.Set(0.0);

	//dvdx.Set(0.0);
	//dvdy.Set(0.0);
	//dvdz.Set(0.0);

	//dwdx.Set(0.0);
	//dwdy.Set(0.0);
	//dwdz.Set(0.0);

	return;
}

void HomoIsoTurb::makeGradient() {
	u.GradientAtCenter(dudx, dudy, dudz);
	v.GradientAtCenter(dvdx, dvdy, dvdz);
	w.GradientAtCenter(dwdx, dwdy, dwdz);
	return;
}


void HomoIsoTurb::update(double dt) {
	if (ifrozen) return;
	if (++_updateStepCount % 20 != 0) return;


	_flowIndexCount++;
	if (_flowIndexCount >= indexlist.size()) {
		cout << "Error: indexcount is out of the bound of indexlist while updating env" << endl;
	}
	loadFlowData(flowfieldpath.c_str(), indexlist[_flowIndexCount]);
	makeGradient();
	if (_flowIndexCount % 10 == 0)	cout << _flowIndexCount << " ";
	
	return;
}