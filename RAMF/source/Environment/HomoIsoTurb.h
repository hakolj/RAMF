#pragma once
#ifndef HOMOISOTURB_H
#define HOMOISOTURB_H


#include "pch.h"
#include "Environment.h"
#include "EnvironmentInterface.h"
#include "Scalar.h"
#include "Interpolation.h"
class HomoIsoTurb:
	public Environment,
	public InfoAtPointAble
{
public:
	HomoIsoTurb(const Mesh& ms);
	~HomoIsoTurb() {};

public:
	int Nx, Ny, Nz;
	double LD; //length of domain (Lx=Ly=Lz=LD)
	Mesh ms;
	Scalar u, v, w;
	Scalar dudx, dudy, dudz;
	Scalar dvdx, dvdy, dvdz;
	Scalar dwdx, dwdy, dwdz;
	int step;
	std::vector<int> indexlist; //list of index of flow data
	//std::array<int,3> steprange; // [step0, step1, interval]
	std::string datapath;
	Lag2nd3D interpolater;

private:
	bool ifrozen = false;
	int _flowIndexCount = 0;
	int _updateStepCount = 0;
	std::string flowfieldpath;


public:
	virtual void initialize(const std::string& path, const Config& config);
	virtual void reset();
	virtual void infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) const;
	virtual void update(double dt);
	virtual std::string boundaryType() { return "PPP"; }
	inline virtual void getDomainSize(double& Lx, double& Ly, double& Lz) { Lx = LD; Ly = LD; Lz = LD; }

	static std::shared_ptr<HomoIsoTurb> makeInstance(const Config& config);

	//HomoIsoTurb(const Config& config);

	void loadFlowData(const char* path,int loadstep);
	void makeGradient();
	

};

#endif // !HOMOISOTURB_H