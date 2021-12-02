#pragma once
#include "Environment.h"
class Quiescent :
	public Environment,
	public InfoAtPointAble
{
public:
	Quiescent() { return; }
	void infoAtPoint(const vectors3d& pos, vectors3d& uf, vectors3d& gradu, vectors3d& gradv, vectors3d& gradw) {
		for (unsigned i = 0; i < pos.size(); i++) {
			uf[i] = vec3d::Zero();
			gradu[i] = vec3d::Zero();
			gradv[i] = vec3d::Zero();
			gradw[i] = vec3d::Zero();
		}
		return;

	}
	void reset() {}
	void initialize(const std::string& path, const Config& config) {};
	void update(double dt) {};
	virtual std::string boundaryType() { return "PPP"; }
	inline virtual void getDomainSize(double& Lx, double& Ly, double& Lz) {
		Lx = 1.0;
		Ly = 1.0;
		Lz = 1.0;
	}
};