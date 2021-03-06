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
	double Lx, Ly, Lz; //domain size

	Mesh ms;
	Scalar *u, *v, *w;
	Scalar dudx, dudy, dudz;
	Scalar dvdx, dvdy, dvdz;
	Scalar dwdx, dwdy, dwdz;

	FieldStoreType fieldstore = FieldStoreType::UDF;

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
	std::string _boundaryType = "NNN";
	std::string _fieldStoreStr = "";
	
	std::string flowfieldpath;


public:
	virtual void initialize(const std::string& path, const Config& config);
	virtual void reset();
	virtual void infoAtPoint (const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) const;
	virtual void update(double dt);
	virtual std::string boundaryType() { return _boundaryType; }
	inline virtual void getDomainSize(double& Lx, double& Ly, double& Lz) { Lx = this->Lx; Ly = this->Ly; Lz = this->Lz; }
	static std::shared_ptr<LoadedFlow> makeInstance(const Config& config);

	//HomoIsoTurb(const Config& config);

	void loadFlowData(int loadstep);
	void makeGradient();
	void makeGradientBoundary();

};



#endif // !LOADEDFLOW_H
