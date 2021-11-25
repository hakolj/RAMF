#pragma once
#ifndef LOADEDFLOW_H
#define LOADEDFLOW_H

#include "Environment.h"
#include "Interpolation.h"
#include "Scalar.h"
#include "FlowFieldDataPool.h"
class LoadedFlow :
	public Environment,
	public InfoAtPointAble {

public:
	
	LoadedFlow(const Mesh& ms);
	~LoadedFlow() {}

public:
	int Nx, Ny, Nz;
	double LD;
	Mesh ms;
	Scalar *u, *v, *w;
	Scalar dudx, dudy, dudz;
	Scalar dvdx, dvdy, dvdz;
	Scalar dwdx, dwdy, dwdz;
	std::vector<int> indexlist; //list of index of flow data

	//std::array<int,3> steprange; // [step0, step1, interval]
	std::string datapath;
	Lag2nd3D interpolater;
	FlowFieldDataPool datapool;

private:
	bool ifrozen = false;
	int _flowIndexCount = 0;
	int _updateStepCount = 0;
	int _nextFieldCount = 1;
	
	std::string flowfieldpath;


public:
	virtual void initialize(const std::string& path, const Config& config);
	virtual void reset();
	virtual void infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw);
	virtual void update(double dt);

	static std::shared_ptr<LoadedFlow> makeInstance(const Config& config);

	//HomoIsoTurb(const Config& config);

	void loadFlowData(int loadstep);
	void makeGradient();

};



#endif // !LOADEDFLOW_H
