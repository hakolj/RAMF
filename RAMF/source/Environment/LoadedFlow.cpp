#include "pch.h"
#include "LoadedFlow.h"
#include "Random.h"
#include <fstream>

using namespace std;


LoadedFlow::LoadedFlow(const Mesh& ms) :
	Nx(ms.Nx), Ny(ms.Ny), Nz(ms.Nz), Lx(ms.Lx), Ly(ms.Ly), Lz(ms.Lz), ms(ms),
	u(NULL), v(NULL), w(NULL), dudx(ms), dudy(ms), dudz(ms), dvdx(ms), dvdy(ms), dvdz(ms),
	dwdx(ms), dwdy(ms), dwdz(ms), indexlist(0, 0), interpolater(),datapool()
{
	interpolater.interpCoef(ms.Lx / (ms.Nx-1), ms.Ly / (ms.Ny-1), ms.Lz / (ms.Nz-1));
	//u = Scalar(ms); v = Scalar(ms); w = Scalar(ms);
	//dudx = Scalar(ms); dudy = Scalar(ms); dudz = Scalar(ms);
	//dvdx = Scalar(ms); dvdy = Scalar(ms); dvdz = Scalar(ms);
	//dwdx = Scalar(ms); dwdy = Scalar(ms); dwdz = Scalar(ms);

	return;
}

void LoadedFlow::infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) const {



#pragma omp parallel for
	for (int pn = 0; pn < pos.size(); pn++) {
		vec3d temppos;
		//temppos[0] = fmod(pos[pn][0], ms.Lx);
		//temppos[1] = fmod(pos[pn][1], ms.Ly);
		//temppos[2] = fmod(pos[pn][2], ms.Lz);
		////cout << temppos << endl;
		//temppos[0] = fmod(temppos[0] + ms.Lx, ms.Lx);
		//temppos[1] = fmod(temppos[1] + ms.Ly, ms.Ly);
		//temppos[2] = fmod(temppos[2] + ms.Lz, ms.Lz);


		temppos[0] = fmod(pos[pn][0], ms.Lx);
		temppos[1] = fmod(pos[pn][1], ms.Ly);
		temppos[2] = fmod(pos[pn][2], ms.Lz);
		//cout << temppos << endl;
		temppos[0] = fmod(temppos[0] + ms.Lx, ms.Lx);
		temppos[1] = fmod(temppos[1] + ms.Ly, ms.Ly);
		temppos[2] = fmod(temppos[2] + ms.Lz, ms.Lz);
		//cout << temppos << endl;
		//interpolater.interp3d(temppos, *u, *v, *w, uf[pn]);
		//interpolater.interp3d(temppos, dudx, dudy, dudz, gradu[pn]);
		//interpolater.interp3d(temppos, dvdx, dvdy, dvdz, gradv[pn]);
		//interpolater.interp3d(temppos, dwdx, dwdy, dwdz, gradw[pn]);

		interpolater.interp3d(temppos, *u, *v, *w, uf[pn], fieldstore);
		interpolater.interp3d(temppos, dudx, dudy, dudz, gradu[pn], FieldStoreType::CCC);
		interpolater.interp3d(temppos, dvdx, dvdy, dvdz, gradv[pn], FieldStoreType::CCC);
		interpolater.interp3d(temppos, dwdx, dwdy, dwdz, gradw[pn], FieldStoreType::CCC);

		//interpolater.interp3d_old(temppos, *u, *v, *w, uf[pn]);
		//interpolater.interp3d_old(temppos, dudx, dudy, dudz, gradu[pn]);
		//interpolater.interp3d_old(temppos, dvdx, dvdy, dvdz, gradv[pn]);
		//interpolater.interp3d_old(temppos, dwdx, dwdy, dwdz, gradw[pn]);
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
		if (interv == 0) {
			cout << "Error: flow field index has zero interv." << endl;
		}
		else {
			while (curridx < end + interv) {
				indexlist.push_back(curridx);
				curridx += interv;
			}
		}

	}
	ifrozen = config.Read<bool>("random frozen", false);
	_nextFieldCount = config.Read<int>("steps for next field", 1);

	_boundaryType = config.Read<string>("Boundary Type");

	_fieldStoreStr = config.Read<string>("Field Store Type", "UDF");
	if (_fieldStoreStr == "UDF") {
		fieldstore = FieldStoreType::UDF; //undefined
		
	}
	else if (_fieldStoreStr == "CCC") {
		fieldstore = FieldStoreType::CCC;
	}
	else if (_fieldStoreStr == "CEC") {
		fieldstore = FieldStoreType::CEC;
	}

	if (fieldstore == FieldStoreType::UDF) {
		cout << "Error: the field store type is not assigned explicitly in config.txt" << endl;
		return;
	}

	flowfieldpath = config.Read<string>("data path");
	flowfieldpath = flowfieldpath + "/";

	datapool.LoadData(ms, flowfieldpath,indexlist); //load flow data to datapool
	loadFlowData(0);
	return;
}

void LoadedFlow::reset() {

	_flowIndexCount = 0;
	_updateStepCount = 0;
	int chosen = ifrozen ? rd::randi(0, indexlist.size()) : 0;
	//loadFlowData(flowfieldpath.c_str(), indexlist[chosen]);
	cout << "chosen flow idx = " << chosen << endl;
	loadFlowData(chosen);
	makeGradient();
}

// to make an instance of the class from the config. 
// cannot be done in constructor because member "mesh" don't have default constructor
std::shared_ptr<LoadedFlow> LoadedFlow::makeInstance(const Config& config) {
	string str = config.Read<string>("Mesh Number");
	stringstream ss;
	int Nx, Ny, Nz;
	double Lx, Ly, Lz;
	ss << str;
	ss >> Nx >> Ny >> Nz;

	size_t iPI;
	bool pflag;

	str = config.Read<string>("DomainX");
	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Lx;
	if (pflag) Lx *= M_PI;


	str = config.Read<string>("DomainY");
	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Ly;
	if (pflag) Ly *= M_PI;

	str = config.Read<string>("DomainZ");
	iPI = str.find("PI");
	pflag = iPI != string::npos;
	if (pflag) {
		str = str.substr(0, iPI);
	}
	ss.str("");
	ss.clear();
	ss << str;
	ss >> Lz;
	if (pflag) Lz *= M_PI;


	string boundType = config.Read<string>("Boundary Type");

	Geometry* geo = NULL;

	if (boundType == "PPP") {
		
		geo = new Geometry_prdXYZ (Nx, Ny, Nz, Lx, Ly, Lz);
	}
	else if (boundType == "PWP") {
		geo = new Geometry_prdXZ(Nx, Ny, Nz, Lx, Ly, Lz);
	}
	else {
		cout << "environment boundary type " << boundType << " is undefined" << endl;
		exit;
	}
	string meshYPath = config.Read<string>("Y Mesh Path", "");
	if (meshYPath.length() == 0) 
		geo->InitMeshEdge(); //uniform mesh
	else 
		geo->InitMeshEdge(true, meshYPath.c_str()); //y mesh from file

	
	geo->InitMesh();
	geo->InitIndices();
	geo->InitInterval();
	geo->InitWaveNumber();
	Mesh mesh(*geo);

	delete geo;

	return make_shared<LoadedFlow>(mesh);
}

void LoadedFlow::loadFlowData(int loadstep) {

	u = &(datapool.upool[loadstep]);
	v = &(datapool.vpool[loadstep]);
	w = &(datapool.wpool[loadstep]);

	//if (u->ms.Nx != 96) {
	//	cout << "debug 96"<< loadstep << endl;
	//}

	return;
}

void LoadedFlow::makeGradient() {
	u->GradientAtCenter(dudx, dudy, dudz, _fieldStoreStr[0]);
	v->GradientAtCenter(dvdx, dvdy, dvdz, _fieldStoreStr[1]);
	w->GradientAtCenter(dwdx, dwdy, dwdz, _fieldStoreStr[2]);
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